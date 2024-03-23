#ifndef __FUNCUNITFACTORY_HH__
#define __FUNCUNITFACTORY_HH__

#include "BaseFuncUnit.hh"
#include "../../Processor.hh"
namespace Emulator
{
namespace FuncUnitFactory
{
    

std::shared_ptr<BaseFuncUnit> 
CreateFuByName(
    Processor*      processor,

    const std::string Name,        

    const uint16_t FuncUnitId,
    const std::string FuncUnitName, 
    const uint64_t Latency, 
    const bool Pipelined
);

 









} // namespace FuncUnitFactory
} // namespace Emulator










#endif