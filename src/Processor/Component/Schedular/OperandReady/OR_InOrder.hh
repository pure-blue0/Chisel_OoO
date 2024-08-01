#ifndef __OR_INORDER_HH__
#define __OR_INORDER_HH__


#include "../BaseSchedular.hh"
#include "../../LoopQueue.hh"

namespace Emulator
{
    
class OR_InOrder : public BaseSchedular
{
private:

    LoopQueue<InsnPtr_t>   m_issueQueue;


public:
    OR_InOrder(
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu
    ):  BaseSchedular(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu),
        m_issueQueue(fmt::format("{}_issueQueue",Name),EntryCount)
    {};

    ~OR_InOrder() {};

    void Reset(){
        this->m_issueQueue.Reset();
    };


    bool IssueQueueFull(){
        return this->m_issueQueue.full();
    };

    bool Empty(){
        return this->m_issueQueue.empty();
    };

    uint64_t AllocateEntry(){
        return this->m_issueQueue.Allocate();
    };

    uint64_t GetAvailibleEntryCount(){
        return this->m_issueQueue.getAvailEntryCount();
    };

    void Dispatch(InsnPtr_t& insn, uint64_t& index){
        this->m_issueQueue[index] = insn;
    };
 
    void IssueSelect(uint64_t& index, InsnPtr_t& insn, bool& Success){
        IssueInfo info;
        Success = false;
        insn = this->m_issueQueue.front();//数据都在这里面，连接的时候输入信号从这里面取
        info.insn = insn;
        int a=0;
        if(((!this->m_Rcu->m_IntBusylist[insn->PhyRs1].done) ||
            (!this->m_Rcu->m_IntBusylist[insn->PhyRs2].done)
        )){//判断busylist中对应的物理寄存器是否储存了完成的数据
            return;                
        }else{
            for(auto rfport : this->m_RFReadPortVec){
                if(!rfport->valid){
                    for(auto fu : this->m_FuncUnitVec){
                        if(fu->m_SupportFunc.count(insn->Fu) && !fu->Busy()){
                            fu->Allocate();  //将function unit设为忙
                            info.desIndex  = fu->m_FuncUnitId;//执行单元的ID
                            info.isToFu    = true;//交叉验证中，需要把success的结果也连到这个变量上，然后最后通过rfport发送数据
                            Success        = true;
                            rfport->set(info);//向rfport端口发送数据
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