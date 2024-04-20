import chisel3._
import chisel3.util._

class IntFreelist extends Module {
  val io = IO(new Bundle {
    // Inputs
    val RR_valid1 = Input(Bool())
    val phyRd1 = Input(UInt(6.W))
    val RR_valid2 = Input(Bool())
    val phyRd2 = Input(UInt(6.W))
    val Commit_valid1 = Input(Bool())
    val Commit_LphyRd1 = Input(UInt(6.W))
    val Commit_valid2 = Input(Bool())
    val Commit_LphyRd2 = Input(UInt(6.W))
    val pop_num = Input(UInt(2.W))
    // Outputs
    val PhyRd1 = Output(UInt(6.W))
    val PhyRd2 = Output(UInt(6.W))
    val AvailEntryCount = Output(UInt(4.W))
  })

  val Freelist_size = 40.U
  val freelist = RegInit(VecInit(Seq.fill(40)(0.U(6.W))))
  val header = RegInit(0.U(log2Ceil(40).W))
  val tail = RegInit(0.U(log2Ceil(40).W))
  val AvailEntryCount = RegInit(Freelist_size(4, 0))

  // Circular functionality
  def wrapAround(index: UInt, max: UInt): UInt = Mux(index >= max, 0.U, index)

  // Reset functionality
  when (reset.asBool()) {
    for (i <- 0 until 40) {
      freelist(i) := i.U
    }
    header := 0.U
    tail := 0.U
    AvailEntryCount := Freelist_size
  }

  // At the rising edge of the clock, update the head pointer and AvailEntryCount
  header := wrapAround(header + io.pop_num, Freelist_size)
  AvailEntryCount := AvailEntryCount - io.pop_num

  // Check and update for RR_valid signals
  when(io.RR_valid1) {
    freelist(tail) := io.phyRd1
    tail := wrapAround(tail + 1.U, Freelist_size)
    AvailEntryCount := AvailEntryCount - 1.U
  }
  when(io.RR_valid2) {
    freelist(tail) := io.phyRd2
    tail := wrapAround(tail + 1.U, Freelist_size)
    AvailEntryCount := AvailEntryCount - 1.U
  }
  // Check and update for Commit_valid signals
  when(io.Commit_valid1) {
    freelist(tail) := io.Commit_LphyRd1
    tail := wrapAround(tail + 1.U, Freelist_size)
    AvailEntryCount := AvailEntryCount - 1.U
  }
  when(io.Commit_valid2) {
    freelist(tail) := io.Commit_LphyRd2
    tail := wrapAround(tail + 1.U, Freelist_size)
    AvailEntryCount := AvailEntryCount - 1.U
  }

  // Output physical register IDs
  io.PhyRd1 := freelist(header)
  io.PhyRd2 := freelist(wrapAround(header + 1.U, Freelist_size))
  // Output available entry count
  io.AvailEntryCount := AvailEntryCount

  // Implement the circular functionality for the head and tail pointers
  when(header === (Freelist_size - 1.U)) {
    header := 0.U
  } .elsewhen(header === 0.U && io.pop_num > 0.U) {
    header := Freelist_size - 1.U
  }

  when(tail === (Freelist_size - 1.U)) {
    tail := 0.U
  } .elsewhen(tail === 0.U && (io.RR_valid1 || io.Commit_valid1)) {
    tail := Freelist_size - 1.U
  }
}

