#include "FuncUnitFactory.hh"

#include "Func_ALU.hh"
#include "Func_CSR.hh"
#include "Func_MUL.hh"
#include "Func_DIV.hh"
#include "Func_BRU.hh"
#include "Func_LDU.hh"
#include "Func_LSU.hh"

namespace Emulator
{
namespace FuncUnitFactory
{


std::shared_ptr<BaseFuncUnit> 
CreateFuByName(
    Processor*        processor,
    const std::string Name,        

    const uint16_t FuncUnitId,
    const std::string FuncUnitName, 
    const uint64_t Latency, 
    const bool Pipelined
){

    if(Name == "ALU"){
        return std::make_shared<Func_ALU>(FuncUnitId,FuncUnitName,Latency,Pipelined);
    }else if(Name == "MUL"){
        return std::make_shared<Func_MUL>(FuncUnitId,FuncUnitName,Latency,Pipelined);
    }else if(Name == "DIV"){
        return std::make_shared<Func_DIV>(FuncUnitId,FuncUnitName,Latency,Pipelined);
    }else if(Name == "CSR"){
        return std::make_shared<Func_CSR>(processor->m_ExecContext,FuncUnitId,FuncUnitName,Latency,Pipelined);
    }else if(Name == "LSU"){
        return std::make_shared<Func_LSU>(processor->getRcuPtr(),processor->getLsqPtr(),processor->getDramPtr(),
                processor->m_dCacheLatency,FuncUnitId,FuncUnitName,Latency,Pipelined);
    }else if(Name == "LDU"){
        return std::make_shared<Func_LDU>(processor->getRcuPtr(),processor->getLsqPtr(),processor->getDramPtr(),
                processor->m_dCacheLatency,FuncUnitId,FuncUnitName,Latency,Pipelined);
    }else if(Name == "BRU"){
        return std::make_shared<Func_BRU>(FuncUnitId,FuncUnitName,Latency,Pipelined);
    }else{
        SPDLOG_ERROR("Unknown Function Unit {} !", Name);
        exit(-1);
    }

    return nullptr;
}
    
} // namespace FuncUnitFactory


    
} // namespace Emulator
