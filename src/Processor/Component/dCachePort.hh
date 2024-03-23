#ifndef __DCACHEPORT_HH__
#define __DCACHEPORT_HH__ 

#include "../Pipeline/Pipe_data.hh"
#include "../../Common/Common.hh"
#include "../../Memory/BaseDRAM.hh"



namespace Emulator
{
class dCachePort : public Trace::TraceObject
{
private:

    BaseDRAM*              m_baseDRAM;

    TimeBuffer<std::pair<std::function<void(MemResp_t)>,MemResp_t>>  m_dCacheRespLatch;
    
public:
    
    dCachePort(uint64_t Latency , BaseDRAM* dram);
    
    ~dCachePort();

    void ReceiveMemReq(MemReq_t mem_req, std::function<void(MemResp_t)> CallBackfunc);

    void Reset();

    void Evaluate();

    void Advance();

};


} // namespace Emulator




#endif	