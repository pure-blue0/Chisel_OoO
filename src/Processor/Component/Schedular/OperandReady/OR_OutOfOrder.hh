#ifndef __OR_OUTOFORDER_HH__
#define __OR_OUTOFORDER_HH__

#include "../BaseSchedular.hh"
#include <deque>

namespace Emulator
{
    
class OR_OutOfOrder : public BaseSchedular
{
private:
    struct issueQueue_entry_t
    {
        bool        valid;
        InsnPtr_t   insn;
    };

    std::deque<issueQueue_entry_t>  m_issueQueue;

    uint64_t                        m_usage;

public:
    OR_OutOfOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(EntryCount,{false,nullptr}),
        m_usage(0)
    {};

    ~OR_OutOfOrder() {};

    void Reset(){
        this->m_usage = 0;
        for(auto entry : this->m_issueQueue){
            entry.insn  = nullptr;
            entry.valid = false;
        }
    };

    bool IssueQueueFull(){
        return this->m_usage == this->m_issueQueue.size();
    };

    bool Empty(){
        return this->m_usage == 0;
    };

    uint64_t GetAvailibleEntryCount(){
        return this->m_issueQueue.size() - this->m_usage;
    };

    uint64_t AllocateEntry(){
        DASSERT((!this->Busy()), "Allocate When there is no available entry!");
        for(size_t i = 0 ; i < this->m_issueQueue.size(); i++){
            if(!this->m_issueQueue[i].valid){
                this->m_issueQueue[i].valid = true;
                this->m_usage++;
                return i;
            }
        }
        return -1;
    };

    void Dispatch(InsnPtr_t& insn, uint64_t& index){
        this->m_issueQueue[index].insn = insn;
    };

    void IssueSelect(uint64_t& index, InsnPtr_t& insn, bool& Success){
        IssueInfo info;
        Success = false;
            for(index = 0 ; index < this->m_issueQueue.size(); index++){
                auto entry = this->m_issueQueue[index];
                if(entry.valid && entry.insn){
                    insn = entry.insn;
                    info.insn = insn;
                    if(((insn->Rs1Type == RegType_t::INT && !this->m_Rcu->m_IntBusylist[insn->PhyRs1].done) ||
                        (insn->Rs2Type == RegType_t::INT && !this->m_Rcu->m_IntBusylist[insn->PhyRs2].done)
                    )){
                        continue; 
                    }else{
                        for(auto rfport : this->m_RFReadPortVec){
                            if(!rfport->valid){
                                for(auto fu : this->m_FuncUnitVec){
                                    if(fu->m_SupportFunc.count(insn->Fu) && !fu->Busy()){
                                        fu->Allocate();
                                        info.desIndex  = fu->m_FuncUnitId;
                                        info.isToFu    = true;
                                        Success        = true;
                                        rfport->set(info);
                                        insn->State = InsnState_t::State_ReadOperand;
                                        return;
                                    }
                                }
                                for(auto subs : this->m_SubSchedular){
                                    if(subs->m_SupportFunc.count(insn->Fu) && !subs->Busy()){
                                        info.entryIndex = subs->Allocate();
                                        info.desIndex   = subs->m_SchedularId;
                                        info.isToFu     = false;
                                        Success         = true;
                                        rfport->set(info);
                                        insn->State = InsnState_t::State_ReadOperand;
                                        return;
                                    }
                                }
                                return;
                            }
                        }
                    }
                }
            }
    };

    void Deallocate(uint64_t& index){
        this->m_issueQueue[index].valid = false;
        this->m_issueQueue[index].insn  = nullptr;
        this->m_usage--;
    }; 

};

} // namespace Emulator




#endif