#ifndef __REGISTERFILE_HH__
#define __REGISTERFILE_HH__

#include "../../Common/Common.hh"
#include "../../Trace/Logging.hh"

#include <vector>

namespace Emulator
{
    

template <typename T, bool ZeroReg>
class RegisterFile : public Trace::TraceObject
{
private:

    std::vector<T> m_Regs;

public:

    RegisterFile(std::string Name, PhyRegId_t phyRegCount)
    :   TraceObject(Name)
    {
        this->m_Regs.resize(phyRegCount);
    };

    ~RegisterFile(){};

    T& operator[](PhyRegId_t index){
        DASSERT((index < this->m_Regs.size()), "index[{}] > {}",this->m_Regs.size());
        return this->m_Regs[index];
    };

    void Reset(){
        for( auto reg : this->m_Regs){
            reg = 0;
        }
    };

};



} // namespace Emulator








#endif