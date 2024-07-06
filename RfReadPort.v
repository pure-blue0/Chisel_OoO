module RfReadPort(
  input         clock,
  input         reset,
  input         io_Valid,
  input         io_Insn_Operand1Ready,
  input  [63:0] io_Insn_PhyRs1_data,
  input         io_Insn_Operand2Ready,
  input  [63:0] io_Insn_PhyRs2_data,
  input  [2:0]  io_desIndex_in,
  input  [3:0]  io_Sub_OP_in,
  input  [63:0] io_imm_in,
  input  [63:0] io_pc_in,
  input         io_Pred_taken_in,
  input  [63:0] io_pred_target_in,
  input  [2:0]  io_Function_type_in,
  input  [63:0] io_Insn_operand1_in,
  input  [63:0] io_Insn_operand2_in,
  input  [3:0]  io_RobTag_in,
  input  [3:0]  io_LSQTag_in,
  output        io_Out_valid,
  output [2:0]  io_desIndex,
  output [63:0] io_Insn_operand1,
  output [63:0] io_Insn_operand2,
  output [3:0]  io_Sub_OP_out,
  output [2:0]  io_desIndex_out,
  output [63:0] io_imm_out,
  output [63:0] io_pc_out,
  output        io_Pred_taken_out,
  output [63:0] io_pred_target_out,
  output [2:0]  io_Function_type_out,
  output [3:0]  io_RobTag_out,
  output [3:0]  io_LSQTag_out
);
  wire [63:0] _io_Insn_operand1_T = io_Insn_Operand1Ready ? io_Insn_operand1_in : io_Insn_PhyRs1_data; // @[RfReadPort.scala 45:28]
  wire [63:0] _io_Insn_operand2_T = io_Insn_Operand2Ready ? io_Insn_operand2_in : io_Insn_PhyRs2_data; // @[RfReadPort.scala 48:28]
  assign io_Out_valid = io_Valid; // @[RfReadPort.scala 40:18 42:18 62:18]
  assign io_desIndex = io_Valid ? io_desIndex_in : 3'h0; // @[RfReadPort.scala 40:18 51:17 64:17]
  assign io_Insn_operand1 = io_Valid ? _io_Insn_operand1_T : 64'h0; // @[RfReadPort.scala 40:18 45:22 65:22]
  assign io_Insn_operand2 = io_Valid ? _io_Insn_operand2_T : 64'h0; // @[RfReadPort.scala 40:18 48:22 66:22]
  assign io_Sub_OP_out = io_Valid ? io_Sub_OP_in : 4'h0; // @[RfReadPort.scala 40:18 52:19 67:19]
  assign io_desIndex_out = io_Valid ? io_desIndex_in : 3'h0; // @[RfReadPort.scala 40:18 51:17 64:17]
  assign io_imm_out = io_Valid ? io_imm_in : 64'h0; // @[RfReadPort.scala 40:18 54:16 69:16]
  assign io_pc_out = io_Valid ? io_pc_in : 64'h0; // @[RfReadPort.scala 40:18 55:15 70:15]
  assign io_Pred_taken_out = io_Valid & io_Pred_taken_in; // @[RfReadPort.scala 40:18 56:23 71:23]
  assign io_pred_target_out = io_Valid ? io_pred_target_in : 64'h0; // @[RfReadPort.scala 40:18 57:24 72:24]
  assign io_Function_type_out = io_Valid ? io_Function_type_in : 3'h0; // @[RfReadPort.scala 40:18 58:26 73:26]
  assign io_RobTag_out = io_Valid ? io_RobTag_in : 4'h0; // @[RfReadPort.scala 40:18 59:19 74:19]
  assign io_LSQTag_out = io_Valid ? io_LSQTag_in : 4'h0; // @[RfReadPort.scala 40:18 60:19 75:19]
endmodule
