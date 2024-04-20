import chisel3._
import chisel3.util._

class RollBack extends Module {
  val io = IO(new Bundle {
    // Inputs
    val Rob_header = Input(UInt(4.W))
    val Rob_tail = Input(UInt(4.W))
    val Rob_Tail_valid = Input(Bool())
    val Rob_Tail_isExcp = Input(Bool())
    val Rob_Tail_phyRd = Input(UInt(6.W))
    val Rob_Tail_LphyRd = Input(UInt(6.W))
    val Rob_Tail_isaRd = Input(UInt(5.W))
    val Rob_Tail_Function = Input(UInt(3.W))
    val Rob_Tail_LSQTag = Input(UInt(4.W))
    val Rob_before_tail = Input(UInt(4.W))
    val Rob_beforeTail_valid = Input(Bool())
    val Rob_beforeTail_isExcp = Input(Bool())
    val Rob_beforeTail_phyRd = Input(UInt(6.W))
    val Rob_beforeTail_LphyRd = Input(UInt(6.W))
    val Rob_beforeTail_isaRd = Input(UInt(5.W))
    val Rob_beforeTail_Function = Input(UInt(3.W))
    val Rob_beforeTail_LSQTag = Input(UInt(4.W))
    val m_RollBackTag = Input(UInt(4.W))
    val m_RobState = Input(UInt(3.W))

    // Outputs
    val m_RobState_new = Output(UInt(3.W))
    val Data_valid1 = Output(Bool())
    val Rob_Tail_phyRd_out = Output(UInt(6.W))
    val Rob_Tail_LphyRd_out = Output(UInt(6.W))
    val Rob_Tail_isaRd_out = Output(UInt(5.W))
    val Rob_Tail_Function_out = Output(UInt(3.W))
    val Rob_Tail_LSQTag_out = Output(UInt(4.W))
    val Data_valid2 = Output(Bool())
    val Rob_beforeTail_phyRd_out = Output(UInt(6.W))
    val Rob_beforeTail_LphyRd_out = Output(UInt(6.W))
    val Rob_beforeTail_isaRd_out = Output(UInt(5.W))
    val Rob_beforeTail_Function_out = Output(UInt(3.W))
    val Rob_beforeTail_LSQTag_out = Output(UInt(4.W))
    val Rob_Tail_update = Output(Bool())
    val Rob_beforeTail_update = Output(Bool())
  })

  // Initialization of outputs
  io.m_RobState_new := io.m_RobState
  io.Data_valid1 := false.B
  io.Rob_Tail_phyRd_out := 0.U
  io.Rob_Tail_LphyRd_out := 0.U
  io.Rob_Tail_isaRd_out := 0.U
  io.Rob_Tail_Function_out := 0.U
  io.Rob_Tail_LSQTag_out := 0.U
  io.Data_valid2 := false.B
  io.Rob_beforeTail_phyRd_out := 0.U
  io.Rob_beforeTail_LphyRd_out := 0.U
  io.Rob_beforeTail_isaRd_out := 0.U
  io.Rob_beforeTail_Function_out := 0.U
  io.Rob_beforeTail_LSQTag_out := 0.U
  io.Rob_Tail_update := false.B
  io.Rob_beforeTail_update := false.B

// Helper function to determine which tag is older
def isOlder(tag1: UInt, tag2: UInt, header: UInt): Bool = {
  val sizeOfROB  = (1 << header.getWidth) // Size of ROB inferred from header width, assuming power of 2
  val mask = sizeOfROB  - 1.U // Mask for wrapping around

  // Calculate the distance from the head for both tags, wrapping around if necessary
  val distance1 = (tag1 - header) & mask
  val distance2 = (tag2 - header) & mask

  // Tag1 is older if its distance from the head is less than tag2's distance
  distance1 < distance2
}

  // Main rollback logic for Rob_tail
  when(isOlder(io.m_RollBackTag, io.Rob_tail) || io.m_RollBackTag === io.Rob_tail) {
    io.m_RobState_new := "b100".U // Rob_WaitForResume

    when(io.Rob_Tail_valid) {
      when(io.m_RollBackTag === io.Rob_tail) {
        when(io.Rob_Tail_isExcp) {
          io.Data_valid1 := true.B
          io.Rob_Tail_phyRd_out := io.Rob_Tail_phyRd
          io.Rob_Tail_LphyRd_out := io.Rob_Tail_LphyRd
          io.Rob_Tail_isaRd_out := io.Rob_Tail_isaRd
          io.Rob_Tail_Function_out := io.Rob_Tail_Function
          io.Rob_Tail_LSQTag_out := io.Rob_Tail_LSQTag
        }.otherwise {
          io.Rob_Tail_update := true.B
        }
      }
    }
  }

  // Main rollback logic for Rob_before_tail
  when(isOlder(io.m_RollBackTag, io.Rob_before_tail) || io.m_RollBackTag === io.Rob_before_tail) {
    io.m_RobState_new := "b100".U // Rob_WaitForResume

    when(io.Rob_beforeTail_valid) {
      when(io.m_RollBackTag === io.Rob_before_tail) {
        when(io.Rob_beforeTail_isExcp) {
          io.Data_valid2 := true.B
          io.Rob_beforeTail_phyRd_out := io.Rob_beforeTail_phyRd
          io.Rob_beforeTail_LphyRd_out := io.Rob_beforeTail_LphyRd
          io.Rob_beforeTail_isaRd_out := io.Rob_beforeTail_isaRd
          io.Rob_beforeTail_Function_out := io.Rob_beforeTail_Function
          io.Rob_beforeTail_LSQTag_out := io.Rob_beforeTail_LSQTag
        }.otherwise {
          io.Rob_beforeTail_update := true.B
        }
      }
    }
  }

  // Implement the isOlder function according to the system's ROB logic
  // This will take into account the circular nature of the ROB
when(isOlder(io.m_RollBackTag, io.Rob_tail, io.Rob_header) || io.m_RollBackTag === io.Rob_tail) {
  when(io.Rob_Tail_valid) {
    io.m_RobState_new := "b100".U // Assuming Rob_WaitForResume is binary 100
    when(io.m_RollBackTag === io.Rob_tail) {
      when(io.Rob_Tail_isExcp) {
        io.Data_valid1 := true.B
        io.Rob_Tail_phyRd_out := io.Rob_Tail_phyRd
        io.Rob_Tail_LphyRd_out := io.Rob_Tail_LphyRd
        io.Rob_Tail_isaRd_out := io.Rob_Tail_isaRd
        io.Rob_Tail_Function_out := io.Rob_Tail_Function
        io.Rob_Tail_LSQTag_out := io.Rob_Tail_LSQTag
      }.otherwise {
        io.Rob_Tail_update := true.B
      }
    }
  }
}

when(isOlder(io.m_RollBackTag, io.Rob_before_tail, io.Rob_header) || io.m_RollBackTag === io.Rob_before_tail) {
  when(io.Rob_beforeTail_valid) {
    io.m_RobState_new := "b100".U // Assuming Rob_WaitForResume is binary 100
    when(io.m_RollBackTag === io.Rob_before_tail) {
      when(io.Rob_beforeTail_isExcp) {
        io.Data_valid2 := true.B
        io.Rob_beforeTail_phyRd_out := io.Rob_beforeTail_phyRd
        io.Rob_beforeTail_LphyRd_out := io.Rob_beforeTail_LphyRd
        io.Rob_beforeTail_isaRd_out := io.Rob_beforeTail_isaRd
        io.Rob_beforeTail_Function_out := io.Rob_beforeTail_Function
        io.Rob_beforeTail_LSQTag_out := io.Rob_beforeTail_LSQTag
      }.otherwise {
        io.Rob_beforeTail_update := true.B
      }
    }
  }
}

}
