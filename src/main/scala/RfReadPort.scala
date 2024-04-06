import chisel3._
import chisel3.util._

class RfReadPort extends Module {
  val io = IO(new Bundle {
    // Inputs
    val valid = Input(Bool())
    val insn_operand1Ready = Input(Bool())
    val insn_phyRs1_data = Input(UInt(64.W))
    val insn_operand2Ready = Input(Bool())
    val insn_phyRs2_data = Input(UInt(64.W))
    val desIndex_in = Input(UInt(3.W))
    val sub_OP_in = Input(UInt(4.W))
    val imm_in = Input(UInt(64.W))
    val pc_in = Input(UInt(64.W))
    val pred_taken_in = Input(Bool())
    val pred_target_in = Input(UInt(64.W))
    val function_type_in = Input(UInt(3.W))

    // Outputs
    val desIndex = Output(UInt(3.W))
    val insn_operand1 = Output(UInt(64.W))
    val insn_operand2 = Output(UInt(64.W))
    val sub_OP_out = Output(UInt(4.W))
    val desIndex_out = Output(UInt(3.W))
    val imm_out = Output(UInt(64.W))
    val pc_out = Output(UInt(64.W))
    val pred_taken_out = Output(Bool())
    val pred_target_out = Output(UInt(64.W))
    val function_type_out = Output(UInt(3.W))
  })

  // Implementing the functionality
  when(io.valid) {
    io.insn_operand1 := Mux(io.insn_operand1Ready, io.insn_phyRs1_data, 0.U)
    io.insn_operand2 := Mux(io.insn_operand2Ready, io.insn_phyRs2_data, 0.U)
  }

  // Direct assignments for signals that pass through
  io.desIndex := io.desIndex_in
  io.desIndex_out := io.desIndex_in
  io.sub_OP_out := io.sub_OP_in
  io.imm_out := io.imm_in
  io.pc_out := io.pc_in
  io.pred_taken_out := io.pred_taken_in
  io.pred_target_out := io.pred_target_in
  io.function_type_out := io.function_type_in
}

// Generate the Verilog code

