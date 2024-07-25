import chisel3._

class GenAddress(FetchByteWidth: Int, ICacheAlignByte: Int) extends Module {
  val io = IO(new Bundle {
    val SendSuccess = Input(Bool())
    val pcregister_out = Input(UInt(32.W))
    val PCRegOut_valid = Input(Bool())
    val decode_redirect_target = Input(UInt(32.W))
    val decode_redirect_valid = Input(Bool())
    val IEW_redirect_target = Input(UInt(32.W))
    val IEW_redirect_valid = Input(Bool())
    val commit_redirect_target = Input(UInt(32.W))
    val commit_redirect_valid = Input(Bool())
    val pcregister_in = Output(UInt(32.W))
  })

  // 默认输出为上一周期的PC值
  io.pcregister_in := io.pcregister_out

  // 根据SendSuccess和PCRegOut_valid的状态调整PC地址
  when(io.commit_redirect_valid) {
    io.pcregister_in := io.commit_redirect_target
  } .elsewhen(io.IEW_redirect_valid) {
    io.pcregister_in := io.IEW_redirect_target
  } .elsewhen(io.decode_redirect_valid) {
    io.pcregister_in := io.decode_redirect_target
  } .elsewhen(io.SendSuccess) {
    // 对齐到ICacheAlignByte，然后加上FetchByteWidth
    io.pcregister_in := ((io.pcregister_out & (~((ICacheAlignByte - 1).U(32.W)))) + FetchByteWidth.U(32.W))
  } .elsewhen(io.PCRegOut_valid) {
    io.pcregister_in := io.pcregister_out
  }
}


object VerilogGen extends App {
  println((new chisel3.stage.ChiselStage).emitVerilog(new GenAddress(8,8)))
}

