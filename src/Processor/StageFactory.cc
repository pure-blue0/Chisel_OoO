#include "StageFactory.hh"
#include "Pipeline/Fetch1.hh"
#include "Pipeline/Fetch2.hh"
#include "Pipeline/Decode.hh"
#include "Pipeline/Dispatch.hh"
#include "Pipeline/IEW.hh"
#include "Pipeline/Commit.hh"
namespace Emulator
{

namespace StageFactory{

const std::map<std::string,createfunc> m_createFuncMap = {
    {"Fetch1",Create_Fetch1_Instance},
    {"Fetch2",Create_Fetch2_Instance},
    {"Decode",Create_Decode_Instance},
    {"Dispatch",Create_Dispatch_Instance},
    {"IEW",Create_IEW_Instance},
    {"Commit",Create_Commit_Instance}
};

const std::map<std::string,InsnState_t> m_StageEnumMap = {
    {"Fetch1"   ,State_Fetch1},
    {"Fetch2"   ,State_Fetch2},
    {"Decode"   ,State_Decode},
    {"Dispatch" ,State_Dispatch},
    {"IEW"      ,State_WriteBack},
    {"Commit"   ,State_Commit}
};

std::shared_ptr<BaseStage> 
CreateStageByName(const std::string Name, Processor* p, const YAML::Node& config){
    std::map<std::string,createfunc>::const_iterator it = m_createFuncMap.find(Name);
    if(it != m_createFuncMap.end()){
        return it->second(p,config);
    }else{
        SPDLOG_ERROR("Unknown Stage {} !",Name);
        exit(-1);
    }
}

const InsnState_t
LookupEnumByName(std::string Name){
    std::map<std::string,InsnState_t>::const_iterator it = m_StageEnumMap.find(Name);
    if(it != m_StageEnumMap.end()){
        return it->second;
    }else{
        SPDLOG_ERROR("Unknown Stage {} !",Name);
        exit(-1);
    }
}

}
} // namespace Emulator




