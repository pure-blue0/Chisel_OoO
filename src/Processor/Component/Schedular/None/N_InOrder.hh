#ifndef __N_INORDER_HH__
#define __N_INORDER_HH__

#include "../BaseSchedular.hh"
#include "../../LoopQueue.hh"

namespace Emulator
{
    
class N_InOrder : public BaseSchedular
{
private:

    LoopQueue<InsnPtr_t>   m_issueQueue;


public:
    N_InOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(fmt::format("{}_issueQueue",Name),EntryCount)
    {};

    ~N_InOrder() {};

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
        Success = false;
            insn = this->m_issueQueue.front();
            if(insn){
                for(auto fu : this->m_FuncUnitVec){
                    if(fu->m_SupportFunc.count(insn->Fu) && !fu->Busy()){
                        Success = true;
                        fu->Allocate();
                        fu->Issue(insn);
                        return;
                    }
                }
                for(auto subs : this->m_SubSchedular){
                    if(subs->m_SupportFunc.count(insn->Fu) && !subs->Busy()){
                        Success = true;
                        uint64_t index = subs->Allocate();
                        subs->Schedule(insn,index);
                        return;
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