import chisel3._
import chisel3.util._

// Define the enumeration types as Scala Enums.
object rob_state_t extends ChiselEnum {
  val Rob_Idle, Rob_Undo, Rob_Redo, Rob_WaitForResume, Rob_FlushBackend = Value
}
object funcType_t extends ChiselEnum {
  val ALU, BRU, CSR, LDU, STU = Value
}
object InsnState_t extends ChiselEnum {
  val State_Fetch1, State_Fetch2, _Decode, State_Dispatch, State_Issue, State_ReadOperand, State_Execute, State_WriteBack, State_Commit, State_Done = Value
}

// Define the WriteBack module.
class WriteBack extends Module {
  val io = IO(new Bundle {
    // Inputs
    val rob_empty = Input(Bool())
    val insn_ROBTag_in = Input(UInt(4.W))
    val rob_header = Input(UInt(4.W))
    val rob_lastest = Input(UInt(4.W))
    val excp_valid = Input(Bool())
    val excp_cause = Input(UInt(6.W))
    val excp_tval = Input(UInt(32.W))
    val Function_type = Input(UInt(3.W))
    val BruMisPred = Input(Bool())
    val BruTarget = Input(UInt(32.W))
    val m_RollBackTag = Input(UInt(4.W))
    val m_RobState = Input(rob_state_t())
    val m_ExcpCause = Input(UInt(6.W))
    val m_ExcpTval = Input(UInt(32.W))
    val isa_rd = Input(UInt(5.W))
    val PhyRd_in = Input(UInt(6.W))
    val RdResult_in = Input(UInt(64.W))

    // Outputs
    val needRedirect = Output(Bool())
    val Redirect_ID = Output(UInt(4.W))
    val Redirect_Target = Output(UInt(32.W))
    val ROB_valid = Output(Bool())
    val ROB_done = Output(Bool())
    val ROB_isstable = Output(Bool())
    val ROB_isexcp = Output(Bool())
    val insn_ROBTag_out = Output(UInt(4.W))
    val isMisPred = Output(Bool())
    val fetch_flush = Output(Bool())
    val decode_flush = Output(Bool())
    val dispatch_flush = Output(Bool())
    val Reg_busy_valid = Output(Bool())
    val PhyRd_out = Output(UInt(6.W))
    val RdResult_out = Output(UInt(64.W))
    val Reg_busy_forwarding = Output(Bool())
    val Reg_busy_done = Output(Bool())
    val m_RollBackTag_new = Output(UInt(4.W))
    val m_RobState_new = Output(rob_state_t())
    val m_ExcpCause_new = Output(UInt(6.W))
    val m_ExcpTval_new = Output(UInt(32.W))
  })

  // Initialize outputs to default values.
  io.needRedirect := false.B
  io.Redirect_ID := 0.U
  io.Redirect_Target := 0.U
  io.ROB_valid := false.B
  io.ROB_done := false.B
  io.ROB_isstable := false.B
  io.ROB_isexcp := false.B
  io.insn_ROBTag_out := 0.U
  io.isMisPred := false.B
  io.fetch_flush := false.B
  io.decode_flush := false.B
  io.dispatch_flush := false.B
  io.Reg_busy_valid := false.B
  io.PhyRd_out := 0.U
  io.RdResult_out := 0.U
  io.Reg_busy_forwarding := false.B
  io.Reg_busy_done := false.B
  io.m_RollBackTag_new := io.m_RollBackTag
  io.m_RobState_new := io.m_RobState
  io.m_ExcpCause_new := io.m_ExcpCause
  io.m_ExcpTval_new := io.m_ExcpTval

  // Logic for updating the ROB, handling mispredictions, exceptions, and redirection.
  when(!io.rob_empty && (isOlder(io.insn_ROBTag_in, io.rob_lastest, io.rob_header) || io.insn_ROBTag_in === io.rob_lastest)) {
    io.ROB_valid := true.B
    io.ROB_done := true.B
    io.ROB_isstable := true.B
    io.insn_ROBTag_out := io.insn_ROBTag_in

    when(!io.excp_valid) {
      when(io.BruMisPred) {
        io.isMisPred := true.B
        when(io.m_RobState === rob_state_t.Rob_Idle || isOlder(io.insn_ROBTag_in, io.m_RollBackTag, io.rob_header)) {
          io.m_RobState_new := rob_state_t.Rob_Undo
          io.m_RollBackTag_new := io.insn_ROBTag_in
          io.fetch_flush := true.B
          io.decode_flush := true.B
          io.dispatch_flush := true.B
          io.needRedirect := true.B
          io.Redirect_Target := io.BruTarget
          io.Redirect_ID := InsnState_t.State_Issue.asUInt
        }
      }.otherwise {
        when(io.m_RobState === rob_state_t.Rob_WaitForResume && io.insn_ROBTag_in === io.m_RollBackTag) {
          io.m_RobState_new := rob_state_t.Rob_Idle
        }
      }

      when(io.isa_rd =/= 0.U) {
        io.Reg_busy_valid := true.B
        io.PhyRd_out := io.PhyRd_in
        io.RdResult_out := io.RdResult_in
        io.Reg_busy_forwarding := false.B
        io.Reg_busy_done := true.B
      }
    }.otherwise {
      io.ROB_isexcp := true.B
      when(io.m_RobState === rob_state_t.Rob_Idle || isOlder(io.insn_ROBTag_in, io.m_RollBackTag, io.rob_header)) {
        io.m_RobState_new := rob_state_t.Rob_Undo
        io.m_RollBackTag_new := io.insn_ROBTag_in
        io.m_ExcpCause_new := io.excp_cause
        io.m_ExcpTval_new := io.excp_tval
      }
    }
  }

  // Define a method for checking if an instruction is older than the latest instruction.
def isOlder(tag1: UInt, tag2: UInt, header: UInt): Bool = {
  val wrapAround = tag2 < header // Indicates that tag2 has wrapped around
  when(wrapAround) {
    // If tag2 has wrapped and tag1 is greater than or equal to header, tag1 is older
    tag1 >= header || tag1 < tag2
  } .otherwise {
    // If tag2 has not wrapped, tag1 is older if it is less than tag2 and greater than or equal to header
    tag1 < tag2 && tag1 >= header
  }
}
}
