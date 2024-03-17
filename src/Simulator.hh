#ifndef __SIMULATOR_HH__
#define __SIMULATOR_HH__ 

#include "Processor/Processor.hh"
#include "Memory/BaseDRAM.hh"
#include "Common/Common.hh"
namespace Emulator
{
    
class Simulator : public Trace::TraceObject
{
private:

    std::vector<Processor*>     m_threads;

    BaseDRAM*                   m_dram;

public:
    Simulator(const YAML::Node& Config);

    ~Simulator();

    BaseDRAM*  GetDRAM();

    Processor* Create_Processor(const YAML::Node& Config, const ThreadId tid);

    void Tick();

    void Reset(Addr_t boot_address);

};

} // namespace Emulator



#endif	