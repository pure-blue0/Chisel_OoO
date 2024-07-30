import chisel3._
import chisel3.util._

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

class DecodeQueue extends Module {
  val DecodeQueueSize = 64
  val io = IO(new Bundle {
    val reset_n = Input(Bool())
    val decode_flush_flag = Input(Bool())
    val decoded_insn1 = Input(UInt(322.W))
    val decoded_insn2 = Input(UInt(322.W))
    val decoded_insn3 = Input(UInt(322.W))
    val decoded_insn4 = Input(UInt(322.W))
    val WEN1 = Input(Bool())
    val WEN2 = Input(Bool())
    val WEN3 = Input(Bool())
    val WEN4 = Input(Bool())
    val isStalled = Input(Bool())
    val pop_count = Input(UInt(2.W))
    val entry_out1 = Output(UInt(322.W))
    val entry_out2 = Output(UInt(322.W))
    val entry_out3 = Output(UInt(322.W))
    val entry_out4 = Output(UInt(322.W))
    val available_entries = Output(UInt(8.W))
  })

  val queue = Reg(Vec(DecodeQueueSize, new DecodeQueueEntry))
  val header_ptr = RegInit(0.U(6.W))
  val tail_ptr = RegInit(0.U(6.W))
  val usage_count = RegInit(0.U(8.W))

  val insn = Wire(Vec(4, new DecodeQueueEntry))

  when (!io.reset_n || io.decode_flush_flag) {
    for (i <- 0 until DecodeQueueSize) {
      queue(i) := 0.U.asTypeOf(new DecodeQueueEntry)
    }
    header_ptr := 0.U
    tail_ptr := 0.U
    usage_count := 0.U
    for (i <- 0 until 4) {
      insn(i) := 0.U.asTypeOf(new DecodeQueueEntry)
    }
  } .otherwise {
    for (i <- 0 until 4) {
      insn(i) := 0.U.asTypeOf(new DecodeQueueEntry)
      when (!io.isStalled && i.U < usage_count - io.pop_count) {
        val header_ptr_next = (header_ptr + io.pop_count + i.U) % DecodeQueueSize.U
        insn(i) := queue(header_ptr_next)
      }
    }

    when (io.WEN1) {
      queue(tail_ptr) := io.decoded_insn1.asTypeOf(new DecodeQueueEntry)
    }
    when (io.WEN2) {
      queue((tail_ptr + io.WEN1) % DecodeQueueSize.U) := io.decoded_insn2.asTypeOf(new DecodeQueueEntry)
    }
    when (io.WEN3) {
      queue((tail_ptr + io.WEN1 + io.WEN2) % DecodeQueueSize.U) := io.decoded_insn3.asTypeOf(new DecodeQueueEntry)
    }
    when (io.WEN4) {
      queue((tail_ptr + io.WEN1 + io.WEN2 + io.WEN3) % DecodeQueueSize.U) := io.decoded_insn4.asTypeOf(new DecodeQueueEntry)
    }
    header_ptr := (header_ptr + io.pop_count) % DecodeQueueSize.U
    tail_ptr := (tail_ptr + io.WEN1 + io.WEN2 + io.WEN3 + io.WEN4) % DecodeQueueSize.U
    usage_count := usage_count - io.pop_count + io.WEN1 + io.WEN2 + io.WEN3 + io.WEN4
  }

  io.entry_out1 := insn(0).asUInt
  io.entry_out2 := insn(1).asUInt
  io.entry_out3 := insn(2).asUInt
  io.entry_out4 := insn(3).asUInt
  io.available_entries := DecodeQueueSize.U - usage_count
}

object DecodeQueue extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new DecodeQueue)
}