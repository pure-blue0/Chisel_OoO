#ifndef __SCHEDULARFACTORY_HH__
#define __SCHEDULARFACTORY_HH__


#include <memory>
#include "RfQueue/CreateRfQueueSchedular.hh"
#include "ResvrStation/CreateResvrStationSchedular.hh"
#include "OperandReady/CreateOperandReadySchedular.hh"
#include "None/CreateNoneSchedular.hh"
#include "Commit/CreateCommitSchedular.hh"

namespace Emulator
{

namespace SchedularFactory{

using createfunc = std::function<std::shared_ptr<BaseSchedular>(
    const IssuePolicy_t  ,
    const std::string    , 
    const uint16_t       ,
    const uint64_t       ,
    const uint64_t       ,
    const uint64_t       ,
    std::shared_ptr<Rcu> 
    )>;

std::shared_ptr<BaseSchedular> 
CreateSchedularByName(     
    const std::string    IssuePolicy,
    const std::string    WakeUpPolicy,

    const std::string    SchedularName,
    const uint16_t       SchedularId,
    const uint64_t       EntryCount,
    const uint64_t       EnqueWidth,
    const uint64_t       DequeWidth,
    std::shared_ptr<Rcu> rcu
);

}    
} // namespace Emulator





#endif