#include "SchedularFactory.hh"

namespace Emulator
{
namespace SchedularFactory{

const std::map<std::string,IssuePolicy_t> m_issuePolicyMap = {
    {"In-Order",IssuePolicy_t::InOrder},
    {"Out-of-Order",IssuePolicy_t::OutofOrder},
    {"In-Age-Order",IssuePolicy_t::InAgeOrder}
};

const std::map<std::string,createfunc> m_createFuncMap = {
    {"None",           CreateNoneSchedular              },
    {"OperandReady",   CreateOperandReadySchedular      },
    {"ResvrStation",   CreateResvrStationSchedular      },
    {"RfQueue",        CreateRfQueueSchedular           },
    {"Commit",         CreateCommitSchedular            }
};

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
){
    const std::map<std::string,IssuePolicy_t>::const_iterator Ip_it = m_issuePolicyMap.find(IssuePolicy);
    if(Ip_it != m_issuePolicyMap.end()){
        const std::map<std::string,createfunc>::const_iterator Wp_it = m_createFuncMap.find(WakeUpPolicy);
        if(Wp_it != m_createFuncMap.end()){
            return Wp_it->second(Ip_it->second,SchedularName,SchedularId,EntryCount,EnqueWidth,DequeWidth,rcu);
        }else{
            SPDLOG_ERROR("Unknown WakeUp Policy {} !",WakeUpPolicy);
            exit(-1);
        }
    }else{
        SPDLOG_ERROR("Unknown Issue Policy {} !", IssuePolicy);
        exit(-1);
    }
    return nullptr;
}

}
}
    