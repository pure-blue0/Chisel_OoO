module IsOlder(
  input  [3:0] io_tag1,
  input  [3:0] io_tag2,
  input  [3:0] io_header,
  output       io_older
);
  wire  tag1GeHeader = io_tag1 >= io_header; // @[ReadyForCommit.scala 78:30]
  wire  tag2GeHeader = io_tag2 >= io_header; // @[ReadyForCommit.scala 79:30]
  wire  comparisonCase = tag1GeHeader ^ tag2GeHeader; // @[ReadyForCommit.scala 82:37]
  assign io_older = comparisonCase ? io_tag1 > io_tag2 : io_tag1 < io_tag2; // @[ReadyForCommit.scala 85:25 86:14 88:14]
endmodule
module ReadyForCommit(
  input        clock,
  input        reset,
  input  [3:0] io_Rob_Tag,
  input  [3:0] io_Rob_Header,
  input  [3:0] io_Rob_NextHeader,
  input  [2:0] io_m_RobState,
  input  [3:0] io_m_RollBackTag,
  input  [3:0] io_Rob_Usage,
  input        io_Header_isStable,
  input  [2:0] io_Header_Function_type,
  output       io_Ready
);
  wire [3:0] isOlderModule_io_tag1; // @[ReadyForCommit.scala 25:29]
  wire [3:0] isOlderModule_io_tag2; // @[ReadyForCommit.scala 25:29]
  wire [3:0] isOlderModule_io_header; // @[ReadyForCommit.scala 25:29]
  wire  isOlderModule_io_older; // @[ReadyForCommit.scala 25:29]
  wire  _T_4 = io_Rob_Header == io_Rob_Tag; // @[ReadyForCommit.scala 40:26]
  wire  _T_14 = io_Rob_NextHeader == io_Rob_Tag; // @[ReadyForCommit.scala 52:34]
  wire  _GEN_2 = ~io_Header_isStable & (io_Header_Function_type == 3'h3 | io_Header_Function_type == 3'h4 |
    io_Header_Function_type == 3'h1) ? 1'h0 : _T_14; // @[ReadyForCommit.scala 49:142 50:17]
  wire  _GEN_3 = _T_4 | _GEN_2; // @[ReadyForCommit.scala 46:42 47:15]
  wire  _GEN_4 = io_Rob_Usage >= 4'h2 & _GEN_3; // @[ReadyForCommit.scala 45:37 34:9]
  wire  _GEN_5 = io_Rob_Usage == 4'h1 ? _T_4 : _GEN_4; // @[ReadyForCommit.scala 39:38]
  wire  _GEN_6 = io_Rob_Usage == 4'h0 ? 1'h0 : _GEN_5; // @[ReadyForCommit.scala 37:32 38:13]
  IsOlder isOlderModule ( // @[ReadyForCommit.scala 25:29]
    .io_tag1(isOlderModule_io_tag1),
    .io_tag2(isOlderModule_io_tag2),
    .io_header(isOlderModule_io_header),
    .io_older(isOlderModule_io_older)
  );
  assign io_Ready = io_m_RobState == 3'h0 | isOlderModule_io_older ? _GEN_6 : 1'h1; // @[ReadyForCommit.scala 36:62 61:11]
  assign isOlderModule_io_tag1 = io_Rob_Tag; // @[ReadyForCommit.scala 28:25]
  assign isOlderModule_io_tag2 = io_m_RollBackTag; // @[ReadyForCommit.scala 29:25]
  assign isOlderModule_io_header = io_Rob_Header; // @[ReadyForCommit.scala 30:27]
endmodule
