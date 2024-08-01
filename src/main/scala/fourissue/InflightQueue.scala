import chisel3._
import chisel3.util._

class InflightQueueEntry(val FetchByteWidth: Int = 16) extends Bundle {
  val Address = UInt(32.W)
  val InsnByte = UInt((FetchByteWidth*8).W)
  val Excp_Valid = Bool()
  val Excp_Cause = UInt(6.W)
  val Excp_Tval = UInt(32.W)
  val Busy = Bool()
  val Killed = Bool()
}

class InflightQueue(val FetchByteWidth: Int = 16, val InflightQueueSize: Int = 16) extends Module {
  val io = IO(new Bundle {
    // val clk = Input(Clock())
    val reset_n = Input(Bool())
    val fetch_flush_flag = Input(Bool())
    val SendSuccess = Input(Bool())
    val EntryData = Input(new InflightQueueEntry())
    val mem_valid = Input(Bool())
    val mem_TransId = Input(UInt(4.W))
    val mem_Excp_valid = Input(Bool())
    val mem_Excp_Cause = Input(UInt(6.W))
    val mem_Excp_Tval = Input(UInt(32.W))
    val mem_insndata = Input(UInt((FetchByteWidth*8).W))
    val data_valid = Output(Bool())
    val Address = Output(UInt(32.W))
    val data = Output(UInt((FetchByteWidth*8).W))
    val Excp_Valid = Output(Bool())
    val Excp_Cause = Output(UInt(6.W))
    val Excp_Tval = Output(UInt(32.W))
    val tail_ptr = Output(UInt(log2Ceil(InflightQueueSize).W))

    val full = Output(Bool())
  })

  val FetchQueue = Reg(Vec(InflightQueueSize, new InflightQueueEntry()))
  val header_ptr = RegInit(0.U(log2Ceil(InflightQueueSize).W))
  val tail_ptr = RegInit(0.U(log2Ceil(InflightQueueSize).W))
  val usage_count = RegInit(0.U(log2Ceil(InflightQueueSize + 1).W))

  // Flags
  val empty = usage_count === 0.U
  val full = usage_count === InflightQueueSize.U

  //Initialization
  io.data_valid := false.B
  io.Address := 0.U
  io.data := 0.U
  io.Excp_Valid := false.B
  io.Excp_Cause := 0.U
  io.Excp_Tval := 0.U
  io.full := full

  // Reset or Flush
  when(!io.reset_n || io.fetch_flush_flag) {
    for (entry <- FetchQueue) {
      entry.Address := 0.U
      entry.InsnByte := 0.U
      entry.Excp_Valid := false.B
      entry.Excp_Cause := 0.U
      entry.Excp_Tval := 0.U
      entry.Busy := true.B
      entry.Killed := true.B
    }
    usage_count := 0.U
    header_ptr := 0.U
    tail_ptr := 0.U
    io.data_valid := false.B
    io.full := false.B
  } .otherwise {
    // Outputs
    io.data_valid := !empty && !FetchQueue(header_ptr).Busy && !FetchQueue(header_ptr).Killed
    when(io.data_valid) {
      io.Address := FetchQueue(header_ptr).Address
      io.data := FetchQueue(header_ptr).InsnByte
      io.Excp_Valid := FetchQueue(header_ptr).Excp_Valid
      io.Excp_Cause := FetchQueue(header_ptr).Excp_Cause
      io.Excp_Tval := FetchQueue(header_ptr).Excp_Tval
    }

    // Pop the entry
    when(!empty && !FetchQueue(header_ptr).Busy) {
      header_ptr := header_ptr + 1.U
      when(header_ptr === (InflightQueueSize-1).U) {
        header_ptr := 0.U
      }
    //  usage_count := usage_count - 1.U
    }

    // Push new entry
    when(io.SendSuccess || io.EntryData.Excp_Valid) {
      FetchQueue(tail_ptr) := io.EntryData
      tail_ptr := tail_ptr + 1.U
      when(tail_ptr === (InflightQueueSize-1).U) {
        tail_ptr := 0.U
      }
    //  usage_count := usage_count + 1.U
    }

    usage_count := usage_count - (!empty && !FetchQueue(header_ptr).Busy) + (io.SendSuccess || io.EntryData.Excp_Valid)

    // Handle memory response
    when(io.mem_valid) {
      FetchQueue(io.mem_TransId).Busy := false.B
      FetchQueue(io.mem_TransId).InsnByte := io.mem_insndata
      FetchQueue(io.mem_TransId).Excp_Valid := io.mem_Excp_valid
      FetchQueue(io.mem_TransId).Excp_Cause := io.mem_Excp_Cause
      FetchQueue(io.mem_TransId).Excp_Tval := io.mem_Excp_Tval
    }
  }

  // Output the tail pointer and full status
  io.tail_ptr := tail_ptr
  io.full := full
}

object InflightQueue extends App {
  (new chisel3.stage.ChiselStage).execute(args, Seq(chisel3.stage.ChiselGeneratorAnnotation(() => new InflightQueue())))
}


