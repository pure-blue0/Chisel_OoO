import chisel3._
import chisel3.util._

class IsOlder extends Module {
  val io = IO(new Bundle {
    val tag1 = Input(UInt(4.W))
    val tag2 = Input(UInt(4.W))
    val header = Input(UInt(4.W))
    val older = Output(Bool())
  })

  // Define comparison logic
  val tag1GeHeader = io.tag1 >= io.header
  val tag2GeHeader = io.tag2 >= io.header

  // XOR to determine which comparison to use
  val comparisonCase = tag1GeHeader ^ tag2GeHeader

  // Compute 'older' based on the comparison case
  when (comparisonCase) {
    io.older := io.tag1 > io.tag2
  } .otherwise {
    io.older := io.tag1 < io.tag2
  }
}

object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new IsOlder))
}