import chisel3._
import chisel3.util._

class DecodeStage extends Module {
  val io = IO(new Bundle {
    // Inputs
    val data_valid = Input(Bool())
    val Address = Input(UInt(32.W))
    val data = Input(UInt(128.W))
    val excp_valid = Input(Bool())
    val excp_cause = Input(UInt(6.W))
    val excpt_tval = Input(UInt(32.W))
    val decode_flush_flag = Input(Bool())
    val isStalled = Input(Bool())
    val pop_count = Input(UInt(2.W))

    // Outputs
    val entry_out1 = Output(UInt(322.W))
    val entry_out2 = Output(UInt(322.W))
    val entry_out3 = Output(UInt(322.W))
    val entry_out4 = Output(UInt(322.W))
    val available_entries = Output(UInt(8.W))
    val Redirect_valid = Output(Bool())
    val Redirect_Target = Output(UInt(32.W))
    val fetch_flush = Output(Bool())
  })

  // Instantiate Predecode module
  val predecode = Module(new Predecode)
  predecode.io.Address := io.Address
  predecode.io.data := io.data
  predecode.io.excp_valid := io.excp_valid
  predecode.io.excp_cause := io.excp_cause
  predecode.io.excpt_tval := io.excpt_tval

  // Instantiate Decode_Func modules for each instruction
  val decodeFunc1 = Module(new Decode_Func)
  decodeFunc1.io.Pc := predecode.io.pc1
  decodeFunc1.io.Insn := predecode.io.insn1
  decodeFunc1.io.predecode_excp_valid := predecode.io.excp_valid1
  decodeFunc1.io.predecode_excp_cause := predecode.io.excp_cause1
  decodeFunc1.io.predecode_excpt_tval := predecode.io.excpt_tval1

  val decodeFunc2 = Module(new Decode_Func)
  decodeFunc2.io.Pc := predecode.io.pc2
  decodeFunc2.io.Insn := predecode.io.insn2
  decodeFunc2.io.predecode_excp_valid := predecode.io.excp_valid2
  decodeFunc2.io.predecode_excp_cause := predecode.io.excp_cause2
  decodeFunc2.io.predecode_excpt_tval := predecode.io.excpt_tval2

  val decodeFunc3 = Module(new Decode_Func)
  decodeFunc3.io.Pc := predecode.io.pc3
  decodeFunc3.io.Insn := predecode.io.insn3
  decodeFunc3.io.predecode_excp_valid := predecode.io.excp_valid3
  decodeFunc3.io.predecode_excp_cause := predecode.io.excp_cause3
  decodeFunc3.io.predecode_excpt_tval := predecode.io.excpt_tval3

  val decodeFunc4 = Module(new Decode_Func)
  decodeFunc4.io.Pc := predecode.io.pc4
  decodeFunc4.io.Insn := predecode.io.insn4
  decodeFunc4.io.predecode_excp_valid := predecode.io.excp_valid4
  decodeFunc4.io.predecode_excp_cause := predecode.io.excp_cause4
  decodeFunc4.io.predecode_excpt_tval := predecode.io.excpt_tval4

  // Pack decoded instructions into 322-bit entries
  def packDecodedInsn(decodeFunc: Decode_Func): UInt = {
    Cat(
      decodeFunc.io.excpt_tval,
      decodeFunc.io.excp_cause,
      decodeFunc.io.excp_valid,
      decodeFunc.io.Sub_OP,
      decodeFunc.io.Function_type,
      decodeFunc.io.ControlFlowInsn,
      decodeFunc.io.IsaRd,
      decodeFunc.io.Operand2Ready,
      decodeFunc.io.IsaRs2,
      decodeFunc.io.Operand1Ready,
      decodeFunc.io.IsaRs1,
      decodeFunc.io.imm,
      decodeFunc.io.Operand2,
      decodeFunc.io.Operand1,
      decodeFunc.io.Insn,
      decodeFunc.io.Pc
    )
  }

  val decodedInsn1 = Cat(Mux(io.data_valid,predecode.io.insn_valid1,0.U),predecode.io.Perd_Taken1,predecode.io.Pred_Target1,packDecodedInsn(decodeFunc1))
  val decodedInsn2 = Cat(Mux(io.data_valid,predecode.io.insn_valid2,0.U),predecode.io.Perd_Taken2,predecode.io.Pred_Target2,packDecodedInsn(decodeFunc2))
  val decodedInsn3 = Cat(Mux(io.data_valid,predecode.io.insn_valid3,0.U),predecode.io.Perd_Taken3,predecode.io.Pred_Target3,packDecodedInsn(decodeFunc3))
  val decodedInsn4 = Cat(Mux(io.data_valid,predecode.io.insn_valid4,0.U),predecode.io.Perd_Taken4,predecode.io.Pred_Target4,packDecodedInsn(decodeFunc4))

  // Instantiate DecodeQueue module
  val decodeQueue = Module(new DecodeQueue)
  decodeQueue.io.reset_n := !reset.asBool()
  decodeQueue.io.decode_flush_flag := io.decode_flush_flag
  decodeQueue.io.decoded_insn1 := decodedInsn1
  decodeQueue.io.decoded_insn2 := decodedInsn2
  decodeQueue.io.decoded_insn3 := decodedInsn3
  decodeQueue.io.decoded_insn4 := decodedInsn4
  decodeQueue.io.WEN1 := decodedInsn1(decodedInsn1.getWidth-1)
  decodeQueue.io.WEN2 := decodedInsn2(decodedInsn2.getWidth-1)
  decodeQueue.io.WEN3 := decodedInsn3(decodedInsn3.getWidth-1)
  decodeQueue.io.WEN4 := decodedInsn4(decodedInsn4.getWidth-1)
  decodeQueue.io.isStalled := io.isStalled
  decodeQueue.io.pop_count := io.pop_count

  // Connect outputs
  io.entry_out1 := decodeQueue.io.entry_out1
  io.entry_out2 := decodeQueue.io.entry_out2
  io.entry_out3 := decodeQueue.io.entry_out3
  io.entry_out4 := decodeQueue.io.entry_out4
  io.available_entries := decodeQueue.io.available_entries
  io.Redirect_valid := Mux(io.data_valid,predecode.io.Redirect_valid,0.U)
  io.Redirect_Target := predecode.io.Redirect_Target
  io.fetch_flush := predecode.io.fetch_flush

}

object DecodeStage extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new DecodeStage)
}
