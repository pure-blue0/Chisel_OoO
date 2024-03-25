import chisel3._
import chisel3.util._

class InflighQueue extends Module {
  val io = IO(new Bundle {
    val clk = Input(Clock())
    val reset = Input(Bool())
    val pop = Input(Bool())
    val push = Input(Bool())
    val write_data = Input(UInt(95.W))
    val full = Output(Bool())
    val empty = Output(Bool())
    val read_data = Output(UInt(95.W))
    val head_ptr = Output(UInt(3.W))
    val tail_ptr = Output(UInt(3.W))
  })

  val queue = Reg(Vec(8, UInt(95.W)))
  val head = RegInit(0.U(3.W))
  val tail = RegInit(0.U(3.W))

  // Reset logic
  when(io.reset) {
    head := 0.U
    tail := 0.U
  }

  // FIFO full and empty logic
  io.full := ((tail + 1.U) % 8.U) === head
  io.empty := head === tail

  // FIFO read and write logic
  when(io.push && !io.full) {
    queue(tail) := io.write_data
    tail := (tail + 1.U) % 8.U
  }

  when(io.pop && !io.empty) {
    head := (head + 1.U) % 8.U
  }

  // Output current data at head
  io.read_data := queue(head)
  io.head_ptr := head
  io.tail_ptr := tail
}

