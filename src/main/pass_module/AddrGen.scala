import chisel3._
import chisel3.util._

class AddrGen extends Module {
  val io = IO(new Bundle {
    // Inputs
    val Insn_operand1 = Input(UInt(64.W))
    val imm = Input(UInt(64.W))
    val Function_type = Input(UInt(3.W))
    val Sub_OP = Input(UInt(4.W))
    // Outputs
    val Agu_addr = Output(UInt(32.W))
    val Agu_addr_ready = Output(Bool())
    val excp_valid = Output(Bool())
    val excp_cause = Output(UInt(6.W))
    val excpt_tval = Output(UInt(32.W))
  })

  // Default assignments
  io.Agu_addr := (io.Insn_operand1 + io.imm)(31, 0)
  io.Agu_addr_ready := true.B
  io.excp_valid := false.B
  io.excp_cause := 0.U
  io.excpt_tval := 0.U

  // Alignment check based on function type and specific sub-operation
  when(io.Function_type === 3.U) { // LDU
    switch(io.Sub_OP) {
      is(1.U) { // LDU_LH
        when((io.Agu_addr & 1.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 4.U // LD_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(2.U) { // LDU_LW
        when((io.Agu_addr & 3.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 4.U // LD_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(3.U) { // LDU_LD
        when((io.Agu_addr & 7.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 4.U // LD_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
    }
  }.elsewhen(io.Function_type === 4.U) { // STU
    switch(io.Sub_OP) {
      is(1.U) { // STU_SH
        when((io.Agu_addr & 1.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 6.U // ST_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(2.U) { // STU_SW
        when((io.Agu_addr & 3.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 6.U // ST_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(3.U) { // STU_SD
        when((io.Agu_addr & 7.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 6.U // ST_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
    }
  }
}

object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new AddrGen))
}