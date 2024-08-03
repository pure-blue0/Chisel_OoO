#include "RfReadPort.hh"
#include "../Pipeline/Pipe_data.hh"
#include "../Processor.hh"
//#include "./obj_dir/VRfReadPort.h"
//#include "./obj_dir/VRfReadPort.h"
namespace Emulator
{


RfReadPort::RfReadPort(
    Processor* processor, 
    const uint16_t RfPortid,
    const uint64_t Latency
):  TraceObject(fmt::format("RfReadPort[{}]",RfPortid)),
    m_Processor(processor),
    m_CalcuPipe(fmt::format("RfReadPort[{}]_CalCuPipe",RfPortid),Latency)
{}

RfReadPort::~RfReadPort(){}

void
RfReadPort::Reset(){
    this->m_CalcuPipe.reset();
}

void
RfReadPort::Flush(){
    this->m_CalcuPipe.flush();
}
// VRfReadPort *IsOlder;
        // IsOlder=new VIsOlder;//创建对象
        // //连接输入
        // IsOlder->io_tag1=tag1;
        // IsOlder->io_tag2=tag2;
        // IsOlder->io_header=header;
        // //连接输出
        // IsOlder->eval();
        // return IsOlder->io_older;
        // delete IsOlder;//删除创建的对象

// void RfReadPort::Evaluate(){
//     InsnPtr_t insn;
//     uint32_t index;
//     bool         Operand1Ready;
//     bool         Operand2Ready;
//     xReg_t       Operand1;
//     xReg_t       Operand2;
//     xReg_t       phyRs1_data;
//     xReg_t       phyRs2_data;
//     uint8_t      SubOp;
//     funcType_t   Fu;
//     Imm_t        imm;
//     Addr_t       Pc;
//     Prediction_t Pred;
//     uint64_t     RobTag;
//     uint64_t     LSQTag;
//     //Wrapping 由于OutPort->data中可能没有数据，因此如果直接将里面的insn赋值给module会报错
//     //因此需要在这里做一个包装，如果端口内没数据，就传输0.
//     if(this->m_CalcuPipe.OutPort->valid)
//     {
//         IssueInfo& data_in   = this->m_CalcuPipe.OutPort->data;
//         insn = data_in.insn;
//         index=data_in.desIndex;
//         Operand1Ready=insn->Operand1Ready;
//         phyRs1_data=this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRs1];
//         Operand2Ready=insn->Operand2Ready;
//         phyRs2_data=this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRs2];
//         SubOp=insn->SubOp;
//         imm=insn->imm;
//         Pc=insn->Pc;
//         Pred.Taken=insn->Pred.Taken;
//         Pred.Target=insn->Pred.Target;
//         Fu=insn->Fu;
//         Operand1=insn->Operand1;
//         Operand2=insn->Operand2;
//         RobTag=insn->RobTag;
//         LSQTag=insn->LSQTag;
//     }else{
//         index=0;
//         Operand1Ready=0;
//         phyRs1_data=0;
//         Operand2Ready=0;
//         phyRs2_data=0;
//         SubOp=0;
//         imm=0;
//         Pc=0;
//         Pred.Taken=0;
//         Pred.Target=0;
//         Fu=funcType_t(0);
//         Operand1=0;
//         Operand2=0;
//         RobTag=0;
//         LSQTag=0;
//     }
//     VRfReadPort *RfReadPort;
//     RfReadPort=new VRfReadPort;//创建对象
    
//     //连接输入
//     RfReadPort->io_Valid=this->m_CalcuPipe.OutPort->valid;
//     RfReadPort->io_Insn_Operand1Ready=Operand1Ready;
//     RfReadPort->io_Insn_PhyRs1_data=phyRs1_data;
//     RfReadPort->io_Insn_Operand2Ready=Operand2Ready;
//     RfReadPort->io_Insn_PhyRs2_data=phyRs2_data;
//     RfReadPort->io_desIndex_in=index;
//     RfReadPort->io_Sub_OP_in=SubOp;
//     RfReadPort->io_imm_in=imm;
//     RfReadPort->io_pc_in=Pc;
//     RfReadPort->io_Pred_taken_in=Pred.Taken;
//     RfReadPort->io_pred_target_in=Pred.Target;
//     RfReadPort->io_Function_type_in=Fu;
//     RfReadPort->io_Insn_operand1_in=Operand1;
//     RfReadPort->io_Insn_operand2_in=Operand2;
//     RfReadPort->io_RobTag_in=RobTag;
//     RfReadPort->io_LSQTag_in=LSQTag;
    
//     RfReadPort->eval();//实现操作数的读取以及数据的传递
//     //模拟Fun Unit接收数据包的操作
//     if(RfReadPort->io_Out_valid){
//         insn->SubOp = RfReadPort->io_Sub_OP_out;
//         insn->imm = RfReadPort->io_imm_out;
//         insn->Pc = RfReadPort->io_pc_out;
//         insn->Pred.Taken = RfReadPort->io_Pred_taken_out;
//         insn->Pred.Target = RfReadPort->io_pred_target_out;
//         insn->Fu = funcType_t(RfReadPort->io_Function_type_out);
//         insn->Operand1 = RfReadPort->io_Insn_operand1;
//         insn->Operand2 = RfReadPort->io_Insn_operand2;
//         insn->RobTag = RfReadPort->io_RobTag_out;
//         insn->LSQTag = RfReadPort->io_LSQTag_out;
//         this->m_Processor->m_FuncUnitVec[RfReadPort->io_desIndex_out]->Issue(insn);//根据匹配的function模块的id，将数据发送给对应的function unit
//     }
//     delete RfReadPort;//删除创建的对象
// }

void
RfReadPort::Evaluate(){
    if(this->m_CalcuPipe.OutPort->valid){
        
        IssueInfo& t    = this->m_CalcuPipe.OutPort->data;
        InsnPtr_t& insn = t.insn;
        
        if(t.insn->data_valid){
            if(!insn->Operand1Ready){ 
                insn->Operand1 =  this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRs1];
                //insn->Operand1Ready = true;
                DPRINTF(ReadOperand,"RobTag[{}],Pc[{:#x}] -> Get Operand1[Rs1[{}],PRs1[{}]]({:#x})",            
                    insn->RobTag,insn->Pc,insn->IsaRs1,insn->PhyRs1,insn->Operand1
                );
            
            }
            if(!insn->Operand2Ready)
            {
                    insn->Operand2 =  this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRs2];
                    //insn->Operand2Ready = true;
                    DPRINTF(ReadOperand,"RobTag[{}],Pc[{:#x}] -> Get Operand2[Rs2[{}],PRs2[{}]]({:#x})",            
                        insn->RobTag,insn->Pc,insn->IsaRs1,insn->PhyRs1,insn->Operand1
                    );
            }
            this->m_Processor->m_FuncUnitVec[t.desIndex]->Issue(insn);//根据匹配的function模块的id，将数据发送给对应的function unit
        }
    }
}


void
RfReadPort::Advance(){
    this->m_CalcuPipe.advance();
}

std::shared_ptr<TimeBuffer<IssueInfo>::Port> 
RfReadPort::GetInPort(){
    return this->m_CalcuPipe.InPort;
}


std::shared_ptr<RfReadPort>
CreateRfReadPort(Processor* processor, const uint16_t RfPortid , const uint64_t Latency){
    return std::make_shared<RfReadPort>(processor,RfPortid,Latency);
}




    
} // namespace Emulator
