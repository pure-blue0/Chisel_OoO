import chisel3._
import chisel3.util._

class Dispatch extends Module {
  val io = IO(new Bundle {
    // Inputs
    val DispatchCount = Input(UInt(2.W))
    val insn1 = Input(UInt(128.W))
    val insn2 = Input(UInt(128.W))
    val Insn1_LSQTag_in = Input(UInt(4.W))
    val Insn2_LSQTag_in = Input(UInt(4.W))
    val Insn1_RobTag_in = Input(UInt(4.W))
    val Insn2_RobTag_in = Input(UInt(4.W))
    val insn1_PhyRs1_in = Input(UInt(6.W))
    val insn1_PhyRs2_in = Input(UInt(6.W))
    val insn1_PhyRd_in = Input(UInt(6.W))
    val insn1_LPhyRd_in = Input(UInt(6.W))
    val insn2_PhyRs1_in = Input(UInt(6.W))
    val insn2_PhyRs2_in = Input(UInt(6.W))
    val insn2_PhyRd_in = Input(UInt(6.W))
    val insn2_LPhyRd_in = Input(UInt(6.W))

    // Outputs
    val Issue_num1 = Output(UInt(2.W))
    val insn1_Out = Output(UInt(128.W))
    val Insn1_LSQTag = Output(UInt(4.W))
    val insn1_PhyRs1 = Output(UInt(6.W))
    val insn1_PhyRs2 = Output(UInt(6.W))
    val insn1_PhyRd = Output(UInt(6.W))
    val insn1_LPhyRd = Output(UInt(6.W))
    val Insn1_RobTag = Output(UInt(4.W))

    val Issue_num2 = Output(UInt(2.W))
    val insn2_Out = Output(UInt(128.W))
    val Insn2_LSQTag = Output(UInt(4.W))
    val insn2_PhyRs1 = Output(UInt(6.W))
    val insn2_PhyRs2 = Output(UInt(6.W))
    val insn2_PhyRd = Output(UInt(6.W))
    val insn2_LPhyRd = Output(UInt(6.W))
    val Insn2_RobTag = Output(UInt(4.W))
  })

  // Initialize all outputs
  io.Issue_num1 := 3.U
  io.insn1_Out := 0.U
  io.Insn1_LSQTag := 0.U
  io.insn1_PhyRs1 := 0.U
  io.insn1_PhyRs2 := 0.U
  io.insn1_PhyRd := 0.U
  io.insn1_LPhyRd := 0.U
  io.Insn1_RobTag := 0.U

  io.Issue_num2 := 3.U
  io.insn2_Out := 0.U
  io.Insn2_LSQTag := 0.U
  io.insn2_PhyRs1 := 0.U
  io.insn2_PhyRs2 := 0.U
  io.insn2_PhyRd := 0.U
  io.insn2_LPhyRd := 0.U
  io.Insn2_RobTag := 0.U

  def extractFields(insn: UInt) = {
    val Sub_OP = insn(56, 53)
    val Function_type = insn(52, 50)
    val IsaRd = insn(48, 44)
    val Insn_excp_vaild = insn(89)
    (Sub_OP, Function_type, IsaRd, Insn_excp_vaild)
  }

  // Function to check for NOP, FENCE, or MRET
  def isSpecialInstruction(Function_type: UInt, Sub_OP: UInt, IsaRd: UInt, Insn_excp_valid: Bool): Bool = {
    // Logic derived from reference model
    (Insn_excp_valid) ||
    (Function_type === 2.U && (Sub_OP === 9.U || Sub_OP === 7.U)) || // CSR with specific Sub_OP values
    (Function_type === 0.U && IsaRd === 0.U) // ALU with IsaRd 0 indicating NOP
  }
  when(io.DispatchCount > 0.U) {
    val extractedFields1 = extractFields(io.insn1)
    val Sub_OP1 = extractedFields1._1
    val Function_type1 = extractedFields1._2
    val IsaRd1 = extractedFields1._3
    val Insn_excp_vaild1 = extractedFields1._4
    when(!isSpecialInstruction(Function_type1, Sub_OP1, IsaRd1, Insn_excp_vaild1)) {
      io.insn1_Out := io.insn1
      io.Insn1_LSQTag := io.Insn1_LSQTag_in
      io.insn1_PhyRs1 := io.insn1_PhyRs1_in
      io.insn1_PhyRs2 := io.insn1_PhyRs2_in
      io.insn1_PhyRd := io.insn1_PhyRd_in
      io.insn1_LPhyRd := io.insn1_LPhyRd_in
      io.Insn1_RobTag := io.Insn1_RobTag_in
      when(Function_type1 === 2.U) {
        io.Issue_num1 := 0.U // CSR to ISSUE0
      }.elsewhen(Function_type1 === 3.U || Function_type1 === 4.U) {
        io.Issue_num1 := 1.U // LDU and STU to ISSUE1
      }.elsewhen(Function_type1 === 0.U || Function_type1 === 1.U) {
        io.Issue_num1 := 2.U // ALU and BRU to ISSUE2
      }
    }.otherwise {
      io.Issue_num1 := 3.U
    }
  }

  when(io.DispatchCount === 2.U) {
    val extractedFields2 = extractFields(io.insn2)
    val Sub_OP2 = extractedFields2._1
    val Function_type2 = extractedFields2._2
    val IsaRd2= extractedFields2._3
    val Insn_excp_vaild2 = extractedFields2._4
    when(!isSpecialInstruction(Function_type2, Sub_OP2, IsaRd2, Insn_excp_vaild2)) {
      io.insn2_Out := io.insn2
      io.Insn2_LSQTag := io.Insn2_LSQTag_in
      io.insn2_PhyRs1 := io.insn2_PhyRs1_in
      io.insn2_PhyRs2 := io.insn2_PhyRs2_in
      io.insn2_PhyRd := io.insn2_PhyRd_in
      io.insn2_LPhyRd := io.insn2_LPhyRd_in
      io.Insn2_RobTag := io.Insn2_RobTag_in
      when(Function_type2 === 2.U) {
        io.Issue_num2 := 0.U // CSR to ISSUE0
      }.elsewhen(Function_type2 === 3.U || Function_type2 === 4.U) {
        io.Issue_num2 := 1.U // LDU and STU to ISSUE1
      }.elsewhen(Function_type2 === 0.U || Function_type2 === 1.U) {
        io.Issue_num2 := 2.U // ALU and BRU to ISSUE2
      }
    }.otherwise {
      io.Issue_num2 := 3.U
    }
  }
}


object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new Dispatch))
}

