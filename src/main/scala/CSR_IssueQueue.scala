import chisel3._
import chisel3.util._

class CSR_IssueQueue extends Module {
  val io = IO(new Bundle {
    // Clock and Reset are implicitly defined in Chisel Modules
    val entry_data1 = Input(UInt(160.W))
    val entry_data2 = Input(UInt(160.W))
    val issue_num1 = Input(Bool())
    val issue_num2 = Input(Bool())
    val pop_valid1 = Input(Bool())
    val pop_valid2 = Input(Bool())
    val availEntryCount = Output(UInt(4.W))
    val front_data1_out = Output(UInt(164.W))
    val front_data2_out = Output(UInt(164.W))
  })

  val queueSize = 4
  val queue = Reg(Vec(queueSize, UInt(160.W)))
  val head = RegInit(0.U(2.W))
  val tail = RegInit(0.U(2.W))
  val numEntries = RegInit(0.U(3.W)) // Can go up to 4, thus need 3 bits

  // Reset logic is implicit in Chisel for RegInit

  def wrapAround(index: UInt, max: UInt): UInt = {
    Mux(index >= max, 0.U, index)
  }

  // Enqueue logic
  when(io.issue_num1 && numEntries =/= queueSize.U) {
    queue(tail) := io.entry_data1
    tail := wrapAround(tail + 1.U, queueSize.U)
    numEntries := numEntries + 1.U
  }
  when(io.issue_num2 && numEntries =/= queueSize.U && numEntries =/= (queueSize-1).U) {
    queue(tail) := io.entry_data2
    tail := wrapAround(tail + 1.U, queueSize.U)
    numEntries := numEntries + 1.U
  }

  // Dequeue logic
  when(io.pop_valid1 && numEntries =/= 0.U) {
    head := wrapAround(head + 1.U, queueSize.U)
    numEntries := numEntries - 1.U
  }
  when(io.pop_valid2 && numEntries > 1.U) {
    head := wrapAround(head + 2.U, queueSize.U)
    numEntries := numEntries - 2.U
  }

  // Output logic
  io.availEntryCount := (queueSize.U - numEntries)(3, 0)
  io.front_data1_out := Mux(numEntries =/= 0.U, Cat(0.U(4.W), queue(head)), 0.U)
  io.front_data2_out := Mux(numEntries > 1.U, Cat(0.U(4.W), queue(wrapAround(head + 1.U, queueSize.U))), 0.U)

  // Debugging purposes - could be removed or commented out in production
  printf(p"Queue State: ${queue}\n")
  printf(p"Head: $head, Tail: $tail, Entries: $numEntries\n")
}

// Note: The actual implementation details, especially for handling edge cases and ensuring all functionality is accurately captured, might require adjustments based on the specific hardware design verification and synthesis tools used.
