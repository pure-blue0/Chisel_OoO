#ifndef __COMMON_HH__
#define __COMMON_HH__

#include <iostream>
#include <stdint.h>
#include <string>
#include <stdint.h>
#include "../Trace/Logging.hh"
#include "../Timing/Tick.hh"
#include "../Timing/TimeBuffer.hh"
#include <yaml-cpp/yaml.h>


namespace Emulator
{
    
typedef uint16_t ThreadId;

typedef uint64_t Addr_t;

typedef uint32_t Insn_t;

typedef uint8_t  IsaRegId_t;

typedef uint16_t PhyRegId_t;

typedef int32_t  Imm_t;

typedef uint64_t xlen_t;


typedef uint64_t xReg_t;


} // namespace Emulator

#endif	