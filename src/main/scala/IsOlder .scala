import chisel3._
import chisel3.util._

class IsOlder extends Module {
  val io = IO(new Bundle {
    val tag1 = Input(UInt(4.W))
    val tag2 = Input(UInt(4.W))
    val header = Input(UInt(4.W))
    val older = Output(Bool())
  })

  // Determine if tags are greater than or equal to the header
  val tag1GeHeader = io.tag1 >= io.header
  val tag2GeHeader = io.tag2 >= io.header

  // Compute the older logic
  // If one and only one of the tags is greater than or equal to the header, compare directly
  // Otherwise, compare inversely
  io.older := (tag1GeHeader ^ tag2GeHeader) ? (io.tag1 > io.tag2) : (io.tag1 < io.tag2)
}

