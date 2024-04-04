#include "WriteBackPort.hh"
#include "../Processor.hh"

namespace Emulator
{


WriteBackPort::WriteBackPort(Processor* processor, const uint16_t WbPortid , const uint64_t Latency)
:   TraceObject(fmt::format("WbPort[{}]",WbPortid)),
    m_Processor(processor),
    m_CalcuPipe(fmt::format("WbPort[{}]_CalcuPipe",WbPortid),Latency)
{

}

WriteBackPort::~WriteBackPort(){}

void 
WriteBackPort::Reset(){
    this->m_CalcuPipe.reset();
}

void 
WriteBackPort::Flush(){
    this->m_CalcuPipe.flush();
}

void 
WriteBackPort::Evaluate(Redirect_t& RedirectReq, bool& needRedirect){
    if(this->m_CalcuPipe.OutPort->valid){
        InsnPtr_t& insn = this->m_CalcuPipe.OutPort->data;
        insn->State = InsnState_t::State_Commit;
        this->m_Processor->getRcuPtr()->WriteBack(insn,needRedirect);
        RedirectReq.StageId = InsnState_t::State_Issue;
        RedirectReq.target  = insn->BruTarget;
        DPRINTF(WriteBack,"RobTag[{}],Pc[{:#x}]",insn->RobTag,insn->Pc);  
    }
}

void 
WriteBackPort::Advance(){
    this->m_CalcuPipe.advance();
}

void  WriteBackPort::Forwarding(){
    if(this->m_CalcuPipe.InPort->valid){
        InsnPtr_t& insn = this->m_CalcuPipe.InPort->data;
        if(insn->IsaRd != 0){
            this->m_Processor->getRcuPtr()->m_IntBusylist[insn->PhyRd].forwarding = true;
            this->m_Processor->getRcuPtr()->m_IntBusylist[insn->PhyRd].done = true;//说明当前指令的rd是被前递了的
            this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRd] = insn->RdResult;//把rd的数据直接写到对应的rd物理寄存器中，之后的读操作数可以直接从这里面读取
        }
    }
}

void
WriteBackPort::AddForwardingSchedularId(uint16_t SchedularId){
    this->m_forwardingSchedularIdVec.emplace_back(SchedularId);
}

std::shared_ptr<TimeBuffer<InsnPtr_t>::Port> 
WriteBackPort::GetInPort(){
    return this->m_CalcuPipe.InPort;
}

std::shared_ptr<WriteBackPort>
CreateWriteBackPort(Processor* processor, const uint16_t WbPortid , const uint64_t Latency){
    return std::make_shared<WriteBackPort>(processor,WbPortid,Latency);
}

} // namespace Emulator





