#ifndef __WRITEBACKPORT_HH__
#define __WRITEBACKPORT_HH__

#include "../../Timing/TimeBuffer.hh"
#include "../../Trace/TraceObject.hh"
#include "../Pipeline/Pipe_data.hh"

namespace Emulator
{

class Processor;
class WriteBackPort : public Trace::TraceObject
{
private:

    Processor*              m_Processor;

    TimeBuffer<InsnPtr_t>   m_CalcuPipe;

    std::vector<uint16_t>   m_forwardingSchedularIdVec;

public:

    WriteBackPort(Processor* processor, const uint16_t WbPortid , const uint64_t Latency);

    ~WriteBackPort();

    void Reset();

    void Flush();

    void Evaluate(Redirect_t& RedirectReq, bool& needRedirect);

    void Advance();

    void Forwarding();

    void AddForwardingSchedularId(uint16_t SchedularId);



    std::shared_ptr<TimeBuffer<InsnPtr_t>::Port> GetInPort();

};

std::shared_ptr<WriteBackPort>
CreateWriteBackPort(Processor* processor, const uint16_t WbPortid , const uint64_t Latency);


    
} // namespace Emulator






#endif