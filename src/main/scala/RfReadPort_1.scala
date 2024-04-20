import chisel3._
import chisel3.util._

class RfReadPort extends Module {
  val io = IO(new Bundle {
    // Define Inputs
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
    val insn_operand1_in = Input(UInt(64.W))
    val insn_operand2_in = Input(UInt(64.W))
    val robTag_in = Input(UInt(4.W))
    val lsqTag_in = Input(UInt(4.W))

    // Define Outputs
    val desIndex_out = Output(UInt(3.W))
    val insn_operand1_out = Output(UInt(64.W))
    val insn_operand2_out = Output(UInt(64.W))
    val sub_OP_out = Output(UInt(4.W))
    val imm_out = Output(UInt(64.W))
    val pc_out = Output(UInt(64.W))
    val pred_taken_out = Output(Bool())
    val pred_target_out = Output(UInt(64.W))
    val function_type_out = Output(UInt(3.W))
    val robTag_out = Output(UInt(4.W))
    val lsqTag_out = Output(UInt(4.W))
  })

  // Logic for Operand assignment
  when(io.valid) {
    io.insn_operand1_out := Mux(io.insn_operand1Ready, io.insn_phyRs1_data, io.insn_operand1_in)
    io.insn_operand2_out := Mux(io.insn_operand2Ready, io.insn_phyRs2_data, io.insn_operand2_in)

    // Logic for signal assignment to '_out' suffix
    io.desIndex_out := io.desIndex_in
    io.sub_OP_out := io.sub_OP_in
    io.imm_out := io.imm_in
    io.pc_out := io.pc_in
    io.pred_taken_out := io.pred_taken_in
    io.pred_target_out := io.pred_target_in
    io.function_type_out := io.function_type_in
    io.robTag_out := io.robTag_in
    io.lsqTag_out := io.lsqTag_in
  }.otherwise {
    io.insn_operand1_out := 0.U
    io.insn_operand2_out := 0.U
    io.desIndex_out := 0.U
    io.sub_OP_out := 0.U
    io.imm_out := 0.U
    io.pc_out := 0.U
    io.pred_taken_out := false.B
    io.pred_target_out := 0.U
    io.function_type_out := 0.U
    io.robTag_out := 0.U
    io.lsqTag_out := 0.U
  }
}

