import chisel3._
import chisel3.util._

// Define the Branch Module
class Branch extends Module {
  val io = IO(new Bundle {
    // Inputs
    val insn_operand1 = Input(UInt(64.W))
    val insn_operand2 = Input(UInt(64.W))
    val imm = Input(UInt(64.W))
    val pc = Input(UInt(64.W))
    val pred_taken = Input(Bool())
    val pred_target = Input(UInt(64.W))
    val sub_OP_out = Input(UInt(4.W))
    
    // Outputs
    val insn_rdresult = Output(UInt(64.W))
    val insn_bruTarget = Output(UInt(64.W))
    val bruMisPred = Output(Bool())
  })

  // Default values
  io.insn_rdresult := 0.U
  io.insn_bruTarget := 0.U
  io.bruMisPred := false.B

  // Constants for instruction types
  val BRU_JAR = 0.U
  val BRU_JALR = 1.U
  val BRU_BEQ = 2.U
  val BRU_BNE = 3.U
  val BRU_BLT = 4.U
  val BRU_BGE = 5.U
  val BRU_BLTU = 6.U
  val BRU_BGEU = 7.U

  // Calculate RdResult
  io.insn_rdresult := io.pc + 4.U

  // Branch logic
  val taken = Wire(Bool())
  taken := false.B

  switch(io.sub_OP_out) {
    is(BRU_JAR) { taken := true.B }
    is(BRU_JALR) { taken := true.B }
    is(BRU_BEQ) { taken := io.insn_operand1 === io.insn_operand2 }
    is(BRU_BNE) { taken := io.insn_operand1 =/= io.insn_operand2 }
    is(BRU_BLT) { taken := io.insn_operand1.asSInt < io.insn_operand2.asSInt }
    is(BRU_BGE) { taken := io.insn_operand1.asSInt >= io.insn_operand2.asSInt }
    is(BRU_BLTU) { taken := io.insn_operand1 < io.insn_operand2 }
    is(BRU_BGEU) { taken := io.insn_operand1 >= io.insn_operand2 }
  }

  // Calculate BruTarget
  when(io.sub_OP_out === BRU_JALR) {
    io.insn_bruTarget := io.insn_operand1 + io.imm
  } .otherwise {
    io.insn_bruTarget := io.pc + Mux(taken, io.imm, 4.U)
  }

  // Update BruMisPred
  io.bruMisPred := (taken =/= io.pred_taken) || (io.insn_bruTarget =/= io.pred_target)
}

// Generate the Verilog code

object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new Branch))
}