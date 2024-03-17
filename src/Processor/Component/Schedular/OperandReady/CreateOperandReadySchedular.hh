#ifndef __CREATEOPERANDREADYSCHEDULAR_HH__
#define __CREATEOPERANDREADYSCHEDULAR_HH__

#include "OR_InAgeOrder.hh"
#include "OR_InOrder.hh"
#include "OR_OutOfOrder.hh"

namespace Emulator
{
    
std::shared_ptr<BaseSchedular>
CreateOperandReadySchedular(
        const IssuePolicy_t  IssuePolicy,
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu);


} // namespace Emulator






#endif