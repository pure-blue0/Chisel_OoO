import chisel3._
import chisel3.util._

class RobIO(ROB_Size: Int) extends Bundle {
  // Inputs
  val Clk = Input(Clock())
  val reset = Input(Bool())
  val Entry_data1 = Input(UInt(61.W))
  val Entry_data2 = Input(UInt(61.W))
  val ROB_EN1 = Input(Bool())
  val ROB_EN2 = Input(Bool())
  val WB1_valid = Input(Bool())
  val WB1_ROBTag = Input(UInt(4.W))
  val WB1_done = Input(Bool())
  val WB1_isStable = Input(Bool())
  val WB1_isexcp = Input(Bool())
  val WB1_isMisPred = Input(Bool())
  val WB2_valid = Input(Bool())
  val WB2_ROBTag = Input(UInt(4.W))
  val WB2_done = Input(Bool())
  val WB2_isStable = Input(Bool())
  val WB2_isexcp = Input(Bool())
  val WB2_isMisPred = Input(Bool())
  val rob_pop_num = Input(UInt(2.W))
  val AGU_valid = Input(Bool())
  val AGU_isStable = Input(Bool())
  val AGU_Done = Input(Bool())
  val AGU_isExcp = Input(Bool())
  val AGU_RobTag = Input(UInt(4.W))
  val RB_valid1 = Input(Bool())
  val RB_valid2 = Input(Bool())

  // Outputs
  val AvailEntryCount = Output(UInt(4.W))
  val Empty = Output(Bool())
  val Rob_Header = Output(UInt(4.W))
  val Header_valid = Output(Bool())
  val Header_isExcp = Output(Bool())
  val Header_done = Output(Bool())
  val Header_LphyRd = Output(UInt(6.W))
  val Header_isaRd = Output(UInt(5.W))
  val Header_phyRd = Output(UInt(64.W))
  val Header_Function_type = Output(UInt(3.W))
  val Header_LSQtag = Output(UInt(4.W))

  val Rob_NextHeader = Output(UInt(4.W))
  val NextHeader_valid = Output(Bool())
  val NextHeader_isExcp = Output(Bool())
  val NextHeader_done = Output(Bool())
  val NextHeader_LphyRd = Output(UInt(6.W))
  val NextHeader_isaRd = Output(UInt(5.W))
  val NextHeader_phyRd = Output(UInt(64.W))
  val NextHeader_Function_type = Output(UInt(3.W))
  val NextHeader_LSQtag = Output(UInt(4.W))
  
  val RobTail = Output(UInt(4.W))
  val RobNextTail = Output(UInt(4.W))
  val RobBeforeTail = Output(UInt(4.W))
  val Rob_Tail_valid = Output(Bool())
  val Rob_Tail_isExcp = Output(Bool())
  val Rob_Tail_phyRd = Output(UInt(6.W))
  val Rob_Tail_LphyRd = Output(UInt(6.W))
  val Rob_Tail_isaRd = Output(UInt(5.W))
  val Rob_Tail_Function = Output(UInt(3.W))
  val Rob_Tail_LSQTag = Output(UInt(4.W))
  val Rob_beforeTail_valid = Output(Bool())
  val Rob_beforeTail_isExcp = Output(Bool())
  val Rob_beforeTail_phyRd = Output(UInt(6.W))
  val Rob_beforeTail_LphyRd = Output(UInt(6.W))
  val Rob_beforeTail_isaRd = Output(UInt(5.W))
  val Rob_beforeTail_Function = Output(UInt(3.W))
  val Rob_beforeTail_LSQTag = Output(UInt(4.W))
}

class Rob(val ROB_Size: Int = 12) extends Module {
  val io = IO(new RobIO(ROB_Size))

  val robEntries = Reg(Vec(ROB_Size, new RobEntry))
  val queue = RegInit(VecInit(Seq.fill(ROB_Size)(0.U(61.W))))
  val Header = RegInit(0.U(4.W))
  val NextHeader = RegInit(1.U(4.W))
  val RobTail = RegInit(0.U(4.W))
  val RobNextTail = RegInit(1.U(4.W))
  val RobBeforeTail = RegInit((ROB_Size - 1).U(4.W))
  val usage = RegInit(0.U(log2Ceil(ROB_Size + 1).W)) // log2Ceil to get enough bits to represent ROB_Size

  // Internal status indicators
  val Empty = RegInit(true.B)
  val AvailEntryCount = RegInit(ROB_Size.U(4.W))  

  // Registers to hold the state between clock cycles
  val headPtr = RegInit(0.U(log2Ceil(ROB_Size).W))
  val tailPtr = RegInit(0.U(log2Ceil(ROB_Size).W))
  val entryCount = RegInit(0.U(log2Ceil(ROB_Size + 1).W))

  // Calculate the number of available entries and set the Empty flag
  val availEntries = ROB_Size.U - entryCount
  io.AvailEntryCount := availEntries
  io.Empty := (entryCount === 0.U)

  when(io.reset) {
    Header := 0.U
    NextHeader := 1.U
    RobTail := 0.U
    RobNextTail := 1.U
    RobBeforeTail := (ROB_Size - 1).U
    usage := 0.U
    Empty := true.B
    queue.foreach(_ := 0.U)
    AvailEntryCount := ROB_Size.U
  }

  // Logic to handle the enqueue of instructions into the ROB
  when(io.ROB_EN1 || io.ROB_EN2) {
    when(io.ROB_EN1) {
      queue(Header) := io.Entry_data1
      Header := Mux(Header === (ROB_Size - 1).U, 0.U, Header + 1.U)
      RobBeforeTail := Mux(RobBeforeTail === (ROB_Size - 1).U, 0.U, RobBeforeTail + 1.U)
      usage := usage + 1.U
    }
    when(io.ROB_EN2) {
      queue(NextHeader) := io.Entry_data2
      NextHeader := Mux(NextHeader === (ROB_Size - 1).U, 0.U, NextHeader + 1.U)
      RobTail := Mux(RobTail === (ROB_Size - 1).U, 0.U, RobTail + 1.U)
      usage := usage + 1.U
    }
    Empty := false.B
    AvailEntryCount := (ROB_Size.U - usage)
  }

  // Logic to handle the dequeue/pop of instructions from the ROB
  when(io.rob_pop_num > 0.U) {
    Header := Mux(Header + io.rob_pop_num >= ROB_Size.U, 
                  (Header + io.rob_pop_num) - ROB_Size.U, 
                  Header + io.rob_pop_num)
    usage := usage - io.rob_pop_num
    Empty := usage === 0.U
    AvailEntryCount := (ROB_Size.U - usage)
  }

  // Logic to handle the AGU update
  when(io.AGU_valid) {
    val aguEntry = queue(io.AGU_RobTag)
    aguEntry := aguEntry.bitSet(2.U, io.AGU_isStable)
    aguEntry := aguEntry.bitSet(3.U, io.AGU_Done)
    aguEntry := aguEntry.bitSet(0.U, io.AGU_isExcp)
  }

  // Logic to handle WriteBack updates
  when(io.WB1_valid) {
    val wbEntry = queue(io.WB1_ROBTag)
    wbEntry := wbEntry.bitSet(3.U, io.WB1_done)
    wbEntry := wbEntry.bitSet(2.U, io.WB1_isStable)
    wbEntry := wbEntry.bitSet(1.U, io.WB1_isMisPred)
    wbEntry := wbEntry.bitSet(0.U, io.WB1_isexcp)
  }
  when(io.WB2_valid) {
    val wbEntry = queue(io.WB2_ROBTag)
    wbEntry := wbEntry.bitSet(3.U, io.WB2_done)
    wbEntry := wbEntry.bitSet(2.U, io.WB2_isStable)
    wbEntry := wbEntry.bitSet(1.U, io.WB2_isMisPred)
    wbEntry := wbEntry.bitSet(0.U, io.WB2_isexcp)
  }

  // Logic to handle rollback based on RB_valid1 and RB_valid2
  when(io.RB_valid1) {
    queue(RobTail) := 0.U // Invalidate entry at the tail
    RobTail := Mux(RobTail === 0.U, (ROB_Size - 1).U, RobTail - 1.U)
    usage := usage - 1.U
    Empty := usage === 0.U
    AvailEntryCount := (ROB_Size.U - usage)
  }
  when(io.RB_valid2) {
    queue(RobBeforeTail) := 0.U // Invalidate the entry before the tail
    RobBeforeTail := Mux(RobBeforeTail === 0.U, (ROB_Size - 1).U, RobBeforeTail - 1.U)
    usage := usage - 1.U
    Empty := usage === 0.U
    AvailEntryCount := (ROB_Size.U - usage)
  }

  // Update ROB entries based on execution results, writebacks, and exceptions
  for (i <- 0 until ROB_Size) {
    when(io.execute(i).valid) {
      robEntries(i).instruction := io.execute(i).bits
      robEntries(i).valid := true.B
    }

    when(io.writeback(i).valid) {
      robEntries(i).result := io.writeback(i).bits
      robEntries(i).done := true.B
    }

    when(io.exception(i).valid) {
      robEntries(i).exception := io.exception(i).bits
      robEntries(i).valid := false.B  // Optionally invalidating the entry on exception
    }
  }


// Update the Empty flag and available entries
    entryCount := entryCount + io.ROB_EN1.asUInt + io.ROB_EN2.asUInt - io.rob_pop_num
    io.Empty := entryCount === 0.U
    io.AvailEntryCount := availEntries

  // Module logic goes here...
}
