import chisel3._
import chisel3.util._
import scala.math._

class DecodeQueueEntry extends Bundle {
  val data_valid = Bool()
  val Pred_Taken = Bool()
  val Pred_Target = UInt(32.W)
  val Excp_Tval = UInt(32.W)
  val Excp_Cause = UInt(6.W)
  val Excp_valid = Bool()
  val Sub_OP = UInt(4.W)
  val Function_type = UInt(3.W)
  val ControlFlowInsn = Bool()
  val IsaRd = UInt(5.W)
  val Operand2Ready = Bool()
  val IsaRs2 = UInt(5.W)
  val Operand1Ready = Bool()
  val IsaRs1 = UInt(5.W)
  val imm = UInt(64.W)
  val Operand2 = UInt(64.W)
  val Operand1 = UInt(64.W)
  val insn = UInt(32.W)
  val pc = UInt(32.W)
}

class DecodeQueue(DecodeQueueSize: Int) extends Module {
  val io = IO(new Bundle {
    val decode_flush_flag = Input(Bool())
    val decoded_insn1 = Input(new DecodeQueueEntry)
    val decoded_insn2 = Input(new DecodeQueueEntry)
    val WEN1 = Input(Bool())
    val WEN2 = Input(Bool())
    val isStalled = Input(Bool())
    val pop_count = Input(UInt(2.W))
    val entry_out1 = Output(new DecodeQueueEntry)
    val entry_out2 = Output(new DecodeQueueEntry)
    val available_entries = Output(UInt(7.W))
  })

  val queue = Reg(Vec(DecodeQueueSize, new DecodeQueueEntry))
  val header_ptr = RegInit(0.U(6.W))
  val tail_ptr = RegInit(0.U(6.W))
  val usage_count = RegInit(0.U(7.W))

  val avail_count = DecodeQueueSize.U - usage_count
  val send_header_ptr = (header_ptr + io.pop_count) % DecodeQueueSize.U
  val next_send_header_ptr = (send_header_ptr + 1.U) % DecodeQueueSize.U

  val empty = (usage_count - io.pop_count) === 0.U
  val one_entry_flag = (usage_count - io.pop_count) === 1.U

  when(io.decode_flush_flag) {
    for (i <- 0 until DecodeQueueSize) {
      queue(i) := 0.U.asTypeOf(new DecodeQueueEntry)
    }
    header_ptr := 0.U
    tail_ptr := 0.U
    usage_count := 0.U
    io.entry_out1 := 0.U.asTypeOf(new DecodeQueueEntry)
    io.entry_out2 := 0.U.asTypeOf(new DecodeQueueEntry)
  } .otherwise {
    when(!empty && !io.isStalled) {
      io.entry_out1 := queue(send_header_ptr)
      when(!one_entry_flag) {
        io.entry_out2 := queue(next_send_header_ptr)
      } .otherwise {
        io.entry_out2 := 0.U.asTypeOf(new DecodeQueueEntry)
      }
    } .otherwise {
      io.entry_out1 := 0.U.asTypeOf(new DecodeQueueEntry)
      io.entry_out2 := 0.U.asTypeOf(new DecodeQueueEntry)
    }

    header_ptr := (header_ptr + io.pop_count) % DecodeQueueSize.U
    usage_count := usage_count - io.pop_count + io.WEN1.asUInt + io.WEN2.asUInt

    when(io.WEN1 && io.WEN2) {
      queue(tail_ptr) := io.decoded_insn1
      queue((tail_ptr+1.U) % DecodeQueueSize.U) := io.decoded_insn2
      tail_ptr := (tail_ptr + 2.U) % DecodeQueueSize.U
    }
    .elsewhen(io.WEN1) {
      queue(tail_ptr) := io.decoded_insn1
      tail_ptr := (tail_ptr + 1.U) % DecodeQueueSize.U
    }
    .elsewhen(io.WEN2) {
      queue(tail_ptr) := io.decoded_insn2
      tail_ptr := (tail_ptr + 1.U) % DecodeQueueSize.U
    }
  }
  io.available_entries := avail_count
}

object DecodeQueue extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new DecodeQueue(64))
}