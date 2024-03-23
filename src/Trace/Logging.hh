#ifndef __LOGGING_HH__
#define __LOGGING_HH__ 

#include <string>
#include <memory>
#include <iostream>
#include "spdlog/spdlog.h"
#include "../Timing/Tick.hh"
#include "DebugFlags.hh"
#include "TraceObject.hh"
#include <set>

inline const std::string Name()
{
    return std::string("global");
};

namespace Emulator
{
namespace Trace
{

template<typename ...Args>
inline void TracePrint(uint64_t time, const std::string Name, const std::string Flag,
                        fmt::v8::format_string<Args...> fmt, Args&&... args)
{   
    if(!getFlagEnable(Flag) && !getObjectEnable(Name)){
        return ;
    }
    std::string info = fmt::format("Cycle {:>5}, {:^15} ({:^25}) {}",time,Flag,Name,fmt);
    spdlog::get("Model")->trace(info,std::forward<Args>(args)...);
}
template<typename ...Args>
inline void ErrorPrint(uint64_t time, const std::string Name,
                        fmt::v8::format_string<Args...> fmt,  Args&&... args)
{
    std::string info = fmt::format("Cycle {:>5}, ({:^25}) {}",time,Name,fmt);
    spdlog::get("Model")->error(info,std::forward<Args>(args)...);
}
#ifdef TRACE_ON
#define DPRINTF(flag, ...) do {  \
    Emulator::Trace::TracePrint(Emulator::Clock::CurTick(),Name(),#flag,__VA_ARGS__); \
} while(0); 

#define DPRINT(...) do {  \
    Emulator::Trace::TracePrint(Emulator::Clock::CurTick(),Name(),"Global",__VA_ARGS__); \
} while(0);

#define DERROR(...) do{ \
    Emulator::Trace::ErrorPrint(Emulator::Clock::CurTick(),Name(),__VA_ARGS__); \
} while(0);

#define DASSERT(cond,...) do{ \
    if(!cond) {Emulator::Trace::ErrorPrint(Emulator::Clock::CurTick(),Name(),__VA_ARGS__);throw "Assert Failed";} \
} while(0);

#else
#define DPRINTF(flag, ...) 
#define DPRINT(...) 
#define DERROR(...)
#define DASSERT(cond, ...)
#endif

void initLogger(const bool debug, const bool console, const std::string filePath = "");

} // namespace Trace
} // namespace Emulator



#endif	