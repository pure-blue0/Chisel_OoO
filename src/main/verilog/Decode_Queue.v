module Decode_Queue(
  input         clock,
  input         reset,
  input         io_clk,
  input         io_reset,
  input  [95:0] io_decoded_insn1,
  input  [95:0] io_decoded_insn2,
  input         io_wr_en1,
  input         io_wr_en2,
  input  [1:0]  io_pop_count,
  output [95:0] io_entry_out1,
  output [95:0] io_entry_out2,
  output        io_fifo_full,
  output        io_fifo_empty,
  output [3:0]  io_head_ptr,
  output [3:0]  io_tail_ptr,
  output [1:0]  io_out_count
);
`ifdef RANDOMIZE_REG_INIT
  reg [95:0] _RAND_0;
  reg [95:0] _RAND_1;
  reg [95:0] _RAND_2;
  reg [95:0] _RAND_3;
  reg [95:0] _RAND_4;
  reg [95:0] _RAND_5;
  reg [95:0] _RAND_6;
  reg [95:0] _RAND_7;
  reg [95:0] _RAND_8;
  reg [95:0] _RAND_9;
  reg [95:0] _RAND_10;
  reg [95:0] _RAND_11;
  reg [95:0] _RAND_12;
  reg [95:0] _RAND_13;
  reg [95:0] _RAND_14;
  reg [95:0] _RAND_15;
  reg [31:0] _RAND_16;
  reg [31:0] _RAND_17;
  reg [31:0] _RAND_18;
`endif // RANDOMIZE_REG_INIT
  reg [95:0] fifo_0; // @[gshare.scala 24:21]
  reg [95:0] fifo_1; // @[gshare.scala 24:21]
  reg [95:0] fifo_2; // @[gshare.scala 24:21]
  reg [95:0] fifo_3; // @[gshare.scala 24:21]
  reg [95:0] fifo_4; // @[gshare.scala 24:21]
  reg [95:0] fifo_5; // @[gshare.scala 24:21]
  reg [95:0] fifo_6; // @[gshare.scala 24:21]
  reg [95:0] fifo_7; // @[gshare.scala 24:21]
  reg [95:0] fifo_8; // @[gshare.scala 24:21]
  reg [95:0] fifo_9; // @[gshare.scala 24:21]
  reg [95:0] fifo_10; // @[gshare.scala 24:21]
  reg [95:0] fifo_11; // @[gshare.scala 24:21]
  reg [95:0] fifo_12; // @[gshare.scala 24:21]
  reg [95:0] fifo_13; // @[gshare.scala 24:21]
  reg [95:0] fifo_14; // @[gshare.scala 24:21]
  reg [95:0] fifo_15; // @[gshare.scala 24:21]
  reg [3:0] head; // @[gshare.scala 25:21]
  reg [3:0] tail; // @[gshare.scala 26:21]
  reg [4:0] count; // @[gshare.scala 27:22]
  wire [4:0] _GEN_2 = io_reset ? 5'h0 : count; // @[gshare.scala 30:18 33:11 27:22]
  wire  _T = ~io_fifo_full; // @[gshare.scala 37:21]
  wire [95:0] _GEN_3 = 4'h0 == tail ? io_decoded_insn1 : fifo_0; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_4 = 4'h1 == tail ? io_decoded_insn1 : fifo_1; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_5 = 4'h2 == tail ? io_decoded_insn1 : fifo_2; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_6 = 4'h3 == tail ? io_decoded_insn1 : fifo_3; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_7 = 4'h4 == tail ? io_decoded_insn1 : fifo_4; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_8 = 4'h5 == tail ? io_decoded_insn1 : fifo_5; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_9 = 4'h6 == tail ? io_decoded_insn1 : fifo_6; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_10 = 4'h7 == tail ? io_decoded_insn1 : fifo_7; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_11 = 4'h8 == tail ? io_decoded_insn1 : fifo_8; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_12 = 4'h9 == tail ? io_decoded_insn1 : fifo_9; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_13 = 4'ha == tail ? io_decoded_insn1 : fifo_10; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_14 = 4'hb == tail ? io_decoded_insn1 : fifo_11; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_15 = 4'hc == tail ? io_decoded_insn1 : fifo_12; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_16 = 4'hd == tail ? io_decoded_insn1 : fifo_13; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_17 = 4'he == tail ? io_decoded_insn1 : fifo_14; // @[gshare.scala 38:{16,16} 24:21]
  wire [95:0] _GEN_18 = 4'hf == tail ? io_decoded_insn1 : fifo_15; // @[gshare.scala 38:{16,16} 24:21]
  wire [3:0] _tail_T_1 = tail + 4'h1; // @[gshare.scala 39:18]
  wire [4:0] _count_T_1 = count + 5'h1; // @[gshare.scala 40:20]
  wire [95:0] _GEN_19 = io_wr_en1 & ~io_fifo_full ? _GEN_3 : fifo_0; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_20 = io_wr_en1 & ~io_fifo_full ? _GEN_4 : fifo_1; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_21 = io_wr_en1 & ~io_fifo_full ? _GEN_5 : fifo_2; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_22 = io_wr_en1 & ~io_fifo_full ? _GEN_6 : fifo_3; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_23 = io_wr_en1 & ~io_fifo_full ? _GEN_7 : fifo_4; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_24 = io_wr_en1 & ~io_fifo_full ? _GEN_8 : fifo_5; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_25 = io_wr_en1 & ~io_fifo_full ? _GEN_9 : fifo_6; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_26 = io_wr_en1 & ~io_fifo_full ? _GEN_10 : fifo_7; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_27 = io_wr_en1 & ~io_fifo_full ? _GEN_11 : fifo_8; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_28 = io_wr_en1 & ~io_fifo_full ? _GEN_12 : fifo_9; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_29 = io_wr_en1 & ~io_fifo_full ? _GEN_13 : fifo_10; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_30 = io_wr_en1 & ~io_fifo_full ? _GEN_14 : fifo_11; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_31 = io_wr_en1 & ~io_fifo_full ? _GEN_15 : fifo_12; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_32 = io_wr_en1 & ~io_fifo_full ? _GEN_16 : fifo_13; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_33 = io_wr_en1 & ~io_fifo_full ? _GEN_17 : fifo_14; // @[gshare.scala 24:21 37:36]
  wire [95:0] _GEN_34 = io_wr_en1 & ~io_fifo_full ? _GEN_18 : fifo_15; // @[gshare.scala 24:21 37:36]
  wire [4:0] _GEN_36 = io_wr_en1 & ~io_fifo_full ? _count_T_1 : _GEN_2; // @[gshare.scala 37:36 40:11]
  wire  _T_5 = ~io_fifo_empty; // @[gshare.scala 51:31]
  wire [95:0] _GEN_72 = 4'h1 == head ? fifo_1 : fifo_0; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_73 = 4'h2 == head ? fifo_2 : _GEN_72; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_74 = 4'h3 == head ? fifo_3 : _GEN_73; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_75 = 4'h4 == head ? fifo_4 : _GEN_74; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_76 = 4'h5 == head ? fifo_5 : _GEN_75; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_77 = 4'h6 == head ? fifo_6 : _GEN_76; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_78 = 4'h7 == head ? fifo_7 : _GEN_77; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_79 = 4'h8 == head ? fifo_8 : _GEN_78; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_80 = 4'h9 == head ? fifo_9 : _GEN_79; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_81 = 4'ha == head ? fifo_10 : _GEN_80; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_82 = 4'hb == head ? fifo_11 : _GEN_81; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_83 = 4'hc == head ? fifo_12 : _GEN_82; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_84 = 4'hd == head ? fifo_13 : _GEN_83; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_85 = 4'he == head ? fifo_14 : _GEN_84; // @[gshare.scala 52:{19,19}]
  wire [95:0] _GEN_86 = 4'hf == head ? fifo_15 : _GEN_85; // @[gshare.scala 52:{19,19}]
  wire [3:0] _head_T_1 = head + 4'h1; // @[gshare.scala 53:18]
  wire [4:0] _count_T_5 = count - 5'h1; // @[gshare.scala 54:20]
  wire [4:0] _GEN_93 = {{3'd0}, io_pop_count}; // @[gshare.scala 67:36]
  wire [4:0] _io_out_count_T_1 = _GEN_93 > count ? count : {{3'd0}, io_pop_count}; // @[gshare.scala 67:22]
  assign io_entry_out1 = io_pop_count >= 2'h1 & ~io_fifo_empty ? _GEN_86 : 96'h0; // @[gshare.scala 49:17 51:47 52:19]
  assign io_entry_out2 = io_pop_count == 2'h2 & _T_5 ? _GEN_86 : 96'h0; // @[gshare.scala 50:17 56:48 57:19]
  assign io_fifo_full = count == 5'h10; // @[gshare.scala 63:25]
  assign io_fifo_empty = count == 5'h0; // @[gshare.scala 64:26]
  assign io_head_ptr = head; // @[gshare.scala 65:15]
  assign io_tail_ptr = tail; // @[gshare.scala 66:15]
  assign io_out_count = _io_out_count_T_1[1:0]; // @[gshare.scala 67:16]
  always @(posedge clock) begin
    if (reset) begin // @[gshare.scala 24:21]
      fifo_0 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h0 == tail) begin // @[gshare.scala 43:16]
        fifo_0 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_0 <= _GEN_19;
      end
    end else begin
      fifo_0 <= _GEN_19;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_1 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h1 == tail) begin // @[gshare.scala 43:16]
        fifo_1 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_1 <= _GEN_20;
      end
    end else begin
      fifo_1 <= _GEN_20;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_2 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h2 == tail) begin // @[gshare.scala 43:16]
        fifo_2 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_2 <= _GEN_21;
      end
    end else begin
      fifo_2 <= _GEN_21;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_3 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h3 == tail) begin // @[gshare.scala 43:16]
        fifo_3 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_3 <= _GEN_22;
      end
    end else begin
      fifo_3 <= _GEN_22;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_4 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h4 == tail) begin // @[gshare.scala 43:16]
        fifo_4 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_4 <= _GEN_23;
      end
    end else begin
      fifo_4 <= _GEN_23;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_5 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h5 == tail) begin // @[gshare.scala 43:16]
        fifo_5 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_5 <= _GEN_24;
      end
    end else begin
      fifo_5 <= _GEN_24;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_6 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h6 == tail) begin // @[gshare.scala 43:16]
        fifo_6 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_6 <= _GEN_25;
      end
    end else begin
      fifo_6 <= _GEN_25;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_7 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h7 == tail) begin // @[gshare.scala 43:16]
        fifo_7 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_7 <= _GEN_26;
      end
    end else begin
      fifo_7 <= _GEN_26;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_8 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h8 == tail) begin // @[gshare.scala 43:16]
        fifo_8 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_8 <= _GEN_27;
      end
    end else begin
      fifo_8 <= _GEN_27;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_9 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'h9 == tail) begin // @[gshare.scala 43:16]
        fifo_9 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_9 <= _GEN_28;
      end
    end else begin
      fifo_9 <= _GEN_28;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_10 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'ha == tail) begin // @[gshare.scala 43:16]
        fifo_10 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_10 <= _GEN_29;
      end
    end else begin
      fifo_10 <= _GEN_29;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_11 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'hb == tail) begin // @[gshare.scala 43:16]
        fifo_11 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_11 <= _GEN_30;
      end
    end else begin
      fifo_11 <= _GEN_30;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_12 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'hc == tail) begin // @[gshare.scala 43:16]
        fifo_12 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_12 <= _GEN_31;
      end
    end else begin
      fifo_12 <= _GEN_31;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_13 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'hd == tail) begin // @[gshare.scala 43:16]
        fifo_13 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_13 <= _GEN_32;
      end
    end else begin
      fifo_13 <= _GEN_32;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_14 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'he == tail) begin // @[gshare.scala 43:16]
        fifo_14 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_14 <= _GEN_33;
      end
    end else begin
      fifo_14 <= _GEN_33;
    end
    if (reset) begin // @[gshare.scala 24:21]
      fifo_15 <= 96'h0; // @[gshare.scala 24:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      if (4'hf == tail) begin // @[gshare.scala 43:16]
        fifo_15 <= io_decoded_insn2; // @[gshare.scala 43:16]
      end else begin
        fifo_15 <= _GEN_34;
      end
    end else begin
      fifo_15 <= _GEN_34;
    end
    if (reset) begin // @[gshare.scala 25:21]
      head <= 4'h0; // @[gshare.scala 25:21]
    end else if (io_pop_count == 2'h2 & _T_5) begin // @[gshare.scala 56:48]
      head <= _head_T_1; // @[gshare.scala 58:10]
    end else if (io_pop_count >= 2'h1 & ~io_fifo_empty) begin // @[gshare.scala 51:47]
      head <= _head_T_1; // @[gshare.scala 53:10]
    end else if (io_reset) begin // @[gshare.scala 30:18]
      head <= 4'h0; // @[gshare.scala 31:10]
    end
    if (reset) begin // @[gshare.scala 26:21]
      tail <= 4'h0; // @[gshare.scala 26:21]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      tail <= _tail_T_1; // @[gshare.scala 44:10]
    end else if (io_wr_en1 & ~io_fifo_full) begin // @[gshare.scala 37:36]
      tail <= _tail_T_1; // @[gshare.scala 39:10]
    end else if (io_reset) begin // @[gshare.scala 30:18]
      tail <= 4'h0; // @[gshare.scala 32:10]
    end
    if (reset) begin // @[gshare.scala 27:22]
      count <= 5'h0; // @[gshare.scala 27:22]
    end else if (io_pop_count == 2'h2 & _T_5) begin // @[gshare.scala 56:48]
      count <= _count_T_5; // @[gshare.scala 59:11]
    end else if (io_pop_count >= 2'h1 & ~io_fifo_empty) begin // @[gshare.scala 51:47]
      count <= _count_T_5; // @[gshare.scala 54:11]
    end else if (io_wr_en2 & _T) begin // @[gshare.scala 42:36]
      count <= _count_T_1; // @[gshare.scala 45:11]
    end else begin
      count <= _GEN_36;
    end
  end
// Register and memory initialization
`ifdef RANDOMIZE_GARBAGE_ASSIGN
`define RANDOMIZE
`endif
`ifdef RANDOMIZE_INVALID_ASSIGN
`define RANDOMIZE
`endif
`ifdef RANDOMIZE_REG_INIT
`define RANDOMIZE
`endif
`ifdef RANDOMIZE_MEM_INIT
`define RANDOMIZE
`endif
`ifndef RANDOM
`define RANDOM $random
`endif
`ifdef RANDOMIZE_MEM_INIT
  integer initvar;
`endif
`ifndef SYNTHESIS
`ifdef FIRRTL_BEFORE_INITIAL
`FIRRTL_BEFORE_INITIAL
`endif
initial begin
  `ifdef RANDOMIZE
    `ifdef INIT_RANDOM
      `INIT_RANDOM
    `endif
    `ifndef VERILATOR
      `ifdef RANDOMIZE_DELAY
        #`RANDOMIZE_DELAY begin end
      `else
        #0.002 begin end
      `endif
    `endif
`ifdef RANDOMIZE_REG_INIT
  _RAND_0 = {3{`RANDOM}};
  fifo_0 = _RAND_0[95:0];
  _RAND_1 = {3{`RANDOM}};
  fifo_1 = _RAND_1[95:0];
  _RAND_2 = {3{`RANDOM}};
  fifo_2 = _RAND_2[95:0];
  _RAND_3 = {3{`RANDOM}};
  fifo_3 = _RAND_3[95:0];
  _RAND_4 = {3{`RANDOM}};
  fifo_4 = _RAND_4[95:0];
  _RAND_5 = {3{`RANDOM}};
  fifo_5 = _RAND_5[95:0];
  _RAND_6 = {3{`RANDOM}};
  fifo_6 = _RAND_6[95:0];
  _RAND_7 = {3{`RANDOM}};
  fifo_7 = _RAND_7[95:0];
  _RAND_8 = {3{`RANDOM}};
  fifo_8 = _RAND_8[95:0];
  _RAND_9 = {3{`RANDOM}};
  fifo_9 = _RAND_9[95:0];
  _RAND_10 = {3{`RANDOM}};
  fifo_10 = _RAND_10[95:0];
  _RAND_11 = {3{`RANDOM}};
  fifo_11 = _RAND_11[95:0];
  _RAND_12 = {3{`RANDOM}};
  fifo_12 = _RAND_12[95:0];
  _RAND_13 = {3{`RANDOM}};
  fifo_13 = _RAND_13[95:0];
  _RAND_14 = {3{`RANDOM}};
  fifo_14 = _RAND_14[95:0];
  _RAND_15 = {3{`RANDOM}};
  fifo_15 = _RAND_15[95:0];
  _RAND_16 = {1{`RANDOM}};
  head = _RAND_16[3:0];
  _RAND_17 = {1{`RANDOM}};
  tail = _RAND_17[3:0];
  _RAND_18 = {1{`RANDOM}};
  count = _RAND_18[4:0];
`endif // RANDOMIZE_REG_INIT
  `endif // RANDOMIZE
end // initial
`ifdef FIRRTL_AFTER_INITIAL
`FIRRTL_AFTER_INITIAL
`endif
`endif // SYNTHESIS
endmodule

