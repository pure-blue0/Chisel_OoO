#include "Tick.hh"

namespace Emulator
{

namespace Clock{

static uint64_t m_CurTick = 0;

extern uint64_t CurTick(){
    return Emulator::Clock::m_CurTick;
};

extern void Tick(){
     Emulator::Clock::m_CurTick++;     
};

}

} // namespace Emulator
