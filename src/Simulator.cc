#include "Simulator.hh"
#include "Timing/Tick.hh"
namespace Emulator
{

Simulator::Simulator(const YAML::Node& Config)
:   TraceObject("Simulator")
{
    const YAML::Node DRAMCfg = Config["DRAM"];
    this->m_dram = new BaseDRAM(DRAMCfg["MemoryMap"]["Base"].as<uint64_t>(),
                          DRAMCfg["MemoryMap"]["Length"].as<uint64_t>());

    const YAML::Node Processor_Vec = Config["Processors"];
    for(size_t i = 0; i < Processor_Vec.size();i++){
        this->m_threads.emplace_back(this->Create_Processor(Processor_Vec[i],i));
    }
}

Simulator::~Simulator(){
    for(size_t i = 0; i < this->m_threads.size(); i++){
        delete this->m_threads[i];
    }
    delete this->m_dram;
}


Processor*
Simulator::Create_Processor(const YAML::Node& Config, const ThreadId tid){
    return new Processor(Config,this->m_dram,tid);
}

void 
Simulator::Tick(){
    Emulator::Clock::Tick();
    for(size_t i = 0; i < this->m_threads.size(); i++){
        this->m_threads[i]->Tick();
    }
}


void 
Simulator::Reset(Addr_t boot_address){
    for(size_t i = 0; i < this->m_threads.size(); i++){
        this->m_threads[i]->Reset(boot_address);
    }
}

BaseDRAM* 
Simulator::GetDRAM(){
    return this->m_dram;
}

}