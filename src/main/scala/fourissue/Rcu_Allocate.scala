import chisel3._
import chisel3.util._

class RcuAllocateIO extends Bundle {
  val reset_n = Input(Bool())
  val allocCount = Input(UInt(3.W))
  val Rob_Tail = Input(UInt(5.W))

  val Freelist_phyrd = Input(Vec(4, UInt(6.W)))

  val RN_Release_EN = Input(Vec(4, Bool()))
  val RN_Release_IsaRd = Input(Vec(4, UInt(5.W)))
  val RN_Release_Data = Input(Vec(4, UInt(6.W)))

  val insn_IsaRs1 = Input(Vec(4, UInt(5.W)))
  val insn_IsaRs2 = Input(Vec(4, UInt(5.W)))
  val insn_IsaRd = Input(Vec(4, UInt(5.W)))

  val BusyList_Update_EN = Output(Vec(4, Bool()))
  val BusyList_Update_PhyRd = Output(Vec(4, UInt(6.W)))

  val ROB_Entry_WEN = Output(Vec(4, Bool()))
  val insn_ROBTag = Output(Vec(4, UInt(5.W)))
  val insn_PhyRs1 = Output(Vec(4, UInt(6.W)))
  val insn_PhyRs2 = Output(Vec(4, UInt(6.W)))
  val insn_PhyRd = Output(Vec(4, UInt(6.W)))
  val insn_LPhyRd = Output(Vec(4, UInt(6.W)))

  val freelist_pop_num = Output(UInt(3.W))
}

class Rcu_Allocate extends Module {
  val io = IO(new RcuAllocateIO)

  // Rename table
  val m_IntRenameTable = RegInit(VecInit(Seq.fill(32)(0.U(6.W))))

  // Temporary variables
  val freelist_use = Wire(Vec(4,Bool())) //&&
  freelist_use := VecInit(Seq.fill(4)(false.B))

  // Initialize  signals
  for (i <- 0 until 4) {
    io.BusyList_Update_EN(i) := false.B
    io.BusyList_Update_PhyRd(i) := 0.U
    io.ROB_Entry_WEN(i) := false.B
    io.insn_ROBTag(i) := 0.U
    io.insn_PhyRs1(i) := 0.U
    io.insn_PhyRs2(i) := 0.U
    io.insn_PhyRd(i) := 0.U
    io.insn_LPhyRd(i) := 0.U
  }

  // Combinational logic
  for (i <- 0 until 4) {
    when (i.U < io.allocCount) {
      when (io.insn_IsaRd(i) =/= 0.U) {
        val freelist_usecount_temp = PopCount(freelist_use.take(i).map(_ === true.B))//&&
        io.insn_PhyRd(i) := io.Freelist_phyrd(freelist_usecount_temp)
        io.BusyList_Update_EN(i) := true.B
        io.BusyList_Update_PhyRd(i) := io.insn_PhyRd(i)
        freelist_use(i) := true.B
      }
    }
  }

  for (i <- 0 until 4) {
    when (i.U < io.allocCount) {
      io.insn_ROBTag(i) := (io.Rob_Tail + i.U) % 24.U
    }
  }

  for (i <- 0 until 4) {
    io.insn_PhyRs1(i) := (0 until i).foldLeft(m_IntRenameTable(io.insn_IsaRs1(i))) { (phyRs1, j) =>
      Mux(io.insn_IsaRs1(i) === io.insn_IsaRd(j), io.insn_PhyRd(j), phyRs1)
    }

    io.insn_PhyRs2(i) := (0 until i).foldLeft(m_IntRenameTable(io.insn_IsaRs2(i))) { (phyRs2, j) =>
      Mux(io.insn_IsaRs2(i) === io.insn_IsaRd(j), io.insn_PhyRd(j), phyRs2)
    }

    io.insn_LPhyRd(i) := (0 until i).foldLeft(m_IntRenameTable(io.insn_IsaRd(i))) { (lPhyRd, j) =>
      Mux(io.insn_IsaRd(i) === io.insn_IsaRd(j), io.insn_PhyRd(j), lPhyRd)
    }
  }

  io.ROB_Entry_WEN(0) := io.allocCount > 0.U
  io.ROB_Entry_WEN(1) := io.allocCount > 1.U
  io.ROB_Entry_WEN(2) := io.allocCount > 2.U
  io.ROB_Entry_WEN(3) := io.allocCount > 3.U

  io.freelist_pop_num := io.BusyList_Update_EN.count(_ === true.B)

  // Sequential logic
  when (!io.reset_n) {
    for (i <- 0 until 32) {
      m_IntRenameTable(i) := 0.U
    }
  } .otherwise {
    for (i <- 0 until 4) {
      when (i.U < io.allocCount && io.insn_IsaRd(i) =/= 0.U) {
        m_IntRenameTable(io.insn_IsaRd(i)) := io.insn_PhyRd(i)
      }
    }
    for (i <- 0 until 4) {
      when (io.RN_Release_EN(i)) {
        m_IntRenameTable(io.RN_Release_IsaRd(i)) := io.RN_Release_Data(i)
      }
    }
  }
}

object Rcu_Allocate extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Rcu_Allocate())
}
