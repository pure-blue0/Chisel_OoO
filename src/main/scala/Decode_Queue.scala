import chisel3._
import chisel3.util._

class Decode_Queue extends Module {
  val io = IO(new Bundle {
    val clk = Input(Clock())
    val reset = Input(Bool())
    val decoded_insn1 = Input(UInt(96.W))
    val decoded_insn2 = Input(UInt(96.W))
    val wr_en1 = Input(Bool())
    val wr_en2 = Input(Bool())
    val pop_count = Input(UInt(2.W))
    val entry_out1 = Output(UInt(96.W))
    val entry_out2 = Output(UInt(96.W))
    val fifo_full = Output(Bool())
    val fifo_empty = Output(Bool())
    val head_ptr = Output(UInt(4.W))
    val tail_ptr = Output(UInt(4.W))
    val out_count = Output(UInt(2.W))
  })

  // Assuming a FIFO depth of 16 entries
  val fifoDepth = 16
  val fifo = RegInit(VecInit(Seq.fill(fifoDepth)(0.U(96.W))))
  val head = RegInit(0.U(4.W))
  val tail = RegInit(0.U(4.W))
  val count = RegInit(0.U(5.W)) // 5 bits to represent 0-16

  // Reset logic
  when(io.reset) {
    head := 0.U
    tail := 0.U
    count := 0.U
  }

  // Write logic
  when(io.wr_en1 && !io.fifo_full) {
    fifo(tail) := io.decoded_insn1
    tail := tail + 1.U
    count := count + 1.U
  }
  when(io.wr_en2 && !io.fifo_full) {
    fifo(tail) := io.decoded_insn2
    tail := tail + 1.U
    count := count + 1.U
  }

  // Read logic
  io.entry_out1 := 0.U
  io.entry_out2 := 0.U
  when(io.pop_count >= 1.U && !io.fifo_empty) {
    io.entry_out1 := fifo(head)
    head := head + 1.U
    count := count - 1.U
  }
  when(io.pop_count === 2.U && !io.fifo_empty) {
    io.entry_out2 := fifo(head)
    head := head + 1.U
    count := count - 1.U
  }

  // Status signals
  io.fifo_full := count === fifoDepth.U
  io.fifo_empty := count === 0.U
  io.head_ptr := head
  io.tail_ptr := tail
  io.out_count := Mux(io.pop_count > count, count, io.pop_count)

  // Clock not directly used since Chisel handles it
}


