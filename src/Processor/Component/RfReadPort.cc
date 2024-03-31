#include "RfReadPort.hh"
#include "../Pipeline/Pipe_data.hh"

#include "../Processor.hh"
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

void
RfReadPort::Evaluate(){
    if(this->m_CalcuPipe.OutPort->valid){
        IssueInfo& t    = this->m_CalcuPipe.OutPort->data;
        InsnPtr_t& insn = t.insn;
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
