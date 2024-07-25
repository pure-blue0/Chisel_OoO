import chisel3._
import chisel3.util._

class Predecode extends Module {
  val iCacheAlignByte = 8
  val FetchByteWidth = 8

  val io = IO(new Bundle {
    val Address = Input(UInt(32.W))
    val data = Input(UInt(64.W))
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


    val Redirect_ID = Output(UInt(4.W))
    val Redirect_Target = Output(UInt(32.W))
    val fetch_flush = Output(Bool())
  })

  val offset = io.Address % iCacheAlignByte.U
  val validInstructions = FetchByteWidth.U / 4.U - offset / 4.U

  val insn1 = io.data(31, 0)
  val insn2 = io.data(63, 32)

  val branchRedirect1 = Module(new BranchRedirect)
  branchRedirect1.io.Insn := insn1
  branchRedirect1.io.Insn_PC := io.Address

  val branchRedirect2 = Module(new BranchRedirect)
  branchRedirect2.io.Insn := insn2
  branchRedirect2.io.Insn_PC := io.Address + 4.U

  io.insn_valid1 := validInstructions > 1.U
  io.pc1 := io.Address
  io.insn1 := insn1
  io.Perd_Taken1 := branchRedirect1.io.Pred_Taken
  io.Pred_Target1 := branchRedirect1.io.Pred_Target
  io.excp_valid1 := io.excp_valid
  io.excp_cause1 := io.excp_cause
  io.excpt_tval1 := io.excpt_tval

  io.insn_valid2 := !(validInstructions > 1.U && io.insn_valid1 && branchRedirect1.io.needRedirect)
  io.pc2 := Mux(io.insn_valid1,io.Address + 4.U,io.Address)
  io.insn2 := insn2
  io.Perd_Taken2 := branchRedirect2.io.Pred_Taken
  io.Pred_Target2 := branchRedirect2.io.Pred_Target
  io.excp_valid2 := io.excp_valid
  io.excp_cause2 := io.excp_cause
  io.excpt_tval2 := io.excpt_tval

  io.Redirect_ID := Mux(io.insn_valid1 && branchRedirect1.io.needRedirect, branchRedirect1.io.Redirect_ID, Mux(io.insn_valid2 && branchRedirect2.io.needRedirect,branchRedirect2.io.Redirect_ID, 0.U))
  io.Redirect_Target := Mux(io.insn_valid1 && branchRedirect1.io.needRedirect, branchRedirect1.io.Redirect_Target, Mux(io.insn_valid2 && branchRedirect2.io.needRedirect, branchRedirect2.io.Redirect_Target, 0.U))
  io.fetch_flush := (io.insn_valid1 && branchRedirect1.io.needRedirect) || (io.insn_valid2 && branchRedirect2.io.needRedirect)
}

// Generate the Verilog code
object Predecode extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Predecode)
}
