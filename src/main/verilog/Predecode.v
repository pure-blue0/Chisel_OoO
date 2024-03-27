module Predecode(
  input         clock,
  input         reset,
  input  [31:0] io_Address,
  input  [63:0] io_data,
  input         io_excp_vaild,
  input  [5:0]  io_excp_cause,
  input  [31:0] io_excpt_tval,
  output [31:0] io_pc1,
  output [31:0] io_insn1,
  output        io_Perd_Taken1,
  output [31:0] io_Pred_Target1,
  output        io_excp_vaild1,
  output [5:0]  io_excp_cause1,
  output [31:0] io_excpt_tval1,
  output [31:0] io_pc2,
  output [31:0] io_insn2,
  output        io_Perd_Taken2,
  output [31:0] io_Pred_Target2,
  output        io_excp_vaild2,
  output [5:0]  io_excp_cause2,
  output [31:0] io_excpt_tval2,
  output [3:0]  io_Redirect_ID,
  output [31:0] io_Redirect_Target,
  output        io_fetch_flush
);
  assign io_pc1 = 32'h0; // @[gshare.scala 40:10]
  assign io_insn1 = 32'h0; // @[gshare.scala 41:12]
  assign io_Perd_Taken1 = 1'h0; // @[gshare.scala 42:18]
  assign io_Pred_Target1 = 32'h0; // @[gshare.scala 43:19]
  assign io_excp_vaild1 = 1'h0; // @[gshare.scala 44:18]
  assign io_excp_cause1 = 6'h0; // @[gshare.scala 45:18]
  assign io_excpt_tval1 = 32'h0; // @[gshare.scala 46:18]
  assign io_pc2 = 32'h0; // @[gshare.scala 48:10]
  assign io_insn2 = 32'h0; // @[gshare.scala 49:12]
  assign io_Perd_Taken2 = 1'h0; // @[gshare.scala 50:18]
  assign io_Pred_Target2 = 32'h0; // @[gshare.scala 51:19]
  assign io_excp_vaild2 = 1'h0; // @[gshare.scala 52:18]
  assign io_excp_cause2 = 6'h0; // @[gshare.scala 53:18]
  assign io_excpt_tval2 = 32'h0; // @[gshare.scala 54:18]
  assign io_Redirect_ID = 4'h0; // @[gshare.scala 56:18]
  assign io_Redirect_Target = 32'h0; // @[gshare.scala 57:22]
  assign io_fetch_flush = 1'h0; // @[gshare.scala 58:18]
endmodule

