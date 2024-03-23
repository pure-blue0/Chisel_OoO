#ifndef __FUNC_ALU_HH__
#define __FUNC_ALU_HH__

#include "BaseFuncUnit.hh"

namespace Emulator
{

class Func_ALU : public BaseFuncUnit
{
private:

public:
    
    Func_ALU(        
        const uint16_t FuncUnitId,
        
        const std::string Name, 
        const uint64_t Latency, 
        const bool Pipelined
    ):  BaseFuncUnit(Name,FuncUnitId,{funcType_t::ALU},Latency,Pipelined)
    {};

    ~Func_ALU(){};

    void Compute(InsnPtr_t& insn){
        switch (insn->SubOp)
        {
        case ALU_ADD  :
            insn->RdResult = (int64_t)insn->Operand1 + (int64_t)insn->Operand2;
            break;       
        case ALU_SLL  :
            insn->RdResult = (uint64_t)insn->Operand1 << (uint64_t)insn->Operand2;
            break;
        case ALU_SUB  :
            insn->RdResult = (int64_t)insn->Operand1 - (int64_t)insn->Operand2;
            break;
        case ALU_SLT  :
            insn->RdResult = (int64_t)insn->Operand1 < (int64_t)insn->Operand2;
            break;
        case ALU_SLTU :          
            insn->RdResult = (uint64_t)insn->Operand1 < (uint64_t)insn->Operand2;
            break;
        case ALU_XOR  :
            insn->RdResult = (uint64_t)insn->Operand1 ^ (uint64_t)insn->Operand2;
            break;
        case ALU_SRL  :
            insn->RdResult = (uint64_t)insn->Operand1 >> (uint64_t)insn->Operand2;
            break;
        case ALU_SRA  :
            insn->RdResult = (int64_t)insn->Operand1 >> (uint64_t)insn->Operand2;
            break;
        case ALU_OR   :
            insn->RdResult = (uint64_t)insn->Operand1 | (uint64_t)insn->Operand2;
            break;
        case ALU_AND  :
            insn->RdResult = (uint64_t)insn->Operand1 & (uint64_t)insn->Operand2;
            break;
        case ALU_ADDW :
            insn->RdResult = (int32_t)insn->Operand1 + (int32_t)insn->Operand2;
            break;
        case ALU_SUBW :
            insn->RdResult = (int32_t)insn->Operand1 - (int32_t)insn->Operand2;
            break;
        case ALU_SLLW :
            insn->RdResult = (int32_t)insn->Operand1 << (uint32_t)insn->Operand2;
            break;
        case ALU_SRLW :
            insn->RdResult = (int32_t)((uint32_t)insn->Operand1 >> (uint32_t)insn->Operand2);
            break;
        case ALU_SRAW :
            insn->RdResult = (int32_t)insn->Operand1 >> (uint32_t)insn->Operand2;
            break;
        default:
            break;
        }
    };

};


    
} // namespace Emulator



#endif