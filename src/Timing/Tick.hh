#ifndef __TICK_HH__
#define __TICK_HH__ 

#include <stdint.h>

namespace Emulator
{

namespace Clock{

uint64_t CurTick();

extern void Tick();

}

} // namespace Emulator




#endif	