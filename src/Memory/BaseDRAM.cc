#include "BaseDRAM.hh"
namespace Emulator
{
    
BaseDRAM::BaseDRAM(const uint64_t base , const uint64_t length)
: TraceObject("DRAM")
{
    this->m_base = base; this->m_length = length;
    this->m_ram  = new char[length];
}
BaseDRAM::~BaseDRAM(){
    delete[] this->m_ram;
}

void 
BaseDRAM::read(uint64_t address, char** data, uint64_t len){
    DASSERT(((address >= m_base) && (address <= m_base+m_length)),"Access Unknown Address : {:#x}",address);
    *data = m_ram + (address - m_base);
}   

bool
BaseDRAM::checkRange(uint64_t address){
    return (address >= m_base) && (address <= m_base+m_length);
}

uint64_t 
BaseDRAM::readDouble(uint64_t address){
    DASSERT(((address >= m_base) && (address <= m_base+m_length)),"Access Unknown Address : {:#x}",address);
    return *(uint64_t*)(m_ram + (address - m_base)); 
}   

char 
BaseDRAM::readByte(uint64_t address){
    DASSERT(((address >= m_base) && (address <= m_base+m_length)),"Access Unknown Address : {:#x}",address);
    return *(m_ram + (address - m_base)); 
}   

void 
BaseDRAM::write(uint64_t address,const char* data, const uint64_t len){
    DASSERT(((address >= m_base) && (address <= m_base+m_length)),"Access Unknown Address : {:#x}",address);
    std::copy(data,data+len,m_ram + address - m_base);
}

void 
BaseDRAM::writeDouble(uint64_t address,const uint64_t data){
    DASSERT(((address >= m_base) && (address <= m_base+m_length)),"Access Unknown Address : {:#x}",address);
    std::copy(&data,&data+8, m_ram+address-m_base);
}

void 
BaseDRAM::write(uint64_t address,const char* data, const uint64_t len, const uint64_t mask){
    DASSERT(((address >= m_base) && (address <= m_base+m_length)),"Access Unknown Address : {:#x}",address);
    for(size_t i = 0; i < len; i++){
        if((mask >> i) & 1){
            m_ram[address-m_base+i] = data[i];
        }
    }
}


const uint64_t& 
BaseDRAM::size(){
    return this->m_length;
}


} // namespace Emulator
