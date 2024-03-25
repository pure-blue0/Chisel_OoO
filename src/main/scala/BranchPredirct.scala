import chisel3._
import chisel3.util._

class BranchRedirect extends Module {
  val io = IO(new Bundle {
    val Insn = Input(UInt(32.W))
    val Perd_Taken = Output(Bool())
    val Pred_Target = Output(UInt(32.W))
    val Redirect_ID = Output(UInt(4.W))
    val Redirect_Target = Output(UInt(32.W))
    val needRedirect = Output(Bool())
  })

  io.Perd_Taken := false.B
  io.Pred_Target := 0.U
  io.Redirect_ID := 0.U
  io.Redirect_Target := 0.U
  io.needRedirect := false.B

  when(io.Insn(6,0) === "b1101111".U) {
    io.Perd_Taken := true.B
    io.Pred_Target := io.Insn // This should be calculated based on PC + offset, adjust as per actual logic
    io.Redirect_ID := 2.U
    io.Redirect_Target := io.Pred_Target
    io.needRedirect := true.B
  } .otherwise {
    io.Perd_Taken := false.B
    io.Pred_Target := io.Insn // Adjust based on default behavior, typically PC + 4
    io.Redirect_ID := 0.U
    io.needRedirect := false.B
  }
}