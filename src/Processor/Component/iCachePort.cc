#include "iCachePort.hh"

namespace Emulator
{

iCachePort::iCachePort(uint64_t Latency, BaseDRAM* dram)
    : TraceObject("iCachePort"),
      m_iCacheRespLatch("iCache",Latency),
      m_baseDRAM(dram)
{}

iCachePort::~iCachePort(){}

void 
iCachePort::ReceiveFetchReq(MemReq_t mem_req,bool SendSuccess){
    MemResp_t resp;
    resp.Id         = mem_req.Id;
    resp.Address    = mem_req.Address;
    resp.Opcode     = mem_req.Opcode;
    resp.Length     = mem_req.Length;
    resp.Data       = NULL;
    resp.Excp.valid = false;
    resp.Excp.Cause = 0;
    resp.Excp.Tval  = 0;
    if(SendSuccess)this->m_baseDRAM->read(mem_req.Address,&resp.Data,mem_req.Length);
    //从内存响应的数据会输入到icache的输入端口（虽然这里并没有具体的icache），等到下一拍开始时会通过outport口处理这些数据
    if(SendSuccess)this->m_resp->InPort->set(resp);
    DPRINTF(ICacheReq,"address {:#x}",mem_req.Address);
}

void 
iCachePort::Reset(){
    this->m_iCacheRespLatch.reset();
}   

void 
iCachePort::Evaluate(MemResp_t& resp,bool& valid){

    if(this->m_resp->OutPort->valid){
        resp=this->m_resp->OutPort->data;
    }
    valid=this->m_resp->OutPort->valid;
}

void 
iCachePort::Advance(){
    this->m_iCacheRespLatch.advance();
    this->m_resp->advance();
}
    
} // namespace Emulator
