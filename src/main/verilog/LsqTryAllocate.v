module LsqTryAllocate(
  input        clock,
  input        reset,
  input  [2:0] io_insn_num,
  input  [2:0] io_insn1_Function_type,
  input  [2:0] io_insn2_Function_type,
  input  [3:0] io_LoadQueue_unuse_count,
  input  [3:0] io_StoreQueue_unuse_count,
  output [2:0] io_lsq_success_count
);
  wire  _loadCount_T_1 = io_insn2_Function_type == 3'h1; // @[gshare.scala 22:43]
  wire  loadCount = (io_insn1_Function_type == 3'h1) + _loadCount_T_1; // @[gshare.scala 21:68]
  wire  _storeCount_T_1 = io_insn2_Function_type == 3'h2; // @[gshare.scala 24:44]
  wire  storeCount = (io_insn1_Function_type == 3'h2) + _storeCount_T_1; // @[gshare.scala 23:70]
  wire [3:0] _GEN_0 = {{3'd0}, loadCount}; // @[gshare.scala 27:55]
  wire [3:0] possibleLdqCount = io_LoadQueue_unuse_count < _GEN_0 ? io_LoadQueue_unuse_count : {{3'd0}, loadCount}; // @[gshare.scala 27:29]
  wire [3:0] _GEN_1 = {{3'd0}, storeCount}; // @[gshare.scala 28:56]
  wire [3:0] possibleStqCount = io_StoreQueue_unuse_count < _GEN_1 ? io_StoreQueue_unuse_count : {{3'd0}, storeCount}; // @[gshare.scala 28:29]
  wire [3:0] totalPossible = possibleLdqCount + possibleStqCount; // @[gshare.scala 31:40]
  wire [3:0] _GEN_2 = {{1'd0}, io_insn_num}; // @[gshare.scala 32:45]
  wire [3:0] _io_lsq_success_count_T_1 = totalPossible < _GEN_2 ? totalPossible : {{1'd0}, io_insn_num}; // @[gshare.scala 32:30]
  assign io_lsq_success_count = _io_lsq_success_count_T_1[2:0]; // @[gshare.scala 32:24]
endmodule

