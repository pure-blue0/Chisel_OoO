import chisel3._
import chisel3.util._

class IntBusylist extends Module {
  val Busylist_size = 40
  val io = IO(new Bundle {
    // Inputs
    val clk = Input(Clock())
    val reset = Input(Bool())
    val RA_IntBusylist1 = Input(Bool())
    val RA_IntBusylist2 = Input(Bool())
    val RA_PhyRd1 = Input(UInt(6.W))
    val RA_PhyRd2 = Input(UInt(6.W))
    val Forward1_valid = Input(Bool())
    val Forward1_PhyRd = Input(UInt(6.W))
    val Forward1_forwarding = Input(Bool())
    val Forward1_done = Input(Bool())
    val Forward2_valid = Input(Bool())
    val Forward2_PhyRd = Input(UInt(6.W))
    val Forward2_forwarding = Input(Bool())
    val Forward2_done = Input(Bool())
    val WB1_valid = Input(Bool())
    val WB1_PhyRd = Input(UInt(6.W))
    val WB1_forwarding = Input(Bool())
    val WB1_done = Input(Bool())
    val WB2_valid = Input(Bool())
    val WB2_PhyRd = Input(UInt(6.W))
    val WB2_forwarding = Input(Bool())
    val WB2_done = Input(Bool())
    val RR_valid1 = Input(Bool())
    val phyRd1 = Input(UInt(6.W))
    val RR_valid2 = Input(Bool())
    val phyRd2 = Input(UInt(6.W))
    val Commit_valid1 = Input(Bool())
    val Commit_LphyRd1 = Input(UInt(6.W))
    val Commit_valid2 = Input(Bool())
    val Commit_LphyRd2 = Input(UInt(6.W))
    val Inquire1_PhyRs1 = Input(UInt(6.W))
    val Inquire1_PhyRs2 = Input(UInt(6.W))
    val Inquire2_PhyRs1 = Input(UInt(6.W))
    val Inquire2_PhyRs2 = Input(UInt(6.W))
    val Inquire3_PhyRs1 = Input(UInt(6.W))
    val Inquire3_PhyRs2 = Input(UInt(6.W))

    // Outputs
    val Require1_PhyRs1_done = Output(Bool())
    val Require1_PhyRs2_done = Output(Bool())
    val Require2_PhyRs1_done = Output(Bool())
    val Require2_PhyRs2_done = Output(Bool())
    val Require3_PhyRs1_done = Output(Bool())
    val Require3_PhyRs2_done = Output(Bool())
  })

  // Define the busy list
  val busyList = RegInit(VecInit(Seq.fill(Busylist_size)(0.U(3.W))))

  // Logic for initializing the busy list on reset
  when(io.reset) {
    for (i <- 0 until Busylist_size) {
      busyList(i) := 0.U
    }
  }

  // Logic for updating the busy list on the rising edge of the clock
  // Note: The Chisel code for the full functionality is quite extensive and will not be completely provided here.
  // Below is a partial implementation that should be expanded to match the full functional description provided by the user.
  
  // Pseudo-code for handling the update requests and releases from the modules
  // Update the busy list according to the requests and releases

  // Combinational logic for queue inquiry requests
  // Note: The Chisel code for this functionality should be implemented based on the user's need, an example is given below.
// Combinational logic for queue inquiry
io.Require1_PhyRs1_done := busyList(io.Inquire1_PhyRs1)(1)
io.Require1_PhyRs2_done := busyList(io.Inquire1_PhyRs2)(1)
io.Require2_PhyRs1_done := busyList(io.Inquire2_PhyRs1)(1)
io.Require2_PhyRs2_done := busyList(io.Inquire2_PhyRs2)(1)
io.Require3_PhyRs1_done := busyList(io.Inquire3_PhyRs1)(1)
io.Require3_PhyRs2_done := busyList(io.Inquire3_PhyRs2)(1)

  // Register update logic should be implemented here
  // ... This includes handling the different valid signals and updating the busyList accordingly
  // On every positive edge of the clock
when(io.clk.asBool().risingEdge()) {
  // Handle allocation updates
  when(io.RA_IntBusylist1) {
    busyList(io.RA_PhyRd1) := "b100".U // Allocated, but not done or forwarded
  }
  when(io.RA_IntBusylist2) {
    busyList(io.RA_PhyRd2) := "b100".U // Allocated, but not done or forwarded
  }

  // Handle updates from forward and write-back paths
  // Forward1
  when(io.Forward1_valid) {
    val entry = busyList(io.Forward1_PhyRd)
    busyList(io.Forward1_PhyRd) := Cat(entry(2), io.Forward1_forwarding, io.Forward1_done)
  }
  // Repeat for Forward2, WB1, WB2
  // Forward2
when(io.Forward2_valid) {
  val entry = busyList(io.Forward2_PhyRd)
  busyList(io.Forward2_PhyRd) := Cat(entry(2), io.Forward2_forwarding, io.Forward2_done)
}

// WriteBack1
when(io.WB1_valid) {
  val entry = busyList(io.WB1_PhyRd)
  busyList(io.WB1_PhyRd) := Cat(entry(2), io.WB1_forwarding, io.WB1_done)
}

// WriteBack2
when(io.WB2_valid) {
  val entry = busyList(io.WB2_PhyRd)
  busyList(io.WB2_PhyRd) := Cat(entry(2), io.WB2_forwarding, io.WB2_done)
}

  // Handle release requests
  // Release Resource 1
  when(io.RR_valid1) {
    busyList(io.phyRd1) := 0.U // Reset the entry
  }
  // Repeat for Release Resource 2
// Release Resource 2
when(io.RR_valid2) {
  busyList(io.phyRd2) := 0.U // Reset the entry
}
  // Handle commit updates
  // Commit 1
  when(io.Commit_valid1) {
    busyList(io.Commit_LphyRd1) := 0.U // Release the entry
  }
  // Repeat for Commit 2
  // Commit 2
when(io.Commit_valid2) {
  busyList(io.Commit_LphyRd2) := 0.U // Release the entry
}
}


}
