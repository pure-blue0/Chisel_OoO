import chisel3._
import chisel3.util._

//class GenAddress(val FetchByteWidth: Int, val iCacheAlignByte: Int) extends Module {
//  val io = IO(new Bundle {
//    val sendsuccess = Input(Bool())
//    val PcRegisterout_valid = Input(Bool())
//    val Decode_redirectTag = Input(UInt(32.W))
//    val Decode_redirect_valid = Input(Bool())
//    val IEW_redirectTag = Input(UInt(32.W))
//    val IEW_redirect_valid = Input(Bool())
//    val commit_redirectTag = Input(UInt(32.W))
//    val commit_redirect_valid = Input(Bool())
//    val PcRegister_in = Output(UInt(32.W))
//  })
//
//  // Default next PC is current PC + 4, aligned to iCacheAlignByte
//  val defaultNextPC = (io.PcRegister_in + FetchByteWidth.U) & ~(iCacheAlignByte.U - 1.U)
//
//  // Check for redirects and prioritize them
//  val redirectAddress = MuxCase(defaultNextPC, Seq(
//    io.commit_redirect_valid -> io.commit_redirectTag,
//    io.IEW_redirect_valid -> io.IEW_redirectTag,
//    io.Decode_redirect_valid -> io.Decode_redirectTag
//  ))
//
//  // Set next fetch address
//  io.PcRegister_in := Mux(io.sendsuccess, redirectAddress, defaultNextPC)
//}
import chisel3._
import chisel3.util._

import chisel3._
import chisel3.util._

class GenAddress(val FetchByteWidth: Int, val iCacheAlignByte: Int) extends Module {
  val io = IO(new Bundle {
    val sendSuccess = Input(Bool())
    val PcRegisteroutVaild = Input(Bool())
    val PcRegisterout = Input(UInt(32.W))
    val DecodeRedirectTag = Input(UInt(32.W))
    val DecodeRedirectVaild = Input(Bool())
    val IEWRedirectTag = Input(UInt(32.W))
    val IEWRedirectVaild = Input(Bool())
    val CommitRedirectTag = Input(UInt(32.W))
    val CommitRedirectVaild = Input(Bool())
    val PcRegisterIn = Output(UInt(32.W))
  })

  // Priority logic
  when(io.CommitRedirectVaild) {
    io.PcRegisterIn := io.CommitRedirectTag
  }.elsewhen(io.IEWRedirectVaild) {
    io.PcRegisterIn := io.IEWRedirectTag
  }.elsewhen(io.DecodeRedirectVaild) {
    io.PcRegisterIn := io.DecodeRedirectTag
  }.elsewhen(io.PcRegisteroutVaild) {
    io.PcRegisterIn := io.PcRegisterout
  }.otherwise {
    val alignedPC = (io.PcRegisterout & ~(iCacheAlignByte.U - 1.U)) + FetchByteWidth.U
    io.PcRegisterIn := alignedPC
  }
}



//object VerilogGen extends App {
//  println((new chisel3.stage.ChiselStage).emitVerilog(new GenAddress(8,8)))
//}