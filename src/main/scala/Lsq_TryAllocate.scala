import chisel3._
import chisel3.util._

class LsqTryAllocate extends Module {
  val io = IO(new Bundle {
    // Inputs
    val insn_num = Input(UInt(3.W))
    val insn1_Function_type = Input(UInt(3.W))
    val insn2_Function_type = Input(UInt(3.W))
    val LoadQueue_unuse_count = Input(UInt(4.W))
    val StoreQueue_unuse_count = Input(UInt(4.W))
    // Output
    val lsq_success_count = Output(UInt(3.W))
  })

  // Constants for functional unit types
  val LDU = 3.U // Assuming '3' represents Load Unit
  val STU = 4.U // Assuming '4' represents Store Unit

  // Count how many instructions need Load/Store Units
  val loadCount = PopCount(Seq(io.insn1_Function_type === LDU, io.insn2_Function_type === LDU))
  val storeCount = PopCount(Seq(io.insn1_Function_type === STU, io.insn2_Function_type === STU))

  // Determine the success count based on available entries and needed entries
  when(loadCount > io.LoadQueue_unuse_count) {
    io.lsq_success_count := io.LoadQueue_unuse_count
  } .elsewhen(storeCount > io.StoreQueue_unuse_count) {
    io.lsq_success_count := io.StoreQueue_unuse_count
  } .otherwise {
    io.lsq_success_count := io.insn_num
  }
}

// Generate the Verilog code
object LsqTryAllocateGen extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new LsqTryAllocate())
}
