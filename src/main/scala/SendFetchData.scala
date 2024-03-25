import chisel3._

class SendFetchData extends Module {
  val io = IO(new Bundle {
    // Inputs
    val Inflightqueue_empty = Input(Bool())
    val busy = Input(Bool())
    val Killed = Input(Bool())
    val Address = Input(UInt(32.W))
    val Excp_vaild = Input(Bool())
    val Excp_Cause = Input(UInt(6.W))
    val Excp_Tval = Input(UInt(32.W))
    val InsnByte = Input(UInt(64.W))

    // Outputs
    val pop_flag = Output(Bool())
    val Out_Address = Output(UInt(32.W))
    val Out_Excp_vaild = Output(Bool())
    val Out_Excp_Cause = Output(UInt(6.W))
    val Out_Excp_Tval = Output(UInt(32.W))
    val Out_InsnByte = Output(UInt(64.W))
  })

  // Initial assignment to outputs
  io.pop_flag := false.B
  io.Out_Address := 0.U
  io.Out_Excp_vaild := false.B
  io.Out_Excp_Cause := 0.U
  io.Out_Excp_Tval := 0.U
  io.Out_InsnByte := 0.U

  // Logic for pop_flag and outputs based on the input conditions
  when(!io.Inflightqueue_empty && !io.busy) {
    when(!io.Killed) {
      io.Out_Address := io.Address
      io.Out_InsnByte := io.InsnByte
      io.Out_Excp_vaild := io.Excp_vaild
      io.Out_Excp_Cause := io.Excp_Cause
      io.Out_Excp_Tval := io.Excp_Tval
    }
    io.pop_flag := true.B
  } .otherwise {
    // If none of the conditions are met, all output ports are set to 0/default.
    io.pop_flag := false.B
    io.Out_Address := 0.U
    io.Out_Excp_vaild := false.B
    io.Out_Excp_Cause := 0.U
    io.Out_Excp_Tval := 0.U
    io.Out_InsnByte := 0.U
  }
}

