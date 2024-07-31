import chisel3._
import chisel3.util._

class RcuAllocate extends Module {
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
  })

  // Initialize the IntRenameTable
  val IntRenameTable = RegInit(VecInit(Seq.fill(40)(0.U(6.W))))

  // Outputs initialization
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

  // Update IntRenameTable on EN_Update
  when(io.EN_Update) {
    IntRenameTable(io.IsaRd_Update) := io.Data_Update
  }

  // Allocation logic for insn1
  when(io.allocCount > 0.U) {
    io.insn1_PhyRs1 := IntRenameTable(io.insn1_IsaRs1)
    io.insn1_PhyRs2 := IntRenameTable(io.insn1_IsaRs2)
    io.insn1_LPhyRd := IntRenameTable(io.insn1_IsaRd)

    when(io.insn1_IsaRd =/= 0.U) {
      io.WEN1_IntBusylist := true.B
      io.insn1_PhyRd := io.IntFreelist_phyrd1
      IntRenameTable(io.insn1_IsaRd) := io.insn1_PhyRd
    }
  }

  // Allocation logic for insn2
  when(io.allocCount === 2.U) {
    io.insn2_PhyRs1 := IntRenameTable(io.insn2_IsaRs1)
    io.insn2_PhyRs2 := IntRenameTable(io.insn2_IsaRs2)
    io.insn2_LPhyRd := IntRenameTable(io.insn2_IsaRd)

    when(io.insn2_IsaRd =/= 0.U) {
      io.WEN2_IntBusylist := true.B
      when(io.WEN1_IntBusylist) {
        io.insn2_PhyRd := io.IntFreelist_phyrd2
      }.otherwise {
        io.insn2_PhyRd := io.IntFreelist_phyrd1
      }
      IntRenameTable(io.insn2_IsaRd) := io.insn2_PhyRd
    }

    // Handling dependencies between insn1 and insn2
    when(io.insn1_IsaRd =/= 0.U && io.insn1_IsaRd === io.insn2_IsaRs1) {
      io.insn2_PhyRs1 := io.insn1_PhyRd
    }
    when(io.insn1_IsaRd =/= 0.U && io.insn1_IsaRd === io.insn2_IsaRs2) {
      io.insn2_PhyRs2 := io.insn1_PhyRd
    }
    when(io.insn1_IsaRd =/= 0.U && io.insn1_IsaRd === io.insn2_IsaRd) {
      io.insn2_LPhyRd := io.insn1_PhyRd
    }
  }

  // Update Freelist_pop_num based on allocation
  when(io.WEN1_IntBusylist && io.WEN2_IntBusylist){
    io.Freelist_pop_num :=2.U
  }.elsewhen(io.WEN1_IntBusylist || io.WEN2_IntBusylist){
    io.Freelist_pop_num :=1.U
  }.otherwise{
    io.Freelist_pop_num :=0.U
  }

}
object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new RcuAllocate))
}
