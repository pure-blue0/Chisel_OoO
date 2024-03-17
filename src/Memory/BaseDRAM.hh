#ifndef __BASEDRAM_HH__
#define __BASEDRAM_HH__ 

#include <stdint.h>
#include <iostream>
#include <cstring>
#include "../Trace/Logging.hh"

namespace Emulator
{
    
class BaseDRAM : public Trace::TraceObject
{
private:
    uint64_t             m_base;
    uint64_t             m_length;
    char*                m_ram;
public:
    BaseDRAM();
    BaseDRAM(const uint64_t base , const uint64_t length);
    ~BaseDRAM();
    const uint64_t& size();
    bool checkRange(uint64_t address);
    void read(uint64_t address,  char** data, uint64_t len);
    uint64_t readDouble(uint64_t address);
    char readByte(uint64_t address);
    void write(uint64_t address, const char* data, const uint64_t len);
    void writeDouble(uint64_t address,const uint64_t data);
    void write(uint64_t address,const char* data, const uint64_t len, const uint64_t mask);
};

} // namespace Emulator





#endif	