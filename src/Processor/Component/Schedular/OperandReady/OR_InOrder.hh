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

    // void IssueSelect(uint64_t& index, InsnPtr_t& insn, bool& Success){
    //     IssueInfo info;
    //     Success = false;
    //     insn = this->m_issueQueue.front();//数据都在这里面，连接的时候输入信号从这里面取

  
    //     info.insn = insn;
    //     if(((!this->m_Rcu->m_IntBusylist[insn->PhyRs1].done) ||
    //         (!this->m_Rcu->m_IntBusylist[insn->PhyRs2].done)
    //     )){//判断busylist中对应的物理寄存器是否储存了完成的数据
    //         return;                
    //     }else{
    //         for(auto rfport : this->m_RFReadPortVec){
    //             if(!rfport->valid){
    //                 for(auto fu : this->m_FuncUnitVec){
    //                     if(fu->m_SupportFunc.count(insn->Fu) && !fu->Busy()){
    //                         fu->Allocate();  //将function unit设为忙
    //                         info.desIndex  = fu->m_FuncUnitId;//执行单元的ID
    //                         info.isToFu    = true;//交叉验证中，需要把success的结果也连到这个变量上，然后最后通过rfport发送数据
    //                         Success        = true;
    //                         rfport->set(info);//向rfport端口发送数据
    //                         insn->State = InsnState_t::State_ReadOperand;
    //                         return;    
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // };

    void IssueSelect(uint64_t& index, InsnPtr_t& insn1, bool& Success_find){
        IssueInfo info;
        Success_find = false;
       // insn = this->m_issueQueue.front();//数据都在这里面，连接的时候输入信号从这里面取
        InsnPtr_t insn =std::make_shared<DynInsn>();
        insn->Operand1Ready=this->m_issueQueue.front()->Operand1Ready;
        insn->Operand2Ready=this->m_issueQueue.front()->Operand2Ready;
        insn->Operand1=this->m_issueQueue.front()->Operand1;
        insn->Operand2=this->m_issueQueue.front()->Operand2;
        insn->SubOp=this->m_issueQueue.front()->SubOp;
        insn->Fu=this->m_issueQueue.front()->Fu;
        insn->Pc=this->m_issueQueue.front()->Pc;
        insn->RobTag=this->m_issueQueue.front()->RobTag;
        insn->LSQTag=this->m_issueQueue.front()->LSQTag;
        insn->Pred.Taken=this->m_issueQueue.front()->Pred.Taken;
        insn->Pred.Target=this->m_issueQueue.front()->Pred.Target;
        insn->RobTag=this->m_issueQueue.front()->RobTag;
        insn->imm=this->m_issueQueue.front()->imm;
        insn->IsaRd=this->m_issueQueue.front()->IsaRd;
        insn->PhyRs1=this->m_issueQueue.front()->PhyRs1;
        insn->PhyRs2=this->m_issueQueue.front()->PhyRs2;
        insn->PhyRd=this->m_issueQueue.front()->PhyRd;
        insn->LPhyRd=this->m_issueQueue.front()->LPhyRd;
        insn->IsaRs1=this->m_issueQueue.front()->IsaRs1;
        insn->IsaRs2=this->m_issueQueue.front()->IsaRs2;
        insn->ControlFlowInsn=this->m_issueQueue.front()->ControlFlowInsn;  
        insn->Excp=this->m_issueQueue.front()->Excp;
        info.insn = insn;
        info.insn->data_valid=false;
        //data forwarding prio:WB[3]>WB[2]>WB[1]>WB[0]>Busylist
        bool PhyRs1_done;
        bool PhyRs2_done;
        if(this->m_Rcu->BusyList_Forward_Update_EN[3]&&(insn->PhyRs1==this->m_Rcu->BusyList_Forward_Update_PhyRd[3]))PhyRs1_done=true;
        else if(this->m_Rcu->BusyList_Forward_Update_EN[2]&&(insn->PhyRs1==this->m_Rcu->BusyList_Forward_Update_PhyRd[2]))PhyRs1_done=true;
        else if(this->m_Rcu->BusyList_Forward_Update_EN[1]&&(insn->PhyRs1==this->m_Rcu->BusyList_Forward_Update_PhyRd[1]))PhyRs1_done=true;
        else if(this->m_Rcu->BusyList_Forward_Update_EN[0]&&(insn->PhyRs1==this->m_Rcu->BusyList_Forward_Update_PhyRd[0]))PhyRs1_done=true;
        else PhyRs1_done=this->m_Rcu->m_IntBusylist[insn->PhyRs1].done;

        if(this->m_Rcu->BusyList_Forward_Update_EN[3]&&(insn->PhyRs2==this->m_Rcu->BusyList_Forward_Update_PhyRd[3]))PhyRs2_done=true;
        else if(this->m_Rcu->BusyList_Forward_Update_EN[2]&&(insn->PhyRs2==this->m_Rcu->BusyList_Forward_Update_PhyRd[2]))PhyRs2_done=true;
        else if(this->m_Rcu->BusyList_Forward_Update_EN[1]&&(insn->PhyRs2==this->m_Rcu->BusyList_Forward_Update_PhyRd[1]))PhyRs2_done=true;
        else if(this->m_Rcu->BusyList_Forward_Update_EN[0]&&(insn->PhyRs2==this->m_Rcu->BusyList_Forward_Update_PhyRd[0]))PhyRs2_done=true;
        else PhyRs2_done=this->m_Rcu->m_IntBusylist[insn->PhyRs2].done;

        if(((!PhyRs1_done) ||(!PhyRs2_done)
        )){
            info.insn->data_valid=false;                 
        }else{
                if(!this->m_RFReadPortVec[0]->valid)
                for(auto fu : this->m_FuncUnitVec){
                    if(!Success_find&&fu->m_SupportFunc.count(insn->Fu)){
                        info.insn->data_valid=true;
                        info.desIndex  = fu->m_FuncUnitId;
                        info.isToFu    = true;
                        Success_find   = true;
                    }
                }
                
            
        }
        
        if(!this->m_RFReadPortVec[0]->valid)    this->m_RFReadPortVec[0]->set(info);//向rfport端口发送数据 
        
    };

    void Deallocate(uint64_t& index){
        this->m_issueQueue.Pop();
    }; 

};

} // namespace Emulator




#endif