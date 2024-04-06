import chisel3._
import chisel3.util._

class TryDispatch extends Module {
  val io = IO(new Bundle {
    // Inputs
    val scheduler0_AvailPort = Input(UInt(2.W))
    val scheduler1_AvailPort = Input(UInt(4.W))
    val scheduler2_AvailPort = Input(UInt(4.W))
    val insn1_excp_valid = Input(Bool())
    val insn2_excp_valid = Input(Bool())
    val insn1_Function_type = Input(UInt(3.W))
    val insn2_Function_type = Input(UInt(3.W))
    val insn1_rd = Input(UInt(5.W))
    val insn2_rd = Input(UInt(5.W))
    // Output
    val issue_success_count = Output(UInt(3.W))
  })

  // Parameters
  val Issue0_function_type = "b010".U
  val Issue1_function_type1 = "b011".U
  val Issue1_function_type2 = "b100".U
  val Issue2_function_type1 = "b000".U
  val Issue2_function_type2 = "b001".U
  val EnqueWidth = 1.U

  // Initialize success count
  val successCount = RegInit(0.U(3.W))

  // Reset success count at the beginning of the operation
  successCount := 0.U

  // Logic for checking instruction 1 and 2
  def checkInstruction(excp_valid: Bool, func_type: UInt, availPort: UInt): UInt = {
    when(excp_valid) {
      successCount := successCount + 1.U
    }.elsewhen(func_type === Issue0_function_type && availPort > 0.U) {
      successCount := successCount + 1.U
    }.elsewhen((func_type === Issue1_function_type1 || func_type === Issue1_function_type2) && availPort > 0.U) {
      successCount := successCount + 1.U
    }.elsewhen((func_type === Issue2_function_type1 || func_type === Issue2_function_type2) && availPort > 0.U) {
      successCount := successCount + 1.U
    }
    successCount
  }

  // Implementing the logic for instruction dispatch based on the functional description
  // Assuming `insn1_rd` and `insn2_rd` are used for NOP checks where rd == 0 implies NOP
  when(io.insn1_rd =/= 0.U || io.insn2_rd =/= 0.U) {
    successCount := checkInstruction(io.insn1_excp_valid, io.insn1_Function_type, io.scheduler0_AvailPort)
    successCount := checkInstruction(io.insn2_excp_valid, io.insn2_Function_type, io.scheduler1_AvailPort)
  }

  // Assign the calculated success count to the output
  io.issue_success_count := successCount
}

// Generate the Verilog code
object TryDispatchApp extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new TryDispatch())
}
