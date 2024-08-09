import chisel3._
import chisel3.util._

class TryDispatch extends Module {
  val io = IO(new Bundle {
    val insn_valid = Input(Vec(4, Bool()))
    val Scheduler_AvailPort = Input(Vec(5, UInt(4.W)))
    val Insn_excp_valid = Input(Vec(4, Bool()))
    val insn_Function_type = Input(Vec(4, UInt(3.W)))
    val insn_rd = Input(Vec(4, UInt(5.W)))
    val Insn_controlflow_valid = Input(Vec(4, Bool()))
    val success_dispatch_flag = Output(Vec(4, Bool()))
    val insn_match_num = Output(Vec(4, UInt(4.W)))
    val issue_success_count = Output(UInt(3.W))
  })

  // Initialization of variables
  val stop_flag = Wire(Vec(5, Bool()))
  val insn_match_num = Wire(Vec(4, UInt(3.W)))
  val success_dispatch_flag = Wire(Vec(4, Bool()))
  val success = Wire(Vec(21, Bool())) //&&

  stop_flag := VecInit(Seq.fill(5)(false.B))
  insn_match_num := VecInit(Seq.fill(4)("b111".U))
  success_dispatch_flag := VecInit(Seq.fill(4)(false.B))
  success := VecInit(Seq.fill(21)(false.B)) //&&

  // Calculate number of valid instructions
  val avail_count = io.insn_valid.map(_.asUInt).reduce(_ +& _)

  // Function Types for Each Scheduler
  val scheduler_function_types = Seq(
    Seq("b010".U), // Scheduler 0 supports CSR
    Seq("b011".U, "b100".U), // Scheduler 1 supports LDU, STU
    Seq("b000".U, "b001".U), // Scheduler 2 supports ALU, BRU
    Seq("b011".U, "b100".U, "b001".U), // Scheduler 3 supports LDU, STU, BRU
    Seq("b000".U)  // Scheduler 4 supports ALU
  )

  for (i <- 0 until 4) {
    when (i.U < avail_count) {
      when (stop_flag(i)) {
        stop_flag(i + 1) := true.B
      } .otherwise {
        when (io.Insn_excp_valid(i)) {
          success_dispatch_flag(i) := true.B
          stop_flag(i + 1) := true.B
        } .elsewhen (io.insn_rd(i) === 0.U && io.insn_Function_type(i) === "b000".U) { // NOP instruction
          success_dispatch_flag(i) := true.B
        } .otherwise {
          for (j <- 0 until 5) {
            when (!success(i*5+j)){
              val functionMatch = scheduler_function_types(j).map(_ === io.insn_Function_type(i)).reduce(_ || _) //&&
              printf("functionMatch=%d: i=:%d,j=%d,io.insn_Function_type(i) = %d\n", functionMatch,i.U,j.U,io.insn_Function_type(i))
              when (functionMatch) {

                val allocate_count = if (i == 0) 0.U else (0 until i).map(k => (insn_match_num(k) === j.U).asUInt).reduce(_ +& _)
                when (io.Scheduler_AvailPort(j) > allocate_count) {
                  printf("success_dispatch!: insn %d\n", i.U)
                  insn_match_num(i) := j.U
                  success_dispatch_flag(i) := true.B
                  when(j.U<4.U){success(i * 5 + j + 1) := true.B} //&&
                }
              }
            }.elsewhen(j.U<4.U){
              success(i * 5 + j + 1) := true.B //&&
            }
          }
          when (!success_dispatch_flag(i) || io.Insn_controlflow_valid(i)) {
            stop_flag(i + 1) := true.B
          }
        }
      }
    }
  }
  io.success_dispatch_flag := success_dispatch_flag
  io.insn_match_num := insn_match_num
  io.issue_success_count := success_dispatch_flag.map(_.asUInt).reduce(_ +& _)
}

// Generate the Verilog code
object TryDispatch extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new TryDispatch)
}
