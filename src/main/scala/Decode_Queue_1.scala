import chisel3._
import chisel3.util._

class Decode_Queue extends Module {
  val io = IO(new Bundle {
    // Inputs
    val clk = Input(Clock())
    val reset = Input(Bool())
    val decoded_insn1 = Input(UInt(128.W))
    val decoded_insn2 = Input(UInt(128.W))
    val wr_en1 = Input(Bool())
    val wr_en2 = Input(Bool())
    val pop_count = Input(UInt(2.W))
    
    // Outputs
    val entry_out1 = Output(UInt(128.W))
    val entry_out2 = Output(UInt(128.W))
    val fifo_full = Output(Bool())
    val fifo_empty = Output(Bool())
    val head_ptr = Output(UInt(4.W))
    val tail_ptr = Output(UInt(4.W))
  })

  // Assuming the FIFO capacity is 16 entries
  val fifo = Reg(Vec(16, UInt(128.W)))
  val head = RegInit(0.U(4.W))
  val tail = RegInit(0.U(4.W))
  val count = RegInit(0.U(5.W)) // 5 bits to cover 0-16
  
  def isFull: Bool = count === 16.U
  def isEmpty: Bool = count === 0.U
  
  // Reset logic
  when(io.reset) {
    head := 0.U
    tail := 0.U
    count := 0.U
  }

  // Write logic
  when(io.wr_en1 && !isFull) {
    fifo(tail) := io.decoded_insn1
    tail := tail + 1.U
    count := count + 1.U
  }
  when(io.wr_en2 && !isFull && (!io.wr_en1 || (io.wr_en1 && count < 15.U))) {
    fifo(tail) := io.decoded_insn2
    tail := tail + 1.U
    count := count + 1.U
  }
  
  // Read logic (non-destructive)
  io.entry_out1 := 0.U
  io.entry_out2 := 0.U
  when(count >= 1.U) {
    io.entry_out1 := fifo(head)
  }
  when(count >= 2.U) {
    io.entry_out2 := fifo((head + 1.U) & 0xF.U)
  }
  
  // Pop logic
  when(io.pop_count === 1.U && count >= 1.U) {
    head := head + 1.U
    count := count - 1.U
  } .elsewhen(io.pop_count === 2.U && count >= 2.U) {
    head := head + 2.U
    count := count - 2.U
  }
  
  // Output logic
  io.fifo_full := isFull
  io.fifo_empty := isEmpty
  io.head_ptr := head
  io.tail_ptr := tail
}

// Generate the Verilog code
object Decode_QueueDriver extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Decode_Queue)
}
