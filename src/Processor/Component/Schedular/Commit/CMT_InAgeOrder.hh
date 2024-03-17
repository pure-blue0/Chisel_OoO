#ifndef __CMT_INAGEORDER_HH__
#define __CMT_INAGEORDER_HH__



#include "../BaseSchedular.hh"
#include <deque>

namespace Emulator
{
    
class CMT_InAgeOrder : public BaseSchedular
{
private:

    std::deque<InsnPtr_t>           m_issueQueue;

    std::deque<uint64_t>            m_freeList;

    std::deque<uint64_t>            m_bustList;

public:
    CMT_InAgeOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(EntryCount,nullptr)
    {};

    ~CMT_InAgeOrder() {};

    void Reset(){
        this->m_bustList.clear();
        this->m_freeList.clear();
        for(size_t i = 0; i < this->m_issueQueue.size(); i++){
            this->m_freeList.emplace_back(i);
        }
    };

    bool IssueQueueFull(){
        return this->m_freeList.empty();
    };

    bool Empty(){
        return this->m_bustList.empty();
    };

    uint64_t GetAvailibleEntryCount(){
        return this->m_freeList.size();
    };

    uint64_t AllocateEntry(){
        DASSERT((!this->Busy()), "Allocate When there is no available entry!");
        uint64_t index = this->m_freeList.front();
        this->m_bustList.emplace_back(index);
        this->m_freeList.pop_front();
        this->m_issueQueue[index] = nullptr;
        return index;
    };

    void Dispatch(InsnPtr_t& insn, uint64_t& index){
        this->m_issueQueue[index] = insn;
    };



    void IssueSelect(uint64_t& index, InsnPtr_t& insn, bool& Success){
        IssueInfo info;
        Success = false;
            for(auto i = 0; i < this->m_bustList.size(); i++){
                index = this->m_bustList[i];
                insn = this->m_issueQueue[index];
                info.insn = insn;
                if(insn){
                    if((!this->m_Rcu->ReadyForCommit(insn->RobTag)                                                ||
                        (insn->Rs1Type == RegType_t::INT && !this->m_Rcu->m_IntBusylist[insn->PhyRs1].forwarding) ||
                        (insn->Rs2Type == RegType_t::INT && !this->m_Rcu->m_IntBusylist[insn->PhyRs2].forwarding)
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
                                        this->m_bustList.erase(this->m_bustList.begin()+i);
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
                                        this->m_bustList.erase(this->m_bustList.begin()+i);
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
        this->m_freeList.emplace_back(index);
        this->m_issueQueue[index] = nullptr;
    }; 

};

} // namespace Emulator




#endif