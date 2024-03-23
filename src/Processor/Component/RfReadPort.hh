#ifndef __RFREADPORT_HH__
#define __RFREADPORT_HH__

#include "../../Trace/TraceObject.hh"
#include "../../Timing/TimeBuffer.hh"
#include "Schedular/BaseSchedular.hh"

namespace Emulator
{
    
class Processor;
class RfReadPort : public Trace::TraceObject
{
private:

    Processor*              m_Processor;

    TimeBuffer<IssueInfo>   m_CalcuPipe;

public:

    RfReadPort(Processor* processor, const uint16_t RfPortid , const uint64_t Latency);

    ~RfReadPort();

    void Reset();

    void Flush();

    void Evaluate();

    void Advance();

    std::shared_ptr<TimeBuffer<IssueInfo>::Port> GetInPort();

};


std::shared_ptr<RfReadPort>
CreateRfReadPort(Processor* processor, const uint16_t RfPortid , const uint64_t Latency);


} // namespace Emulator


#endif