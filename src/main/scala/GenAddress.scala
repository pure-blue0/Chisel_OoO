import chisel3._
import chisel3.util._

class GenAddress(val FetchByteWidth: Int, val iCacheAlignByte: Int) extends Module {
  val io = IO(new Bundle {
    val sendsuccess = Input(Bool())
    val PcRegisterout_valid = Input(Bool())
    val Decode_redirectTag = Input(UInt(32.W))
    val Decode_redirect_valid = Input(Bool())
    val IEW_redirectTag = Input(UInt(32.W))
    val IEW_redirect_valid = Input(Bool())
    val commit_redirectTag = Input(UInt(32.W))
    val commit_redirect_valid = Input(Bool())
    val PcRegister_in = Output(UInt(32.W))
  })

  // Default next PC is current PC + 4, aligned to iCacheAlignByte
  val defaultNextPC = (io.PcRegister_in + FetchByteWidth.U) & ~(iCacheAlignByte.U - 1.U)

  // Check for redirects and prioritize them
  val redirectAddress = MuxCase(defaultNextPC, Seq(
    io.commit_redirect_valid -> io.commit_redirectTag,
    io.IEW_redirect_valid -> io.IEW_redirectTag,
    io.Decode_redirect_valid -> io.Decode_redirectTag
  ))

  // Set next fetch address
  io.PcRegister_in := Mux(io.sendsuccess, redirectAddress, defaultNextPC)
}