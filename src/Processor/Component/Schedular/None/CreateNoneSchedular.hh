#ifndef __CREATENONESCHEDULAR_HH__
#define __CREATENONESCHEDULAR_HH__

#include "N_InAgeOrder.hh"
#include "N_InOrder.hh"
#include "N_OutOfOrder.hh"

namespace Emulator
{
    
std::shared_ptr<BaseSchedular>
CreateNoneSchedular(
        const IssuePolicy_t  IssuePolicy,
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu);


} // namespace Emulator






#endif