import chisel3._
import chisel3.util._

class Decode_Func extends Module {
  val io = IO(new Bundle {
    val Insn = Input(UInt(32.W))
    val IsaRs1 = Output(UInt(5.W))
    val Operand1Ready = Output(Bool())
    val IsaRs2 = Output(UInt(5.W))
    val Operand2Ready = Output(Bool())
    val IsaRd = Output(UInt(5.W))
    val ControlFlowInsn = Output(Bool())
    val Function_type = Output(UInt(3.W))
    val Sub_OP = Output(UInt(4.W))
    val imm = Output(UInt(32.W))
    val excp_valid = Output(Bool())
    val excp_cause = Output(UInt(6.W))
    val excpt_tval = Output(UInt(32.W))
  })

  // Extracting fields from the instruction
  val opcode = io.Insn(6, 0)
  val func3 = io.Insn(14, 12)
  val func7 = io.Insn(31, 25)
  val rd = io.Insn(11, 7)
  val rs1 = io.Insn(19, 15)
  val rs2 = io.Insn(24, 20)
  val csr = io.Insn(31, 20)
  val i_imm = Cat(Fill(21,io.Insn(31)),io.Insn(30, 20))//&&
  val s_imm = Cat(Fill(21,io.Insn(31)), io.Insn(30, 25), io.Insn(11, 7))
  val b_imm = Cat(Fill(20,io.Insn(31)), io.Insn(7), io.Insn(30, 25), io.Insn(11, 8),0.U(1.W))
  val u_imm = Cat(io.Insn(31, 12), 0.U(12.W))
  val j_imm = Cat(Fill(12,io.Insn(31)), io.Insn(19, 12), io.Insn(20), io.Insn(30, 21),0.U(1.W))


  // Default values
  io.IsaRs1 := rs1
  io.IsaRs2 := rs2
  io.IsaRd := rd
  io.Operand1Ready := false.B
  io.Operand2Ready := false.B
  io.ControlFlowInsn := false.B
  io.imm := 0.U
  io.Function_type := 0.U // ALU
  io.Sub_OP := 0.U // ALU_ADD
  io.excp_valid := false.B
  io.excp_cause := 0.U
  io.excpt_tval := 0.U

  // Illegal instruction flag
  val illegal_instr = Wire(Bool())
  illegal_instr := false.B

  // Ecall and Ebreak flags
  val ecall = Wire(Bool())
  val ebreak = Wire(Bool())
  ecall := false.B
  ebreak := false.B

  // Decoding logic
  when(opcode === "b1110011".U) {
    io.Function_type := 2.U // CSR
    when(func3(2)) {
      io.Operand1Ready := true.B
    }
    io.imm := csr
    when((func3 & "b11".U) === "b00".U) {
      io.IsaRd := 0.U
      when(rd === 0.U && rs1 === 0.U) {
        io.ControlFlowInsn := true.B
        when(i_imm === "b000000000000".U) {
          io.Sub_OP := 5.U // CSR_ECALL
          ecall := true.B
        } .elsewhen(i_imm === "b000000000001".U) {
          io.Sub_OP := 6.U // CSR_EBREAK
          ebreak := true.B
        } .elsewhen(i_imm === "b001100000010".U) {
          io.Sub_OP := 7.U // CSR_MRET
        } .otherwise {
          illegal_instr := true.B
        }
      } .otherwise {
        illegal_instr := true.B
      }
    } .elsewhen((func3 & "b11".U) === "b01".U) {
      io.Sub_OP := 0.U // CSR_CSRRW
    } .elsewhen((func3 & "b11".U) === "b10".U) {
      when(rs1 === 0.U) {
        io.Sub_OP := 3.U // CSR_CSRR
      } .otherwise {
        io.Sub_OP := 1.U // CSR_CSRRS
      }
    } .elsewhen((func3 & "b11".U) === "b11".U) {
      when(rs1 === 0.U) {
        io.Sub_OP := 3.U // CSR_CSRR
      } .otherwise {
        io.Sub_OP := 2.U // CSR_CSRRC
      }
    } .otherwise {
      illegal_instr := true.B
    }
  } .elsewhen(opcode === "b0001111".U) {
    io.Function_type := 2.U // CSR
    io.imm := i_imm.asSInt.asUInt
    io.IsaRd := 0.U
    when(rs1 === 0.U && rd === 0.U) {
      when(func3 === "b000".U) {
        io.Sub_OP := 9.U // CSR_FENCE
      } .elsewhen(func3 === "b001".U) {
        when(i_imm =/= 0.U) {
          illegal_instr := true.B
        }
        io.Sub_OP := 4.U // CSR_FENCEI
        io.ControlFlowInsn := true.B
      } .otherwise {
        illegal_instr := true.B
      }
    } .otherwise {
      illegal_instr := true.B
    }
  } .elsewhen(opcode === "b0000011".U) {
    io.Function_type := 3.U // LDU
    io.imm := i_imm
    when(func3 === "b000".U) { io.Sub_OP := 0.U } // LDU_LB
      .elsewhen(func3 === "b001".U) { io.Sub_OP := 1.U } // LDU_LH
      .elsewhen(func3 === "b010".U) { io.Sub_OP := 2.U } // LDU_LW
      .elsewhen(func3 === "b011".U) { io.Sub_OP := 3.U } // LDU_LD
      .elsewhen(func3 === "b100".U) { io.Sub_OP := 4.U } // LDU_LBU
      .elsewhen(func3 === "b101".U) { io.Sub_OP := 5.U } // LDU_LHU
      .elsewhen(func3 === "b110".U) { io.Sub_OP := 6.U } // LDU_LWU
      .otherwise { illegal_instr := true.B }
  } .elsewhen(opcode === "b0100011".U) {
    io.Function_type := 4.U // STU
    io.imm := s_imm
    io.IsaRd := 0.U
    when(func3 === "b000".U) { io.Sub_OP := 0.U } // STU_SB
      .elsewhen(func3 === "b001".U) { io.Sub_OP := 1.U } // STU_SH
      .elsewhen(func3 === "b010".U) { io.Sub_OP := 2.U } // STU_SW
      .elsewhen(func3 === "b011".U) { io.Sub_OP := 3.U } // STU_SD
      .otherwise { illegal_instr := true.B }
  } .elsewhen(opcode === "b1100011".U) {
    io.Function_type := 1.U // BRU
    io.imm := b_imm
    io.ControlFlowInsn := true.B
    io.IsaRd := 0.U
    when(func3 === "b000".U) { io.Sub_OP := 2.U } // BRU_BEQ
      .elsewhen(func3 === "b001".U) { io.Sub_OP := 3.U } // BRU_BNE
      .elsewhen(func3 === "b100".U) { io.Sub_OP := 4.U } // BRU_BLT
      .elsewhen(func3 === "b101".U) { io.Sub_OP := 5.U } // BRU_BGE
      .elsewhen(func3 === "b110".U) { io.Sub_OP := 6.U } // BRU_BLTU
      .elsewhen(func3 === "b111".U) { io.Sub_OP := 7.U } // BRU_BGEU
      .otherwise { illegal_instr := true.B }
  } .elsewhen(opcode === "b1101111".U) {
    io.Function_type := 0.U // ALU
    io.Sub_OP := 0.U // ALU_ADD
    io.Operand1Ready := true.B
    io.Operand2Ready := true.B
  } .elsewhen(opcode === "b1100111".U) {
    io.Function_type := 1.U // BRU
    io.Sub_OP := 1.U // BRU_JALR
    when(func3 =/= 0.U) { illegal_instr := true.B }
    io.imm := i_imm
    io.ControlFlowInsn := true.B
  } .elsewhen(opcode === "b0110011".U) {
    when(func7 === "b0000000".U) {
      io.Function_type := 0.U // ALU
      when(func3 === "b000".U) { io.Sub_OP := 0.U } // ALU_ADD
        .elsewhen(func3 === "b001".U) { io.Sub_OP := 2.U } // ALU_SLL
        .elsewhen(func3 === "b010".U) { io.Sub_OP := 3.U } // ALU_SLT
        .elsewhen(func3 === "b011".U) { io.Sub_OP := 4.U } // ALU_SLTU
        .elsewhen(func3 === "b100".U) { io.Sub_OP := 5.U } // ALU_XOR
        .elsewhen(func3 === "b101".U) { io.Sub_OP := 6.U } // ALU_SRL
        .elsewhen(func3 === "b110".U) { io.Sub_OP := 8.U } // ALU_OR
        .elsewhen(func3 === "b111".U) { io.Sub_OP := 9.U } // ALU_AND
        .otherwise { illegal_instr := true.B }
    } .elsewhen(func7 === "b0100000".U) {
      io.Function_type := 0.U // ALU
      when(func3 === "b000".U) { io.Sub_OP := 1.U } // ALU_SUB
        .elsewhen(func3 === "b101".U) { io.Sub_OP := 7.U } // ALU_SRA
        .otherwise { illegal_instr := true.B }
    } .otherwise { illegal_instr := true.B }
  } .elsewhen(opcode === "b0111011".U) {
    when(func7 === "b0000000".U) {
      io.Function_type := 0.U // ALU
      when(func3 === "b000".U) { io.Sub_OP := 10.U } // ALU_ADDW
        .elsewhen(func3 === "b001".U) { io.Sub_OP := 12.U } // ALU_SLLW
        .elsewhen(func3 === "b101".U) { io.Sub_OP := 13.U } // ALU_SRLW
        .otherwise { illegal_instr := true.B }
    } .elsewhen(func7 === "b0100000".U) {
      io.Function_type := 0.U // ALU
      when(func3 === "b000".U) { io.Sub_OP := 11.U } // ALU_SUBW
        .elsewhen(func3 === "b101".U) { io.Sub_OP := 14.U } // ALU_SRAW
        .otherwise { illegal_instr := true.B }
    } .otherwise { illegal_instr := true.B }
  } .elsewhen(opcode === "b0010011".U) {
    io.Function_type := 0.U // ALU
    io.imm := i_imm
    io.Operand2Ready := true.B
    when(func3 === "b000".U) { io.Sub_OP := 0.U } // ALU_ADD
      .elsewhen(func3 === "b001".U) {
        io.Sub_OP := 2.U // ALU_SLL
        when(func7(5, 1) =/= 0.U) { illegal_instr := true.B }
      } .elsewhen(func3 === "b010".U) { io.Sub_OP := 3.U } // ALU_SLT
      .elsewhen(func3 === "b011".U) { io.Sub_OP := 4.U } // ALU_SLTU
      .elsewhen(func3 === "b100".U) { io.Sub_OP := 5.U } // ALU_XOR
      .elsewhen(func3 === "b101".U) {
        when(func7(5, 1) === 0.U) { io.Sub_OP := 6.U } // ALU_SRL
          .elsewhen(func7(5, 1) === "b010000".U) { io.Sub_OP := 7.U } // ALU_SRA
          .otherwise { illegal_instr := true.B }
      } .elsewhen(func3 === "b110".U) { io.Sub_OP := 8.U } // ALU_OR
      .elsewhen(func3 === "b111".U) { io.Sub_OP := 9.U } // ALU_AND
      .otherwise { illegal_instr := true.B }
  } .elsewhen(opcode === "b0011011".U) {
    io.Function_type := 0.U // ALU
    io.imm := i_imm
    io.Operand2Ready := true.B
    when(func3 === "b000".U) { io.Sub_OP := 10.U } // ALU_ADDW
      .elsewhen(func3 === "b001".U) {
        io.Sub_OP := 12.U // ALU_SLLW
        when(func7 =/= 0.U) { illegal_instr := true.B }
      } .elsewhen(func3 === "b101".U) {
        when(func7 === "b0000000".U) { io.Sub_OP := 13.U } // ALU_SRLW
          .elsewhen(func7 === "b0100000".U) { io.Sub_OP := 14.U } // ALU_SRAW
          .otherwise { illegal_instr := true.B }
      } .otherwise { illegal_instr := true.B }
  } .elsewhen(opcode === "b0010111".U) {
    io.Function_type := 0.U // ALU
    io.Sub_OP := 0.U // ALU_ADD
    io.Operand1Ready := true.B
    io.Operand2Ready := true.B
    io.imm := u_imm
  } .elsewhen(opcode === "b0110111".U) {
    io.Function_type := 0.U // ALU
    io.Sub_OP := 0.U // ALU_ADD
    io.Operand1Ready := true.B
    io.Operand2Ready := true.B
    io.imm := u_imm
  } .otherwise {
    illegal_instr := true.B
  }

  // Exception handling
  when(ecall) {
    io.excp_valid := true.B
    io.excp_cause := 11.U // ECALL exception
    io.excpt_tval := 0.U
  } .elsewhen(ebreak) {
    io.excp_valid := true.B
    io.excp_cause := 3.U // EBREAK exception
    io.excpt_tval := 0.U
  } .elsewhen(illegal_instr) {
    io.excp_valid := true.B
    io.excp_cause := 2.U // Illegal instruction exception
    io.excpt_tval := io.Insn
  }
}

object Decode_Func extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Decode_Func())
}
