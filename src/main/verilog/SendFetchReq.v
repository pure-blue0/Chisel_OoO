module SendFetchReq(
  input         clock,
  input         reset,
  input         io_Inflightqueue_full,
  input  [1:0]  io_m_state,
  input         io_isStalled,
  input  [63:0] io_PcRegisterOut,
  output        io_Sendsuccess,
  output [63:0] io_FetchReq_address,
  output        io_Busy,
  output [3:0]  io_fetchReq_Length,
  output [1:0]  io_fetchReq_Opcode,
  output        io_Excp_vaild,
  output [5:0]  io_Excp_Cause,
  output [63:0] io_Excp_Tval
);
  wire [63:0] _io_FetchReq_address_T = io_PcRegisterOut & 64'hfffffffffffffff8; // @[SendFetchReq.scala 42:47]
  wire [63:0] _GEN_2 = io_PcRegisterOut[0] ? io_PcRegisterOut : 64'h0; // @[SendFetchReq.scala 28:16 35:39 38:20]
  wire  _GEN_3 = io_PcRegisterOut[0] ? 1'h0 : 1'h1; // @[SendFetchReq.scala 24:18 35:39 40:22]
  wire [63:0] _GEN_4 = io_PcRegisterOut[0] ? 64'h0 : _io_FetchReq_address_T; // @[SendFetchReq.scala 29:23 35:39 42:27]
  wire [3:0] _GEN_5 = io_PcRegisterOut[0] ? 4'h0 : 4'h8; // @[SendFetchReq.scala 30:22 35:39 43:26]
  assign io_Sendsuccess = ~io_Inflightqueue_full & io_m_state == 2'h0 & ~io_isStalled & _GEN_3; // @[SendFetchReq.scala 24:18 34:71]
  assign io_FetchReq_address = ~io_Inflightqueue_full & io_m_state == 2'h0 & ~io_isStalled ? _GEN_4 : 64'h0; // @[SendFetchReq.scala 29:23 34:71]
  assign io_Busy = ~io_Inflightqueue_full & io_m_state == 2'h0 & ~io_isStalled & _GEN_3; // @[SendFetchReq.scala 24:18 34:71]
  assign io_fetchReq_Length = ~io_Inflightqueue_full & io_m_state == 2'h0 & ~io_isStalled ? _GEN_5 : 4'h0; // @[SendFetchReq.scala 30:22 34:71]
  assign io_fetchReq_Opcode = 2'h0;
  assign io_Excp_vaild = ~io_Inflightqueue_full & io_m_state == 2'h0 & ~io_isStalled & io_PcRegisterOut[0]; // @[SendFetchReq.scala 26:17 34:71]
  assign io_Excp_Cause = 6'h0;
  assign io_Excp_Tval = ~io_Inflightqueue_full & io_m_state == 2'h0 & ~io_isStalled ? _GEN_2 : 64'h0; // @[SendFetchReq.scala 28:16 34:71]
endmodule
