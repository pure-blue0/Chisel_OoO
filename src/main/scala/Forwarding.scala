import chisel3._
import chisel3.util._

class Forwarding extends Module {
  val io = IO(new Bundle {
    val data_valid = Input(Bool())
    val isa_rd = Input(UInt(5.W))
    val PhyRd_in = Input(UInt(6.W))
    val RdResult_in = Input(UInt(64.W))
    val update_valid = Output(Bool())
    val forwarding = Output(Bool())
    val done = Output(Bool())
    val RdResult_out = Output(UInt(64.W))
    val PhyRd_out = Output(UInt(6.W))
  })

  // Initialize all outputs to 0/false
  io.update_valid := false.B
  io.forwarding := false.B
  io.done := false.B
  io.RdResult_out := 0.U
  io.PhyRd_out := 0.U

  // Check if the data is valid
  when(io.data_valid) {
    // Check if isa_rd is not 0, indicating a valid target register is present
    when(io.isa_rd =/= 0.U) {
      // Set outputs accordingly
      io.update_valid := true.B
      io.forwarding := true.B
      io.done := true.B
      io.RdResult_out := io.RdResult_in
      io.PhyRd_out := io.PhyRd_in
    }
  }
}

// Generate the Verilog code
object Forwarding extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Forwarding())
}
