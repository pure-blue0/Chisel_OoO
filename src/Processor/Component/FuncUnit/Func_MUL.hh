#ifndef __FUNC_MUL_HH__
#define __FUNC_MUL_HH__

#include "BaseFuncUnit.hh"

namespace Emulator
{

class Func_MUL : public BaseFuncUnit
{
private:

public:
    
    Func_MUL(        
        const uint16_t FuncUnitId,
        
        const std::string Name, 
        const uint64_t Latency, 
        const bool Pipelined
    ):  BaseFuncUnit(Name,FuncUnitId,{funcType_t::MUL},Latency,Pipelined)
    {};

    ~Func_MUL(){};

    void Compute(InsnPtr_t& insn){
        switch (insn->SubOp)
        {
        case MUL_MUL    :
            insn->RdResult  = (int64_t)insn->Operand1 * (int64_t)insn->Operand2;
            break;
        case MUL_MULH   :
            insn->RdResult  = ((__int128_t)((int64_t)insn->Operand1) * (__int128_t)((int64_t)insn->Operand2)) >> 64;
            break;
        case MUL_MULHSU :
            insn->RdResult  = ((__int128_t)((int64_t)insn->Operand1) * (__int128_t)((uint64_t)insn->Operand2)) >> 64;
            break;
        case MUL_MULHU  :
            insn->RdResult  = ((__uint128_t)insn->Operand1 * (__uint128_t)insn->Operand2) >> 64;
            break;
        case MUL_MULW   :
            insn->RdResult  = (int32_t)insn->Operand1 * (int32_t)insn->Operand2;
            break;
        default:
            break;
        }
    };

};


    
} // namespace Emulator



#endif