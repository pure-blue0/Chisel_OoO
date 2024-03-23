#ifndef __FUNC_DIV_HH__
#define __FUNC_DIV_HH__

#include "BaseFuncUnit.hh"

namespace Emulator
{

class Func_DIV : public BaseFuncUnit
{
private:

public:
    
    Func_DIV(        
        const uint16_t FuncUnitId,
        
        const std::string Name, 
        const uint64_t Latency, 
        const bool Pipelined
    ):  BaseFuncUnit(Name,FuncUnitId,{funcType_t::DIV},Latency,Pipelined)
    {};

    ~Func_DIV(){};

    void Compute(InsnPtr_t& insn){
        switch (insn->SubOp)
        {
        case DIV_DIV   :
            if((int64_t)insn->Operand2 == 0){
                insn->RdResult = -1;
            }else if(((int64_t)insn->Operand1 == (uint64_t)1 << 63) && (int64_t)insn->Operand2 == -1){
                insn->RdResult = insn->Operand1;
            }else{
                insn->RdResult = (int64_t)insn->Operand1 / (int64_t)insn->Operand2;
            }
            break; 
        case DIV_DIVU  :
            if((uint64_t)insn->Operand2 == 0){
                insn->RdResult = (uint64_t)-1;
            }else{
                insn->RdResult = (uint64_t)insn->Operand1 / (uint64_t)insn->Operand2;
            }
            break; 
        case DIV_REM   :
            if((int64_t)insn->Operand2 == 0){
                insn->RdResult = (int64_t)insn->Operand1;
            }else if(((int64_t)insn->Operand1 == (uint64_t)1 << 63) && (int64_t)insn->Operand2 == -1){
                insn->RdResult = 0;
            }else{
                insn->RdResult = (int64_t)insn->Operand1 % (int64_t)insn->Operand2;
            }
            break; 
        case DIV_REMU  :
            if((uint64_t)insn->Operand2 == 0){
                insn->RdResult = (uint64_t)insn->Operand1;
            }else{
                insn->RdResult = (uint64_t)insn->Operand1 % (uint64_t)insn->Operand2;
            }
            break; 
        case DIV_DIVW  :
            if((int32_t)insn->Operand2 == 0){
                insn->RdResult = -1;
            }else if(((int32_t)insn->Operand1 == (uint32_t)1 << 31) && (int32_t)insn->Operand2 == -1){
                insn->RdResult = (int32_t)insn->Operand1;
            }else{
                insn->RdResult = (int32_t)insn->Operand1 / (int32_t)insn->Operand2;
            }
            break; 
        case DIV_DIVUW :
            if((uint32_t)insn->Operand2 == 0){
                insn->RdResult = -1;
            }else{
                insn->RdResult = (int32_t)((uint32_t)insn->Operand1 / (uint32_t)insn->Operand2);
            }
            break; 
        case DIV_REMW  :
            if((int32_t)insn->Operand2 == 0){
                insn->RdResult = (int32_t)insn->Operand1;
            }else if(((int32_t)insn->Operand1 == (uint32_t)1 << 31) && (int32_t)insn->Operand2 == -1){
                insn->RdResult = 0;
            }else{
                insn->RdResult = (int32_t)insn->Operand1 % (int32_t)insn->Operand2;
            }
            break; 
        case DIV_REMUW :
            if((uint32_t)insn->Operand2 == 0){
                insn->RdResult = (int32_t)insn->Operand1;
            }else{
                insn->RdResult = (int32_t)((uint32_t)insn->Operand1 % (uint32_t)insn->Operand2);
            }
            break; 
        default:
            break;
        }
    };

};


    
} // namespace Emulator



#endif