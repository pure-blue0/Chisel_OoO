module InflighQueue(
  input         clock,
  input         reset,
  input         io_clk,
  input         io_reset,
  input         io_pop,
  input         io_push,
  input  [94:0] io_write_data,
  output        io_full,
  output        io_empty,
  output [94:0] io_read_data,
  output [2:0]  io_head_ptr,
  output [2:0]  io_tail_ptr
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
  reg [31:0] _RAND_8;
  reg [31:0] _RAND_9;
`endif // RANDOMIZE_REG_INIT
  reg [94:0] queue_0; // @[gshare.scala 18:18]
  reg [94:0] queue_1; // @[gshare.scala 18:18]
  reg [94:0] queue_2; // @[gshare.scala 18:18]
  reg [94:0] queue_3; // @[gshare.scala 18:18]
  reg [94:0] queue_4; // @[gshare.scala 18:18]
  reg [94:0] queue_5; // @[gshare.scala 18:18]
  reg [94:0] queue_6; // @[gshare.scala 18:18]
  reg [94:0] queue_7; // @[gshare.scala 18:18]
  reg [2:0] head; // @[gshare.scala 19:21]
  reg [2:0] tail; // @[gshare.scala 20:21]
  wire [2:0] _io_full_T_1 = tail + 3'h1; // @[gshare.scala 29:21]
  wire [3:0] _GEN_10 = {{1'd0}, _io_full_T_1}; // @[gshare.scala 29:28]
  wire [3:0] _GEN_0 = _GEN_10 % 4'h8; // @[gshare.scala 29:28]
  wire [2:0] _io_full_T_2 = _GEN_0[2:0]; // @[gshare.scala 29:28]
  wire [2:0] _head_T_1 = head + 3'h1; // @[gshare.scala 39:19]
  wire [3:0] _GEN_11 = {{1'd0}, _head_T_1}; // @[gshare.scala 39:26]
  wire [3:0] _GEN_1 = _GEN_11 % 4'h8; // @[gshare.scala 39:26]
  wire [2:0] _head_T_2 = _GEN_1[2:0]; // @[gshare.scala 39:26]
  wire [94:0] _GEN_21 = 3'h1 == head ? queue_1 : queue_0; // @[gshare.scala 43:{16,16}]
  wire [94:0] _GEN_22 = 3'h2 == head ? queue_2 : _GEN_21; // @[gshare.scala 43:{16,16}]
  wire [94:0] _GEN_23 = 3'h3 == head ? queue_3 : _GEN_22; // @[gshare.scala 43:{16,16}]
  wire [94:0] _GEN_24 = 3'h4 == head ? queue_4 : _GEN_23; // @[gshare.scala 43:{16,16}]
  wire [94:0] _GEN_25 = 3'h5 == head ? queue_5 : _GEN_24; // @[gshare.scala 43:{16,16}]
  wire [94:0] _GEN_26 = 3'h6 == head ? queue_6 : _GEN_25; // @[gshare.scala 43:{16,16}]
  assign io_full = _io_full_T_2 == head; // @[gshare.scala 29:35]
  assign io_empty = head == tail; // @[gshare.scala 30:20]
  assign io_read_data = 3'h7 == head ? queue_7 : _GEN_26; // @[gshare.scala 43:{16,16}]
  assign io_head_ptr = head; // @[gshare.scala 44:15]
  assign io_tail_ptr = tail; // @[gshare.scala 45:15]
  always @(posedge clock) begin
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h0 == tail) begin // @[gshare.scala 34:17]
        queue_0 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h1 == tail) begin // @[gshare.scala 34:17]
        queue_1 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h2 == tail) begin // @[gshare.scala 34:17]
        queue_2 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h3 == tail) begin // @[gshare.scala 34:17]
        queue_3 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h4 == tail) begin // @[gshare.scala 34:17]
        queue_4 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h5 == tail) begin // @[gshare.scala 34:17]
        queue_5 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h6 == tail) begin // @[gshare.scala 34:17]
        queue_6 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      if (3'h7 == tail) begin // @[gshare.scala 34:17]
        queue_7 <= io_write_data; // @[gshare.scala 34:17]
      end
    end
    if (reset) begin // @[gshare.scala 19:21]
      head <= 3'h0; // @[gshare.scala 19:21]
    end else if (io_pop & ~io_empty) begin // @[gshare.scala 38:29]
      head <= _head_T_2; // @[gshare.scala 39:10]
    end else if (io_reset) begin // @[gshare.scala 23:18]
      head <= 3'h0; // @[gshare.scala 24:10]
    end
    if (reset) begin // @[gshare.scala 20:21]
      tail <= 3'h0; // @[gshare.scala 20:21]
    end else if (io_push & ~io_full) begin // @[gshare.scala 33:29]
      tail <= _io_full_T_2; // @[gshare.scala 35:10]
    end else if (io_reset) begin // @[gshare.scala 23:18]
      tail <= 3'h0; // @[gshare.scala 25:10]
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
  queue_0 = _RAND_0[94:0];
  _RAND_1 = {3{`RANDOM}};
  queue_1 = _RAND_1[94:0];
  _RAND_2 = {3{`RANDOM}};
  queue_2 = _RAND_2[94:0];
  _RAND_3 = {3{`RANDOM}};
  queue_3 = _RAND_3[94:0];
  _RAND_4 = {3{`RANDOM}};
  queue_4 = _RAND_4[94:0];
  _RAND_5 = {3{`RANDOM}};
  queue_5 = _RAND_5[94:0];
  _RAND_6 = {3{`RANDOM}};
  queue_6 = _RAND_6[94:0];
  _RAND_7 = {3{`RANDOM}};
  queue_7 = _RAND_7[94:0];
  _RAND_8 = {1{`RANDOM}};
  head = _RAND_8[2:0];
  _RAND_9 = {1{`RANDOM}};
  tail = _RAND_9[2:0];
`endif // RANDOMIZE_REG_INIT
  `endif // RANDOMIZE
end // initial
`ifdef FIRRTL_AFTER_INITIAL
`FIRRTL_AFTER_INITIAL
`endif
`endif // SYNTHESIS
endmodule

