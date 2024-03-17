#ifndef __FUNC_CSR_HH__
#define __FUNC_CSR_HH__

#include "BaseFuncUnit.hh"
#include "../../../RISCV/riscv.hh"
namespace Emulator
{

class Func_CSR : public BaseFuncUnit
{
private:

    std::shared_ptr<RISCV::ExecContext> m_ExecContext;

public:
    
    Func_CSR(        
        std::shared_ptr<RISCV::ExecContext> execContext,
        const uint16_t FuncUnitId,
        const std::string Name, 
        const uint64_t Latency, 
        const bool Pipelined
    ):  BaseFuncUnit(Name,FuncUnitId,{funcType_t::CSR},Latency,Pipelined),
        m_ExecContext(execContext)
    {};

    ~Func_CSR(){};


    void Compute(InsnPtr_t& insn){
        switch (insn->SubOp)
        {
        case CSR_CSRRW  :
            this->m_ExecContext->ReadCsr(insn->imm,insn->RdResult);
            this->m_ExecContext->WriteCsr(insn->imm,insn->Operand1);
            break;
        case CSR_CSRRS  :
            this->m_ExecContext->ReadCsr(insn->imm,insn->RdResult);
            this->m_ExecContext->WriteCsr(insn->imm,(uint64_t)insn->Operand1 | (uint64_t)insn->RdResult);
            break;
        case CSR_CSRRC  :
            this->m_ExecContext->ReadCsr(insn->imm,insn->RdResult);
            this->m_ExecContext->WriteCsr(insn->imm,(~(uint64_t)insn->Operand1) & (uint64_t)insn->RdResult);
            break;
        case CSR_CSRR   :
            this->m_ExecContext->ReadCsr(insn->imm,insn->RdResult);
            break;
        case CSR_FENCEI :

            break;
        default:
            break;
        }
    };

};


    
} // namespace Emulator



#endif