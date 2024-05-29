#include "Dispatch.hh"
#include "../Processor.hh"
#include <math.h>
//#include "./obj_dir/VTryAllocate.h"
#include "./obj_dir/VDispatch.h"
namespace Emulator
{
    
Dispatch::Dispatch(
    Processor* processor
):  BaseStage(processor,"Dispatch")
{
}

Dispatch::~Dispatch(){}


void 
Dispatch::AddSchedular(std::shared_ptr<BaseSchedular> Schedular){
    Disp_Schedular_t disp_scheduler_element;
    disp_scheduler_element.scheduler = Schedular;
    disp_scheduler_element.AvailPort = Schedular->m_EnqueWidth;
    this->m_SchedularVec.emplace_back(disp_scheduler_element);
}

void 
Dispatch::Reset(){}

void 
Dispatch::Evaluate(){

    std::shared_ptr<Rcu> rcu  = this->m_Processor->getRcuPtr();
    std::shared_ptr<Lsq> lsq  = this->m_Processor->getLsqPtr();
    InsnPkg_t& insnPkg        = this->m_StageInPort->data;

    uint64_t TryAllocSuccessCount[3];

    StageAck_t  Ack = {0};

    DASSERT((insnPkg.size()<=rcu->m_AllocWidth),
        "Dispatch[{}] & RCU Allocate[{}] Width MisMatch!",insnPkg.size(),rcu->m_AllocWidth
    );

    if(this->m_StageInPort->valid){
        if(rcu->m_RobState != rob_state_t::Rob_Idle){
            this->m_StageInPort->stall();
        }else{
                rcu->TryAllocate(insnPkg,TryAllocSuccessCount[0]);
                lsq->TryAllocate(insnPkg,TryAllocSuccessCount[1]);
                this->TryDispatch(insnPkg,TryAllocSuccessCount[2],true);
                Ack.takenInsnCount = *std::min_element(TryAllocSuccessCount,TryAllocSuccessCount+3);
                lsq->Allocate(insnPkg,Ack.takenInsnCount);
                rcu->Allocate(insnPkg,Ack.takenInsnCount);
                this->DispatchInsn(insnPkg,Ack.takenInsnCount);

                this->m_StageAckOutPort->set(Ack);
            
            #ifdef TRACE_ON
            std::stringstream insnInfo;
            if(Ack.takenInsnCount){
                for(size_t i = 0; i < Ack.takenInsnCount; i++){
                    auto t = this->m_StageInPort->data[i];
                    insnInfo << fmt::format("\n\tInsn_{:02} -> Pc({:#x}) , Insn({:#>08x})",i, t->Pc, t->UncompressedInsn);
                }
                DPRINTF(SendReq,insnInfo.str());
            }
            #endif
        }
    }
}
// void 
// Dispatch::TryDispatch(InsnPkg_t& insnPkg, uint64_t& SuccessCount, bool CheckControlFlow){
//     VTryAllocate *TryAllocate=new VTryAllocate;//创建对象
//     //连接输入
//     TryAllocate->io_insn_num=insnPkg.size();
//     TryAllocate->io_Scheduler0_AvailPort=this->m_SchedularVec[0].scheduler->GetAvailibleEntryCount();
//     TryAllocate->io_Scheduler1_AvailPort=this->m_SchedularVec[1].scheduler->GetAvailibleEntryCount();
//     TryAllocate->io_Scheduler2_AvailPort=this->m_SchedularVec[2].scheduler->GetAvailibleEntryCount();
//     TryAllocate->io_Insn1_excp_vaild=insnPkg[0]->Excp.valid;
//     TryAllocate->io_Insn2_excp_vaild=insnPkg[1]->Excp.valid;
//     TryAllocate->io_insn1_Function_type=insnPkg[0]->Fu;
//     TryAllocate->io_insn2_Function_type=insnPkg[1]->Fu;
//     TryAllocate->io_insn1_rd=insnPkg[0]->IsaRd;
//     TryAllocate->io_insn2_rd=insnPkg[1]->IsaRd;
//     TryAllocate->io_Insn1_controlflow_vaild=insnPkg[0]->ControlFlowInsn;
//     //连接输出
//     TryAllocate->eval();
//     SuccessCount=TryAllocate->io_issue_success_count;
//     delete TryAllocate;//删除创建的对象
// }
void 
Dispatch::TryDispatch(InsnPkg_t& insnPkg, uint64_t& SuccessCount, bool CheckControlFlow){
    // VTryAllocate *TryAllocate;
    // TryAllocate=new VTryAllocate;//创建对象
    // //连接输入
    // TryAllocate->io_insn_num=insnPkg.size();
    // TryAllocate->io_Scheduler0_AvailPort=this->m_SchedularVec[0].scheduler->GetAvailibleEntryCount();
    // TryAllocate->io_Scheduler1_AvailPort=this->m_SchedularVec[1].scheduler->GetAvailibleEntryCount();
    // TryAllocate->io_Scheduler2_AvailPort=this->m_SchedularVec[2].scheduler->GetAvailibleEntryCount();
    // TryAllocate->io_Insn1_excp_vaild=insnPkg[0]->Excp.valid;
    // TryAllocate->io_Insn2_excp_vaild=insnPkg[1]->Excp.valid;
    // TryAllocate->io_insn1_Function_type=insnPkg[0]->Fu;
    // TryAllocate->io_insn2_Function_type=insnPkg[1]->Fu;
    // TryAllocate->io_insn1_rd=insnPkg[0]->IsaRd;
    // TryAllocate->io_insn2_rd=insnPkg[1]->IsaRd;
    // TryAllocate->io_Insn1_controlflow_vaild=insnPkg[0]->ControlFlowInsn;
    // //连接输出
    // TryAllocate->eval();
    // uint64_t a=TryAllocate->io_issue_success_count;

    // Flush Status
    for(auto& scheduler : this->m_SchedularVec){//获取每个调度器的可用，EnqueWidth：表示的是一个周期内最大的允许指令入队的数量
        scheduler.AvailPort = std::min(scheduler.scheduler->GetAvailibleEntryCount(),scheduler.scheduler->m_EnqueWidth);
    }//获取每个队列可用entry数
    SuccessCount = 0;
    bool stop_flag=0;
    for(int i=0;i<insnPkg.size()&&!stop_flag;i++){
        InsnPtr_t insn = insnPkg[i];
        if(insn->Excp.valid){//判断指令是否为异常指令
            SuccessCount++;
            stop_flag=1;
        }
        if((insn->Fu == funcType_t::ALU && insn->IsaRd == 0)){ // 判断该指令是否为nop指令
            SuccessCount++;  
        }
        else{
            bool Success=false;
            for(auto& schedular : this->m_SchedularVec){//遍历调度器列表
                //找到一个支持当前指令功能类型（Fu），并且该调度器有可用的端口
                //if(schedular.scheduler->m_SupportFunc.count(insn->Fu)&& !schedular.scheduler->Busy() && schedular.AvailPort)//加上busy判断，更好的性能
                if(schedular.scheduler->m_SupportFunc.count(insn->Fu) && schedular.AvailPort)
                {
                    SuccessCount++;
                    schedular.AvailPort--;//可用端口数-1
                    Success=true;
                    break;
                }
            }
            //如果指令没有匹配到ISSUE队列或指令是控制流指令，则不再对后续指令进行判断
            if(!Success || insn->ControlFlowInsn){
                stop_flag=1;
            }
        }
    }
    // if(SuccessCount!=a){
    //     DPRINTF(temptest,"error-------------------------");
    //     DPRINTF(temptest,"V:I1 num {:#x} port {:#x} {:#x} {:#x} valid {:#x} {:#x} fu {:#x} {:#x} rd {:#x} {:#x} C {:#x} out {:#x}",
    //         TryAllocate->io_insn_num,TryAllocate->io_Scheduler0_AvailPort,TryAllocate->io_Scheduler1_AvailPort,TryAllocate->io_Scheduler2_AvailPort,
    //         TryAllocate->io_Insn1_excp_vaild,TryAllocate->io_Insn2_excp_vaild,TryAllocate->io_insn1_Function_type,
    //         TryAllocate->io_insn2_Function_type,TryAllocate->io_insn1_rd,TryAllocate->io_insn2_rd, TryAllocate->io_Insn1_controlflow_vaild,a);
    //     DPRINTF(temptest,"O:I1 num {:#x} port {:#x} {:#x} {:#x} valid {:#x} {:#x} fu {:#x} {:#x} rd {:#x} {:#x}  out {:#x}",
    //         insnPkg.size(),this->m_SchedularVec[0].scheduler->GetAvailibleEntryCount(),this->m_SchedularVec[1].scheduler->GetAvailibleEntryCount(),
    //         this->m_SchedularVec[2].scheduler->GetAvailibleEntryCount(),insnPkg[0]->Excp.valid,insnPkg[1]->Excp.valid,
    //         insnPkg[0]->Fu,insnPkg[1]->Fu,insnPkg[0]->IsaRd,insnPkg[1]->IsaRd,insnPkg[0]->ControlFlowInsn,SuccessCount);
    //         exit(1);
    // }
}
void 
Dispatch::DispatchInsn(InsnPkg_t& insnPkg, uint64_t DispatchCount){
    //对输入进行拼接，在verilog中，insn1和insn2内得到的数据都是从decode queue中取出来的
    //而取出来的数据是一个128位的，为了模拟实际的效果，因此在这里对数据进行拼接，来模拟decode queue的数据
    __int128 insn1=(__int128)insnPkg[0]->Excp.Tval<<96|(__int128)insnPkg[0]->Excp.Cause<<90|(__int128)insnPkg[0]->Excp.valid<<89|
                   (__int128)(uint32_t)insnPkg[0]->imm<<57|(__int128)insnPkg[0]->SubOp<<53|(__int128)insnPkg[0]->Fu<<50|(__int128)insnPkg[0]->ControlFlowInsn<<49|
                   (__int128)insnPkg[0]->IsaRd<<44|(__int128)insnPkg[0]->Operand2Ready<<43|(__int128)insnPkg[0]->IsaRs2<<38|(__int128)insnPkg[0]->Operand1Ready<<37|
                   (__int128)insnPkg[0]->IsaRs1<<32|(__int128)insnPkg[0]->Pc;
    __int128 insn2=(__int128)insnPkg[1]->Excp.Tval<<96|(__int128)insnPkg[1]->Excp.Cause<<90|(__int128)insnPkg[1]->Excp.valid<<89|
                   (__int128)(uint32_t)insnPkg[1]->imm<<57|(__int128)insnPkg[1]->SubOp<<53|(__int128)insnPkg[1]->Fu<<50|(__int128)insnPkg[1]->ControlFlowInsn<<49|
                   (__int128)insnPkg[1]->IsaRd<<44|(__int128)insnPkg[1]->Operand2Ready<<43|(__int128)insnPkg[1]->IsaRs2<<38|(__int128)insnPkg[1]->Operand1Ready<<37|
                   (__int128)insnPkg[1]->IsaRs1<<32|(__int128)insnPkg[1]->Pc;
                 
    VDispatch *Dispatch=new VDispatch;//创建对象
    //连接输入，由于insn1的位数超过32位，因此这里需要采用数组的方式进行赋值，每个元素是32位
    //在verilog中，LSQtag--LPhyRd这些数据都是在Rename Dispatch阶段被赋值的，其余的数据是在Decode阶段被赋值并放入Decode Queue的，因此像下面这样赋值
    Dispatch->io_DispatchCount=DispatchCount;
    Dispatch->io_insn1[0]=(uint32_t)insn1;
    Dispatch->io_insn1[1]=(uint32_t)(insn1>>32);
    Dispatch->io_insn1[2]=(uint32_t)(insn1>>64);
    Dispatch->io_insn1[3]=(uint32_t)(insn1>>96);
    Dispatch->io_Insn1_LSQTag_in=insnPkg[0]->LSQTag;
    Dispatch->io_Insn1_RobTag_in=insnPkg[0]->RobTag;
    Dispatch->io_insn1_PhyRs1_in=insnPkg[0]->PhyRs1;
    Dispatch->io_insn1_PhyRs2_in=insnPkg[0]->PhyRs2;
    Dispatch->io_insn1_PhyRd_in=insnPkg[0]->PhyRd;
    Dispatch->io_insn1_LPhyRd_in=insnPkg[0]->LPhyRd;

    Dispatch->io_insn2[0]=(uint32_t)insn2;
    Dispatch->io_insn2[1]=(uint32_t)(insn2>>32);
    Dispatch->io_insn2[2]=(uint32_t)(insn2>>64);
    Dispatch->io_insn2[3]=(uint32_t)(insn2>>96);
    Dispatch->io_Insn2_LSQTag_in=insnPkg[1]->LSQTag;
    Dispatch->io_Insn2_RobTag_in=insnPkg[1]->RobTag;
    Dispatch->io_insn2_PhyRs1_in=insnPkg[1]->PhyRs1;
    Dispatch->io_insn2_PhyRs2_in=insnPkg[1]->PhyRs2;
    Dispatch->io_insn2_PhyRd_in=insnPkg[1]->PhyRd;
    Dispatch->io_insn2_LPhyRd_in=insnPkg[1]->LPhyRd;
    
    Dispatch->eval();//计算需要分配到哪个队列，并将输入的数据传输到输出
    //连接输出
    uint16_t num1=Dispatch->io_Issue_num1;
    uint16_t num2=Dispatch->io_Issue_num2;
    //下面的操作是模拟ISSUE Queue接收到数据后的操作，当num！=3，表示有ISSUE Queue匹配到数据
    //因此需要先对输入的128位数据进行拆解，同时将LSQtag--LPhyRd接收
    if(num1!=3){
        __int128 insn1_out=(__int128)Dispatch->io_insn1_Out[3]<<96|(__int128)Dispatch->io_insn1_Out[2]<<64|
                           (__int128)Dispatch->io_insn1_Out[1]<<32|(__int128)Dispatch->io_insn1_Out[0];
        insnPkg[0]->Excp.Tval  = insn1_out>>96;
        insnPkg[0]->Excp.Cause = insn1_out>>90&0X3F;
        insnPkg[0]->Excp.valid = insn1_out>>89&0X1;
        insnPkg[0]->imm = insn1_out>>57&0XFFFFFFFF;  
        insnPkg[0]->SubOp = insn1_out>>53&0XF;  
        insnPkg[0]->Fu = funcType_t(insn1_out>>50&0X7);  
        insnPkg[0]->ControlFlowInsn = insn1_out>>49&0X1;  
        insnPkg[0]->IsaRd = insn1_out>>44&0X1F;  
        insnPkg[0]->Operand2Ready = insn1_out>>43&0X1;  
        insnPkg[0]->IsaRs2 = insn1_out>>38&0X1F;  
        insnPkg[0]->Operand1Ready = insn1_out>>37&0X1; 
        insnPkg[0]->IsaRs2 = insn1_out>>32&0X1F;   
        insnPkg[0]->Pc = insn1_out&0XFFFFFFFF; 
        insnPkg[0]->LSQTag=Dispatch->io_Insn1_LSQTag;
        insnPkg[0]->RobTag=Dispatch->io_Insn1_RobTag;
        insnPkg[0]->PhyRs1=Dispatch->io_insn1_PhyRs1;
        insnPkg[0]->PhyRs2=Dispatch->io_insn1_PhyRs2;
        insnPkg[0]->PhyRd=Dispatch->io_insn1_PhyRd;
        insnPkg[0]->LPhyRd=Dispatch->io_insn1_LPhyRd; 
    }
    if(num2!=3){
        __int128 insn2_out=(__int128)Dispatch->io_insn2_Out[3]<<96|(__int128)Dispatch->io_insn2_Out[2]<<64|
                           (__int128)Dispatch->io_insn2_Out[1]<<32|(__int128)Dispatch->io_insn2_Out[0];
        insnPkg[1]->Excp.Tval  = insn2_out>>96;
        insnPkg[1]->Excp.Cause = insn2_out>>90&0X3F;
        insnPkg[1]->Excp.valid = insn2_out>>89&0X1;
        insnPkg[1]->imm = insn2_out>>57&0XFFFFFFFF;  
        insnPkg[1]->SubOp = insn2_out>>53&0XF;  
        insnPkg[1]->Fu = funcType_t(insn2_out>>50&0X7);  
        insnPkg[1]->ControlFlowInsn = insn2_out>>49&0X1;  
        insnPkg[1]->IsaRd = insn2_out>>44&0X1F;  
        insnPkg[1]->Operand2Ready = insn2_out>>43&0X1;  
        insnPkg[1]->IsaRs2 = insn2_out>>38&0X1F;  
        insnPkg[1]->Operand1Ready = insn2_out>>37&0X1; 
        insnPkg[1]->IsaRs2 = insn2_out>>32&0X1F;   
        insnPkg[1]->Pc = insn2_out&0XFFFFFFFF; 
        insnPkg[1]->LSQTag=Dispatch->io_Insn2_LSQTag;
        insnPkg[1]->RobTag=Dispatch->io_Insn2_RobTag;
        insnPkg[1]->PhyRs1=Dispatch->io_insn2_PhyRs1;
        insnPkg[1]->PhyRs2=Dispatch->io_insn2_PhyRs2;
        insnPkg[1]->PhyRd=Dispatch->io_insn2_PhyRd;
        insnPkg[1]->LPhyRd=Dispatch->io_insn2_LPhyRd; 
    }
    //模拟ISSUE Queue的接收数据
    if(num1==0) this->m_SchedularVec[0].scheduler->Schedule(insnPkg[0],this->m_SchedularVec[0].scheduler->Allocate());
    else if(num1==1) this->m_SchedularVec[1].scheduler->Schedule(insnPkg[0],this->m_SchedularVec[1].scheduler->Allocate());
    else if(num1==2) this->m_SchedularVec[2].scheduler->Schedule(insnPkg[0],this->m_SchedularVec[2].scheduler->Allocate());

    if(num2==0) this->m_SchedularVec[0].scheduler->Schedule(insnPkg[1],this->m_SchedularVec[0].scheduler->Allocate());
    else if(num2==1) this->m_SchedularVec[1].scheduler->Schedule(insnPkg[1],this->m_SchedularVec[1].scheduler->Allocate());
    else if(num2==2) this->m_SchedularVec[2].scheduler->Schedule(insnPkg[1],this->m_SchedularVec[2].scheduler->Allocate());

}
// void 
// Dispatch::DispatchInsn(InsnPkg_t& insnPkg, uint64_t DispatchCount){
//     for(size_t i = 0 ; i < DispatchCount; i++){
//         InsnPtr_t insn = insnPkg[i];
//         if( !(insn->Excp.valid ||
//              ((insn->Fu == funcType_t::CSR) && (insn->SubOp == 9)) ||
//              ((insn->Fu == funcType_t::ALU) && (insn->IsaRd == 0)) ||
//              ((insn->Fu == funcType_t::CSR) && (insn->SubOp == 7)))//Check whether it is a FENCE/NOP/MRET command
//         ){
//             for(auto scheduler : this->m_SchedularVec){
//                 if(scheduler.scheduler->m_SupportFunc.count(insn->Fu) )
//                 {
                    
//                     scheduler.scheduler->Schedule(insn,scheduler.scheduler->Allocate());
//                     break;
//                 }
//             } 
//         }
//     }

// }

//     // for(size_t i = 0 ; i < DispatchCount; i++){
//     //     InsnPtr_t insn = insnPkg[i];
//     //     if(  insn->Excp.valid ||
//     //          ((insn->Fu == funcType_t::CSR) && (insn->SubOp == CSR_FENCE)) ||
//     //          ((insn->Fu == funcType_t::ALU) && (insn->IsaRd == 0)) ||
//     //          ((insn->Fu == funcType_t::CSR) && (insn->SubOp == CSR_MRET))
//     //     ){
//     //         ;//insn->State = InsnState_t::State_Commit;//直接提交
//     //     }else{
//     //         //insn->State = InsnState_t::State_Issue;
//     //         for(auto scheduler : this->m_SchedularVec){
//     //             if(scheduler.scheduler->m_SupportFunc.count(insn->Fu) )
//     //             {
                    
//     //                 scheduler.scheduler->Schedule(insn,scheduler.scheduler->Allocate());
//     //                 break;
//     //             }
//     //         } 
//     //     }
//     // }

std::shared_ptr<BaseStage> Create_Dispatch_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
){
    const YAML::Node& SchedularInfo = StageConfig["Schedulers"];
    auto t = std::make_shared<Dispatch>(processor);
    for(size_t i = 0 ; i < SchedularInfo.size(); i++){
        const uint16_t SchedularId = SchedularInfo[i].as<uint16_t>();
        t->AddSchedular(processor->m_SchedularVec[SchedularId]);
    }
    return t;
}



} // namespace Emualtor
