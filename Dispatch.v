module Dispatch(
  input          clock,
  input          reset,
  input  [1:0]   io_DispatchCount,
  input  [127:0] io_insn1,
  input  [127:0] io_insn2,
  input  [3:0]   io_Insn1_LSQTag_in,
  input  [3:0]   io_Insn2_LSQTag_in,
  input  [3:0]   io_Insn1_RobTag_in,
  input  [3:0]   io_Insn2_RobTag_in,
  input  [5:0]   io_insn1_PhyRs1_in,
  input  [5:0]   io_insn1_PhyRs2_in,
  input  [5:0]   io_insn1_PhyRd_in,
  input  [5:0]   io_insn1_LPhyRd_in,
  input  [5:0]   io_insn2_PhyRs1_in,
  input  [5:0]   io_insn2_PhyRs2_in,
  input  [5:0]   io_insn2_PhyRd_in,
  input  [5:0]   io_insn2_LPhyRd_in,
  output [1:0]   io_Issue_num1,
  output [127:0] io_insn1_Out,
  output [3:0]   io_Insn1_LSQTag,
  output [5:0]   io_insn1_PhyRs1,
  output [5:0]   io_insn1_PhyRs2,
  output [5:0]   io_insn1_PhyRd,
  output [5:0]   io_insn1_LPhyRd,
  output [3:0]   io_Insn1_RobTag,
  output [1:0]   io_Issue_num2,
  output [127:0] io_insn2_Out,
  output [3:0]   io_Insn2_LSQTag,
  output [5:0]   io_insn2_PhyRs1,
  output [5:0]   io_insn2_PhyRs2,
  output [5:0]   io_insn2_PhyRd,
  output [5:0]   io_insn2_LPhyRd,
  output [3:0]   io_Insn2_RobTag
);
  wire [3:0] Sub_OP = io_insn1[56:53]; // @[Dispatch.scala 63:22]
  wire [2:0] Function_type = io_insn1[52:50]; // @[Dispatch.scala 64:29]
  wire [4:0] IsaRd = io_insn1[48:44]; // @[Dispatch.scala 65:21]
  wire  Insn_excp_vaild = io_insn1[89]; // @[Dispatch.scala 66:31]
  wire  _T_1 = Function_type == 3'h2; // @[Dispatch.scala 76:20]
  wire  _T_5 = Function_type == 3'h2 & (Sub_OP == 4'h9 | Sub_OP == 4'h7); // @[Dispatch.scala 76:28]
  wire  _T_6 = Insn_excp_vaild | _T_5; // @[Dispatch.scala 75:23]
  wire  _T_7 = Function_type == 3'h0; // @[Dispatch.scala 77:20]
  wire  _T_9 = Function_type == 3'h0 & IsaRd == 5'h0; // @[Dispatch.scala 77:28]
  wire  _T_10 = _T_6 | _T_9; // @[Dispatch.scala 76:67]
  wire [1:0] _GEN_0 = _T_7 | Function_type == 3'h1 ? 2'h2 : 2'h3; // @[Dispatch.scala 44:17 97:68 98:23]
  wire [1:0] _GEN_1 = Function_type == 3'h3 | Function_type == 3'h4 ? 2'h1 : _GEN_0; // @[Dispatch.scala 95:68 96:23]
  wire [1:0] _GEN_2 = _T_1 ? 2'h0 : _GEN_1; // @[Dispatch.scala 93:36 94:23]
  wire [127:0] _GEN_3 = ~_T_10 ? io_insn1 : 128'h0; // @[Dispatch.scala 45:16 85:84 86:20]
  wire [3:0] _GEN_4 = ~_T_10 ? io_Insn1_LSQTag_in : 4'h0; // @[Dispatch.scala 46:19 85:84 87:23]
  wire [5:0] _GEN_5 = ~_T_10 ? io_insn1_PhyRs1_in : 6'h0; // @[Dispatch.scala 47:19 85:84 88:23]
  wire [5:0] _GEN_6 = ~_T_10 ? io_insn1_PhyRs2_in : 6'h0; // @[Dispatch.scala 48:19 85:84 89:23]
  wire [5:0] _GEN_7 = ~_T_10 ? io_insn1_PhyRd_in : 6'h0; // @[Dispatch.scala 49:18 85:84 90:22]
  wire [5:0] _GEN_8 = ~_T_10 ? io_insn1_LPhyRd_in : 6'h0; // @[Dispatch.scala 50:19 85:84 91:23]
  wire [3:0] _GEN_9 = ~_T_10 ? io_Insn1_RobTag_in : 4'h0; // @[Dispatch.scala 51:19 85:84 92:23]
  wire [1:0] _GEN_10 = ~_T_10 ? _GEN_2 : 2'h3; // @[Dispatch.scala 101:21 85:84]
  wire [3:0] Sub_OP_1 = io_insn2[56:53]; // @[Dispatch.scala 63:22]
  wire [2:0] Function_type_1 = io_insn2[52:50]; // @[Dispatch.scala 64:29]
  wire [4:0] IsaRd_1 = io_insn2[48:44]; // @[Dispatch.scala 65:21]
  wire  Insn_excp_vaild_1 = io_insn2[89]; // @[Dispatch.scala 66:31]
  wire  _T_20 = Function_type_1 == 3'h2; // @[Dispatch.scala 76:20]
  wire  _T_24 = Function_type_1 == 3'h2 & (Sub_OP_1 == 4'h9 | Sub_OP_1 == 4'h7); // @[Dispatch.scala 76:28]
  wire  _T_25 = Insn_excp_vaild_1 | _T_24; // @[Dispatch.scala 75:23]
  wire  _T_26 = Function_type_1 == 3'h0; // @[Dispatch.scala 77:20]
  wire  _T_28 = Function_type_1 == 3'h0 & IsaRd_1 == 5'h0; // @[Dispatch.scala 77:28]
  wire  _T_29 = _T_25 | _T_28; // @[Dispatch.scala 76:67]
  wire [1:0] _GEN_19 = _T_26 | Function_type_1 == 3'h1 ? 2'h2 : 2'h3; // @[Dispatch.scala 125:68 126:23 53:17]
  wire [1:0] _GEN_20 = Function_type_1 == 3'h3 | Function_type_1 == 3'h4 ? 2'h1 : _GEN_19; // @[Dispatch.scala 123:68 124:23]
  wire [1:0] _GEN_21 = _T_20 ? 2'h0 : _GEN_20; // @[Dispatch.scala 121:36 122:23]
  wire [127:0] _GEN_22 = ~_T_29 ? io_insn2 : 128'h0; // @[Dispatch.scala 113:84 114:20 54:16]
  wire [3:0] _GEN_23 = ~_T_29 ? io_Insn2_LSQTag_in : 4'h0; // @[Dispatch.scala 113:84 115:23 55:19]
  wire [5:0] _GEN_24 = ~_T_29 ? io_insn2_PhyRs1_in : 6'h0; // @[Dispatch.scala 113:84 116:23 56:19]
  wire [5:0] _GEN_25 = ~_T_29 ? io_insn2_PhyRs2_in : 6'h0; // @[Dispatch.scala 113:84 117:23 57:19]
  wire [5:0] _GEN_26 = ~_T_29 ? io_insn2_PhyRd_in : 6'h0; // @[Dispatch.scala 113:84 118:22 58:18]
  wire [5:0] _GEN_27 = ~_T_29 ? io_insn2_LPhyRd_in : 6'h0; // @[Dispatch.scala 113:84 119:23 59:19]
  wire [3:0] _GEN_28 = ~_T_29 ? io_Insn2_RobTag_in : 4'h0; // @[Dispatch.scala 113:84 120:23 60:19]
  wire [1:0] _GEN_29 = ~_T_29 ? _GEN_21 : 2'h3; // @[Dispatch.scala 113:84 129:21]
  assign io_Issue_num1 = io_DispatchCount > 2'h0 ? _GEN_10 : 2'h3; // @[Dispatch.scala 44:17 79:32]
  assign io_insn1_Out = io_DispatchCount > 2'h0 ? _GEN_3 : 128'h0; // @[Dispatch.scala 45:16 79:32]
  assign io_Insn1_LSQTag = io_DispatchCount > 2'h0 ? _GEN_4 : 4'h0; // @[Dispatch.scala 46:19 79:32]
  assign io_insn1_PhyRs1 = io_DispatchCount > 2'h0 ? _GEN_5 : 6'h0; // @[Dispatch.scala 47:19 79:32]
  assign io_insn1_PhyRs2 = io_DispatchCount > 2'h0 ? _GEN_6 : 6'h0; // @[Dispatch.scala 48:19 79:32]
  assign io_insn1_PhyRd = io_DispatchCount > 2'h0 ? _GEN_7 : 6'h0; // @[Dispatch.scala 49:18 79:32]
  assign io_insn1_LPhyRd = io_DispatchCount > 2'h0 ? _GEN_8 : 6'h0; // @[Dispatch.scala 50:19 79:32]
  assign io_Insn1_RobTag = io_DispatchCount > 2'h0 ? _GEN_9 : 4'h0; // @[Dispatch.scala 51:19 79:32]
  assign io_Issue_num2 = io_DispatchCount == 2'h2 ? _GEN_29 : 2'h3; // @[Dispatch.scala 107:34 53:17]
  assign io_insn2_Out = io_DispatchCount == 2'h2 ? _GEN_22 : 128'h0; // @[Dispatch.scala 107:34 54:16]
  assign io_Insn2_LSQTag = io_DispatchCount == 2'h2 ? _GEN_23 : 4'h0; // @[Dispatch.scala 107:34 55:19]
  assign io_insn2_PhyRs1 = io_DispatchCount == 2'h2 ? _GEN_24 : 6'h0; // @[Dispatch.scala 107:34 56:19]
  assign io_insn2_PhyRs2 = io_DispatchCount == 2'h2 ? _GEN_25 : 6'h0; // @[Dispatch.scala 107:34 57:19]
  assign io_insn2_PhyRd = io_DispatchCount == 2'h2 ? _GEN_26 : 6'h0; // @[Dispatch.scala 107:34 58:18]
  assign io_insn2_LPhyRd = io_DispatchCount == 2'h2 ? _GEN_27 : 6'h0; // @[Dispatch.scala 107:34 59:19]
  assign io_Insn2_RobTag = io_DispatchCount == 2'h2 ? _GEN_28 : 4'h0; // @[Dispatch.scala 107:34 60:19]
endmodule
