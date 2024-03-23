#ifndef __RFQ_INAGEORDER_HH__
#define __RFQ_INAGEORDER_HH__

#include "../BaseSchedular.hh"
#include <deque>

namespace Emulator
{
    
class RFQ_InAgeOrder : public BaseSchedular
{
private:

    std::deque<InsnPtr_t>           m_issueQueue;

    std::deque<uint64_t>            m_freeList;

    std::deque<uint64_t>            m_bustList;

public:
    RFQ_InAgeOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(EntryCount,nullptr)
    {};

    ~RFQ_InAgeOrder() {};

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
    //当指令在dispatch中被放入dispatchscheduler时，会更新m_bustList
    //或当指令在dispatchscheduler中被放入issuescheduler时，会更新m_bustList
    uint64_t AllocateEntry(){//index 只是一个m_issueQueue索引，用来说明entry中有哪些是空闲的，所以可以重复使用
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
                insn = this->m_issueQueue[index];//从发射队列中取指
                info.insn = insn;
                if(insn){
                    for(auto rfport : this->m_RFReadPortVec){
                        if(!rfport->valid){//遍历每个调度器的读端口，看是否有效，如果不是，说明这个端口还不需要读操作数，则可以使用
                            for(auto fu : this->m_FuncUnitVec){
                                if(fu->m_SupportFunc.count(insn->Fu) && !fu->Busy()){//找到可用的功能单元
                                    fu->Allocate();//设置功能单元为被占用状态
                                    info.desIndex  = fu->m_FuncUnitId;
                                    info.isToFu    = true;
                                    Success        = true;
                                    rfport->set(info);
                                    insn->State = InsnState_t::State_ReadOperand;
                                    this->m_bustList.erase(this->m_bustList.begin()+i);//擦除执行完操作的指令
                                    return;
                                }
                            }
                            for(auto subs : this->m_SubSchedular){//没找到可用的功能单元，才会执行到这一步
                                if(subs->m_SupportFunc.count(insn->Fu) && !subs->Busy()){//从调度器的对应的子调度器寻找
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
    };

    void Deallocate(uint64_t& index){
        this->m_freeList.emplace_back(index);
        this->m_issueQueue[index] = nullptr;
    }; 

};

} // namespace Emulator




#endif