module SendFetchData(
  input         clock,
  input         reset,
  input         io_Inflightqueue_empty,
  input         io_busy,
  input         io_Killed,
  input  [31:0] io_Address,
  input         io_Excp_vaild,
  input  [5:0]  io_Excp_Cause,
  input  [31:0] io_Excp_Tval,
  input  [63:0] io_InsnByte,
  output        io_pop_flag,
  output [31:0] io_Out_Address,
  output        io_Out_Excp_vaild,
  output [5:0]  io_Out_Excp_Cause,
  output [31:0] io_Out_Excp_Tval,
  output [63:0] io_Out_InsnByte
);
  wire [31:0] _GEN_0 = ~io_Killed ? io_Address : 32'h0; // @[gshare.scala 27:18 35:22 36:22]
  wire [63:0] _GEN_1 = ~io_Killed ? io_InsnByte : 64'h0; // @[gshare.scala 31:19 35:22 37:23]
  wire  _GEN_2 = ~io_Killed & io_Excp_vaild; // @[gshare.scala 28:21 35:22 38:25]
  wire [5:0] _GEN_3 = ~io_Killed ? io_Excp_Cause : 6'h0; // @[gshare.scala 29:21 35:22 39:25]
  wire [31:0] _GEN_4 = ~io_Killed ? io_Excp_Tval : 32'h0; // @[gshare.scala 30:20 35:22 40:24]
  assign io_pop_flag = ~io_Inflightqueue_empty & ~io_busy; // @[gshare.scala 34:32]
  assign io_Out_Address = ~io_Inflightqueue_empty & ~io_busy ? _GEN_0 : 32'h0; // @[gshare.scala 34:45 46:20]
  assign io_Out_Excp_vaild = ~io_Inflightqueue_empty & ~io_busy & _GEN_2; // @[gshare.scala 34:45 47:23]
  assign io_Out_Excp_Cause = ~io_Inflightqueue_empty & ~io_busy ? _GEN_3 : 6'h0; // @[gshare.scala 34:45 48:23]
  assign io_Out_Excp_Tval = ~io_Inflightqueue_empty & ~io_busy ? _GEN_4 : 32'h0; // @[gshare.scala 34:45 49:22]
  assign io_Out_InsnByte = ~io_Inflightqueue_empty & ~io_busy ? _GEN_1 : 64'h0; // @[gshare.scala 34:45 50:21]
endmodule

