import chisel3._
import chisel3.util._

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
    val Allocate_fun1 = Output(Bool())
    val Allocate_fun2 = Output(Bool())
    val Allocate_fun3 = Output(Bool())
    val desIndex = Output(UInt(3.W))
    val Insn_out = Output(UInt(164.W))
    val Success = Output(Bool())
  })

  // Defaulting all outputs to 0
  io.Allocate_fun1 := false.B
  io.Allocate_fun2 := false.B
  io.Allocate_fun3 := false.B
  io.desIndex := 0.U
  io.Insn_out := 0.U
  io.Success := false.B

  // Constants (Parameters)
  val supportFunction1 = "b001".U
  val supportFunction2 = "b000".U
  val supportFunction3 = "b000".U
  val indexFunction1 = "b010".U
  val indexFunction2 = "b011".U
  val indexFunction3 = "b100".U

  // Check if both physical registers have received data
  when(io.PhyRs1_done.andR && io.PhyRs2_done.andR) { // .andR is reduction AND, equivalent to checking if all bits are 1
    // Check if the RF port is valid
    when(io.RF_port_valid) {
      // Extract function type from instruction
      val functionType = io.Insn(51, 49)
      
      // Match the function type with support functions and check if the corresponding execution unit is not busy
      when(functionType === supportFunction1 && !io.Execute_fun1_busy) {
        io.Allocate_fun1 := true.B
        io.desIndex := indexFunction1
        io.Success := true.B
        io.Insn_out := io.Insn
      }.elsewhen(functionType === supportFunction2 && !io.Execute_fun2_busy) {
        io.Allocate_fun2 := true.B
        io.desIndex := indexFunction2
        io.Success := true.B
        io.Insn_out := io.Insn
      }.elsewhen(functionType === supportFunction3 && !io.Execute_fun3_busy) {
        io.Allocate_fun3 := true.B
        io.desIndex := indexFunction3
        io.Success := true.B
        io.Insn_out := io.Insn
      }
    }
  }
}

