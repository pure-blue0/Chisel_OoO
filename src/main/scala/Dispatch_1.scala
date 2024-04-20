import chisel3._
import chisel3.util._

// Define the functional and instruction state enumerations
object funcType extends ChiselEnum {
  val ALU, BRU, CSR, LDU, STU = Value
}

object InsnState extends ChiselEnum {
  val State_Fetch1, State_Fetch2, State_Decode, State_Dispatch, State_Issue, State_ReadOperand,
  State_Execute, State_WriteBack, State_Commit, State_Done = Value
}

// Dispatch module definition
class Dispatch extends Module {
  val io = IO(new Bundle {
    // Define all inputs and outputs according to the XML specification
    val DispatchCount = Input(UInt(2.W))
    val insn1 = Input(UInt(128.W))
    val insn2 = Input(UInt(128.W))
    val Scheduler0_AvailPort = Input(UInt(2.W))
    val Scheduler1_AvailPort = Input(UInt(4.W))
    val Scheduler2_AvailPort = Input(UInt(4.W))
    val Insn1_excp_vaild = Input(Bool())
    val Insn2_excp_vaild = Input(Bool())
    val insn1_Function_type = Input(UInt(3.W))
    val insn2_Function_type = Input(UInt(3.W))
    val insn1_rd = Input(UInt(5.W))
    val insn2_rd = Input(UInt(5.W))
    val Insn1_LSQTag_in = Input(UInt(4.W))
    val insn1_PhyRs1_in = Input(UInt(6.W))
    val insn1_PhyRs2_in = Input(UInt(6.W))
    val insn1_PhyRd_in = Input(UInt(6.W))
    val insn1_LPhyRd_in = Input(UInt(6.W))
    val Insn1_RobTag_in = Input(UInt(4.W))
    val Insn1_State_in = Input(InsnState())
    val Insn2_LSQTag_in = Input(UInt(4.W))
    val insn2_PhyRs1_in = Input(UInt(6.W))
    val insn2_PhyRs2_in = Input(UInt(6.W))
    val insn2_PhyRd_in = Input(UInt(6.W))
    val insn2_LPhyRd_in = Input(UInt(6.W))
    val Insn2_RobTag_in = Input(UInt(4.W))
    val Insn2_State_in = Input(InsnState())

    // Outputs
    val Issue_num1 = Output(UInt(2.W))
    val Issue_num2 = Output(UInt(2.W))
    // Repeating the inputs as outputs as well since they're part of the output interface
    val insn1_out = Output(UInt(128.W))
    val insn2_out = Output(UInt(128.W))
    val Insn1_LSQTag = Output(UInt(4.W))
    val insn1_PhyRs1 = Output(UInt(6.W))
    val insn1_PhyRs2 = Output(UInt(6.W))
    val insn1_PhyRd = Output(UInt(6.W))
    val insn1_LPhyRd = Output(UInt(6.W))
    val Insn1_RobTag = Output(UInt(4.W))
    val Insn1_State = Output(InsnState())
    val Insn2_LSQTag = Output(UInt(4.W))
    val insn2_PhyRs1 = Output(UInt(6.W))
    val insn2_PhyRs2 = Output(UInt(6.W))
    val insn2_PhyRd = Output(UInt(6.W))
    val insn2_LPhyRd = Output(UInt(6.W))
    val Insn2_RobTag = Output(UInt(4.W))
    val Insn2_State = Output(InsnState())
  })

  // Parameters as specified
  val Issue0_function_type = "b010".U(3.W)
  val Issue1_function_type1 = "b011".U(3.W)
  val Issue1_function_type2 = "b100".U(3.W)
  val Issue2_function_type1 = "b000".U(3.W)
  val Issue2_function_type2 = "b001".U(3.W)

  // Repeat for insn2 as well
  when(io.Insn1_excp_vaild) {
    // Handle exceptions for instruction 1
    io.Insn1_State := InsnState.State_Commit
    io.Issue_num1 := 0.U // Set to an invalid queue number
  }.elsewhen(io.insn1_Function_type === Issue0_function_type) {
    // Check against Issue0 function type
    io.Insn1_State := InsnState.State_Issue
    io.Issue_num1 := 0.U
  }.elsewhen(io.insn1_Function_type === Issue1_function_type1 || io.insn1_Function_type === Issue1_function_type2) {
    // Check against Issue1 function types
    io.Insn1_State := InsnState.State_Issue
    io.Issue_num1 := 1.U
  }.elsewhen(io.insn1_Function_type === Issue2_function_type1 || io.insn1_Function_type === Issue2_function_type2) {
    // Check against Issue2 function types
    io.Insn1_State := InsnState.State_Issue
    io.Issue_num1 := 2.U
  }.otherwise {
    io.Issue_num1 := "b11".U // Indicate no match found with queue number set to 4 (outside valid range)
    io.Insn1_State := InsnState.State_Issue
  }
  
// Logic for insn2
when(io.Insn2_excp_vaild) {
  // Handle exceptions for instruction 2
  io.Insn2_State := InsnState.State_Commit
  io.Issue_num2 := 0.U // Set to an invalid queue number
}.elsewhen(io.insn2_Function_type === Issue0_function_type) {
  // Check against Issue0 function type
  io.Insn2_State := InsnState.State_Issue
  io.Issue_num2 := 0.U
}.elsewhen(io.insn2_Function_type === Issue1_function_type1 || io.insn2_Function_type === Issue1_function_type2) {
  // Check against Issue1 function types
  io.Insn2_State := InsnState.State_Issue
  io.Issue_num2 := 1.U
}.elsewhen(io.insn2_Function_type === Issue2_function_type1 || io.insn2_Function_type === Issue2_function_type2) {
  // Check against Issue2 function types
  io.Insn2_State := InsnState.State_Issue
  io.Issue_num2 := 2.U
}.otherwise {
  io.Issue_num2 := "b11".U // Indicate no match found with queue number set to 4 (outside valid range)
  io.Insn2_State := InsnState.State_Issue
}

  
  // Complete output assignments (assuming pass-through logic)
io.insn1_out := io.insn1
io.Insn1_LSQTag := io.Insn1_LSQTag_in
io.insn1_PhyRs1 := io.insn1_PhyRs1_in
io.insn1_PhyRs2 := io.insn1_PhyRs2_in
io.insn1_PhyRd := io.insn1_PhyRd_in
io.insn1_LPhyRd := io.insn1_LPhyRd_in
io.Insn1_RobTag := io.Insn1_RobTag_in
io.Insn1_State := io.Insn1_State_in

io.insn2_out := io.insn2
io.Insn2_LSQTag := io.Insn2_LSQTag_in
io.insn2_PhyRs1 := io.insn2_PhyRs1_in
io.insn2_PhyRs2 := io.insn2_PhyRs2_in
io.insn2_PhyRd := io.insn2_PhyRd_in
io.insn2_LPhyRd := io.insn2_LPhyRd_in
io.Insn2_RobTag := io.Insn2_RobTag_in
io.Insn2_State := io.Insn2_State_in
}

// The Chisel main object to generate the Verilog code
object DispatchMain extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Dispatch)
}
