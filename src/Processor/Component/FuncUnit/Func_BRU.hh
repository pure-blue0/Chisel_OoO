#ifndef __FUNC_BRU_HH__
#define __FUNC_BRU_HH__

#include "BaseFuncUnit.hh"
// #include "./obj_dir/Vbranch.h"
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

    // void Compute(InsnPtr_t& insn){
    //     Vbranch *branch;
    //     branch=new Vbranch;
    //     branch->io_sub_OP_out=insn->SubOp;
    //     branch->io_insn_operand1=insn->Operand1;
    //     branch->io_insn_operand2=insn->Operand2;
    //     branch->io_imm=insn->imm;
    //     branch->io_pc=insn->Pc;
    //     branch->io_pred_taken=insn->Pred.Taken;
    //     branch->io_pred_target=insn->Pred.Target;
    //     branch->eval(insn->RdResult,insn->BruTarget,insn->BruMisPred);
        
    // };

    void Compute(InsnPtr_t& insn){
        // Vbranch *branch;
        // branch=new Vbranch;
        // branch->io_sub_OP_out=insn->SubOp;
        // branch->io_insn_operand1=insn->Operand1;
        // branch->io_insn_operand2=insn->Operand2;
        // branch->io_imm=insn->imm;
        // branch->io_pc=insn->Pc;
        // branch->io_pred_taken=insn->Pred.Taken;
        // branch->io_pred_target=insn->Pred.Target;
        // branch->eval(insn->RdResult,insn->BruTarget,insn->BruMisPred);
        // //输出verilog模块的输入输出信号
        // DPRINTF(temptest,"Verilog:SubOp {:#x} OP1 {:#x} OP2 {:#x} imm {:#x} RD {:#x} BT {:#x} BMISS {:#x} ",
        //         branch->io_sub_OP_out,branch->io_insn_operand1,branch->io_insn_operand2,branch->io_imm,
        //         insn->RdResult,insn->BruTarget,insn->BruMisPred);
        // //记录verilog的输入输出信号
        // uint64_t a=insn->RdResult;
        // uint64_t b=insn->BruTarget;
        // bool c=insn->BruMisPred;
        //原来的compute代码
        bool taken;
        insn->RdResult = (uint64_t)insn->Pc +  4;
        switch (insn->SubOp)
        {
        case BRU_JAR  :taken = true;break;
        case BRU_JALR :taken = true;break;
        case BRU_BEQ  :taken = (int64_t)insn->Operand1 == (int64_t)insn->Operand2;break;
        case BRU_BNE  :taken = (int64_t)insn->Operand1 != (int64_t)insn->Operand2;break;
        case BRU_BLT  :taken = (int64_t)insn->Operand1 < (int64_t)insn->Operand2;break;
        case BRU_BGE  :taken = (int64_t)insn->Operand1 >= (int64_t)insn->Operand2;break;
        case BRU_BLTU :taken = (uint64_t)insn->Operand1 < (uint64_t)insn->Operand2;break;
        case BRU_BGEU :taken = (uint64_t)insn->Operand1 >= (uint64_t)insn->Operand2;break;         
        default:taken = false;break;
        }
        if(insn->SubOp == BRU_JALR)insn->BruTarget = (int64_t)insn->Operand1 + (int64_t)insn->imm;
        else insn->BruTarget = (uint64_t)insn->Pc + (taken ? (int64_t)insn->imm :  4);
        insn->BruMisPred = (taken != insn->Pred.Taken) || (insn->BruTarget != insn->Pred.Target);
        // //输出原来的输如输出信号
        // DPRINTF(temptest,"Origin: SubOp {:#x} OP1 {:#x} OP2 {:#x} imm {:#x} RD {:#x} BT {:#x} BMISS {:#x} ",
        //         insn->SubOp,insn->Operand1,insn->Operand2,insn->imm,
        //         insn->RdResult,insn->BruTarget,insn->BruMisPred);
        // //对比两者的输出，如果出现不一致则退出程序，并输出不同的数据
        // if(a!=insn->RdResult||b!=insn->BruTarget||c!=insn->BruMisPred){
        //     DPRINTF(temptest,"ERROR:module RD{:#x} origin RD {:#x} module MT{:#x} origin MT {:#x} module MISS{:#x} origin MISS {:#x}",
        //     a,insn->RdResult,b,insn->BruTarget,c,insn->BruMisPred);
        //     exit(1);
        // }
    };
    
};


    
} // namespace Emulator



#endif