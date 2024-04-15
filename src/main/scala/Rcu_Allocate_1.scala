import chisel3._
import chisel3.util._

class Rcu_Allocate extends Module {
  val io = IO(new Bundle {
    // Inputs
    val allocCount = Input(UInt(2.W))
    val EN_Update = Input(Bool())
    val IsaRd_Update = Input(UInt(5.W))
    val Data_Update = Input(UInt(6.W))
    val IntFreelist_phyrd1 = Input(UInt(6.W))
    val IntFreelist_phyrd2 = Input(UInt(6.W))
    val insn1_IsaRs1 = Input(UInt(5.W))
    val insn1_IsaRs2 = Input(UInt(5.W))
    val insn1_IsaRd = Input(UInt(5.W))
    val insn2_IsaRs1 = Input(UInt(5.W))
    val insn2_IsaRs2 = Input(UInt(5.W))
    val insn2_IsaRd = Input(UInt(5.W))

    // Outputs
    val WEN1_IntBusylist = Output(Bool())
    val WEN2_IntBusylist = Output(Bool())
    val insn1_PhyRs1 = Output(UInt(6.W))
    val insn1_PhyRs2 = Output(UInt(6.W))
    val insn1_PhyRd = Output(UInt(6.W))
    val insn1_LPhyRd = Output(UInt(6.W))
    val insn2_PhyRs1 = Output(UInt(6.W))
    val insn2_PhyRs2 = Output(UInt(6.W))
    val insn2_PhyRd = Output(UInt(6.W))
    val insn2_LPhyRd = Output(UInt(6.W))
    val Freelist_pop_num = Output(UInt(2.W))
    val WEN1_CreateRob = Output(Bool())
    val WEN2_CreateRob = Output(Bool())
  })

  // Integer Rename Table, depth of 32, width of 6
  val IntRenameTable = Reg(Vec(32, UInt(6.W)))

  // Default Output Values
  io.WEN1_IntBusylist := false.B
  io.WEN2_IntBusylist := false.B
  io.insn1_PhyRs1 := 0.U
  io.insn1_PhyRs2 := 0.U
  io.insn1_PhyRd := 0.U
  io.insn1_LPhyRd := 0.U
  io.insn2_PhyRs1 := 0.U
  io.insn2_PhyRs2 := 0.U
  io.insn2_PhyRd := 0.U
  io.insn2_LPhyRd := 0.U
  io.Freelist_pop_num := 0.U
  io.WEN1_CreateRob := false.B
  io.WEN2_CreateRob := false.B

  // Update IntRenameTable if EN_Update is true
  when(io.EN_Update) {
    IntRenameTable(io.IsaRd_Update) := io.Data_Update
  }

  // Function to map ISA registers to physical registers
  def lookupPhyReg(isaReg: UInt, freelistPhyRd: UInt): UInt = {
    when(isaReg === 0.U) {
      0.U // Zero register maps to zero
    }.otherwise {
      IntRenameTable(isaReg)
    }
  }

  // Allocate and update for instruction 1
  val insn1_PhyRs1 = lookupPhyReg(io.insn1_IsaRs1, io.IntFreelist_phyrd1)
  val insn1_PhyRs2 = lookupPhyReg(io.insn1_IsaRs2, io.IntFreelist_phyrd1)
  val insn1_LPhyRd = lookupPhyReg(io.insn1_IsaRd, io.IntFreelist_phyrd1)
  
  // Allocate and update for instruction 2
  val insn2_PhyRs1 = lookupPhyReg(io.insn2_IsaRs1, io.IntFreelist_phyrd2)
  val insn2_PhyRs2 = lookupPhyReg(io.insn2_IsaRs2, io.IntFreelist_phyrd2)
  val insn2_LPhyRd = lookupPhyReg(io.insn2_IsaRd, io.IntFreelist_phyrd2)

  // Assigning physical register IDs based on freelist availability
  when(io.allocCount > 0.U) {
    io.insn1_PhyRs1 := insn1_PhyRs1
    io.insn1_PhyRs2 := insn1_PhyRs2
    io.insn1_LPhyRd := insn1_LPhyRd
    io.insn1_PhyRd := io.IntFreelist_phyrd1
    io.WEN1_IntBusylist := true.B
    io.WEN1_CreateRob := true.B
    io.Freelist_pop_num := 1.U

    when(io.allocCount === 2.U) {
      io.insn2_PhyRs1 := insn2_PhyRs1
      io.insn2_PhyRs2 := insn2_PhyRs2
      io.insn2_LPhyRd := insn2_LPhyRd
      io.insn2_PhyRd := io.IntFreelist_phyrd2
      io.WEN2_IntBusylist := true.B
      io.WEN2_CreateRob := true.B
      io.Freelist_pop_num := 2.U
    }
  }

  // Handle data dependency between insn1 and insn2
  when(io.allocCount === 2.U && io.insn1_IsaRd =/= 0.U && (io.insn1_IsaRd === io.insn2_IsaRs1 || io.insn1_IsaRd === io.insn2_IsaRs2)) {
    when(io.insn1_IsaRd === io.insn2_IsaRs1) {
      io.insn2_PhyRs1 := insn1_PhyRs1
    }

    when(io.insn1_IsaRd === io.insn2_IsaRs2) {
      io.insn2_PhyRs2 := insn1_PhyRs2
    }
  }
}

object GenerateRcuAllocate extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Rcu_Allocate(), Array("--target-dir", "generated"))
}
