import chisel3._
import chisel3.util._

class Rcu_TryAllocate extends Module {
  val io = IO(new Bundle {
    // Define the inputs
    val insn_num = Input(UInt(3.W))
    val Rob_unuse_count = Input(UInt(4.W))
    val IntFreelist_unuse_count = Input(UInt(8.W))

    // Define the output
    val Rcu_success_count = Output(UInt(3.W))
  })

  // Step 1: Find the minimum between Rob_unuse_count and IntFreelist_unuse_count
  val minRobIntFreeList = Mux(io.Rob_unuse_count < io.IntFreelist_unuse_count, io.Rob_unuse_count, io.IntFreelist_unuse_count)

  // Step 2: Find the minimum between the result of Step 1 and insn_num
  io.Rcu_success_count := Mux(io.insn_num < minRobIntFreeList, io.insn_num, minRobIntFreeList)
}

object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new Rcu_TryAllocate))
}