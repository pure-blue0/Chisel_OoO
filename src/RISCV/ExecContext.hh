#ifndef __EXECCONTEXT_HH__
#define __EXECCONTEXT_HH__


#include "../Common/Common.hh"
#include "../Trace/TraceObject.hh"
#include "Encoding.hh"
#include <vector>
#include <map>
#include <bitset>
namespace RISCV
{


struct ProcState_t{
    uint64_t cycle;
    uint64_t instret;
    RISCV::Priv_level_t priv;
};

class ExecContext : public Emulator::Trace::TraceObject
{
private:

    struct Csr_t{
        std::string         Name;
        Emulator::xlen_t    Value;
    };

    std::vector<Emulator::xlen_t>         m_IntIsaRegFile; 

    std::map<uint16_t,Csr_t>              m_CsrRegFile;


public:

    ExecContext(/* args */);

    ~ExecContext();

    Priv_level_t getPriv();

    void CycleInc();

    void InstretInc();

    void Reset(Emulator::ThreadId tid, Emulator::Addr_t BootAddress);

    void WriteCsr(uint16_t CsrAddr, Emulator::xlen_t Data);

    void ReadCsr(uint16_t CsrAddr, Emulator::xlen_t& Data);

    void WriteIntReg(uint16_t index, Emulator::xlen_t Data);

    void ReadIntReg(uint16_t index, Emulator::xlen_t& Data);

};



    
} // namespace RISCV



#endif