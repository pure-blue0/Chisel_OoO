#ifndef __RS_INAGEORDER_HH__
#define __RS_INAGEORDER_HH__


#include "../BaseSchedular.hh"
#include <deque>

namespace Emulator
{
    
class RS_InAgeOrder : public BaseSchedular
{
private:

    std::deque<InsnPtr_t>           m_issueQueue;

    std::deque<uint64_t>            m_freeList;

    std::deque<uint64_t>            m_bustList;

public:
    RS_InAgeOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(EntryCount,nullptr)
    {};

    ~RS_InAgeOrder() {};

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
        Success = false;
        for(auto i = 0; i < this->m_bustList.size(); i++){
            index = this->m_bustList[i];
            insn = this->m_issueQueue[index];
            if(insn){
                if(((insn->Rs1Type == RegType_t::INT && !insn->Operand1Ready) ||
                    (insn->Rs2Type == RegType_t::INT && !insn->Operand2Ready)
                )){
                    continue;
                }else{
                    for(auto fu : this->m_FuncUnitVec){
                        if(fu->m_SupportFunc.count(insn->Fu) && !fu->Busy()){
                            Success = true;
                            fu->Allocate();
                            fu->Issue(insn);
                            this->m_bustList.erase(this->m_bustList.begin()+i);
                            return;
                        }
                    }
                    for(auto subs : this->m_SubSchedular){
                        if(subs->m_SupportFunc.count(insn->Fu) && !subs->Busy()){
                            Success = true;
                            uint64_t index = subs->Allocate();
                            subs->Schedule(insn,index);
                            this->m_bustList.erase(this->m_bustList.begin()+i);
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

    void Forwarding(InsnPtr_t& insn){
        if(insn->RdType != RegType_t::NONE && insn->PhyRd != 0 && !this->Empty()){
            for(auto index : this->m_bustList){
                InsnPtr_t& insn_wfIssue = this->m_issueQueue[index];
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
            }
        }
    };

};

} // namespace Emulator




#endif