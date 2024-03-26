#ifndef __DECODER_HH__
#define __DECODER_HH__ 

#include "../Processor/DynInsn.hh"
#include "../Common/Common.hh"
#include "Encoding.hh"
#include "StaticInsn.hh"
#include "rvcExpender.hh"
namespace RISCV
{

void Decode_Func(Emulator::DynInsn& insn){
    StaticInsn instr(insn.UncompressedInsn);

    insn.IsaRs1  = instr.rs1();//insn[19:15]
    insn.IsaRs2  = instr.rs2();//insn[24:20]
    insn.IsaRd   = instr.rd();//insn[11:7]

    insn.Operand1Ready = false;
    insn.Operand2Ready = false;
    insn.ControlFlowInsn = false;
    insn.imm     = 0;

    insn.Fu          = Emulator::funcType_t::ALU;
    insn.SubOp       = ALU_ADD;

    bool    illegal_instr = false;
    bool    ecall   = false;
    bool    ebreak  = false;
    switch (instr.opcode())////insn[6:0]
    {
    case 0b1110011:
        insn.Fu = Emulator::funcType_t::CSR;
        if(instr.func3() >> 2){
            insn.Operand1      = instr.rs1();
            insn.Operand1Ready = true;
        }
        insn.imm = instr.csr();
        switch (instr.func3() & 0b11)
        {
        case 0b00:
            insn.IsaRd=0;
            if(instr.rd() == 0 && instr.rs1() == 0){
                insn.ControlFlowInsn = true;
                switch (instr.iimm())
                {
                case 0b000000000000:
                    insn.SubOp = CSR_ECALL;
                    ecall = true;
                	break;
                case 0b000000000001:
                    insn.SubOp = CSR_EBREAK;
                    ebreak = true;
                	break;
                case 0b001100000010:
                    insn.SubOp = CSR_MRET;
                	break;
                default: illegal_instr = true;break;
                }
            }else illegal_instr = true;
        	break;
        case 0b01: insn.SubOp = CSR_CSRRW; break;
        case 0b10:
            if(instr.rs1() == 0)insn.SubOp = CSR_CSRR ;
            else insn.SubOp = CSR_CSRRS;
        	break;
        case 0b11:
            if(instr.rs1() == 0) insn.SubOp = CSR_CSRR ;
            else insn.SubOp = CSR_CSRRC;
        	break;
        default:illegal_instr = true; break;
        }
    	break;
    case 0b0001111:
        insn.Fu = Emulator::funcType_t::CSR;
        insn.imm = instr.iimm();
        insn.IsaRd=0;
        if(instr.rs1() == 0 && instr.rd() == 0){
            switch (instr.func3())
            {
            case 0b000:
                insn.Fu = Emulator::funcType_t::CSR;
                insn.SubOp = CSR_FENCE;
            	break;
            case 0b001:
                if(instr.iimm() != 0){
                    illegal_instr = true;
                    	break;
                }
                insn.SubOp = CSR_FENCEI;
                insn.ControlFlowInsn = true;
            	break;
            default: illegal_instr = true;break;
            }
        }else illegal_instr = true;
    	break;
    case 0b0000011:
        insn.Fu = Emulator::funcType_t::LDU;
        insn.imm = instr.iimm();
        switch (instr.func3())
        {
        case 0b000:insn.SubOp = LDU_LB;break;
        case 0b001:insn.SubOp = LDU_LH;break;
        case 0b010:insn.SubOp = LDU_LW;break;
        case 0b011:insn.SubOp = LDU_LD;break;
        case 0b100:insn.SubOp = LDU_LBU;break;
        case 0b101:insn.SubOp = LDU_LHU;break;
        case 0b110:insn.SubOp = LDU_LWU;break;
        default:illegal_instr = true;break;
        }
        break;
    case 0b0100011:
        insn.Fu = Emulator::funcType_t::STU;
        insn.imm = instr.simm();
        insn.IsaRd=0;
        switch (instr.func3())
        {
        case 0b000:insn.SubOp = STU_SB;break;
        case 0b001:insn.SubOp = STU_SH;break;
        case 0b010:insn.SubOp = STU_SW;break;
        case 0b011:insn.SubOp = STU_SD;break;
        default:illegal_instr = true;break;
        }
    	break;
    case 0b1100011:
        insn.Fu  = Emulator::funcType_t::BRU;
        insn.imm = instr.sbimm();
        insn.ControlFlowInsn = true;
        insn.IsaRd=0;
        switch (instr.func3())
        {
        case 0b000:insn.SubOp = BRU_BEQ;break;
        case 0b001:insn.SubOp = BRU_BNE;break;
        case 0b100:insn.SubOp = BRU_BLT;break;
        case 0b101:insn.SubOp = BRU_BGE;break;
        case 0b110:insn.SubOp = BRU_BLTU;break;
        case 0b111:insn.SubOp = BRU_BGEU;break;
        default:illegal_instr = true;break;
        }
    	break;
    case 0b1101111:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.SubOp = ALU_ADD;
        insn.Operand1 = insn.Pc;
        insn.Operand2 = 4;
        insn.Operand1Ready = true;
        insn.Operand2Ready = true;//转目标地址的计算不涉及任何寄存器的值，所以在解码阶段就可以直接确定这个值
    	break;
    case 0b1100111:
        insn.Fu  = Emulator::funcType_t::BRU;
        insn.SubOp = BRU_JALR;
        if(instr.func3() != 0)illegal_instr = true;
        insn.imm = instr.iimm();
        insn.ControlFlowInsn = true;
    	break;
    case 0b0110011:
        switch (instr.func7())
        {
        case 0b0000000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:insn.SubOp = ALU_ADD;break;
            case 0b001:insn.SubOp = ALU_SLL;break;
            case 0b010:insn.SubOp = ALU_SLT;break;
            case 0b011:insn.SubOp = ALU_SLTU;break;
            case 0b100:insn.SubOp = ALU_XOR;break;
            case 0b101:insn.SubOp = ALU_SRL;break;
            case 0b110:insn.SubOp = ALU_OR;break;
            case 0b111:insn.SubOp = ALU_AND;break;
            default:illegal_instr = true;break;
            }
            break;
        case 0b0100000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:insn.SubOp = ALU_SUB;break;
            case 0b101:insn.SubOp = ALU_SRA;break;
            default:illegal_instr = true;break;
            }
            break;
        default:illegal_instr = true;break;
        }
    	break;
    case 0b0111011:
        switch (instr.func7())
        {
        case 0b0000000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:insn.SubOp = ALU_ADDW;break;
            case 0b001:insn.SubOp = ALU_SLLW;break;
            case 0b101:insn.SubOp = ALU_SRLW;break;
            default:illegal_instr = true;break;
            }
            break;
        case 0b0100000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:insn.SubOp = ALU_SUBW;break;
            case 0b101:insn.SubOp = ALU_SRAW;break;
            default:illegal_instr = true;break;
            }
            break;
        default:illegal_instr = true;break;
        }
    	break;
    case 0b0010011:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.imm      = instr.iimm();
        insn.Operand2 = instr.iimm();
        insn.Operand2Ready = true;
        switch (instr.func3())
        {
        case 0b000:insn.SubOp = ALU_ADD;break;
        case 0b001:
            insn.Operand2 = instr.shamt();
            insn.SubOp = ALU_SLL;
            if((instr.func7() >> 1) != 0){
                illegal_instr = true;
            }
        	break;
        case 0b010:insn.SubOp = ALU_SLT;break;
        case 0b011:insn.SubOp = ALU_SLTU;break;
        case 0b100:insn.SubOp = ALU_XOR;break;
        case 0b101:
            insn.Operand2 = instr.shamt();
            if((instr.func7() >> 1) == 0b000000){
                insn.SubOp = ALU_SRL;
            }else if((instr.func7() >> 1) == 0b010000){
                insn.SubOp = ALU_SRA;
            }else{
                illegal_instr = true;
            }
        	break;
        case 0b110:insn.SubOp = ALU_OR;break;
        case 0b111:insn.SubOp = ALU_AND;break;
        default:illegal_instr = true;break;
        }
    	break;
    case 0b0011011:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.imm      = instr.iimm();
        insn.Operand2 = instr.iimm();
        insn.Operand2Ready = true;
        switch (instr.func3())
        {
        case 0b000:insn.SubOp = ALU_ADDW;break;
        case 0b001:
            insn.Operand2 = instr.shamt();
            insn.SubOp = ALU_SLLW;
            if(instr.func7() != 0){
                illegal_instr = true;
            }
        	break;
        case 0b101:
            insn.Operand2 = instr.shamt();
            if(instr.func7() == 0b0000000){
                insn.SubOp    = ALU_SRLW;
            }else if(instr.func7() == 0b0100000){
                insn.SubOp = ALU_SRAW;
            }else{
                illegal_instr = true;
            }
        	break;
        default:illegal_instr = true;break;
        }
    	break;
    case 0b0010111:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.SubOp     = ALU_ADD;
        insn.Operand1  = instr.uimm();
        insn.Operand2  = insn.Pc;
        insn.Operand1Ready = true;
        insn.Operand2Ready = true;
        insn.imm  = instr.uimm();
    	break;
    case 0b0110111:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.SubOp     = ALU_ADD;
        insn.Operand1  = instr.uimm();
        insn.Operand2  = 0;
        insn.Operand1Ready = true;
        insn.Operand2Ready = true;
        insn.imm  = instr.uimm();
    	break;
    default:illegal_instr = true;break;
    }
  
    if(ecall){
        insn.Excp.valid = true;
        insn.Excp.Tval  = 0;
        insn.Excp.Cause = RISCV::ENV_CALL_MMODE;
    }else if(ebreak){
        insn.Excp.valid = true;
        insn.Excp.Cause = RISCV::BREAKPOINT;
        insn.Excp.Tval  = 0;
    }
    if(illegal_instr){
        insn.Excp.valid = true;
        insn.Excp.Cause = RISCV::ILLEGAL_INSTR;
        insn.Excp.Tval  = insn.UncompressedInsn;
    }
    else{
        insn.Excp.valid    = 0;
        insn.Excp.Cause    = 0;
        insn.Excp.Tval     = 0;
    }
}

} // namespace RISCV
#endif	