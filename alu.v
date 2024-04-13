module ALU(
  input         clock,
  input         reset,
  input  [63:0] io_Insn_operand1,
  input  [63:0] io_Insn_operand2,
  input  [3:0]  io_Sub_OP_out,
  output [63:0] io_Insn_rdresult
);
  wire [63:0] _io_Insn_rdresult_T_1 = io_Insn_operand1 + io_Insn_operand2; // @[alu.scala 35:64]
  wire [63:0] _io_Insn_rdresult_T_3 = io_Insn_operand1 - io_Insn_operand2; // @[alu.scala 36:64]
  wire [126:0] _GEN_15 = {{63'd0}, io_Insn_operand1}; // @[alu.scala 37:64]
  wire [126:0] _io_Insn_rdresult_T_5 = _GEN_15 << io_Insn_operand2[5:0]; // @[alu.scala 37:64]
  wire [63:0] _io_Insn_rdresult_T_6 = io_Insn_operand1; // @[alu.scala 38:65]
  wire [63:0] _io_Insn_rdresult_T_7 = io_Insn_operand2; // @[alu.scala 38:91]
  wire [63:0] _io_Insn_rdresult_T_10 = io_Insn_operand1 ^ io_Insn_operand2; // @[alu.scala 40:64]
  wire [63:0] _io_Insn_rdresult_T_12 = io_Insn_operand1 >> io_Insn_operand2[5:0]; // @[alu.scala 41:64]
  wire [63:0] _io_Insn_rdresult_T_16 = $signed(io_Insn_operand1) >>> io_Insn_operand2[5:0]; // @[alu.scala 42:99]
  wire [63:0] _io_Insn_rdresult_T_17 = io_Insn_operand1 | io_Insn_operand2; // @[alu.scala 43:64]
  wire [63:0] _io_Insn_rdresult_T_18 = io_Insn_operand1 & io_Insn_operand2; // @[alu.scala 44:64]
  wire [31:0] _io_Insn_rdresult_T_20 = io_Insn_operand1[31:0]; // @[alu.scala 45:72]
  wire [31:0] _io_Insn_rdresult_T_22 = io_Insn_operand2[31:0]; // @[alu.scala 45:105]
  wire [31:0] _io_Insn_rdresult_T_25 = $signed(_io_Insn_rdresult_T_20) + $signed(_io_Insn_rdresult_T_22); // @[alu.scala 45:79]
  wire [63:0] _io_Insn_rdresult_T_27 = {{32{_io_Insn_rdresult_T_25[31]}},_io_Insn_rdresult_T_25}; // @[alu.scala 45:121]
  wire [31:0] _io_Insn_rdresult_T_34 = $signed(_io_Insn_rdresult_T_20) - $signed(_io_Insn_rdresult_T_22); // @[alu.scala 46:80]
  wire [63:0] _io_Insn_rdresult_T_36 = {{32{_io_Insn_rdresult_T_34[31]}},_io_Insn_rdresult_T_34}; // @[alu.scala 46:122]
  wire [62:0] _GEN_16 = {{31'd0}, io_Insn_operand1[31:0]}; // @[alu.scala 47:72]
  wire [62:0] _io_Insn_rdresult_T_39 = _GEN_16 << io_Insn_operand2[4:0]; // @[alu.scala 47:72]
  wire [63:0] _io_Insn_rdresult_T_40 = {{1'd0}, _io_Insn_rdresult_T_39}; // @[alu.scala 47:102]
  wire [31:0] _io_Insn_rdresult_T_43 = io_Insn_operand1[31:0] >> io_Insn_operand2[4:0]; // @[alu.scala 48:79]
  wire [63:0] _io_Insn_rdresult_T_44 = {{32'd0}, _io_Insn_rdresult_T_43}; // @[alu.scala 48:109]
  wire [31:0] _io_Insn_rdresult_T_48 = $signed(_io_Insn_rdresult_T_20) >>> io_Insn_operand2[4:0]; // @[alu.scala 49:79]
  wire [63:0] _io_Insn_rdresult_T_50 = {{32{_io_Insn_rdresult_T_48[31]}},_io_Insn_rdresult_T_48}; // @[alu.scala 49:114]
  wire [63:0] _GEN_0 = 4'he == io_Sub_OP_out ? _io_Insn_rdresult_T_50 : 64'h0; // @[alu.scala 31:20 34:25 49:44]
  wire [63:0] _GEN_1 = 4'hd == io_Sub_OP_out ? _io_Insn_rdresult_T_44 : _GEN_0; // @[alu.scala 34:25 48:44]
  wire [63:0] _GEN_2 = 4'hc == io_Sub_OP_out ? _io_Insn_rdresult_T_40 : _GEN_1; // @[alu.scala 34:25 47:44]
  wire [63:0] _GEN_3 = 4'hb == io_Sub_OP_out ? _io_Insn_rdresult_T_36 : _GEN_2; // @[alu.scala 34:25 46:45]
  wire [63:0] _GEN_4 = 4'ha == io_Sub_OP_out ? _io_Insn_rdresult_T_27 : _GEN_3; // @[alu.scala 34:25 45:44]
  wire [63:0] _GEN_5 = 4'h9 == io_Sub_OP_out ? _io_Insn_rdresult_T_18 : _GEN_4; // @[alu.scala 34:25 44:44]
  wire [63:0] _GEN_6 = 4'h8 == io_Sub_OP_out ? _io_Insn_rdresult_T_17 : _GEN_5; // @[alu.scala 34:25 43:44]
  wire [63:0] _GEN_7 = 4'h7 == io_Sub_OP_out ? _io_Insn_rdresult_T_16 : _GEN_6; // @[alu.scala 34:25 42:44]
  wire [63:0] _GEN_8 = 4'h6 == io_Sub_OP_out ? _io_Insn_rdresult_T_12 : _GEN_7; // @[alu.scala 34:25 41:44]
  wire [63:0] _GEN_9 = 4'h5 == io_Sub_OP_out ? _io_Insn_rdresult_T_10 : _GEN_8; // @[alu.scala 34:25 40:44]
  wire [63:0] _GEN_10 = 4'h4 == io_Sub_OP_out ? {{63'd0}, io_Insn_operand1 < io_Insn_operand2} : _GEN_9; // @[alu.scala 34:25 39:44]
  wire [63:0] _GEN_11 = 4'h3 == io_Sub_OP_out ? {{63'd0}, $signed(_io_Insn_rdresult_T_6) < $signed(_io_Insn_rdresult_T_7
    )} : _GEN_10; // @[alu.scala 34:25 38:44]
  wire [126:0] _GEN_12 = 4'h2 == io_Sub_OP_out ? _io_Insn_rdresult_T_5 : {{63'd0}, _GEN_11}; // @[alu.scala 34:25 37:44]
  wire [126:0] _GEN_13 = 4'h1 == io_Sub_OP_out ? {{63'd0}, _io_Insn_rdresult_T_3} : _GEN_12; // @[alu.scala 34:25 36:44]
  wire [126:0] _GEN_14 = 4'h0 == io_Sub_OP_out ? {{63'd0}, _io_Insn_rdresult_T_1} : _GEN_13; // @[alu.scala 34:25 35:44]
  assign io_Insn_rdresult = _GEN_14[63:0];
endmodule