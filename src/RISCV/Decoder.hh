#ifndef __DECODER_HH__
#define __DECODER_HH__ 

#include "../Processor/DynInsn.hh"
#include "../Common/Common.hh"
#include "Encoding.hh"
#include "StaticInsn.hh"
#include "rvcExpender.hh"
namespace RISCV
{

void Decode_Func(Emulator::DynInsn& insn, Priv_level_t priv_level){
    
    if(insn.IsRvcInsn){//如果是压缩指令，则通过该函数将压缩指令转换为非压缩指令的格式，后面同一进行解码
        insn.UncompressedInsn = crv_decompress_real(insn.CompressedInsn);
        if(insn.UncompressedInsn == 0){
            insn.Excp.valid = true;
            insn.Excp.Cause = RISCV::ILLEGAL_INSTR;
            insn.Excp.Tval  = insn.CompressedInsn;
            return;
        } 
    }else{
        insn.UncompressedInsn = insn.CompressedInsn;
    }

    StaticInsn instr(insn.UncompressedInsn);

    insn.IsaRs1  = instr.rs1();
    insn.IsaRs2  = instr.rs2();
    insn.IsaRd   = instr.rd();

    insn.Rs1Type = Emulator::RegType_t::NONE;
    insn.Rs2Type = Emulator::RegType_t::NONE;
    insn.RdType  = Emulator::RegType_t::NONE;

    insn.Operand1Ready = false;
    insn.Operand2Ready = false;

    insn.ControlFlowInsn = false;

    insn.imm     = 0;

    insn.Excp.valid    = 0;
    insn.Excp.Cause    = RISCV::ILLEGAL_INSTR;
    insn.Excp.Tval     = 0;

    insn.Fu          = Emulator::funcType_t::ALU;
    insn.SubOp       = ALU_ADD;


    bool    illegal_instr = false;
    bool    ecall   = false;
    bool    ebreak  = false;
    switch (instr.opcode())
    {
    case OpcodeSystem:
        insn.Fu = Emulator::funcType_t::CSR;
        if(instr.func3() >> 2){
            insn.Operand1      = instr.rs1();
            insn.Operand1Ready = true;
        }else{
            insn.Rs1Type = Emulator::RegType_t::INT;
        }
        insn.imm = instr.csr();
        switch (instr.func3() & 0b11)
        {
        case 0b00:
            if(instr.rd() == 0 && instr.rs1() == 0){
                insn.Rs1Type = Emulator::RegType_t::NONE;
                switch (instr.iimm())
                {
                case 0b000000000000:
                    insn.SubOp = CSR_ECALL;
                    insn.ControlFlowInsn = true;
                    ecall = true;
                	break;
                case 0b000000000001:
                    insn.SubOp = CSR_EBREAK;
                    insn.ControlFlowInsn = true;
                    ebreak = true;
                	break;
                case 0b001100000010:
                    insn.SubOp = CSR_MRET;
                    insn.ControlFlowInsn = true;
                    if(priv_level != Priv_level_t::PRIV_M){
                        illegal_instr = true;
                    }
                	break;
                case 0b000100000010:
                    insn.SubOp = CSR_SRET;
                    insn.ControlFlowInsn = true;
                    if(priv_level == Priv_level_t::PRIV_U){
                        illegal_instr = true;
                    }
                	break;
                default:
                    illegal_instr = true;
                	break;
                }
            }else{
                illegal_instr = true;
            }
        	break;
        case 0b01:
            insn.SubOp = CSR_CSRRW;
            insn.RdType  = Emulator::RegType_t::INT;
        	break;
        case 0b10:
            if(instr.rs1() == 0){
                insn.SubOp = CSR_CSRR ;
                insn.Rs1Type = Emulator::RegType_t::NONE;
                insn.RdType  = Emulator::RegType_t::INT;
            }else{
                insn.SubOp = CSR_CSRRS;
                insn.RdType  = Emulator::RegType_t::INT;
            }
        	break;
        case 0b11:
            if(instr.rs1() == 0){
                insn.SubOp = CSR_CSRR ;
                insn.Rs1Type = Emulator::RegType_t::NONE;
                insn.RdType  = Emulator::RegType_t::INT;
            }else{
                insn.SubOp = CSR_CSRRC;
                insn.RdType  = Emulator::RegType_t::INT;
            }
        	break;
        default:
            illegal_instr = true;
            	break;
        }
    	break;
    case OpcodeMiscMem:
        insn.Fu = Emulator::funcType_t::CSR;
        insn.imm = instr.iimm();
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
            default:
                	break;
            }
        }else{
            illegal_instr = true;
        }
    	break;
    case OpcodeLoad:
        insn.Fu = Emulator::funcType_t::LDU;
        insn.imm = instr.iimm();
        insn.Rs1Type = Emulator::RegType_t::INT;
        insn.RdType  = Emulator::RegType_t::INT;
        switch (instr.func3())
        {
        case 0b000:
            insn.SubOp = LDU_LB;
        	break;
        case 0b001:
            insn.SubOp = LDU_LH;
        	break;
        case 0b010:
            insn.SubOp = LDU_LW;
        	break;
        case 0b011:
            insn.SubOp = LDU_LD;
        	break;
        case 0b100:
            insn.SubOp = LDU_LBU;
        	break;
        case 0b101:
            insn.SubOp = LDU_LHU;
        	break;
        case 0b110:
            insn.SubOp = LDU_LWU;
        	break;
        default:
            illegal_instr = true;
        	break;
        }
        break;
    case OpcodeStore:
        insn.Fu = Emulator::funcType_t::STU;
        insn.imm = instr.simm();
        insn.Rs1Type = Emulator::RegType_t::INT;
        insn.Rs2Type = Emulator::RegType_t::INT;
        switch (instr.func3())
        {
        case 0b000:
            insn.SubOp = STU_SB;
        	break;
        case 0b001:
            insn.SubOp = STU_SH;
        	break;
        case 0b010:
            insn.SubOp = STU_SW;
        	break;
        case 0b011:
            insn.SubOp = STU_SD;
        	break;
        default:
            illegal_instr = true;
        	break;
        }
    	break;
    // case OpcodeAmo:
        // insn.Fu = Emulator::funcType_t::ST;
    // 	break;
    case OpcodeBranch:
        insn.Fu  = Emulator::funcType_t::BRU;
        insn.imm = instr.sbimm();
        insn.Rs1Type = Emulator::RegType_t::INT;
        insn.Rs2Type = Emulator::RegType_t::INT;
        insn.ControlFlowInsn = true;
        switch (instr.func3())
        {
        case 0b000:
            insn.SubOp = BRU_BEQ;
        	break;
        case 0b001:
            insn.SubOp = BRU_BNE;
        	break;
        case 0b100:
            insn.SubOp = BRU_BLT;
        	break;
        case 0b101:
            insn.SubOp = BRU_BGE;
        	break;
        case 0b110:
            insn.SubOp = BRU_BLTU;
        	break;
        case 0b111:
            insn.SubOp = BRU_BGEU;
        	break;
        default:
            illegal_instr = true;
            	break;
        }
    	break;
    case OpcodeJal:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.SubOp = ALU_ADD;
        insn.Operand1 = insn.Pc;
        insn.Operand2 = insn.IsRvcInsn ? 2 : 4;
        insn.Operand1Ready = true;
        insn.Operand2Ready = true;//转目标地址的计算不涉及任何寄存器的值，所以在解码阶段就可以直接确定这个值
        insn.RdType   = Emulator::RegType_t::INT;
    	break;
    case OpcodeJalr:
        insn.Fu  = Emulator::funcType_t::BRU;
        insn.SubOp = BRU_JALR;
        if(instr.func3() != 0){
            illegal_instr = true;
        }
        insn.Rs1Type = Emulator::RegType_t::INT;
        insn.imm = instr.iimm();
        insn.ControlFlowInsn = true;
        insn.RdType  = Emulator::RegType_t::INT;
    	break;
    case OpcodeOp:
        insn.Rs1Type  = Emulator::RegType_t::INT;
        insn.Rs2Type  = Emulator::RegType_t::INT;
        insn.RdType  = Emulator::RegType_t::INT;
        switch (instr.func7())
        {
        case 0b0000000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:
                insn.SubOp = ALU_ADD;
            	break;
            case 0b001:
                insn.SubOp = ALU_SLL;
            	break;
            case 0b010:
                insn.SubOp = ALU_SLT;
            	break;
            case 0b011:
                insn.SubOp = ALU_SLTU;
            	break;
            case 0b100:
                insn.SubOp = ALU_XOR;
            	break;
            case 0b101:
                insn.SubOp = ALU_SRL;
            	break;
            case 0b110:
                insn.SubOp = ALU_OR;
            	break;
            case 0b111:
                insn.SubOp = ALU_AND;
            	break;
            default:
                illegal_instr = true;
            	break;
            }
            break;
        case 0b0100000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:
                insn.SubOp = ALU_SUB;
                break;
            case 0b101:
                insn.SubOp = ALU_SRA;
                break;
            default:
                illegal_instr = true;
                break;
            }
            break;
        case 0b0000001:
            insn.Fu = ( instr.func3() >> 2 ) ? Emulator::funcType_t::DIV : Emulator::funcType_t::MUL;
            switch (instr.func3())
            {
            case 0b000:
                insn.SubOp = MUL_MUL;
                break;
            case 0b001:
                insn.SubOp = MUL_MULH;
                break;
            case 0b010:
                insn.SubOp = MUL_MULHSU;
                break;
            case 0b011:
                insn.SubOp = MUL_MULHU;
                break;
            case 0b100:
                /* code */
                insn.SubOp = DIV_DIV;
                break;
            case 0b101:
                /* code */
                insn.SubOp = DIV_DIVU;
                break;
            case 0b110:
                /* code */
                insn.SubOp = DIV_REM;
                break;
            case 0b111:
                /* code */
                insn.SubOp = DIV_REMU;
                break;
            default:
                break;
            }
            break;
        default:
            illegal_instr = true;
            break;
        }

    	break;
    case OpcodeOp32:
        insn.Rs1Type  = Emulator::RegType_t::INT;
        insn.Rs2Type  = Emulator::RegType_t::INT;
        insn.RdType  = Emulator::RegType_t::INT;
        switch (instr.func7())
        {
        case 0b0000000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:
                insn.SubOp = ALU_ADDW;
            	break;
            case 0b001:
                insn.SubOp = ALU_SLLW;
            	break;
            case 0b101:
                insn.SubOp = ALU_SRLW;
            	break;
            default:
                illegal_instr = true;
            	break;
            }
            break;
        case 0b0100000:
            insn.Fu = Emulator::funcType_t::ALU;
            switch (instr.func3())
            {
            case 0b000:
                insn.SubOp = ALU_SUBW;
                break;
            case 0b101:
                insn.SubOp = ALU_SRAW;
                break;
            default:
                illegal_instr = true;
                break;
            }
            break;
        case 0b0000001:
            insn.Fu = ( instr.func3() >> 2 ) ? Emulator::funcType_t::DIV : Emulator::funcType_t::MUL;
            switch (instr.func3())
            {
            case 0b000:
                insn.SubOp = MUL_MULW;
                break;
            case 0b100:
                insn.SubOp = DIV_DIVW;
                break;
            case 0b101:
                insn.SubOp = DIV_DIVUW;
                break;
            case 0b110:
                insn.SubOp = DIV_REMW;
                break;
            case 0b111:
                insn.SubOp = DIV_REMUW;
                break;
            default:
                break;
            }
            break;
        default:
            illegal_instr = true;
            break;
        }
    	break;
    case OpcodeOpImm:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.Rs1Type  = Emulator::RegType_t::INT;
        insn.RdType  = Emulator::RegType_t::INT;
        insn.imm      = instr.iimm();
        insn.Operand2 = instr.iimm();
        insn.Operand2Ready = true;
        switch (instr.func3())
        {
        case 0b000:
            insn.SubOp = ALU_ADD;
        	break;
        case 0b001:
            insn.Operand2 = instr.shamt();
            insn.SubOp = ALU_SLL;
            if((instr.func7() >> 1) != 0){
                illegal_instr = true;
            }
        	break;
        case 0b010:
            insn.SubOp = ALU_SLT;
        	break;
        case 0b011:
            insn.SubOp = ALU_SLTU;
        	break;
        case 0b100:
            insn.SubOp = ALU_XOR;
        	break;
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
        case 0b110:
            insn.SubOp = ALU_OR;
        	break;
        case 0b111:
            insn.SubOp = ALU_AND;
        	break;
        default:
            illegal_instr = true;
        	break;
        }
    	break;
    case OpcodeOpImm32:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.Rs1Type  = Emulator::RegType_t::INT;
        insn.RdType   = Emulator::RegType_t::INT;
        insn.imm      = instr.iimm();
        insn.Operand2 = instr.iimm();
        insn.Operand2Ready = true;
        switch (instr.func3())
        {
        case 0b000:
            insn.SubOp = ALU_ADDW; 
        	break;
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
        default:
            illegal_instr = true;
        	break;
        }
    	break;
    case OpcodeAuipc:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.SubOp     = ALU_ADD;
        insn.Operand1  = instr.uimm();
        insn.Operand2  = insn.Pc;
        insn.Operand1Ready = true;
        insn.Operand2Ready = true;
        insn.imm  = instr.uimm();
        insn.RdType = Emulator::RegType_t::INT;
    	break;
    case OpcodeLui:
        insn.Fu = Emulator::funcType_t::ALU;
        insn.SubOp     = ALU_ADD;
        insn.Operand1  = instr.uimm();
        insn.Operand2  = 0;
        insn.Operand1Ready = true;
        insn.Operand2Ready = true;
        insn.imm  = instr.uimm();
        insn.RdType = Emulator::RegType_t::INT;
    	break;
    default:
        illegal_instr = true;
    	break;
    }

    insn.IsaRd  = (insn.RdType  == Emulator::RegType_t::NONE) ? 0 : insn.IsaRd;
    insn.IsaRs1 = (insn.Rs1Type == Emulator::RegType_t::NONE) ? 0 : insn.IsaRs1;
    insn.IsaRs2 = (insn.Rs2Type == Emulator::RegType_t::NONE) ? 0 : insn.IsaRs2;

    if(insn.IsaRd == 0 && insn.RdType == Emulator::RegType_t::INT){
        insn.RdType = Emulator::RegType_t::NONE;
    }


    if(ecall){
        insn.Excp.valid = true;
        insn.Excp.Tval  = 0;
        switch (priv_level)
        {
        case Priv_level_t::PRIV_M :
            insn.Excp.Cause = RISCV::ENV_CALL_MMODE;
        	break;
        case Priv_level_t::PRIV_S :
            insn.Excp.Cause = RISCV::ENV_CALL_SMODE;
        	break;
        case Priv_level_t::PRIV_U :
            insn.Excp.Cause = RISCV::ENV_CALL_UMODE;
        	break;
        default:
            illegal_instr = true;
        	break;
        }
    }else if(ebreak){
        insn.Excp.valid = true;
        insn.Excp.Cause = RISCV::BREAKPOINT;
        insn.Excp.Tval  = 0;
    }
    
    if(illegal_instr){
        insn.Excp.valid = true;
        insn.Excp.Cause = RISCV::ILLEGAL_INSTR;
        insn.Excp.Tval  = insn.CompressedInsn;
    }
}

} // namespace RISCV




#endif	