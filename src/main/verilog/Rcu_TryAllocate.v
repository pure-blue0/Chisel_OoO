module Rcu_TryAllocate(
  input        clock,
  input        reset,
  input  [2:0] io_insn_num,
  input  [3:0] io_Rob_unuse_count,
  input  [7:0] io_IntFreelist_unuse_count,
  output [2:0] io_Rcu_success_count
);
  wire [7:0] _GEN_0 = {{4'd0}, io_Rob_unuse_count}; // @[gshare.scala 16:50]
  wire [7:0] minRobIntFreeList = _GEN_0 < io_IntFreelist_unuse_count ? {{4'd0}, io_Rob_unuse_count} :
    io_IntFreelist_unuse_count; // @[gshare.scala 16:30]
  wire [7:0] _GEN_1 = {{5'd0}, io_insn_num}; // @[gshare.scala 19:43]
  wire [7:0] _io_Rcu_success_count_T_1 = _GEN_1 < minRobIntFreeList ? {{5'd0}, io_insn_num} : minRobIntFreeList; // @[gshare.scala 19:30]
  assign io_Rcu_success_count = _io_Rcu_success_count_T_1[2:0]; // @[gshare.scala 19:24]
endmodule

