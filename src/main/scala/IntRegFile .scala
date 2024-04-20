import chisel3._
import chisel3.util._

class IntRegFile extends Module {
  val Reg_Size = 40
  val io = IO(new Bundle {
    // Inputs
    val RF_Issue1_PhyRs1 = Input(UInt(6.W))
    val RF_Issue1_PhyRs2 = Input(UInt(6.W))
    val RF_Issue2_PhyRs1 = Input(UInt(6.W))
    val RF_Issue2_PhyRs2 = Input(UInt(6.W))
    val RF_Issue3_PhyRs1 = Input(UInt(6.W))
    val RF_Issue3_PhyRs2 = Input(UInt(6.W))
    
    val Forward1_valid = Input(Bool())
    val Forward1_PhyRd = Input(UInt(6.W))
    val Forward1_data = Input(UInt(64.W))
    val Forward2_valid = Input(Bool())
    val Forward2_PhyRd = Input(UInt(6.W))
    val Forward2_data = Input(UInt(64.W))
    
    val WB1_valid = Input(Bool())
    val WB1_PhyRd = Input(UInt(6.W))
    val WB1_RdResult = Input(UInt(64.W))
    val WB2_valid = Input(Bool())
    val WB2_PhyRd = Input(UInt(6.W))
    val WB2_RdResult = Input(UInt(64.W))
    
    val ROB_PhyRd1 = Input(UInt(6.W))
    val ROB_PhyRd2 = Input(UInt(6.W))

    // Outputs
    val RF_Issue1_PhyRs1_data = Output(UInt(64.W))
    val RF_Issue1_PhyRs2_data = Output(UInt(64.W))
    val RF_Issue2_PhyRs1_data = Output(UInt(64.W))
    val RF_Issue2_PhyRs2_data = Output(UInt(64.W))
    val RF_Issue3_PhyRs1_data = Output(UInt(64.W))
    val RF_Issue3_PhyRs2_data = Output(UInt(64.W))
    val CommitInsn_PhyRd1_data = Output(UInt(64.W))
    val CommitInsn_PhyRd2_data = Output(UInt(64.W))
  })

  val regFile = RegInit(VecInit(Seq.fill(Reg_Size)(0.U(64.W))))

  // Reset logic
  when(reset.asBool) {
    for (i <- 0 until Reg_Size) {
      regFile(i) := 0.U
    }
  }

  // Update logic on rising edge
  when(io.Forward1_valid) {
    regFile(io.Forward1_PhyRd) := io.Forward1_data
  }
  when(io.Forward2_valid) {
    regFile(io.Forward2_PhyRd) := io.Forward2_data
  }
  when(io.WB1_valid) {
    regFile(io.WB1_PhyRd) := io.WB1_RdResult
  }
  when(io.WB2_valid) {
    regFile(io.WB2_PhyRd) := io.WB2_RdResult
  }

  // Reading logic
  // Note: It's assumed that the read addresses are valid and within range
  io.RF_Issue1_PhyRs1_data := regFile(io.RF_Issue1_PhyRs1)
  io.RF_Issue1_PhyRs2_data := regFile(io.RF_Issue1_PhyRs2)
  io.RF_Issue2_PhyRs1_data := regFile(io.RF_Issue2_PhyRs1)
  io.RF_Issue2_PhyRs2_data := regFile(io.RF_Issue2_PhyRs2)
  io.RF_Issue3_PhyRs1_data := regFile(io.RF_Issue3_PhyRs1)
  io.RF_Issue3_PhyRs2_data := regFile(io.RF_Issue3_PhyRs2)
  io.CommitInsn_PhyRd1_data := regFile(io.ROB_PhyRd1)
  io.CommitInsn_PhyRd2_data := regFile(io.ROB_PhyRd2)
}

object IntRegFile extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new IntRegFile())
}
