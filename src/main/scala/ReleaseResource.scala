object FuncType extends Enumeration {
  val ALU, BRU, CSR, LDU, STU = Value
}

import FuncType._
import chisel3._
import chisel3.util._

class ReleaseResource extends Module {
  val io = IO(new Bundle {
    //Inputs
    val Data_valid1 = Input(Bool())
    val phyRd1 = Input(UInt(6.W))
    val LphyRd1 = Input(UInt(6.W))
    val isaRd1 = Input(UInt(5.W))
    val Function_type1 = Input(UInt(3.W))
    val LSQTag1 = Input(UInt(4.W))

    val Data_valid2 = Input(Bool())
    val phyRd2 = Input(UInt(6.W))
    val LphyRd2 = Input(UInt(6.W))
    val isaRd2 = Input(UInt(5.W))
    val Function_type2 = Input(UInt(3.W))
    val LSQTag2 = Input(UInt(4.W))

    //Outputs
    val List_valid1 = Output(Bool())
    val Load_kill_valid1 = Output(Bool())
    val Store_kill_valid1 = Output(Bool())
    val phyRd1_out = Output(UInt(6.W))
    val LphyRd1_out = Output(UInt(6.W))
    val isaRd1_out = Output(UInt(5.W))
    val LSQTag1_out = Output(UInt(4.W))

    val List_valid2 = Output(Bool())
    val Load_kill_valid2 = Output(Bool())
    val Store_kill_valid2 = Output(Bool())
    val phyRd2_out = Output(UInt(6.W))
    val LphyRd2_out = Output(UInt(6.W))
    val isaRd2_out = Output(UInt(5.W))
    val LSQTag2_out = Output(UInt(4.W))
  })

//Initialize all outputs to 0
  io.List_valid1 = false.B
  io.Load_kill_valid1 = false.B
  io.Store_kill_valid1 = false.B
  io.phyRd1_out = 0.U
  io.LphyRd1_out = 0.U
  io.isaRd1_out = 0.U
  io.LSQTag1_out = 0.U

  io.List_valid2 = false.B
  io.Load_kill_valid2 = false.B
  io.Store_kill_valid2 = false.B
  io.phyRd2_out = 0.U
  io.LphyRd2_out = 0.U
  io.isaRd2_out = 0.U
  io.LSQTag2_out = 0.U

//Logic for Data_valid1
  when(io.Data_valid1) {
    io.List_valid1 = true.B
    io.phyRd1_out = io.phyRd1
    io.LphyRd1_out = io.LphyRd1
    io.isaRd1_out = io.isaRd1
    when(io.Function_type1 === 4.U) {  Assuming LDU = 4
      io.Load_kill_valid1 = true.B
      io.LSQTag1_out = io.LSQTag1
    } .elsewhen(io.Function_type1 === 5.U) {  Assuming STU = 5
      io.Store_kill_valid1 = true.B
      io.LSQTag1_out = io.LSQTag1
    }
  }

//Logic for Data_valid2
  when(io.Data_valid2) {
    io.List_valid2 = true.B
    io.phyRd2_out = io.phyRd2
    io.LphyRd2_out = io.LphyRd2
    io.isaRd2_out = io.isaRd2
    when(io.Function_type2 === 4.U) {  Assuming LDU = 4
      io.Load_kill_valid2 = true.B
      io.LSQTag2_out = io.LSQTag2
    } .elsewhen(io.Function_type2 === 5.U) {  Assuming STU = 5
      io.Store_kill_valid2 = true.B
      io.LSQTag2_out = io.LSQTag2
    }
  }
}
