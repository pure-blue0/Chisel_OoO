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
    val WEN1_CreateRob = Output(Bool())
    val WEN2_CreateRob = Output(Bool())
  })

  io.WEN1_IntBusylist := false.B
  io.WEN2_IntBusylist := false.B
  io.WEN1_CreateRob := false.B
  io.WEN2_CreateRob := false.B
  
  io.insn1_PhyRd := 0.U
  io.insn2_PhyRd := 0.U
  
  // Mock IntRenameTable for demonstration purposes
  val IntRenameTable = RegInit(VecInit(Seq.fill(32)(0.U(6.W))))

  // Update IntRenameTable logic
  when(io.EN_Update) {
    IntRenameTable(io.IsaRd_Update) := io.Data_Update
  }

  // Allocate Registers and Update IntRenameTable and IntBusylist
  val allocatedRegs = VecInit(io.IntFreelist_phyrd1, io.IntFreelist_phyrd2)
  val busyList = RegInit(VecInit(Seq.fill(64)(false.B))) // Assuming a 64-entry busy list for demonstration

  io.insn1_PhyRs1 := IntRenameTable(io.insn1_IsaRs1)
  io.insn1_PhyRs2 := IntRenameTable(io.insn1_IsaRs2)
  io.insn1_LPhyRd := IntRenameTable(io.insn1_IsaRd)
  
  io.insn2_PhyRs1 := IntRenameTable(io.insn2_IsaRs1)
  io.insn2_PhyRs2 := IntRenameTable(io.insn2_IsaRs2)
  io.insn2_LPhyRd := IntRenameTable(io.insn2_IsaRd)

  when(io.allocCount >= 1.U) {
    io.insn1_PhyRd := allocatedRegs(0)
    busyList(allocatedRegs(0)) := true.B
    IntRenameTable(io.insn1_IsaRd) := allocatedRegs(0)
    io.WEN1_IntBusylist := true.B
    io.WEN1_CreateRob := true.B
  }

  when(io.allocCount === 2.U) {
    io.insn2_PhyRd := allocatedRegs(1)
    busyList(allocatedRegs(1)) := true.B
    IntRenameTable(io.insn2_IsaRd) := allocatedRegs(1)
    io.WEN2_IntBusylist := true.B
    io.WEN2_CreateRob := true.B

    // Additional logic for handling dependencies between insn1 and insn2
    when(io.insn1_IsaRd === io.insn2_IsaRs1) {
      io.insn2_PhyRs1 := allocatedRegs(0)
    }
    when(io.insn1_IsaRd === io.insn2_IsaRs2) {
      io.insn2_PhyRs2 := allocatedRegs(0)
    }
    when(io.insn1_IsaRd === io.insn2_IsaRd) {
      io.insn2_LPhyRd := allocatedRegs(0)
    }
  }
}

// Generate the Verilog code (This line is to be run in your Scala build environment, not included in the Chisel source file)
// (new chisel3.stage.ChiselStage).emitVerilog(new Lsq_Allocate())

// Generate the Verilog code
object Rcu_Allocate extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new Rcu_Allocate(), Array("--target-dir", "generated"))
}
