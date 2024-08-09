import chisel3._
import chisel3.util._

class Lsq_Allocate extends Module {
  val io = IO(new Bundle {
    val Load_queue_tail = Input(UInt(4.W))
    val Store_queue_tail = Input(UInt(4.W))
    val allocCount = Input(UInt(3.W))
    val insn_Function_type = Input(Vec(4, UInt(3.W)))
    val insn_subop = Input(Vec(4, UInt(3.W)))
    val insn_RobTag = Input(Vec(4, UInt(5.W)))
    val insn_IsaRd = Input(Vec(4, UInt(5.W)))
    val insn_PhyRd = Input(Vec(4, UInt(6.W)))

    val Insn_LSQTag = Output(Vec(4, UInt(4.W)))
    val LSQ_Style_Group = Output(Vec(4, UInt(2.W)))
    val LSQ_data_Function_type = Output(Vec(4, UInt(3.W)))
    val LSQ_data_subop = Output(Vec(4, UInt(3.W)))
    val LSQ_data_RobTag = Output(Vec(4, UInt(5.W)))
    val LSQ_data_IsaRd = Output(Vec(4, UInt(5.W)))
    val LSQ_data_PhyRd = Output(Vec(4, UInt(6.W)))
  })

  val m_LoadQueue_count = 16.U // Assuming load queue size is 16
  val m_StoreQueue_count = 16.U // Assuming store queue size is 16

  for (i <- 0 until 4) {
    when (i.U < io.allocCount) {
      when (io.insn_Function_type(i) === 3.U) { // LDU
        io.LSQ_Style_Group(i) := 1.U
        val AllocatedIn_load_count = if ( i==0 ) 0.U else ((0 until i).map(j => (io.insn_Function_type(j) === 3.U).asUInt)).reduce(_ +& _) //&&
        io.Insn_LSQTag(i) := (io.Load_queue_tail + AllocatedIn_load_count) % m_LoadQueue_count
      } .elsewhen (io.insn_Function_type(i) === 4.U) { // STU
        io.LSQ_Style_Group(i) := 2.U
        val AllocatedIn_store_count = if ( i==0 ) 0.U else ((0 until i).map(j => (io.insn_Function_type(j) === 4.U).asUInt)).reduce(_ +& _) //&&
        io.Insn_LSQTag(i) := (io.Store_queue_tail + AllocatedIn_store_count) % m_StoreQueue_count
      } .otherwise {
        io.LSQ_Style_Group(i) := 0.U
        io.Insn_LSQTag(i) := 0.U
      }
      io.LSQ_data_Function_type(i) := io.insn_Function_type(i)
      io.LSQ_data_subop(i) := io.insn_subop(i)
      io.LSQ_data_RobTag(i) := io.insn_RobTag(i)
      io.LSQ_data_IsaRd(i) := io.insn_IsaRd(i)
      io.LSQ_data_PhyRd(i) := io.insn_PhyRd(i)
    } .otherwise {
      io.LSQ_Style_Group(i) := 0.U
      io.Insn_LSQTag(i) := 0.U
      io.LSQ_data_Function_type(i) := 0.U
      io.LSQ_data_subop(i) := 0.U
      io.LSQ_data_RobTag(i) := 0.U
      io.LSQ_data_IsaRd(i) := 0.U
      io.LSQ_data_PhyRd(i) := 0.U
    }
  }
}

object Lsq_Allocate extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Lsq_Allocate)
}
