module BranchRedirect(
  input         clock,
  input         reset,
  input  [31:0] io_Insn,
  output        io_Perd_Taken,
  output [31:0] io_Pred_Target,
  output [3:0]  io_Redirect_ID,
  output [31:0] io_Redirect_Target,
  output        io_needRedirect
);
  wire [1:0] _GEN_2 = io_Insn[6:0] == 7'h6f ? 2'h2 : 2'h0; // @[gshare.scala 20:39 23:20 29:20]
  assign io_Perd_Taken = io_Insn[6:0] == 7'h6f; // @[gshare.scala 20:21]
  assign io_Pred_Target = io_Insn; // @[gshare.scala 20:39 22:20 28:20]
  assign io_Redirect_ID = {{2'd0}, _GEN_2};
  assign io_Redirect_Target = io_Insn[6:0] == 7'h6f ? io_Pred_Target : 32'h0; // @[gshare.scala 17:22 20:39 24:24]
  assign io_needRedirect = io_Insn[6:0] == 7'h6f; // @[gshare.scala 20:21]
endmodule
