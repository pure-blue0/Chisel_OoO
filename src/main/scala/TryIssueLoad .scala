import chisel3._
import chisel3.util._

class TryIssueLoad extends Module {
  val io = IO(new Bundle {
    val dCacheAlignByte = Input(UInt(32.W))
    val X_LEN = Input(UInt(32.W))
    
    val LoadQueue_empty = Input(Bool())
    val FrontQueue_addressReady = Input(Bool())
    val FrontQueue_killed = Input(Bool())
    val FrontQueue_state = Input(UInt(2.W))
    val FrontQueue_Address = Input(UInt(32.W))
    val ldqPtr = Input(UInt(4.W))
    val Sub_OP = Input(UInt(4.W))
    
    val Success = Output(Bool())
    val mem_opcode = Output(UInt(2.W))
    val mem_TransId = Output(UInt(4.W))
    val mem_Address = Output(UInt(32.W))
    val mem_Length = Output(UInt(4.W))
    val FrontQueue_update = Output(Bool())
    val FrontQueue_state_new = Output(UInt(2.W))
    val ldqPtr_out = Output(UInt(4.W))
  })

  // Initialize outputs
  io.Success := false.B
  io.mem_opcode := 0.U
  io.mem_TransId := 0.U
  io.mem_Address := 0.U
  io.mem_Length := 0.U
  io.FrontQueue_update := false.B
  io.FrontQueue_state_new := 0.U
  io.ldqPtr_out := 0.U

  // Load Issue Logic
  when(!io.LoadQueue_empty && io.FrontQueue_state === 0.U && !io.FrontQueue_killed) {
    when(io.FrontQueue_addressReady) {
      io.Success := true.B
      io.FrontQueue_update := true.B
      io.FrontQueue_state_new := 1.U // load_Inflight
      io.ldqPtr_out := io.ldqPtr
      
      io.mem_opcode := 1.U // Load
      io.mem_TransId := io.ldqPtr
      io.mem_Address := io.FrontQueue_Address & ~(io.dCacheAlignByte - 1.U)
      io.mem_Length := (io.X_LEN / 2.U)(3, 0) // Assuming X_LEN is always a multiple of 2
    }
  }
}


