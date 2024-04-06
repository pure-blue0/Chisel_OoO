import chisel3._
import chisel3.util._

class Dispatch extends Module {
  val io = IO(new Bundle {
    // Define all inputs
    val DispatchCount = Input(UInt(2.W))
    val insn = Vec(2, Input(UInt(128.W)))
    val Scheduler_AvailPort = Input(Vec(3, UInt(4.W)))
    val Insn_excp_valid = Input(Vec(2, Bool()))
    val insn_Function_type = Input(Vec(2, UInt(3.W)))
    val insn_rd = Input(Vec(2, UInt(5.W)))
    val Insn_LSQTag_in = Input(Vec(2, UInt(4.W)))
    val insn_PhyRs = Input(Vec(2, Vec(2, UInt(6.W))))
    val insn_PhyRd = Input(Vec(2, UInt(6.W)))
    val insn_LPhyRd = Input(Vec(2, UInt(6.W)))
    val Insn_RobTag_in = Input(Vec(2, UInt(4.W)))
    val Insn_State_in = Input(Vec(2, UInt(4.W)))

    // Define all outputs
    val Issue_num = Output(Vec(2, UInt(2.W)))
    val insn_out = Output(Vec(2, UInt(128.W)))
    val Insn_LSQTag = Output(Vec(2, UInt(4.W)))
    val insn_PhyRs_out = Output(Vec(2, Vec(2, UInt(6.W))))
    val insn_PhyRd_out = Output(Vec(2, UInt(6.W)))
    val insn_LPhyRd_out = Output(Vec(2, UInt(6.W)))
    val Insn_RobTag = Output(Vec(2, UInt(4.W)))
    val Insn_State = Output(Vec(2, UInt(4.W)))
  })

  // Initialize all outputs
  io.Issue_num.foreach(_ := 0.U)
  io.insn_out.foreach(_ := 0.U)
  io.Insn_LSQTag.foreach(_ := 0.U)
  io.insn_PhyRs_out.flatten.foreach(_ := 0.U)
  io.insn_PhyRd_out.foreach(_ := 0.U)
  io.insn_LPhyRd_out.foreach(_ := 0.U)
  io.Insn_RobTag.foreach(_ := 0.U)
  io.Insn_State.foreach(_ := 0.U)

  // Parameters for function types supported by issue queues
  val Issue_function_types = Seq(
    Seq("b010".U),                 // ISSUE0 supports CSR
    Seq("b011".U, "b100".U),       // ISSUE1 supports LDU, STU
    Seq("b000".U, "b001".U)        // ISSUE2 supports ALU, BRU
  )

  // Logic to determine issue number based on function type and exception validity
  for (i <- 0 until 2) { // For each instruction
    when(io.Insn_excp_valid(i)) {
      io.Issue_num(i) := 4.U // Exception case
      io.Insn_State(i) := "b00".U // Assuming "b00" represents the commit state
    } .otherwise {
      io.Issue_num(i) := 4.U // Default to no match
      for (q <- Issue_function_types.indices) { // For each issue queue
        when(Issue_function_types(q).contains(io.insn_Function_type(i))) {
          io.Issue_num(i) := q.U
          io.Insn_State(i) := "b01".U // Assuming "b01" represents the issue state
        }
      }
    }

    // Pass through outputs for each instruction
    io.insn_out(i) := io.insn(i)
    io.Insn_LSQTag(i) := io.Insn_LSQTag_in(i)
    io.insn_PhyRs_out(i) := io.insn_PhyRs(i)
    io.insn_PhyRd_out(i) := io.insn_PhyRd(i)
    io.insn_LPhyRd_out(i) := io.insn_LPhyRd(i)
    io.Insn_RobTag(i) := io.Insn_RobTag_in(i)
  }
}
