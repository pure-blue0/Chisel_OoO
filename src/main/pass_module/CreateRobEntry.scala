import chisel3._
import chisel3.util._

// Define the Chisel module with IO ports based on the XML interface provided
class CreateRobEntry extends Module {
  val io = IO(new Bundle {
    // Inputs
    val allocCount = Input(UInt(2.W))
    val Rob_Tail = Input(UInt(4.W))
    val insn1_Function_type = Input(UInt(3.W))
    val insn1_Sub_OP = Input(UInt(4.W))
    val insn1_excp_vaild = Input(Bool())
    val insn1_excp_cause = Input(UInt(6.W))
    val insn1_excpt_tval = Input(UInt(32.W))
    val insn1_Pc = Input(UInt(32.W))
    val insn1_LSQTag = Input(UInt(4.W))
    val insn1_IsaRd = Input(UInt(5.W))
    val insn1_PhyRd = Input(UInt(6.W))
    val insn1_LPhyRd = Input(UInt(6.W))
    val insn1_ControlFlowInsn = Input(Bool())
    val Rob_NextTail = Input(UInt(4.W))
    val insn2_Function_type = Input(UInt(3.W))
    val insn2_Sub_OP = Input(UInt(4.W))
    val insn2_excp_vaild = Input(Bool())
    val insn2_excp_cause = Input(UInt(6.W))
    val insn2_excpt_tval = Input(UInt(32.W))
    val insn2_Pc = Input(UInt(32.W))
    val insn2_LSQTag = Input(UInt(4.W))
    val insn2_IsaRd = Input(UInt(5.W))
    val insn2_PhyRd = Input(UInt(6.W))
    val insn2_LPhyRd = Input(UInt(6.W))
    val insn2_ControlFlowInsn = Input(Bool())
    val m_RobState = Input(UInt(3.W))
    val m_RollBackTag = Input(UInt(4.W))
    val m_ExcpCause = Input(UInt(6.W))
    val m_ExcpTval = Input(UInt(32.W))

    // Outputs
    val ROB_EN1 = Output(Bool())
    val insn1_Entry_valid = Output(Bool())
    val insn1_Entry_done = Output(Bool())
    val insn1_Entry_isStable = Output(Bool())
    val insn1_Entry_isMisPred = Output(Bool())
    val insn1_Entry_isExcp = Output(Bool())
    val insn1_Entry_pc = Output(UInt(32.W))
    val insn1_Entry_Fu = Output(UInt(3.W))
    val insn1_Entry_LSQtag = Output(UInt(4.W))
    val insn1_Entry_isaRd = Output(UInt(5.W))
    val insn1_Entry_phyRd = Output(UInt(6.W))
    val insn1_Entry_LphyRd = Output(UInt(6.W))
    val insn1_RobTag = Output(UInt(4.W))
    val ROB_EN2 = Output(Bool())
    val insn2_Entry_valid = Output(Bool())
    val insn2_Entry_done = Output(Bool())
    val insn2_Entry_isStable = Output(Bool())
    val insn2_Entry_isMisPred = Output(Bool())
    val insn2_Entry_isExcp = Output(Bool())
    val insn2_Entry_pc = Output(UInt(32.W))
    val insn2_Entry_Fu = Output(UInt(3.W))
    val insn2_Entry_LSQtag = Output(UInt(4.W))
    val insn2_Entry_isaRd = Output(UInt(5.W))
    val insn2_Entry_phyRd = Output(UInt(6.W))
    val insn2_Entry_LphyRd = Output(UInt(6.W))
    val insn2_RobTag = Output(UInt(4.W))
    val m_RobState_new = Output(UInt(3.W))
    val m_RollBackTag_new = Output(UInt(4.W))
    val m_ExcpCause_new = Output(UInt(6.W))
    val m_ExcpTval_new = Output(UInt(32.W))
    val Flush_fetch = Output(Bool())
    val Flush_decode = Output(Bool())
    val Flush_dispatch = Output(Bool())
  })

  // By default, set the new state outputs to the current state inputs
  io.m_RobState_new := io.m_RobState
  io.m_RollBackTag_new := io.m_RollBackTag
  io.m_ExcpCause_new := io.m_ExcpCause
  io.m_ExcpTval_new := io.m_ExcpTval
  io.Flush_fetch := false.B
  io.Flush_decode := false.B
  io.Flush_dispatch := false.B

  // Initialize all outputs related to insn1 and insn2 entries to false/0
  io.ROB_EN1 := false.B
  io.insn1_Entry_valid := false.B
  io.insn1_Entry_done := false.B
  io.insn1_Entry_isStable := false.B
  io.insn1_Entry_isMisPred := false.B
  io.insn1_Entry_isExcp := false.B
  io.insn1_Entry_pc := 0.U
  io.insn1_Entry_Fu := 0.U
  io.insn1_Entry_LSQtag := 0.U
  io.insn1_Entry_isaRd := 0.U
  io.insn1_Entry_phyRd := 0.U
  io.insn1_Entry_LphyRd := 0.U
  io.insn1_RobTag := 0.U

  io.ROB_EN2 := false.B
  io.insn2_Entry_valid := false.B
  io.insn2_Entry_done := false.B
  io.insn2_Entry_isStable := false.B
  io.insn2_Entry_isMisPred := false.B
  io.insn2_Entry_isExcp := false.B
  io.insn2_Entry_pc := 0.U
  io.insn2_Entry_Fu := 0.U
  io.insn2_Entry_LSQtag := 0.U
  io.insn2_Entry_isaRd := 0.U
  io.insn2_Entry_phyRd := 0.U
  io.insn2_Entry_LphyRd := 0.U
  io.insn2_RobTag := 0.U

  // Define the enumeration values for functional types and ROB states
  val funcTypeALU = 0.U(3.W)
  val funcTypeBRU = 1.U(3.W)
  val funcTypeCSR = 2.U(3.W)
  val funcTypeLDU = 3.U(3.W)
  val funcTypeSTU = 4.U(3.W)
  val robStateIdle = 0.U(3.W)
  val robStateUndo = 1.U(3.W)
  val robStateRedo = 2.U(3.W)
  val robStateWaitForResume = 3.U(3.W)
  val robStateFlushBackend = 4.U(3.W)

  // Start of 'switch' statement for allocCount
  switch(io.allocCount) {
    is(1.U) {
      // Logic for when a single instruction is being allocated
      io.ROB_EN1 := true.B
      io.insn1_RobTag := io.Rob_Tail

      // Process instruction 1
      // Determine if instruction 1 is NOP, FENCE, or MRET
      val insn1_isNOP = (io.insn1_Function_type === funcTypeALU) && (io.insn1_IsaRd === 0.U)
      val insn1_isFence = (io.insn1_Function_type === funcTypeCSR) && (io.insn1_Sub_OP === 9.U)
      val insn1_isMret = (io.insn1_Function_type === funcTypeCSR) && (io.insn1_Sub_OP === 7.U)
      
      io.insn1_Entry_done := insn1_isNOP || io.insn1_excp_vaild || insn1_isFence || insn1_isMret
      io.insn1_Entry_isStable := io.insn1_Entry_done
      io.insn1_Entry_valid := true.B
      io.insn1_Entry_isMisPred := false.B
      io.insn1_Entry_isExcp := io.insn1_excp_vaild
      io.insn1_Entry_pc := io.insn1_Pc
      io.insn1_Entry_Fu := io.insn1_Function_type
      io.insn1_Entry_LSQtag := io.insn1_LSQTag
      io.insn1_Entry_isaRd := io.insn1_IsaRd
      io.insn1_Entry_phyRd := io.insn1_PhyRd
      io.insn1_Entry_LphyRd := io.insn1_LPhyRd

      when(io.insn1_excp_vaild) {
        io.m_RobState_new := robStateUndo
        io.m_RollBackTag_new := io.Rob_Tail
        io.m_ExcpCause_new := io.insn1_excp_cause
        io.m_ExcpTval_new := io.insn1_excpt_tval
        io.Flush_fetch := true.B
        io.Flush_decode := true.B
        io.Flush_dispatch := true.B
      } .elsewhen(io.insn1_ControlFlowInsn) {
        io.m_RobState_new := robStateUndo
        io.m_RollBackTag_new := io.Rob_Tail
        when(insn1_isMret) {
          io.m_RobState_new := robStateIdle
        }
      }
    }
    // Continue the 'switch' statement from the previous code, now handling allocCount of 2
    is(2.U) {
      // Enable writing to the ROB for both instructions
      io.ROB_EN1 := true.B
      io.ROB_EN2 := true.B

      // Assign the ROB tags for both instructions
      io.insn1_RobTag := io.Rob_Tail
      io.insn2_RobTag := io.Rob_NextTail

      // Process instruction 1
      val insn1_isNOP = (io.insn1_Function_type === funcTypeALU) && (io.insn1_IsaRd === 0.U)
      val insn1_isFence = (io.insn1_Function_type === funcTypeCSR) && (io.insn1_Sub_OP === 9.U)
      val insn1_isMret = (io.insn1_Function_type === funcTypeCSR) && (io.insn1_Sub_OP === 7.U)

      io.insn1_Entry_done := insn1_isNOP || io.insn1_excp_vaild || insn1_isFence || insn1_isMret
      io.insn1_Entry_isStable := io.insn1_Entry_done
      io.insn1_Entry_valid := true.B
      io.insn1_Entry_isMisPred := false.B
      io.insn1_Entry_isExcp := io.insn1_excp_vaild
      io.insn1_Entry_pc := io.insn1_Pc
      io.insn1_Entry_Fu := io.insn1_Function_type
      io.insn1_Entry_LSQtag := io.insn1_LSQTag
      io.insn1_Entry_isaRd := io.insn1_IsaRd
      io.insn1_Entry_phyRd := io.insn1_PhyRd
      io.insn1_Entry_LphyRd := io.insn1_LPhyRd

      // Process instruction 2 (mirroring instruction 1 logic)
      val insn2_isNOP = (io.insn2_Function_type === funcTypeALU) && (io.insn2_IsaRd === 0.U)
      val insn2_isFence = (io.insn2_Function_type === funcTypeCSR) && (io.insn2_Sub_OP === 9.U)
      val insn2_isMret = (io.insn2_Function_type === funcTypeCSR) && (io.insn2_Sub_OP === 7.U)

      io.insn2_Entry_done := insn2_isNOP || io.insn2_excp_vaild || insn2_isFence || insn2_isMret
      io.insn2_Entry_isStable := io.insn2_Entry_done
      io.insn2_Entry_valid := true.B
      io.insn2_Entry_isMisPred := false.B
      io.insn2_Entry_isExcp := io.insn2_excp_vaild
      io.insn2_Entry_pc := io.insn2_Pc
      io.insn2_Entry_Fu := io.insn2_Function_type
      io.insn2_Entry_LSQtag := io.insn2_LSQTag
      io.insn2_Entry_isaRd := io.insn2_IsaRd
      io.insn2_Entry_phyRd := io.insn2_PhyRd
      io.insn2_Entry_LphyRd := io.insn2_LPhyRd

      // Check for exceptions in instruction 1 or instruction 2
      when(io.insn1_excp_vaild) {
        io.m_RobState_new := robStateUndo
        io.m_RollBackTag_new := io.Rob_Tail
        io.m_ExcpCause_new := io.insn1_excp_cause
        io.m_ExcpTval_new := io.insn1_excpt_tval
        io.Flush_fetch := true.B
        io.Flush_decode := true.B
        io.Flush_dispatch := true.B
      } .elsewhen(io.insn1_ControlFlowInsn) {
        io.m_RobState_new := robStateUndo
        io.m_RollBackTag_new := io.Rob_Tail
        when(insn1_isMret) {
          io.m_RobState_new := robStateIdle
        }
      }

      when(io.insn2_excp_vaild) {
        io.m_RobState_new := robStateUndo
        io.m_RollBackTag_new := io.Rob_NextTail
        io.m_ExcpCause_new := io.insn2_excp_cause
        io.m_ExcpTval_new := io.insn2_excpt_tval
        io.Flush_fetch := true.B
        io.Flush_decode := true.B
        io.Flush_dispatch := true.B
      } .elsewhen(io.insn2_ControlFlowInsn) {
        io.m_RobState_new := robStateUndo
        io.m_RollBackTag_new := io.Rob_NextTail
        when(insn2_isMret) {
          io.m_RobState_new := robStateIdle
        }
      }
    }
  }
}

object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new CreateRobEntry))
}