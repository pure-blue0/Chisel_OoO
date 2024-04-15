import chisel3._
import chisel3.util._

class Decode_Func extends Module {
  val io = IO(new Bundle {
    val Insn = Input(UInt(32.W))
    val IsaRs1 = Output(UInt(5.W))
    val Operand1Ready = Output(Bool())
    val IsaRs2 = Output(UInt(5.W))
    val Operand2Ready = Output(Bool())
    val illegalInsn = Output(Bool())
    val ecall = Output(Bool())
    val ebreak = Output(Bool())
    val IsaRd = Output(UInt(5.W))
    val ControlFlowInsn = Output(Bool())
    val Function_type = Output(UInt(3.W))
    val Sub_OP = Output(UInt(4.W))
    val imm = Output(UInt(32.W))
    val excp_vaild = Output(Bool())
    val excp_cause = Output(UInt(6.W))
    val excpt_tval = Output(UInt(32.W))
    val UncompressedInsn = Output(UInt(32.W))
  })

  // Default assignments
  io := DontCare
  io.Operand1Ready := false.B
  io.Operand2Ready := false.B
  io.illegalInsn := true.B
  io.ControlFlowInsn := false.B
  io.ecall := false.B
  io.ebreak := false.B
  io.excp_vaild := true.B
  io.excp_cause := 2.U
  io.excpt_tval := 0.U

  // Extract fields from instruction
  val opcode = io.Insn(6,0)
  val funct3 = io.Insn(14,12)
  val funct7 = io.Insn(31,25)
  val rs1 = io.Insn(19,15)
  val rs2 = io.Insn(24,20)
  val rd = io.Insn(11,7)
  val immI = io.Insn(31, 20).asUInt
  val immS = Cat(io.Insn(31, 25), io.Insn(11, 7)).asUInt
  val immB = Cat(io.Insn(31), io.Insn(7), io.Insn(30, 25), io.Insn(11, 8), 0.U(1.W)).asUInt
  val immJ = Cat(io.Insn(31), io.Insn(19, 12), io.Insn(20), io.Insn(30, 21), 0.U(1.W)).asUInt
  val immU = Cat(io.Insn(31, 12), Fill(12, 0.U)).asUInt
  
  // CSR operations require reading the csr field
  val csr = io.Insn(31,20)

  // Set register addresses and immediates
  io.IsaRs1 := rs1
  io.IsaRs2 := rs2
  io.IsaRd := io.Insn(11,7)
  io.imm := 0.U // Default immediate to 0, adjust as needed for specific instructions

  // Decoding logic
  switch(opcode) {
    is("b1110011".U) { // System Instructions
      io.Function_type := "b100".U // CSR
      io.IsaRd := 0.U
      io.imm := csr // For CSR instructions, imm field holds the CSR address
      when(funct3 === "b000".U) { // ECALL, EBREAK, MRET
        io.ControlFlowInsn := true.B
        switch(csr) {
          is("b000000000000".U) { io.Sub_OP := "b101".U; io.ecall := true.B /* ECALL */ }
          is("b000000000001".U) { io.Sub_OP := "b110".U; io.ebreak := true.B /* EBREAK */ }
          is("b001100000010".U) { io.Sub_OP := "b111".U /* MRET */ }
        }
          //otherwise { io.excp_vaild := true.B; io.excp_cause := 2.U /* Illegal instruction */ }
        }.elsewhen(funct3 === "b001".U) { // CSRRW
        io.Sub_OP := "b000".U
      }.elsewhen(funct3 === "b010".U) { // CSRRS
        io.Sub_OP := Mux(rs1 === 0.U, "b011".U, "b001".U) // CSRR if rs1 == 0, else CSRRS
      }.elsewhen(funct3 === "b011".U) { // CSRRC
        io.Sub_OP := Mux(rs1 === 0.U, "b011".U, "b010".U) // CSRR if rs1 == 0, else CSRRC
      }
    }
    is("b0001111".U) { // FENCE Instructions
      io.Function_type := "b100".U // CSR, but used for FENCE to simplify
      io.ControlFlowInsn := true.B
      when(funct3 === "b000".U) {
         io.Sub_OP := "b100".U /* FENCE */ 
        }.elsewhen(funct3 === "b001".U && io.Insn(31,20) === 0.U) {
           io.Sub_OP := "b101".U /* FENCE.I */ 
          }.otherwise {
             io.excp_vaild := true.B; io.excp_cause := 2.U /* Illegal instruction */ 
            }
    }

    is("b0000011".U) { // Load instructions
      io.IsaRs1 := rs1
      io.IsaRd := rd
      io.imm := immI.asUInt
      io.Function_type := 3.U // LDU
      switch(funct3) {
        is("b000".U) { io.Sub_OP := 0.U } // LB
        is("b001".U) { io.Sub_OP := 1.U } // LH
        is("b010".U) { io.Sub_OP := 2.U } // LW
        is("b011".U) { io.Sub_OP := 3.U } // LD
        is("b100".U) { io.Sub_OP := 4.U } // LBU
        is("b101".U) { io.Sub_OP := 5.U } // LHU
        is("b110".U) { io.Sub_OP := 6.U } // LWU
        //otherwise { io.illegalInsn := true.B/* Illegal instruction */ }
      }
    }
    is("b0100011".U) { // Store instructions
      io.IsaRs1 := rs1
      io.IsaRs2 := rs2
      io.imm := immS.asUInt
      io.Function_type := 4.U // STU
      switch(funct3) {
        is("b000".U) { io.Sub_OP := 0.U } // SB
        is("b001".U) { io.Sub_OP := 1.U } // SH
        is("b010".U) { io.Sub_OP := 2.U } // SW
        is("b011".U) { io.Sub_OP := 3.U } // SD
        //otherwise { io.illegalInsn := true.B/* Illegal instruction */ }
      }
    }
    is("b1100011".U) { // Branch instructions
      io.IsaRs1 := rs1
      io.IsaRs2 := rs2
      io.imm := immB.asUInt
      io.ControlFlowInsn := true.B
      io.Function_type := 1.U // BRU
      switch(funct3) {
        is("b000".U) { io.Sub_OP := 0.U } // BEQ
        is("b001".U) { io.Sub_OP := 1.U } // BNE
        is("b100".U) { io.Sub_OP := 2.U } // BLT
        is("b101".U) { io.Sub_OP := 3.U } // BGE
        is("b110".U) { io.Sub_OP := 4.U } // BLTU
        is("b111".U) { io.Sub_OP := 5.U } // BGEU
        //otherwise { io.illegalInsn := true.B/* Illegal instruction */ }
      }
    }
    is("b1101111".U) { // JAL
      io.IsaRd := rd
      io.imm := immJ.asUInt
      io.ControlFlowInsn := true.B
      io.Function_type := 1.U // BRU
      io.Sub_OP := 0.U // JAR (Assuming JAR is a typo for JAL)
    }
    is("b1100111".U) { // JALR
      io.IsaRs1 := rs1
      io.IsaRd := rd
      io.imm := immI.asUInt
      io.ControlFlowInsn := true.B
      io.Function_type := 1.U // BRU
      io.Sub_OP := 1.U // JALR
    }

    is("b0110011".U) { // ALU operation instructions
        io.Function_type := 0.U // ALU
        switch(funct3) {
            is("b000".U) { io.Sub_OP := Mux(funct7 === "b0100000".U, 1.U, 0.U) } // ADD/SUB
            is("b001".U) { io.Sub_OP := 2.U } // SLL
            is("b010".U) { io.Sub_OP := 3.U } // SLT
            is("b011".U) { io.Sub_OP := 4.U } // SLTU
            is("b100".U) { io.Sub_OP := 5.U } // XOR
            is("b101".U) { io.Sub_OP := Mux(funct7 === "b0100000".U, 7.U, 6.U) } // SRL/SRA
            is("b110".U) { io.Sub_OP := 8.U } // OR
            is("b111".U) { io.Sub_OP := 9.U } // AND
            //otherwise { io.illegalInsn := true.B/* Illegal instruction */ }
      }
    }

    // Handling 32-bit operation instructions
    is("b0111011".U) { // R-type instructions (e.g., ADDW, SUBW)
        io.Function_type := 0.U // ALU for simplicity, actual type may vary based on broader context
        switch(funct3) {
            is("b000".U) {
            io.Sub_OP := Mux(funct7 === "b0000000".U, 10.U, // ADDW
                        Mux(funct7 === "b0100000".U, 11.U, 0.U)) // SUBW
    }
    // SLLW, SRLW, SRAW would also be differentiated here by funct3 and funct7
            is("b001".U) { io.Sub_OP := 12.U } // SLLW
            is("b101".U) {
        io.Sub_OP := Mux(funct7 === "b0000000".U, 13.U, // SRLW
                       Mux(funct7 === "b0100000".U, 14.U, 0.U)) }// SRAW
            //otherwise { io.illegalInsn := true.B/* Illegal instruction */ }
    
  }
}

    is("b0011011".U) { // I-type instructions (e.g., ADDIW, SLLIW)
        io.Function_type := 0.U // ALU
        io.imm := immI.asSInt.asUInt // Immediate value for 32-bit operations, sign-extended
        io.Operand2Ready := true.B
        switch(funct3) {
            is("b000".U) { io.Sub_OP := 10.U } // ADDIW
    // SLLIW, SRLIW, SRAIW would also be differentiated here by funct3
            is("b001".U) { io.Sub_OP := 12.U } // SLLIW
            is("b101".U) {
        io.Sub_OP := Mux(immI(10), 14.U, // SRAIW if bit 10 of immediate is set
                       13.U) }// SRLIW
           //otherwise { io.illegalInsn := true.B/* Illegal instruction */ }
  }
}   

// IMM operation instructions (0b0010011)
    is("b0010011".U) {
        io.Function_type := 0.U // ALU
        io.imm := immI // Immediate value
        io.Operand2Ready := true.B
        switch(funct3) {
            is("b000".U) { io.Sub_OP := 0.U } // ADDI
            is("b010".U) { io.Sub_OP := 3.U } // SLTI
            is("b011".U) { io.Sub_OP := 4.U } // SLTIU
            is("b100".U) { io.Sub_OP := 5.U } // XORI
            is("b110".U) { io.Sub_OP := 8.U } // ORI
            is("b111".U) { io.Sub_OP := 9.U } // ANDI
            is("b001".U) { io.Sub_OP := 2.U } // SLLI
            is("b101".U) {
        io.Sub_OP := Mux(funct7 === "b0000000".U, 6.U, // SRLI
                       Mux(funct7 === "b0100000".U, 7.U, 0.U)) // SRAI
    }
            //otherwise { io.illegalInsn := true.B/* Illegal instruction */ }
  }
}

// AUIPC/LUI instructions (0b0010111 and 0b0110111)
    is("b0010111".U) { // AUIPC
        io.Function_type := 0.U // ALU
        io.Sub_OP := 0.U // ADD
        io.Operand1Ready := true.B
        io.Operand2Ready := true.B
        io.imm := immU // Immediate value
}
    is("b0110111".U) { // LUI
        io.Function_type := 0.U // ALU
        io.Sub_OP := 0.U // ADD
        io.Operand1Ready := true.B
        io.Operand2Ready := true.B
        io.imm := immU // Immediate value
}

    //otherwise {
    //  io.excp_vaild := true.B
    // io.excp_cause := 2.U // Illegal instruction
    //  io.illegalInsn := true.B
    }

/*otherwise {
      legalInstruction := true.B
    }*/



when(io.ecall) {
    io.excp_vaild := true.B
    io.excpt_tval := 0.U
    io.excp_cause := 11.U // Environment call from M-mode
  } .elsewhen(io.ebreak) {
    io.excp_vaild := true.B
    io.excpt_tval := 0.U
    io.excp_cause := 3.U // Breakpoint
  } .elsewhen(io.illegalInsn) {
    io.excp_vaild := true.B
    io.excpt_tval := io.UncompressedInsn
    io.excp_cause := 2.U // Illegal instruction
  } .otherwise {
    io.excp_vaild := false.B
    io.excpt_tval := 0.U
    io.excp_cause := 0.U
  }
}
  // Exceptions based on specific conditions
/*when(io.excp_vaild) {
    io.excpt_tval := io.Insn // Capture the illegal instruction
  }

when(opcode === "b0000000".U) {
    io.excp_vaild := true.B
    io.excp_cause := 2.U // Illegal instruction
    io.excpt_tval := io.Insn
  } */

// Generate the Verilog code
object Decode_Func extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Decode_Func(), Array("--target-dir", "generated"))
}
