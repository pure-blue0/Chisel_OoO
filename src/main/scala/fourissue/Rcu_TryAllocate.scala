import chisel3._
import chisel3.util._

class Rcu_TryAllocate extends Module {
  val io = IO(new Bundle {
    // Inputs
    val insn_valid0 = Input(Bool())
    val insn_valid1 = Input(Bool())
    val insn_valid2 = Input(Bool())
    val insn_valid3 = Input(Bool())
    val Rob_unuse_count = Input(UInt(4.W)) // 4-bit for available ROB entries
    val IntFreelist_unuse_count = Input(UInt(8.W)) // 8-bit for available IntFreelist entries

    // Outputs
    val Rcu_success_count = Output(UInt(3.W)) // 3-bit for success count
  })

  // Count the number of valid instructions
  val valid_count = (io.insn_valid0 +& io.insn_valid1 +& io.insn_valid2 +& io.insn_valid3) //&&

  // Determine the minimum available entries
  val available_entries = Mux(io.Rob_unuse_count < io.IntFreelist_unuse_count, io.Rob_unuse_count, io.IntFreelist_unuse_count)

  // Calculate the success count
  io.Rcu_success_count := Mux(valid_count < available_entries, valid_count, available_entries)

}



object main extends App {
  println((new chisel3.stage.ChiselStage).emitVerilog(new Rcu_TryAllocate))
}