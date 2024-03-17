#ifndef __FUNC_BRU_HH__
#define __FUNC_BRU_HH__

#include "BaseFuncUnit.hh"

namespace Emulator
{

class Func_BRU : public BaseFuncUnit
{
private:

public:
    
    Func_BRU(        
        const uint16_t FuncUnitId,
        
        const std::string Name, 
        const uint64_t Latency, 
        const bool Pipelined
    ):  BaseFuncUnit(Name,FuncUnitId,{funcType_t::BRU},Latency,Pipelined)
    {};

    ~Func_BRU(){};

    void Compute(InsnPtr_t& insn){
        bool taken;
        insn->RdResult = (uint64_t)insn->Pc + (insn->IsRvcInsn ? 2 : 4);
        switch (insn->SubOp)
        {
        case BRU_JAR  :
            taken = true;
            break;
        case BRU_JALR :
            taken = true;
            break;
        case BRU_BEQ  :
            taken = (int64_t)insn->Operand1 == (int64_t)insn->Operand2;
            break;
        case BRU_BNE  :
            taken = (int64_t)insn->Operand1 != (int64_t)insn->Operand2;
            break;
        case BRU_BLT  :
            taken = (int64_t)insn->Operand1 < (int64_t)insn->Operand2;
            break;
        case BRU_BGE  :
            taken = (int64_t)insn->Operand1 >= (int64_t)insn->Operand2;
            break;
        case BRU_BLTU :
            taken = (uint64_t)insn->Operand1 < (uint64_t)insn->Operand2;
            break;
        case BRU_BGEU :
            taken = (uint64_t)insn->Operand1 >= (uint64_t)insn->Operand2;
            break;         
        default:
            break;
        }
        if(insn->SubOp == BRU_JALR){
            insn->BruTarget = (int64_t)insn->Operand1 + (int64_t)insn->imm;
        }else{
            insn->BruTarget = (uint64_t)insn->Pc + (taken ? (int64_t)insn->imm : insn->IsRvcInsn ? 2 : 4);
        }

        insn->BruMisPred = (taken != insn->Pred.Taken) || (insn->BruTarget != insn->Pred.Target);
    };

};


    
} // namespace Emulator



#endif