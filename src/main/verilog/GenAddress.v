module GenAddress(
  input         clock,
  input         reset,
  input         io_sendSuccess,
  input         io_PcRegisteroutVaild,
  input  [31:0] io_PcRegisterout,
  input  [31:0] io_DecodeRedirectTag,
  input         io_DecodeRedirectVaild,
  input  [31:0] io_IEWRedirectTag,
  input         io_IEWRedirectVaild,
  input  [31:0] io_CommitRedirectTag,
  input         io_CommitRedirectVaild,
  output [31:0] io_PcRegisterIn
);
  wire [3:0] _alignedPC_T_1 = 4'h8 - 4'h1; // @[GenAddress.scala 60:61]
  wire [3:0] _alignedPC_T_2 = ~_alignedPC_T_1; // @[GenAddress.scala 60:41]
  wire [31:0] _GEN_4 = {{28'd0}, _alignedPC_T_2}; // @[GenAddress.scala 60:39]
  wire [31:0] _alignedPC_T_3 = io_PcRegisterout & _GEN_4; // @[GenAddress.scala 60:39]
  wire [31:0] alignedPC = _alignedPC_T_3 + 32'h8; // @[GenAddress.scala 60:69]
  wire [31:0] _GEN_0 = io_PcRegisteroutVaild ? io_PcRegisterout : alignedPC; // @[GenAddress.scala 57:37 58:21 61:21]
  wire [31:0] _GEN_1 = io_DecodeRedirectVaild ? io_DecodeRedirectTag : _GEN_0; // @[GenAddress.scala 55:38 56:21]
  wire [31:0] _GEN_2 = io_IEWRedirectVaild ? io_IEWRedirectTag : _GEN_1; // @[GenAddress.scala 53:35 54:21]
  assign io_PcRegisterIn = io_CommitRedirectVaild ? io_CommitRedirectTag : _GEN_2; // @[GenAddress.scala 51:32 52:21]
endmodule
