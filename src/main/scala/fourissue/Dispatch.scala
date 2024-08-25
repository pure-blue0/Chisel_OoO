import chisel3._
import chisel3.util._

class DecodeQueueEntry extends Bundle {
  val data_valid = Bool()
  val Pred_Taken = Bool()
  val Pred_Target = UInt(32.W)
  val Excp_Tval = UInt(32.W)
  val Excp_Cause = UInt(6.W)
  val Excp_valid = Bool()
  val Sub_OP = UInt(4.W)
  val Function_type = UInt(3.W)
  val ControlFlowInsn = Bool()
  val IsaRd = UInt(5.W)
  val Operand2Ready = Bool()
  val IsaRs2 = UInt(5.W)
  val Operand1Ready = Bool()
  val IsaRs1 = UInt(5.W)
  val imm = UInt(64.W)
  val Operand2 = UInt(64.W)
  val Operand1 = UInt(64.W)
  val insn = UInt(32.W)
  val pc = UInt(32.W)
}

class DispatchInputs extends Bundle {
  val Scheduler_AvailPort = Vec(5, UInt(4.W))
  val DispatchCount = UInt(3.W)
  val insn = Vec(4, new DecodeQueueEntry)
  val Insn_LSQTag_in = Vec(4, UInt(4.W))
  val Insn_RobTag_in = Vec(4, UInt(5.W))
  val insn_PhyRs1_in = Vec(4, UInt(6.W))
  val insn_PhyRs2_in = Vec(4, UInt(6.W))
  val insn_PhyRd_in = Vec(4, UInt(6.W))
  val insn_LPhyRd_in = Vec(4, UInt(6.W))
}

class DispatchOutputs extends Bundle {
  val insn_match_num = Vec(4, UInt(4.W))
  val insn = Vec(4, new DecodeQueueEntry)
  val Insn_LSQTag = Vec(4, UInt(4.W))
  val insn_PhyRs1 = Vec(4, UInt(6.W))
  val insn_PhyRs2 = Vec(4, UInt(6.W))
  val insn_PhyRd = Vec(4, UInt(6.W))
  val insn_LPhyRd = Vec(4, UInt(6.W))
  val Insn_RobTag = Vec(4, UInt(5.W))
}

class Dispatch extends Module {
  val io = IO(new Bundle {
    val in = Input(new DispatchInputs)
    val out = Output(new DispatchOutputs)
  })

  // Initialize insn_match_num to 0x7
  val insn_match_num = Wire(Vec(4,UInt(3.W))) //&&
  insn_match_num := VecInit(Seq.fill(4)(0x7.U))

  for (i <- 0 until 4) {
    when (i.U < io.in.DispatchCount) {
      val insn = io.in.insn(i)
      val Function_type = insn.Function_type
      val Sub_OP = insn.Sub_OP
      val IsaRd = insn.IsaRd
      val Excp_valid = insn.Excp_valid

      when (!Excp_valid && !((Function_type === 3.U && Sub_OP === 9.U) || (Function_type === 0.U && IsaRd === 0.U) || (Function_type === 3.U && Sub_OP === 7.U))) {
        val initialAcc = (false.B, 7.U)

        val (_, updatedInsnMatchNum) = (0 until 5).foldLeft(initialAcc) {
          case ((currentSuccess, currentInsnMatchNum), scheduler) =>
            val conditionMet = scheduler match {
              case 0 => Function_type === 2.U
              case 1 => Function_type === 3.U || Function_type === 4.U
              case 2 => Function_type === 0.U || Function_type === 1.U
              case 3 => Function_type === 3.U || Function_type === 4.U || Function_type === 1.U
              case 4 => Function_type === 0.U
            }

            val Allocate_count = if (i == 0) 0.U else (0 until i).map(j => (insn_match_num(j) === scheduler.U).asUInt).reduce(_ +& _)

            val shouldUpdate = !currentSuccess && conditionMet && (io.in.Scheduler_AvailPort(scheduler) > Allocate_count)

            val newSuccess = currentSuccess || shouldUpdate
            val newInsnMatchNum = Wire(UInt()) // Define the appropriate width for this wire
            newInsnMatchNum := Mux(shouldUpdate, scheduler.U, currentInsnMatchNum)

            (newSuccess, newInsnMatchNum)
        }

        insn_match_num(i) := updatedInsnMatchNum
      }
    }
  }

  // Connect inputs to outputs
    io.out.insn := io.in.insn
    io.out.Insn_LSQTag := io.in.Insn_LSQTag_in
    io.out.insn_PhyRs1 := io.in.insn_PhyRs1_in
    io.out.insn_PhyRs2 := io.in.insn_PhyRs2_in
    io.out.insn_PhyRd := io.in.insn_PhyRd_in
    io.out.insn_LPhyRd := io.in.insn_LPhyRd_in
    io.out.Insn_RobTag := io.in.Insn_RobTag_in
  io.out.insn_match_num := insn_match_num
}

object Dispatch extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Dispatch)
}