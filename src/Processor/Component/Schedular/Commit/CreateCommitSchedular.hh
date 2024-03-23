
#ifndef __CREATECOMMITSCHEDULAR_HH__
#define __CREATECOMMITSCHEDULAR_HH__

#include "CMT_InAgeOrder.hh"
#include "CMT_InOrder.hh"
#include "CMT_OutOfOrder.hh"

namespace Emulator
{
    
std::shared_ptr<BaseSchedular>
CreateCommitSchedular(
        const IssuePolicy_t  IssuePolicy,
        const std::string    Name, 
        const uint16_t       SchedularId,
        const uint64_t       EntryCount,
        const uint64_t       EnqueWidth,
        const uint64_t       DequeWidth,
        std::shared_ptr<Rcu> rcu);


} // namespace Emulator






#endif