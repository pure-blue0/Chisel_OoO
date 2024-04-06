import chisel3._
import chisel3.util._

// Define the enumeration type for instruction states
object InsnState extends ChiselEnum {
  val State_Fetch1, State_Fetch2, State_Decode, State_Dispatch, State_Issue, State_ReadOperand, State_Execute, State_WriteBack, State_Commit, State_Done = Value
}

class IssueSelect extends Module {
  val io = IO(new Bundle {
    // Inputs
    val PhyRs1_done = Input(UInt(6.W))
    val PhyRs2_done = Input(UInt(6.W))
    val Insn = Input(UInt(164.W))
    val RF_port_valid = Input(Bool())
    val Execute_fun1_busy = Input(Bool())
    val Execute_fun2_busy = Input(Bool())
    val Execute_fun3_busy = Input(Bool())
    // Outputs
    val PhyRs1 = Output(UInt(6.W))
    val PhyRs2 = Output(UInt(6.W))
    val Allocate_fun1 = Output(Bool())
    val Allocate_fun2 = Output(Bool())
    val Allocate_fun3 = Output(Bool())
    val desIndex = Output(UInt(3.W))
    val Insn_out = Output(UInt(164.W))
    val Success = Output(Bool())
  })

  // Initialize outputs
  io.PhyRs1 := io.Insn(149, 144)
  io.PhyRs2 := io.Insn(139, 134)
  io.Allocate_fun1 := false.B
  io.Allocate_fun2 := false.B
  io.Allocate_fun3 := false.B
  io.Success := false.B
  io.Insn_out := DontCare
  io.desIndex := DontCare

  // Extract function type from instruction
  val functionType = io.Insn(51, 49)

  // Check if data is ready and RF port is valid
  when(io.PhyRs1_done === "b111111".U && io.PhyRs2_done === "b111111".U && io.RF_port_valid) {
    // Check for each function type and its corresponding execution unit's busy status
    when(functionType === "b001".U) { // Check for function1
      when(!io.Execute_fun1_busy) {
        io.Allocate_fun1 := true.B
        io.desIndex := 0.U // Assuming index 0
        io.Success := true.B
        io.Insn_out := io.Insn
      }
    }.elsewhen(functionType === "b000".U) { // Check for function2
      when(!io.Execute_fun2_busy) {
        io.Allocate_fun2 := true.B
        io.desIndex := 1.U // Assuming index 1
        io.Success := true.B
        io.Insn_out := io.Insn
      }
    }.elsewhen(functionType === "b010".U) { // Additional case for function3
      when(!io.Execute_fun3_busy) {
        io.Allocate_fun3 := true.B
        io.desIndex := 2.U // Assuming index 2
        io.Success := true.B
        io.Insn_out := io.Insn
      }
    }.otherwise { // No execution unit available or function type not supported
      io.Success := false.B
      io.Insn_out := DontCare
      io.desIndex := DontCare
    }
  }.otherwise { // Data not ready or RF port not valid
    io.Success := false.B
    io.Insn_out := DontCare
    io.desIndex := DontCare
  }
}

object IssueSelectGen extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new IssueSelect())
}
