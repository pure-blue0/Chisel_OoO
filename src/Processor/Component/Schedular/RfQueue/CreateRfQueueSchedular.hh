
#ifndef __CREATERFQUEUESCHEDULAR_HH__
#define __CREATERFQUEUESCHEDULAR_HH__

#include "RFQ_InAgeOrder.hh"
#include "RFQ_InOrder.hh"
#include "RFQ_OutOfOrder.hh"

namespace Emulator
{
    
std::shared_ptr<BaseSchedular>
CreateRfQueueSchedular(
        const IssuePolicy_t  IssuePolicy,
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu);


} // namespace Emulator






#endif