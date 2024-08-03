#ifndef __CMT_INORDER_HH__
#define __CMT_INORDER_HH__

#include "../BaseSchedular.hh"
#include "../../LoopQueue.hh"

namespace Emulator
{
    
class CMT_InOrder : public BaseSchedular
{
private:

    LoopQueue<InsnPtr_t>   m_issueQueue;


public:
    CMT_InOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(fmt::format("{}_issueQueue",Name),EntryCount)
    {};

    ~CMT_InOrder() {};

    void Reset(){
        this->m_issueQueue.Reset();
    };

    bool IssueQueueFull(){
        return this->m_issueQueue.full();
    };

    bool Empty(){
        return this->m_issueQueue.empty();
    };

    uint64_t GetAvailibleEntryCount(){
        return this->m_issueQueue.getAvailEntryCount();
    };

    uint64_t AllocateEntry(){
        return this->m_issueQueue.Allocate();
    };
    

    void Dispatch(InsnPtr_t& insn, uint64_t& index){
        this->m_issueQueue[index] = insn;
    };

    void IssueSelect(uint64_t& index, InsnPtr_t& insn, bool& Success){
        IssueInfo info;
        Success = false;
        insn = this->m_issueQueue.front();
        insn->data_valid=true;
        info.insn = insn;
        if(( !this->m_Rcu->ReadyForCommit(insn->RobTag)      ||
            (!this->m_Rcu->m_IntBusylist[insn->PhyRs1].done) ||
            (!this->m_Rcu->m_IntBusylist[insn->PhyRs2].done)
        )){
            return;                
        }else{
            for(auto rfport : this->m_RFReadPortVec){
                if(!rfport->valid){
                    for(auto fu : this->m_FuncUnitVec){
                        if(fu->m_SupportFunc.count(insn->Fu) && !fu->Busy()){
                            fu->Allocate();
                            info.desIndex  = fu->m_FuncUnitId;
                            Success        = true;
                            rfport->set(info);
                            insn->State = InsnState_t::State_ReadOperand;
                            return;
                        }
                    }
                }
            }
        }
    };

    void Deallocate(uint64_t& index){
        this->m_issueQueue.Pop();
    }; 

};

} // namespace Emulator




#endif