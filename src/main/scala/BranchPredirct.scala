import chisel3._
import chisel3.util._

class BranchRedirect extends Module {
  val io = IO(new Bundle {
    val Insn = Input(UInt(32.W))
    val Insn_PC = Input(UInt(32.W))
    val Pred_Taken = Output(Bool())
    val Pred_Target = Output(UInt(32.W))
    val Redirect_ID = Output(UInt(4.W))
    val Redirect_Target = Output(UInt(32.W))
    val needRedirect = Output(Bool())
  })

  // Extract opcode from the instruction
  val opcode = io.Insn(6, 0)

  // Default values
  io.Pred_Taken := false.B
  io.Pred_Target := io.Insn_PC + 4.U
  io.Redirect_ID := 2.U
  io.Redirect_Target := io.Pred_Target
  io.needRedirect := false.B

  // Check if the instruction is a JAL instruction
  when(opcode === "b1101111".U) {
    io.Pred_Taken := true.B
    io.Pred_Target := (io.Insn_PC.asSInt + Cat(io.Insn(31), io.Insn(19, 12), io.Insn(20), io.Insn(30, 21), 0.U(1.W)).asSInt).asUInt
    io.Redirect_Target := io.Pred_Target
    io.needRedirect := true.B
  }
}

object BranchRedirect extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new BranchRedirect())
}
