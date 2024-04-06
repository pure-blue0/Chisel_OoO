import chisel3._
import chisel3.util._

// Define the ALU operations as constants for readability
object ALUOps {
  val ALU_ADD  = 0.U(4.W)
  val ALU_SUB  = 1.U(4.W)
  val ALU_SLL  = 2.U(4.W)
  val ALU_SLT  = 3.U(4.W)
  val ALU_SLTU = 4.U(4.W)
  val ALU_XOR  = 5.U(4.W)
  val ALU_SRL  = 6.U(4.W)
  val ALU_SRA  = 7.U(4.W)
  val ALU_OR   = 8.U(4.W)
  val ALU_AND  = 9.U(4.W)
  val ALU_ADDW = 10.U(4.W)
  val ALU_SUBW = 11.U(4.W)
  val ALU_SLLW = 12.U(4.W)
  val ALU_SRLW = 13.U(4.W)
  val ALU_SRAW = 14.U(4.W)
}

class ALU extends Module {
  val io = IO(new Bundle {
    val Insn_operand1 = Input(UInt(64.W))
    val Insn_operand2 = Input(UInt(64.W))
    val Sub_OP_out    = Input(UInt(4.W))
    val Insn_rdresult = Output(UInt(64.W))
  })

  io.Insn_rdresult := 0.U // Default result

  // Main ALU operation logic
  switch(io.Sub_OP_out) {
    is(ALUOps.ALU_ADD)  { io.Insn_rdresult := io.Insn_operand1 + io.Insn_operand2 }
    is(ALUOps.ALU_SUB)  { io.Insn_rdresult := io.Insn_operand1 - io.Insn_operand2 }
    is(ALUOps.ALU_SLL)  { io.Insn_rdresult := io.Insn_operand1 << io.Insn_operand2(5, 0) }
    is(ALUOps.ALU_SLT)  { io.Insn_rdresult := (io.Insn_operand1.asSInt < io.Insn_operand2.asSInt).asUInt }
    is(ALUOps.ALU_SLTU) { io.Insn_rdresult := (io.Insn_operand1 < io.Insn_operand2).asUInt }
    is(ALUOps.ALU_XOR)  { io.Insn_rdresult := io.Insn_operand1 ^ io.Insn_operand2 }
    is(ALUOps.ALU_SRL)  { io.Insn_rdresult := io.Insn_operand1 >> io.Insn_operand2(5, 0) }
    is(ALUOps.ALU_SRA)  { io.Insn_rdresult := (io.Insn_operand1.asSInt >> io.Insn_operand2(5, 0)).asUInt }
    is(ALUOps.ALU_OR)   { io.Insn_rdresult := io.Insn_operand1 | io.Insn_operand2 }
    is(ALUOps.ALU_AND)  { io.Insn_rdresult := io.Insn_operand1 & io.Insn_operand2 }
    is(ALUOps.ALU_ADDW) { io.Insn_rdresult := (io.Insn_operand1(31, 0).asSInt + io.Insn_operand2(31, 0).asSInt).asUInt.pad(64) }
    is(ALUOps.ALU_SUBW) { io.Insn_rdresult := (io.Insn_operand1(31, 0).asSInt - io.Insn_operand2(31, 0).asSInt).asUInt.pad(64) }
    is(ALUOps.ALU_SLLW) { io.Insn_rdresult := (io.Insn_operand1(31, 0) << io.Insn_operand2(4, 0)).asUInt.pad(64) }
    is(ALUOps.ALU_SRLW) { io.Insn_rdresult := (io.Insn_operand1(31, 0).asUInt >> io.Insn_operand2(4, 0)).pad(64) }
    is(ALUOps.ALU_SRAW) { io.Insn_rdresult := (io.Insn_operand1(31, 0).asSInt >> io.Insn_operand2(4, 0)).asUInt.pad(64) }
    // Default case is already covered
  }
}
