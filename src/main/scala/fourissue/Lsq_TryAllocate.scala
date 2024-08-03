import chisel3._
import chisel3.util._

class Lsq_TryAllocate extends Module {
  val io = IO(new Bundle {
    val insn_valid0 = Input(Bool())
    val insn_valid1 = Input(Bool())
    val insn_valid2 = Input(Bool())
    val insn_valid3 = Input(Bool())
    val insn1_Function_type = Input(UInt(3.W))
    val insn2_Function_type = Input(UInt(3.W))
    val insn3_Function_type = Input(UInt(3.W))
    val insn4_Function_type = Input(UInt(3.W))
    val LoadQueue_unuse_count = Input(UInt(4.W))
    val StoreQueue_unuse_count = Input(UInt(4.W))
    val lsq_success_count = Output(UInt(3.W))
  })

  // Step 1: Initialize variables
  val match_num = Wire(Vec(4, UInt(2.W)))
  val success_dispatch_flag = Wire(Vec(4, Bool()))
  val stop_flag = Wire(Vec(5, Bool())) //&&
  match_num := VecInit(Seq.fill(4)(0.U))
  success_dispatch_flag := VecInit(Seq.fill(4)(false.B))
  stop_flag := VecInit(Seq.fill(5)(false.B))

  // Step 2: Calculate the number of valid instructions
  val avail_count = io.insn_valid0 +& io.insn_valid1 +& io.insn_valid2 +& io.insn_valid3

  // Step 3: Traverse the valid instructions
  for (i <- 0 until 4) {
    when (i.U < avail_count) {
      when (stop_flag(i)) {
        when (i.U < 3.U) { // Ensure we don't access stop_flag(4)
          stop_flag(i + 1) := true.B
        }
      } .otherwise {
        val insn_function_type = MuxLookup(i.U, 0.U, Seq(
          0.U -> io.insn1_Function_type,
          1.U -> io.insn2_Function_type,
          2.U -> io.insn3_Function_type,
          3.U -> io.insn4_Function_type
        ))

        when (insn_function_type === 3.U) { // LDU
          val Load_Allocate_count = Wire(UInt(4.W))
          Load_Allocate_count := PopCount(match_num.take(i).map(_ === 1.U))

          when (io.LoadQueue_unuse_count > Load_Allocate_count) {
            success_dispatch_flag(i) := true.B
            match_num(i) := 1.U
          } .otherwise {
            when (i.U < 3.U) {
              stop_flag(i + 1) := true.B
            }
          }
        } .elsewhen (insn_function_type === 4.U) { // STU
          val Store_Allocate_count = Wire(UInt(4.W))
          Store_Allocate_count := PopCount(match_num.take(i).map(_ === 2.U))

          when (io.StoreQueue_unuse_count > Store_Allocate_count) {
            success_dispatch_flag(i) := true.B
            match_num(i) := 2.U
          } .otherwise {
            when (i.U < 3.U) {
              stop_flag(i + 1) := true.B
            }
          }
        } .otherwise {
          success_dispatch_flag(i) := true.B
        }
      }
    }
  }

  // Step 4: Calculate the number of successfully dispatched instructions
  io.lsq_success_count := PopCount(success_dispatch_flag)
}

// Generate the Verilog code
object Lsq_TryAllocate extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Lsq_TryAllocate())
}