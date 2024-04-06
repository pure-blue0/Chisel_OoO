import chisel3._
import chisel3.util._

object robEntry1_Function_type extends ChiselEnum {
  val Arithmetic, Load, Store, Branch, NOP = Value
  // Add other instruction types as needed
}

// Define RobState enumeration
object m_RobState extends ChiselEnum {
  val Idle, WaitForResume, FlushBackend, Committing = Value
  // Define other states as necessary
}

object robEntry2_Function_type extends ChiselEnum {
  val Arithmetic, Load, Store, Branch, NOP = Value
  // Add other instruction types as needed
}

// Define RobState enumeration
object m_RobState_new extends ChiselEnum {
  val Idle, WaitForResume, FlushBackend, Committing = Value
  // Define other states as necessary
}

class CommitInsn extends Module {
  val io = IO(new Bundle {
    // Inputs
    val insn_num = Input(UInt(2.W))
    val Rob_Header = Input(UInt(4.W))
    val robEntry1_valid = Input(Bool())
    val robEntry1_isExcp = Input(Bool())
    val robEntry1_LphyRd_in = Input(UInt(6.W))
    val robEntry1_isaRd = Input(UInt(5.W))
    val robEntry1_phyRd_data = Input(UInt(64.W))
    val robEntry1_Function_type = Input(UInt(3.W))
    val robEntry1_LSQtag_in = Input(UInt(4.W))
    val robEntry1_pc = Input(UInt(32.W))
    val Rob_NextHeader = Input(UInt(4.W))
    val robEntry2_valid = Input(Bool())
    val robEntry2_isExcp = Input(Bool())
    val robEntry2_LphyRd_in = Input(UInt(6.W))
    val robEntry2_isaRd = Input(UInt(5.W))
    val robEntry2_phyRd_data = Input(UInt(64.W))
    val robEntry2_Function_type = Input(UInt(3.W))
    val robEntry2_LSQtag_in = Input(UInt(4.W))
    val robEntry2_pc = Input(UInt(32.W))
    val m_ExcpCause = Input(UInt(6.W))
    val m_ExcpTval = Input(UInt(32.W))
    val m_RobState = Input(UInt(3.W))
    val m_RollBackTag = Input(UInt(4.W))
    val CSR_MTVEC_data = Input(UInt(64.W))

    // Outputs
    val ISA_Free_Busy_valid1 = Output(Bool())
    val robEntry1_LphyRd_out = Output(UInt(6.W))
    val busy_done1 = Output(Bool())
    val allocated_done1 = Output(Bool())
    val isaRd1 = Output(UInt(5.W))
    val phyRd_data1 = Output(UInt(64.W))
    val commit_Load_Vaild1 = Output(Bool())
    val commit_Store_Vaild1 = Output(Bool())
    val LSQtag1_out = Output(UInt(4.W))
    val ISA_Free_Busy_valid2 = Output(Bool())
    val robEntry2_LphyRd_out = Output(UInt(6.W))
    val busy_done2 = Output(Bool())
    val allocated_done2 = Output(Bool())
    val isaRd2 = Output(UInt(5.W))
    val phyRd_data2 = Output(UInt(64.W))
    val commit_Load_Vaild2 = Output(Bool())
    val commit_Store_Vaild2 = Output(Bool())
    val LSQtag2_out = Output(UInt(4.W))
    val CSR_Write_vaild = Output(Bool())
    val CSR_MEPC_data = Output(UInt(64.W))
    val CSR_MCAUSE_data = Output(UInt(64.W))
    val CSR_MTVAL_data = Output(UInt(64.W))
    val m_RobState_new = Output(UInt(3.W))
    val needRedirect = Output(Bool())
    val Redirect_ID = Output(UInt(4.W))
    val Redirect_Target = Output(UInt(32.W))
    val fetch_flush = Output(Bool())
    val decode_flush = Output(Bool())
    val dispatch_flush = Output(Bool())
    val issue_flush = Output(Bool())
    val commit_flush = Output(Bool())
    val InstretUp = Output(UInt(2.W))
    val rob_pop_num = Output(UInt(2.W))
  })

  // Initialize all outputs to default values
  io.ISA_Free_Busy_valid1 := false.B
  io.robEntry1_LphyRd_out := 0.U
  io.busy_done1 := false.B
  io.allocated_done1 := false.B
  io.isaRd1 := 0.U
  io.phyRd_data1 := 0.U
  io.commit_Load_Vaild1 := false.B
  io.commit_Store_Vaild1 := false.B
  io.LSQtag1_out := 0.U
  io.ISA_Free_Busy_valid2 := false.B
  io.robEntry2_LphyRd_out := 0.U
  io.busy_done2 := false.B
  io.allocated_done2 := false.B
  io.isaRd2 := 0.U
  io.phyRd_data2 := 0.U
  io.commit_Load_Vaild2 := false.B
  io.commit_Store_Vaild2 := false.B
  io.LSQtag2_out := 0.U
  io.CSR_Write_vaild := false.B
  io.CSR_MEPC_data := 0.U
  io.CSR_MCAUSE_data := 0.U
  io.CSR_MTVAL_data := 0.U
  io.m_RobState_new := 0.U
  io.needRedirect := false.B
  io.Redirect_ID := 0.U
  io.Redirect_Target := 0.U
  io.fetch_flush := false.B
  io.decode_flush := false.B
  io.dispatch_flush := false.B
  io.issue_flush := false.B
  io.commit_flush := false.B
  io.InstretUp := 0.U
  io.rob_pop_num := 0.U

  when(io.insn_num === 0.U) {
    // Implement the logic to process instructions
    // This is where you would check each instruction's validity, handle exceptions, 
    // perform necessary updates and redirects, etc., based on the detailed requirements.
  }.elsewhen(io.insn_num >= 1.U) {   // Process when insn_num equals 1
    // Check if the entry is valid
    when(io.robEntry1_valid) {
      // No exception case
      when(!io.robEntry1_isExcp) {
        io.ISA_Free_Busy_valid1 := true.B
        io.busy_done1 := true.B
        io.allocated_done1 := true.B
        io.robEntry1_LphyRd_out := io.robEntry1_LphyRd_in
        io.isaRd1 := io.robEntry1_isaRd
        io.phyRd_data1 := io.robEntry1_phyRd_data

        // Determine if it's a load or store instruction
        when(io.robEntry1_Function_type === 3.U) { // Assuming 3 represents LDU
          io.commit_Load_Vaild1 := true.B
          io.LSQtag1_out := io.robEntry1_LSQtag_in
        }.elsewhen(io.robEntry1_Function_type === 4.U) { // Assuming 4 represents STU
          io.commit_Store_Vaild1 := true.B
          io.LSQtag1_out := io.robEntry1_LSQtag_in
        }
      }.otherwise {
        // Handle exception
        io.needRedirect := true.B
        io.CSR_Write_vaild := true.B
        io.CSR_MEPC_data := io.robEntry1_pc
        io.CSR_MCAUSE_data := io.m_ExcpCause
        io.CSR_MTVAL_data := io.m_ExcpTval
        io.Redirect_ID := io.CSR_MTVEC_data // Assuming Direct ID fetch for simplicity
        io.fetch_flush := true.B
        io.decode_flush := true.B
        io.dispatch_flush := true.B
        io.issue_flush := true.B
        io.commit_flush := true.B
      }
    }.elsewhen(io.insn_num === 2.U) {
    // Check if the entry is valid
    when(io.robEntry2_valid) {
      // No exception case
      when(!io.robEntry2_isExcp) {
        io.ISA_Free_Busy_valid2 := true.B
        io.busy_done2 := true.B
        io.allocated_done2 := true.B
        io.robEntry2_LphyRd_out := io.robEntry2_LphyRd_in
        io.isaRd2 := io.robEntry2_isaRd
        io.phyRd_data2 := io.robEntry2_phyRd_data

        // Determine if it's a load or store instruction
        when(io.robEntry2_Function_type === 3.U) { // Assuming 3 represents LDU
          io.commit_Load_Vaild2 := true.B
          io.LSQtag2_out := io.robEntry2_LSQtag_in
        }.elsewhen(io.robEntry2_Function_type === 4.U) { // Assuming 4 represents STU
          io.commit_Store_Vaild2 := true.B
          io.LSQtag2_out := io.robEntry2_LSQtag_in
        }
      }.otherwise {
        // Handle exception
        io.needRedirect := true.B
        io.CSR_Write_vaild := true.B
        io.CSR_MEPC_data := io.robEntry2_pc
        io.CSR_MCAUSE_data := io.m_ExcpCause
        io.CSR_MTVAL_data := io.m_ExcpTval
        io.Redirect_ID := io.CSR_MTVEC_data // Assuming Direct ID fetch for simplicity
        io.fetch_flush := true.B
        io.decode_flush := true.B
        io.dispatch_flush := true.B
        io.issue_flush := true.B
        io.commit_flush := true.B
      }
    }
}
  }
when(io.m_RobState === 1.U && io.Rob_Header === io.m_RollBackTag) { // Assuming 1 represents Rob_WaitForResume
      io.m_RobState_new := 2.U // Assuming 2 represents Rob_FlushBackend
      io.issue_flush := true.B
      io.commit_flush := true.B
    }

 when(io.robEntry1_valid && io.robEntry2_valid) {
    io.InstretUp := 2.U
  }.elsewhen(io.robEntry1_valid || io.robEntry2_valid) {
    io.InstretUp := 1.U
  }.otherwise {
    io.InstretUp := 0.U
  }

  // Set rob_pop equal to insn_num
  io.rob_pop := io.insn_num

}

