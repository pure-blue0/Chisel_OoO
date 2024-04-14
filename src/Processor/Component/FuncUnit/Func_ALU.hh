#ifndef __FUNC_ALU_HH__
#define __FUNC_ALU_HH__

#include "BaseFuncUnit.hh"
#include <verilated.h>          // Defines common routines
#include "./obj_dir/Valu.h"
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
        Valu *alu;
        alu=new Valu;//创建对象
        //连接输入
        alu->io_Insn_operand1=insn->Operand1;
        alu->io_Insn_operand2=insn->Operand2;
        alu->io_Sub_OP_out=insn->SubOp;
        //连接输出
        alu->eval(insn->RdResult);

        delete alu;//删除创建的对象
    };
    // void Compute(InsnPtr_t& insn){
    //     Valu *alu;
    //     alu=new Valu;//创建对象
    //     //连接输入
    //     alu->io_Insn_operand1=insn->Operand1;
    //     alu->io_Insn_operand2=insn->Operand2;
    //     alu->io_Sub_OP_out=insn->SubOp;
    //     //连接输出
    //     alu->eval(insn->RdResult);
    //     //保存交叉验证的计算结果，用于之后对比
    //     uint64_t a=insn->RdResult;
    //     //输出交叉验证的计算结果
    //     DPRINTF(temptest,"Origin:SubOp{:#x} operand1 {:#x}operand2 {:#x} insn->RdResult {:#x}",
    //             alu->io_Sub_OP_out,alu->io_Insn_operand1,alu->io_Insn_operand2,insn->RdResult);
        
    //     switch (insn->SubOp)
    //     {
    //     case ALU_ADD  :insn->RdResult = (int64_t)insn->Operand1 + (int64_t)insn->Operand2;
    //         break;       
    //     case ALU_SLL  :insn->RdResult = (uint64_t)insn->Operand1 << (uint64_t)insn->Operand2;
    //         break;
    //     case ALU_SUB  :insn->RdResult = (int64_t)insn->Operand1 - (int64_t)insn->Operand2;break;
    //     case ALU_SLT  :insn->RdResult = (int64_t)insn->Operand1 < (int64_t)insn->Operand2;break;
    //     case ALU_SLTU :insn->RdResult = (uint64_t)insn->Operand1 < (uint64_t)insn->Operand2;break;
    //     case ALU_XOR  :insn->RdResult = (uint64_t)insn->Operand1 ^ (uint64_t)insn->Operand2;break;
    //     case ALU_SRL  :insn->RdResult = (uint64_t)insn->Operand1 >> (uint64_t)insn->Operand2;break;
    //     case ALU_SRA  :insn->RdResult = (int64_t)insn->Operand1 >> (uint64_t)insn->Operand2;break;
    //     case ALU_OR   :insn->RdResult = (uint64_t)insn->Operand1 | (uint64_t)insn->Operand2;break;
    //     case ALU_AND  :insn->RdResult = (uint64_t)insn->Operand1 & (uint64_t)insn->Operand2;break;
    //     case ALU_ADDW :insn->RdResult = (int32_t)insn->Operand1 + (int32_t)insn->Operand2;break;
    //     case ALU_SUBW :insn->RdResult = (int32_t)insn->Operand1 - (int32_t)insn->Operand2;break;
    //     case ALU_SLLW :insn->RdResult = (int32_t)insn->Operand1 << (uint32_t)insn->Operand2;break;
    //     case ALU_SRLW :insn->RdResult = (int32_t)((uint32_t)insn->Operand1 >> (uint32_t)insn->Operand2);break;
    //     case ALU_SRAW :insn->RdResult = (int32_t)insn->Operand1 >> (uint32_t)insn->Operand2;break;
    //     default:insn->RdResult=0;break;
    //     }
    //     //输出原始的计算结果
    //     DPRINTF(temptest,"Origin:SubOp{:#x} operand1 {:#x}operand2 {:#x} insn->RdResult {:#x}",
    //         insn->SubOp,insn->Operand1,insn->Operand2,insn->RdResult);
    //     //如果有输出不一致，则直接退出程序
    //     if(a!=insn->RdResult)DPRINTF(temptest,"ERROR:module out{:#x} origin out {:#x}",a,insn->RdResult);
    //     delete alu;//删除创建的对象
    // };
       
};


    
} // namespace Emulator



#endif