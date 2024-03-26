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
  
  // Constants for function types
  val loadFuncType = 1.U // Assuming 1 represents LDU
  val storeFuncType = 2.U // Assuming 2 represents STU
  
  // Counting Load and Store instructions
  val loadCount = (io.insn1_Function_type === loadFuncType).asUInt + 
                  (io.insn2_Function_type === loadFuncType).asUInt
  val storeCount = (io.insn1_Function_type === storeFuncType).asUInt + 
                   (io.insn2_Function_type === storeFuncType).asUInt
  
  // Calculating possible success counts for load and store separately
  val possibleLdqCount = Mux(io.LoadQueue_unuse_count < loadCount, io.LoadQueue_unuse_count, loadCount)
  val possibleStqCount = Mux(io.StoreQueue_unuse_count < storeCount, io.StoreQueue_unuse_count, storeCount)
  
  // Calculating final success count
  val totalPossible = possibleLdqCount + possibleStqCount
  io.lsq_success_count := Mux(totalPossible < io.insn_num, totalPossible, io.insn_num)
}

// Generate the Verilog code
object LsqTryAllocateGen extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new LsqTryAllocate())
}
