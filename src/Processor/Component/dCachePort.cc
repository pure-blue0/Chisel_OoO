#include "dCachePort.hh"

namespace Emulator
{

dCachePort::dCachePort(uint64_t Latency, BaseDRAM* dram)
    : TraceObject("dCachePort"),
      m_dCacheRespLatch("dCache",Latency),
      m_baseDRAM(dram)
{}

dCachePort::~dCachePort(){}

void 
dCachePort::ReceiveMemReq(MemReq_t mem_req,std::function<void(MemResp_t)> CallBackFunc){
    MemResp_t resp;
    resp.Id         = mem_req.Id;
    resp.Address    = mem_req.Address;
    resp.Opcode     = mem_req.Opcode;
    resp.Length     = mem_req.Length;
    if(mem_req.Opcode == MemOp_t::Load){
        resp.Data       = NULL;
        if(this->m_baseDRAM->checkRange(mem_req.Address)){
            resp.Excp.valid = false;
            resp.Excp.Cause = 0;
            resp.Excp.Tval  = 0;
            this->m_baseDRAM->read(mem_req.Address,&resp.Data,mem_req.Length);
        }else{
            resp.Excp.valid = true;
            resp.Excp.Cause = RISCV::LD_ACCESS_FAULT;
            resp.Excp.Tval  = mem_req.Address;
        }
        this->m_dCacheRespLatch.InPort->set({CallBackFunc,resp});
    }else if(mem_req.Opcode == MemOp_t::Store){
        if(this->m_baseDRAM->checkRange(mem_req.Address)){
            resp.Excp.valid = false;
            resp.Excp.Cause = 0;
            resp.Excp.Tval  = 0;
            this->m_baseDRAM->write(mem_req.Address,mem_req.Data,mem_req.Length,mem_req.ByteMask);
        }else{
            resp.Excp.valid = true;
            resp.Excp.Cause = RISCV::ST_ACCESS_FAULT;
            resp.Excp.Tval  = mem_req.Address;
        }
        this->m_dCacheRespLatch.InPort->set({CallBackFunc,resp});
        
    }
    DPRINTF(DCacheReq,"address {:#x}",mem_req.Address);
}

void 
dCachePort::Reset(){
    this->m_dCacheRespLatch.reset();
}   

void 
dCachePort::Evaluate(){
    if(this->m_dCacheRespLatch.OutPort->valid){
        this->m_dCacheRespLatch.OutPort->data.first(this->m_dCacheRespLatch.OutPort->data.second);
    }
}

void 
dCachePort::Advance(){
    this->m_dCacheRespLatch.advance();
}
    
} // namespace Emulator
