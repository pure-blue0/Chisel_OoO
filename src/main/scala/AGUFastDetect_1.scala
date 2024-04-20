import chisel3._
import chisel3.util._

class AGUFastDetect extends Module {
  val io = IO(new Bundle {
    // Inputs
    val RobTag_in = Input(UInt(4.W))
    val Function_type = Input(UInt(3.W))
    val Agu_addr_ready = Input(Bool())
    val Agu_data_ready = Input(Bool())
    val excp_valid_in = Input(Bool())
    val excp_cause_in = Input(UInt(6.W))
    val excpt_tval_in = Input(UInt(32.W))
    val m_RollBackTag = Input(UInt(4.W))
    val m_RobState = Input(UInt(3.W))
    val m_ExcpCause = Input(UInt(6.W))
    val m_ExcpTval = Input(UInt(32.W))
    
    // Outputs
    val ROB_update = Output(Bool())
    val isStable = Output(Bool())
    val Done = Output(Bool())
    val isExcp = Output(Bool())
    val RobTag = Output(UInt(4.W))
    val m_RollBackTag_new = Output(UInt(4.W))
    val m_RobState_new = Output(UInt(3.W))
    val m_ExcpCause_new = Output(UInt(6.W))
    val m_ExcpTval_new = Output(UInt(32.W))
  })

  // Step 1: Initialization
  io.RobTag := io.RobTag_in
  io.ROB_update := false.B
  io.isStable := false.B
  io.Done := false.B
  io.isExcp := false.B
  io.m_RollBackTag_new := io.m_RollBackTag
  io.m_RobState_new := io.m_RobState
  io.m_ExcpCause_new := io.m_ExcpCause
  io.m_ExcpTval_new := io.m_ExcpTval

  // Step 2 and 3: Update ROB and set stable
  when(io.RobTag_in === io.RobTag) {
    io.ROB_update := true.B
    io.isStable := true.B
  }

  // Step 4: Check if store instruction and address/data are ready
  when(io.Function_type === funcType_t.STU && io.Agu_addr_ready && io.Agu_data_ready) {
    io.Done := true.B
  }

  // Step 5: Check for exceptions
  when(io.excp_valid_in) {
    io.Done := true.B
    io.isExcp := true.B
    when(io.m_RobState =/= RobState.RobIdle) {
      io.m_RobState_new := RobState.RobUndo
      io.m_RollBackTag_new := io.RobTag_in
      io.m_ExcpCause_new := io.excp_cause_in
      io.m_ExcpTval_new := io.excpt_tval_in
    }
  }
}

object funcType_t {
  val ALU = 0.U(3.W)
  val BRU = 1.U(3.W)
  val CSR = 2.U(3.W)
  val LDU = 3.U(3.W)
  val STU = 4.U(3.W)
}

object RobState {
  val RobIdle = 0.U(3.W)
  val RobUndo = 1.U(3.W)
  val RobRedo = 2.U(3.W)
  val RobWaitForResume = 3.U(3.W)
  val RobFlushBackend = 4.U(3.W)
}
