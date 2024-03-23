#include "ExecContext.hh"
#include "encoding.out.h"

namespace RISCV
{


ExecContext::ExecContext(/* args */)
    : TraceObject("RISCV_ExecContext"),
      m_IntIsaRegFile(32,0)
{}

ExecContext::~ExecContext(){}

void 
ExecContext::Reset(Emulator::ThreadId tid, Emulator::Addr_t BootAddress){
#define DECLARE_CSR(name,addr) this->m_CsrRegFile[addr] = {#name,0};
    #include "encoding.out.h"
#undef DECLARE_CSR
    this->WriteCsr(CSR_MHARTID,tid);
    this->WriteCsr(CSR_MEPC,BootAddress);
}

Priv_level_t
ExecContext::getPriv(){
    return Priv_level_t::PRIV_M;
}

void 
ExecContext::CycleInc(){
    this->m_CsrRegFile[CSR_MCYCLE].Value++;
}

void 
ExecContext::InstretInc(){
    this->m_CsrRegFile[CSR_MINSTRET].Value++;
}


void 
ExecContext::WriteCsr(uint16_t CsrAddr, Emulator::xlen_t Data){
    this->m_CsrRegFile[CsrAddr].Value = Data;
}

void 
ExecContext::ReadCsr(uint16_t CsrAddr, Emulator::xlen_t& Data){
    Data = this->m_CsrRegFile[CsrAddr].Value;
}

void 
ExecContext::WriteIntReg(uint16_t index, Emulator::xlen_t Data){
    this->m_IntIsaRegFile[index] = Data;
}

void 
ExecContext::ReadIntReg(uint16_t index, Emulator::xlen_t& Data){
    Data = this->m_IntIsaRegFile[index];
}

    
} // namespace RISCV
