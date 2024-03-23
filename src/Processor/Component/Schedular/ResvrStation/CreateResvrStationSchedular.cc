#include "CreateResvrStationSchedular.hh"

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
        std::shared_ptr<Rcu> rcu)
{

    switch (IssuePolicy)
    {
    case IssuePolicy_t::InOrder :
        return std::make_shared<RS_InOrder>(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu);
        break;
    case IssuePolicy_t::InAgeOrder:
        return std::make_shared<RS_InAgeOrder>(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu);
        break;
    case IssuePolicy_t::OutofOrder:
        return std::make_shared<RS_OutOfOrder>(Name,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu);
        break;
    default:
        SPDLOG_ERROR("Unknown IssuePolicy Type {} !",IssuePolicy);
        exit(-1);
    }


}


} // namespace Emulator





