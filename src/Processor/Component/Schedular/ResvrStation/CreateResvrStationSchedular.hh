
#ifndef __CREATERESVRSTATIONSCHEDULAR_HH__
#define __CREATERESVRSTATIONSCHEDULAR_HH__

#include "RS_InAgeOrder.hh"
#include "RS_InOrder.hh"
#include "RS_OutOfOrder.hh"

namespace Emulator
{
    
std::shared_ptr<BaseSchedular>
CreateResvrStationSchedular(
        const IssuePolicy_t  IssuePolicy,
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu);


} // namespace Emulator






#endif