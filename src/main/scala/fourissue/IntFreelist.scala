import chisel3._
import chisel3.util._

class IntFreelist(val FreelistSize: Int = 63) extends Module {
  val io = IO(new Bundle {
    val reset_n = Input(Bool())
    val freelist_pop_num = Input(UInt(3.W))
    val FE_Release_EN = Input(Vec(4, Bool()))
    val FE_Release_phyRd = Input(Vec(4, UInt(6.W)))
    val FE_Commit_EN = Input(Vec(4, Bool()))
    val FE_Commit_LphyRd = Input(Vec(4, UInt(6.W)))
    val Header_PhyID = Output(UInt(6.W))
    val Next_Header_PhyID = Output(UInt(6.W))
    val Third_Header_PhyID = Output(UInt(6.W))
    val Forth_Header_PhyID = Output(UInt(6.W))
    val FreeList_AvailEntryCount = Output(UInt(7.W))
  })

  val freelist = RegInit(VecInit(Seq.tabulate(FreelistSize)(i => (i + 1).U(7.W))))
  val head_ptr = RegInit(0.U(log2Ceil(FreelistSize).W))
  val tail_ptr = RegInit(0.U(log2Ceil(FreelistSize).W))
  val FreeList_AvailEntryCount = RegInit((FreelistSize).U)

  when(!io.reset_n) {
    head_ptr := 0.U
    tail_ptr := 0.U
    FreeList_AvailEntryCount := (FreelistSize).U
    for (i <- 0 until FreelistSize) {
      freelist(i) := (i + 1).U
    }
  } .otherwise {
    // Pop entries logic
    head_ptr := (head_ptr +& io.freelist_pop_num) % FreelistSize.U //&&


    // Push entries logic using foldLeft
    val (new_tail_ptr, new_FreeList_AvailEntryCount) = (0 until 4).foldLeft((tail_ptr, FreeList_AvailEntryCount)) {
      case ((current_tail_ptr, current_FreeList_AvailEntryCount), i) =>
        val updated_tail_ptr = Wire(UInt(log2Ceil(FreelistSize).W))
        val updated_FreeList_AvailEntryCount = Wire(UInt(log2Ceil(FreelistSize).W))

        when(io.FE_Release_EN(i) && io.FE_Commit_EN(i)) {  //&&
          freelist(current_tail_ptr) := io.FE_Release_phyRd(i)
          freelist((current_tail_ptr +& 1.U) % FreelistSize.U) := io.FE_Commit_LphyRd(i)
          updated_tail_ptr := (current_tail_ptr +& 2.U) % FreelistSize.U
          updated_FreeList_AvailEntryCount := current_FreeList_AvailEntryCount + 2.U
        } .elsewhen(io.FE_Release_EN(i)) {
          freelist(current_tail_ptr) := io.FE_Release_phyRd(i)
          updated_tail_ptr := (current_tail_ptr +& 1.U) % FreelistSize.U
          updated_FreeList_AvailEntryCount := current_FreeList_AvailEntryCount + 1.U
        } .elsewhen(io.FE_Commit_EN(i)) {
          freelist(current_tail_ptr) := io.FE_Commit_LphyRd(i)
          updated_tail_ptr := (current_tail_ptr +& 1.U) % FreelistSize.U
          updated_FreeList_AvailEntryCount := current_FreeList_AvailEntryCount + 1.U
        } .otherwise {
          updated_tail_ptr := current_tail_ptr
          updated_FreeList_AvailEntryCount := current_FreeList_AvailEntryCount
        }

        (updated_tail_ptr, updated_FreeList_AvailEntryCount)
    }

    tail_ptr := new_tail_ptr
    FreeList_AvailEntryCount := new_FreeList_AvailEntryCount  - io.freelist_pop_num //&&
  }

  // Wire signals
  io.Header_PhyID := freelist(head_ptr)//&&
  io.Next_Header_PhyID := freelist((head_ptr +& 1.U) % FreelistSize.U) //&&
  io.Third_Header_PhyID := freelist((head_ptr +& 2.U) % FreelistSize.U) //&&
  io.Forth_Header_PhyID := freelist((head_ptr +& 3.U) % FreelistSize.U) //&&
  io.FreeList_AvailEntryCount := FreeList_AvailEntryCount
}

object IntFreelist extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new IntFreelist(63))
}
