import chisel3._
import chisel3.util._

// Define the WriteBack module
class WriteBack extends Module {
  val io = IO(new Bundle {
    // Inputs
    val rob_empty = Input(Bool())
    val insn_ROBTag_in = Input(UInt(4.W))
    val rob_header = Input(UInt(4.W))
    val rob_lastest = Input(UInt(4.W))
    val excp_vaild = Input(Bool())
    val excp_cause = Input(UInt(6.W))
    val excp_tval = Input(UInt(32.W))
    val Function_type = Input(UInt(3.W))
    val BruMisPred = Input(Bool())
    val BruTarget = Input(UInt(32.W))
    val m_RollBackTag = Input(UInt(4.W))
    val m_RobState = Input(UInt(3.W))
    val m_ExcpCause = Input(UInt(6.W))
    val m_ExcpTval = Input(UInt(32.W))
    val isa_rd = Input(UInt(5.W))
    val PhyRd_in = Input(UInt(6.W))
    val RdResult_in = Input(UInt(64.W))
    
    // Outputs
    val needRedirect = Output(Bool())
    val Redirect_ID = Output(UInt(4.W))
    val Redirect_Target = Output(UInt(32.W))
    val ROB_vaild = Output(Bool())
    val ROB_done = Output(Bool())
    val ROB_isstable = Output(Bool())
    val ROB_isexcp = Output(Bool())
    val insn_ROBTag_out = Output(UInt(4.W))
    val isMisPred = Output(Bool())
    val fetch_flush = Output(Bool())
    val decode_flush = Output(Bool())
    val dispatch_flush = Output(Bool())
    val Reg_busy_vaild = Output(Bool())
    val PhyRd_out = Output(UInt(6.W))
    val RdResult_out = Output(UInt(64.W))
    val Reg_busy_forwarding = Output(Bool())
    val Reg_busy_done = Output(Bool())
    val m_RollBackTag_new = Output(UInt(4.W))
    val m_RobState_new = Output(UInt(3.W))
    val m_ExcpCause_new = Output(UInt(6.W))
    val m_ExcpTval_new = Output(UInt(32.W))
  })

  // Initialize default values for outputs
  io.needRedirect := false.B
  io.Redirect_ID := 0.U
  io.Redirect_Target := 0.U
  io.ROB_vaild := false.B
  io.ROB_done := false.B
  io.ROB_isstable := false.B
  io.ROB_isexcp := false.B
  io.insn_ROBTag_out := 0.U
  io.isMisPred := false.B
  io.fetch_flush := false.B
  io.decode_flush := false.B
  io.dispatch_flush := false.B
  io.Reg_busy_vaild := false.B
  io.PhyRd_out := 0.U
  io.RdResult_out := 0.U
  io.Reg_busy_forwarding := false.B
  io.Reg_busy_done := false.B
  io.m_RollBackTag_new := io.m_RollBackTag
  io.m_RobState_new := io.m_RobState
  io.m_ExcpCause_new := io.m_ExcpCause
  io.m_ExcpTval_new := io.m_ExcpTval

  // Main logic
  when(!io.rob_empty && (io.insn_ROBTag_in === io.rob_lastest || io.insn_ROBTag_in < io.rob_lastest)) {
    // Update ROB as valid
    io.ROB_vaild := true.B
    io.ROB_done := true.B
    io.ROB_isstable := true.B
    io.insn_ROBTag_out := io.insn_ROBTag_in

    // Handling exceptions
    when(io.excp_vaild) {
      io.ROB_isexcp := true.B
      io.m_RobState_new := 1.U // Assuming 1 represents Rob_Undo
      io.m_RollBackTag_new := io.insn_ROBTag_in
      io.m_ExcpCause_new := io.excp_cause
      io.m_ExcpTval_new := io.excp_tval
      io.fetch_flush := true.B
      io.decode_flush := true.B
      io.dispatch_flush := true.B
    }.otherwise {
      // Branch misprediction logic
      when(io.BruMisPred) {
        io.isMisPred := true.B
        io.needRedirect := true.B
        io.Redirect_Target := io.BruTarget
        io.fetch_flush := true.B
        io.decode_flush := true.B
        io.dispatch_flush := true.B
        io.m_RobState_new := 1.U // Assuming 1 represents Rob_Undo
        io.m_RollBackTag_new := io.insn_ROBTag_in
      }
    }

    // Register update logic
    when(io.isa_rd =/= 0.U) {
      io.Reg_busy_vaild := true.B
      io.PhyRd_out := io.PhyRd_in
      io.RdResult_out := io.RdResult_in
      io.Reg_busy_done := true.B
      io.Reg_busy_forwarding := false.B
    }
  }
  
  // Additional logic based on detailed functionality can be added here
}

