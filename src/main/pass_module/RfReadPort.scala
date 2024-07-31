import chisel3._
import chisel3.util._

class RfReadPort extends Module {
  val io = IO(new Bundle {
    // Inputs
    val Valid = Input(Bool())
    val Insn_Operand1Ready = Input(Bool())
    val Insn_PhyRs1_data = Input(UInt(64.W))
    val Insn_Operand2Ready = Input(Bool())
    val Insn_PhyRs2_data = Input(UInt(64.W))
    val desIndex_in = Input(UInt(3.W))
    val Sub_OP_in = Input(UInt(4.W))
    val imm_in = Input(UInt(64.W))
    val pc_in = Input(UInt(64.W))
    val Pred_taken_in = Input(Bool())
    val pred_target_in = Input(UInt(64.W))
    val Function_type_in = Input(UInt(3.W))
    val Insn_operand1_in = Input(UInt(64.W))
    val Insn_operand2_in = Input(UInt(64.W))
    val RobTag_in = Input(UInt(4.W))
    val LSQTag_in = Input(UInt(4.W))

    // Outputs
    val Out_valid = Output(Bool())
    val desIndex = Output(UInt(3.W))
    val Insn_operand1 = Output(UInt(64.W))
    val Insn_operand2 = Output(UInt(64.W))
    val Sub_OP_out = Output(UInt(4.W))
    val desIndex_out = Output(UInt(3.W))
    val imm_out = Output(UInt(64.W))
    val pc_out = Output(UInt(64.W))
    val Pred_taken_out = Output(Bool())
    val pred_target_out = Output(UInt(64.W))
    val Function_type_out = Output(UInt(3.W))
    val RobTag_out = Output(UInt(4.W))
    val LSQTag_out = Output(UInt(4.W))
  })

  when(io.Valid) {
    // Set Out_valid to true
    io.Out_valid := true.B

    // Operand1 assignment
    io.Insn_operand1 := Mux(io.Insn_Operand1Ready, io.Insn_operand1_in, io.Insn_PhyRs1_data)

    // Operand2 assignment
    io.Insn_operand2 := Mux(io.Insn_Operand2Ready, io.Insn_operand2_in, io.Insn_PhyRs2_data)

    // Assign other inputs to outputs directly
    io.desIndex := io.desIndex_in
    io.Sub_OP_out := io.Sub_OP_in
    io.desIndex_out := io.desIndex_in
    io.imm_out := io.imm_in
    io.pc_out := io.pc_in
    io.Pred_taken_out := io.Pred_taken_in
    io.pred_target_out := io.pred_target_in
    io.Function_type_out := io.Function_type_in
    io.RobTag_out := io.RobTag_in
    io.LSQTag_out := io.LSQTag_in
  }.otherwise {
    io.Out_valid := false.B
    // Potentially initialize outputs to known values in the case of invalid input
    io.desIndex := 0.U
    io.Insn_operand1 := 0.U
    io.Insn_operand2 := 0.U
    io.Sub_OP_out := 0.U
    io.desIndex_out := 0.U
    io.imm_out := 0.U
    io.pc_out := 0.U
    io.Pred_taken_out := false.B
    io.pred_target_out := 0.U
    io.Function_type_out := 0.U
    io.RobTag_out := 0.U
    io.LSQTag_out := 0.U
  }
}



object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new RfReadPort))
}

