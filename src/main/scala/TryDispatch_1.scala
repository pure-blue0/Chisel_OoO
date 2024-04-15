import chisel3._
import chisel3.util._

class TryDispatch extends Module {
  val io = IO(new Bundle {
    val scheduler0_AvailPort = Input(UInt(2.W))
    val scheduler1_AvailPort = Input(UInt(4.W))
    val scheduler2_AvailPort = Input(UInt(4.W))
    val insn1_excp_valid = Input(Bool())
    val insn2_excp_valid = Input(Bool())
    val insn1_Function_type = Input(UInt(3.W))
    val insn2_Function_type = Input(UInt(3.W))
    val insn1_rd = Input(UInt(5.W))
    val insn2_rd = Input(UInt(5.W))
    val insn_num = Input(UInt(2.W))
    val issue_success_count = Output(UInt(3.W))
  })

  // Parameters (These should be passed as parameters but are hardcoded here for simplicity)
  val Issue0_function_type = "b010".U
  val Issue1_function_type1 = "b011".U
  val Issue1_function_type2 = "b100".U
  val Issue2_function_type1 = "b000".U
  val Issue2_function_type2 = "b001".U
  val EnqueWidth = Seq(1.U, 2.U) // Example of handling multiple enqueue widths

  // Mutable state to track available ports after allocations
  val availPorts0 = RegNext(io.scheduler0_AvailPort)
  val availPorts1 = RegNext(io.scheduler1_AvailPort)
  val availPorts2 = RegNext(io.scheduler2_AvailPort)

  // Initialize success count
  val successCount = RegInit(0.U(3.W))
  successCount := 0.U

  when(io.insn_num > 0.U) {
    // Step 1: Check if instruction 1 is an exception
    when(io.insn1_excp_valid) {
      successCount := successCount + 1.U
    } .otherwise {
      // Step 2: Check if instruction 1 is a NOP (assuming function type "000" is NOP for example)
      when(io.insn1_Function_type === "b000".U) {
        successCount := successCount + 1.U
      } .otherwise {
        // Step 3: Match function type and check available ports
        when(io.insn1_Function_type === "b010".U && availPorts0 > 0.U) { // For ISSUE0
          successCount := successCount + 1.U
          availPorts0 := availPorts0 - 1.U
        } .elsewhen((io.insn1_Function_type === "b011".U || io.insn1_Function_type === "b100".U) && availPorts1 > 0.U) { // For ISSUE1
          successCount := successCount + 1.U
          availPorts1 := availPorts1 - 1.U
        } .elsewhen((io.insn1_Function_type === "b000".U || io.insn1_Function_type === "b001".U) && availPorts2 > 0.U) { // For ISSUE2, assuming "000" is also ALU for simplicity
          successCount := successCount + 1.U
          availPorts2 := availPorts2 - 1.U
        }
        // If instruction 1 does not match or is a control flow instruction, no action is needed due to the direct mapping of steps
      }
    }
  }

  when(io.insn_num === 2.U) {
    // Repeat step 3 for instruction 2
    // Assuming control flow check and other specific checks are incorporated as needed
    when(io.insn2_excp_valid) {
      successCount := successCount + 1.U
    } .elsewhen(io.insn2_Function_type =/= "b000".U) { // Assuming "000" is NOP for simplicity
      when(io.insn2_Function_type === "b010".U && availPorts0 > 0.U) {
        successCount := successCount + 1.U
        availPorts0 := availPorts0 - 1.U
      } .elsewhen((io.insn2_Function_type === "b011".U || io.insn2_Function_type === "b100".U) && availPorts1 > 0.U) {
        successCount := successCount + 1.U
        availPorts1 := availPorts1 - 1.U
      } .elsewhen((io.insn2_Function_type === "b000".U || io.insn2_Function_type === "b001".U) && availPorts2 > 0.U) {
        successCount := successCount + 1.U
        availPorts2 := availPorts2 - 1.U
      }
    }
  }

  // Output the final success count
  io.issue_success_count := successCount
}