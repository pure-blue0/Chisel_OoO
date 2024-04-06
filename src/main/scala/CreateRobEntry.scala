import chisel3._
import chisel3.util._

class CreateRobEntry extends Module {
  val io = IO(new Bundle {
    // Inputs
    val Rob_tail = Input(UInt(4.W))
    val insn_Function_type = Input(UInt(3.W))
    val Sub_OP = Input(UInt(4.W))
    val excp_valid = Input(Bool())
    val excp_cause = Input(UInt(6.W))
    val excpt_tval = Input(UInt(32.W))
    val insn_Pc = Input(UInt(32.W))
    val LSQTag = Input(UInt(4.W))
    val IsaRd = Input(UInt(5.W))
    val PhyRd = Input(UInt(6.W))
    val LPhyRd = Input(UInt(6.W))
    val ControlFlowInsn = Input(Bool())
    val Insn_valid = Input(Bool())
    
    // Outputs
    val Entry_valid = Output(Bool())
    val Entry_done = Output(Bool())
    val Entry_isStable = Output(Bool())
    val Entry_isMisPred = Output(Bool())
    val Entry_isExcp = Output(Bool())
    val Entry_pc = Output(UInt(32.W))
    val Entry_Fu = Output(UInt(3.W))
    val Entry_LSQtag = Output(UInt(4.W))
    val Entry_isaRd = Output(UInt(5.W))
    val Entry_phyRd = Output(UInt(6.W))
    val Entry_LphyRd = Output(UInt(6.W))
    val RobState = Output(UInt(3.W))
    val RollBackTag = Output(UInt(4.W))
    val ExcpCause = Output(UInt(6.W))
    val ExcpTval = Output(UInt(32.W))
    val Flush_fetch = Output(Bool())
    val Flush_decode = Output(Bool())
    val Flush_dispatch = Output(Bool())
    val ROB_EN = Output(Bool())
  })

  // NOP, FENCE, MRET checks
  val isNop = (io.insn_Function_type === 0.U) && (io.IsaRd === 0.U) // Assuming ALU type = 0 for NOP
  val isFence = (io.insn_Function_type === 3.U) && (io.Sub_OP === 9.U) // Assuming CSR type = 3 for FENCE
  val isMret = (io.insn_Function_type === 3.U) && (io.Sub_OP === 7.U) // Assuming CSR type = 3 for MRET

  // Entry done condition
  io.Entry_done := isNop || io.excp_valid || isFence || isMret
  io.Entry_isStable := io.Entry_done
  io.Entry_isMisPred := false.B // Assuming misprediction handling is external
  io.Entry_isExcp := io.excp_valid

  // Setting entry fields
  io.Entry_valid := io.Insn_valid && (isNop || isFence || isMret || io.excp_valid || io.ControlFlowInsn)
  io.Entry_pc := io.insn_Pc
  io.Entry_Fu := io.insn_Function_type
  io.Entry_LSQtag := io.LSQTag
  io.Entry_isaRd := io.IsaRd
  io.Entry_phyRd := io.PhyRd
  io.Entry_LphyRd := io.LPhyRd

  // Handling ROB state based on exception or control flow instruction
  when(io.excp_valid) {
    io.RobState := 1.U // Rob_Undo
    io.RollBackTag := io.Rob_tail
    io.ExcpCause := io.excp_cause
    io.ExcpTval := io.excpt_tval
    io.Flush_fetch := true.B
    io.Flush_decode := true.B
    io.Flush_dispatch := true.B
  }.elsewhen(io.ControlFlowInsn) {
    io.RobState := 1.U // Rob_Undo
    io.RollBackTag := io.Rob_tail
    when(isMret) {
      io.RobState := 0.U // Rob_Idle
    }
    io.Flush_fetch := false.B
    io.Flush_decode := false.B
    io.Flush_dispatch := false.B
  }.otherwise {
    io.RobState := 0.U // Assuming idle as default
    io.Flush_fetch := false.B
    io.Flush_decode := false.B
    io.Flush_dispatch := false.B
  }

  // ROB_EN signal logic
  io.ROB_EN := io.Insn_valid
}

object CreateRobEntry extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new CreateRobEntry())
}
