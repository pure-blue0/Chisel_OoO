import chisel3._
import chisel3.util._

class Predecode extends Module {
  val io = IO(new Bundle {
    val Address = Input(UInt(32.W))
    val data = Input(UInt(128.W))
    val excp_valid = Input(Bool())
    val excp_cause = Input(UInt(6.W))
    val excpt_tval = Input(UInt(32.W))
    val insn_valid1 = Output(Bool())
    val pc1 = Output(UInt(32.W))
    val insn1 = Output(UInt(32.W))
    val Perd_Taken1 = Output(Bool())
    val Pred_Target1 = Output(UInt(32.W))
    val excp_valid1 = Output(Bool())
    val excp_cause1 = Output(UInt(6.W))
    val excpt_tval1 = Output(UInt(32.W))
    val insn_valid2 = Output(Bool())
    val pc2 = Output(UInt(32.W))
    val insn2 = Output(UInt(32.W))
    val Perd_Taken2 = Output(Bool())
    val Pred_Target2 = Output(UInt(32.W))
    val excp_valid2 = Output(Bool())
    val excp_cause2 = Output(UInt(6.W))
    val excpt_tval2 = Output(UInt(32.W))
    val insn_valid3 = Output(Bool())
    val pc3 = Output(UInt(32.W))
    val insn3 = Output(UInt(32.W))
    val Perd_Taken3 = Output(Bool())
    val Pred_Target3 = Output(UInt(32.W))
    val excp_valid3 = Output(Bool())
    val excp_cause3 = Output(UInt(6.W))
    val excpt_tval3 = Output(UInt(32.W))
    val insn_valid4 = Output(Bool())
    val pc4 = Output(UInt(32.W))
    val insn4 = Output(UInt(32.W))
    val Perd_Taken4 = Output(Bool())
    val Pred_Target4 = Output(UInt(32.W))
    val excp_valid4 = Output(Bool())
    val excp_cause4 = Output(UInt(6.W))
    val excpt_tval4 = Output(UInt(32.W))
    val Redirect_valid = Output(Bool())
    val Redirect_Target = Output(UInt(32.W))
    val fetch_flush = Output(Bool())
  })

  val iCacheAlignByte = 16
  val offset = io.Address & (iCacheAlignByte.U(32.W) - 1.U(32.W))
  val dataPtr = io.data >> (offset * 8.U)

  val insn1 = Wire(new Bundle {
    val Pc = UInt(32.W)
    val UncompressedInsn = UInt(32.W)
  })

  val insn2 = Wire(new Bundle {
    val Pc = UInt(32.W)
    val UncompressedInsn = UInt(32.W)
  })

  val insn3 = Wire(new Bundle {
    val Pc = UInt(32.W)
    val UncompressedInsn = UInt(32.W)
  })

  val insn4 = Wire(new Bundle {
    val Pc = UInt(32.W)
    val UncompressedInsn = UInt(32.W)
  })

  insn1.Pc := io.Address
  insn1.UncompressedInsn := dataPtr(31, 0)

  insn2.Pc := io.Address + 4.U
  insn2.UncompressedInsn := dataPtr(63, 32)

  insn3.Pc := io.Address + 8.U
  insn3.UncompressedInsn := dataPtr(95, 64)

  insn4.Pc := io.Address + 12.U
  insn4.UncompressedInsn := dataPtr(127, 96)

  val branchRedirect1 = Module(new BranchRedirect)
  val branchRedirect2 = Module(new BranchRedirect)
  val branchRedirect3 = Module(new BranchRedirect)
  val branchRedirect4 = Module(new BranchRedirect)

  branchRedirect1.io.Insn := insn1.UncompressedInsn
  branchRedirect1.io.Insn_PC := insn1.Pc

  branchRedirect2.io.Insn := insn2.UncompressedInsn
  branchRedirect2.io.Insn_PC := insn2.Pc

  branchRedirect3.io.Insn := insn3.UncompressedInsn
  branchRedirect3.io.Insn_PC := insn3.Pc

  branchRedirect4.io.Insn := insn4.UncompressedInsn
  branchRedirect4.io.Insn_PC := insn4.Pc

  io.insn_valid1 := true.B
  io.insn_valid2 := ((offset === 8.U) || (offset === 4.U) || (offset === 0.U)) && !branchRedirect1.io.needRedirect
  io.insn_valid3 := ((offset === 4.U) || (offset === 0.U)) && !branchRedirect1.io.needRedirect && !branchRedirect2.io.needRedirect
  io.insn_valid4 := (offset === 0.U) && !branchRedirect1.io.needRedirect && !branchRedirect2.io.needRedirect && !branchRedirect3.io.needRedirect

  io.pc1 := insn1.Pc
  io.insn1 := insn1.UncompressedInsn
  io.Perd_Taken1 := branchRedirect1.io.Pred_Taken
  io.Pred_Target1 := branchRedirect1.io.Pred_Target
  io.excp_valid1 := io.excp_valid
  io.excp_cause1 := io.excp_cause
  io.excpt_tval1 := io.excpt_tval

  io.pc2 := insn2.Pc
  io.insn2 := insn2.UncompressedInsn
  io.Perd_Taken2 := branchRedirect2.io.Pred_Taken
  io.Pred_Target2 := branchRedirect2.io.Pred_Target
  io.excp_valid2 := io.excp_valid
  io.excp_cause2 := io.excp_cause
  io.excpt_tval2 := io.excpt_tval

  io.pc3 := insn3.Pc
  io.insn3 := insn3.UncompressedInsn
  io.Perd_Taken3 := branchRedirect3.io.Pred_Taken
  io.Pred_Target3 := branchRedirect3.io.Pred_Target
  io.excp_valid3 := io.excp_valid
  io.excp_cause3 := io.excp_cause
  io.excpt_tval3 := io.excpt_tval

  io.pc4 := insn4.Pc
  io.insn4 := insn4.UncompressedInsn
  io.Perd_Taken4 := branchRedirect4.io.Pred_Taken
  io.Pred_Target4 := branchRedirect4.io.Pred_Target
  io.excp_valid4 := io.excp_valid
  io.excp_cause4 := io.excp_cause
  io.excpt_tval4 := io.excpt_tval

  io.fetch_flush := branchRedirect1.io.needRedirect || branchRedirect2.io.needRedirect || branchRedirect3.io.needRedirect || branchRedirect4.io.needRedirect
  io.Redirect_valid := branchRedirect1.io.needRedirect || branchRedirect2.io.needRedirect || branchRedirect3.io.needRedirect || branchRedirect4.io.needRedirect
  io.Redirect_Target := Mux(branchRedirect1.io.needRedirect, branchRedirect1.io.Redirect_Target,
    Mux(branchRedirect2.io.needRedirect, branchRedirect2.io.Redirect_Target,
      Mux(branchRedirect3.io.needRedirect, branchRedirect3.io.Redirect_Target,
        branchRedirect4.io.Redirect_Target)))
}

object VerilogGen extends App {
  println((new chisel3.stage.ChiselStage).emitVerilog(new Predecode()))
}