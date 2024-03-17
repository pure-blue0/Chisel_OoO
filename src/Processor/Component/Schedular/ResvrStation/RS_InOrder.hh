#ifndef __RS_INORDER_HH__
#define __RS_INORDER_HH__

#include "../BaseSchedular.hh"
#include "../../LoopQueue.hh"

namespace Emulator
{
    
class RS_InOrder : public BaseSchedular
{
private:

    LoopQueue<InsnPtr_t>   m_issueQueue;


public:
    RS_InOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(fmt::format("{}_issueQueue",Name),EntryCount)
    {};

    ~RS_InOrder() {};

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
                if(((insn->Rs1Type == RegType_t::INT && !insn->Operand1Ready) ||
                    (insn->Rs2Type == RegType_t::INT && !insn->Operand2Ready)
                )){
                    return;
                }else{
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
            }
    };

    void Deallocate(uint64_t& index){
        this->m_issueQueue.Pop();
    }; 


    void Forwarding(InsnPtr_t& insn){
        if(insn->RdType != RegType_t::NONE && insn->PhyRd != 0 && !this->Empty()){
            uint16_t iQPtr = this->m_issueQueue.getHeader();
            for(size_t i = 0; i < this->m_issueQueue.getUsage(); i++)
            {
                InsnPtr_t& insn_wfIssue = this->m_issueQueue[iQPtr];
                if(insn_wfIssue){
                    if(insn->RdType == insn_wfIssue->Rs1Type && insn->PhyRd == insn_wfIssue->PhyRs1){
                        insn_wfIssue->Operand1 = insn->RdResult;
                        insn_wfIssue->Operand1Ready  = true;
                        DPRINTF(Forwarding,"RobTag[{}],Pc[{:#x}] -> Get Forwarding Data Rs1[{}],PRs1[{}] - Value[{:#x}]",
                            insn_wfIssue->RobTag, 
                            insn_wfIssue->Pc,
                            insn_wfIssue->IsaRs1,
                            insn_wfIssue->PhyRs1,
                            insn_wfIssue->Operand1
                        )
                    }
                    if(insn->RdType == insn_wfIssue->Rs2Type && insn->PhyRd == insn_wfIssue->PhyRs2){
                        insn_wfIssue->Operand2 = insn->RdResult;
                        insn_wfIssue->Operand2Ready = true;
                        DPRINTF(Forwarding,"RobTag[{}],Pc[{:#x}] -> Get Forwarding Data Rs2[{}],PRs2[{}] - Value[{:#x}]",
                            insn_wfIssue->RobTag, 
                            insn_wfIssue->Pc,
                            insn_wfIssue->IsaRs2,
                            insn_wfIssue->PhyRs2,
                            insn_wfIssue->Operand2
                        )
                    }
                }
                iQPtr = this->m_issueQueue.getNextPtr(iQPtr);
            }
        }
    };
};

} // namespace Emulator




#endif