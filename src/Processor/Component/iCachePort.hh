#ifndef __ICACHEPORT_HH__
#define __ICACHEPORT_HH__ 

#include "../Pipeline/Pipe_data.hh"
#include "../../Common/Common.hh"
#include "../../Memory/BaseDRAM.hh"



namespace Emulator
{
class iCachePort : public Trace::TraceObject
{
private:

    BaseDRAM*              m_baseDRAM;

    TimeBuffer<std::pair<std::function<void(MemResp_t)>,MemResp_t>>  m_iCacheRespLatch;
    TimeBuffer<MemResp_t>* m_resp = new TimeBuffer<MemResp_t>("m_resp", 1);
public:
    
    iCachePort();
    
    iCachePort(uint64_t Latency , BaseDRAM* dram);
    
    ~iCachePort();

    void ReceiveFetchReq(MemReq_t mem_req,bool SendSuccess);

    void Reset();

    void Evaluate(MemResp_t& resp,bool& valid);

    void Advance();

};


} // namespace Emulator




#endif	