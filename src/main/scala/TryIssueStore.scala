import chisel3._
import chisel3.util._

class TryIssueStore extends Module {
  val io = IO(new Bundle {
    val dCacheAlignByte = Input(UInt(8.W))
    val X_LEN = Input(UInt(8.W))
    val StoreQueue_empty = Input(Bool())
    val FrontQueue_commited = Input(Bool())
    val FrontQueue_state = Input(UInt(2.W))
    val FrontQueue_Address = Input(UInt(32.W))
    val FrontQueue_data = Input(UInt(64.W))
    val stqPtr = Input(UInt(4.W))
    val Sub_OP = Input(UInt(4.W))
    val Success = Output(Bool())
    val mem_opcode = Output(UInt(2.W))
    val mem_TransId = Output(UInt(4.W))
    val mem_data = Output(UInt(64.W))
    val mem_Address = Output(UInt(32.W))
    val mem_Length = Output(UInt(4.W))
    val mem_ByteMask = Output(UInt(64.W))
    val FrontQueue_update = Output(Bool())
    val FrontQueue_state_new = Output(UInt(2.W))
    val stqPtr_out = Output(UInt(4.W))
  })

  // Default signal values
  io.Success := false.B
  io.mem_opcode := 0.U
  io.mem_TransId := 0.U
  io.mem_data := 0.U
  io.mem_Address := 0.U
  io.mem_Length := 0.U
  io.mem_ByteMask := 0.U
  io.FrontQueue_update := false.B
  io.FrontQueue_state_new := 0.U
  io.stqPtr_out := 0.U

  // Main logic
  when(!io.StoreQueue_empty && io.FrontQueue_state === "b00".U && io.FrontQueue_commited) {
    val offset = io.FrontQueue_Address & (io.dCacheAlignByte - 1.U)
    io.Success := true.B
    io.mem_opcode := "b10".U // Assuming Store operation
    io.mem_TransId := io.stqPtr
    io.mem_data := io.FrontQueue_data >> offset // Simplified for demonstration
    io.mem_Address := io.FrontQueue_Address & ~(io.dCacheAlignByte - 1.U)
    io.mem_Length := io.X_LEN / 2.U // Simplified for demonstration
    io.FrontQueue_state_new := "b01".U // store_Inflight
    io.FrontQueue_update := true.B
    io.stqPtr_out := io.stqPtr

    // ByteMask logic would go here
    // Simplified for demonstration purposes
    switch(io.Sub_OP) {
    is("b0000".U) { // STU_SB
      io.mem_ByteMask := ("b0000_0001".U(8.W) << offset)(63, 0)
    }
    is("b0001".U) { // STU_SH
      io.mem_ByteMask := ("b0000_0011".U(8.W) << offset)(63, 0)
    }
    is("b0010".U) { // STU_SW
      io.mem_ByteMask := ("b0000_1111".U(8.W) << offset)(63, 0)
    }
    is("b0011".U) { // STU_SD
      io.mem_ByteMask := ("b1111_1111".U(8.W) << offset)(63, 0)
    }
  }
  }
}
