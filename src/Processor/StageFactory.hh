#ifndef __STAGEFACTORY_HH__
#define __STAGEFACTORY_HH__ 

#include "Pipeline/BaseStage.hh"
#include <yaml-cpp/yaml.h>
#include <map>
namespace Emulator
{

namespace StageFactory
{

using createfunc = std::function<std::shared_ptr<BaseStage>(Processor*, const YAML::Node&)>;

std::shared_ptr<BaseStage> 
CreateStageByName(const std::string Name, Processor* p, const YAML::Node& config);
 
const InsnState_t
LookupEnumByName(std::string Name);

} // namespace StageFactory

}

#endif	