import chisel3._
import chisel3.util._

class StoreQueueEntry extends Bundle {
  val Sub_OP = UInt(4.W)
  val LSQTag = UInt(4.W)
  val RobTag = UInt(4.W)
  val PhyRs1 = UInt(6.W)
  val PhyRs2 = UInt(6.W)
  val PhyRd = UInt(6.W)
  val LPhyRd = UInt(6.W)
  val state = UInt(2.W)
  val committed = Bool()
  val killed = Bool()
  val addressReady = Bool()
  val address = UInt(32.W)
  val dataReady = Bool()
  val data = UInt(64.W)
}

class StoreQueue(val store_queue_size: Int) extends Module {
  val io = IO(new Bundle {
    val clk = Input(Clock())
    val reset = Input(Bool())
    val Entry_data1 = Input(UInt(138.W))
    val Entry_data2 = Input(UInt(138.W))
    val LA_EN1 = Input(Bool())
    val LA_EN2 = Input(Bool())
    val Store_Write_valid = Input(Bool())
    val WB_addressReady = Input(Bool())
    val WB_address = Input(UInt(32.W))
    val WB_data = Input(UInt(32.W))
    val WB_LSQTag = Input(UInt(4.W))
    val TryIssue_valid = Input(Bool())
    val TryIssue_state = Input(UInt(2.W))
    val TryIssue_stqPtr = Input(UInt(4.W))
    val RR_valid1 = Input(Bool())
    val LSQtag1 = Input(UInt(6.W))
    val RR_valid2 = Input(Bool())
    val LSQtag2 = Input(UInt(6.W))
    val Commit_vaild1 = Input(Bool())
    val Commit_LSQTag1 = Input(UInt(4.W))
    val Commit_vaild2 = Input(Bool())
    val Commit_LSQTag2 = Input(UInt(4.W))
    val D_state_valid = Input(Bool())
    val D_LSQTag = Input(UInt(4.W))
    val AvailEntryCount = Output(UInt(4.W))
    val Empty = Output(Bool())
    val header = Output(UInt(4.W))
    val FrontQueue_committed = Output(Bool())
    val FrontQueue_state = Output(UInt(2.W))
    val FrontQueue_address = Output(UInt(32.W))
    val FrontQueue_data = Output(UInt(64.W))
    val FrontQueue_subop = Output(UInt(4.W))
  })

  val queue = RegInit(VecInit(Seq.fill(store_queue_size)(0.U.asTypeOf(new StoreQueueEntry))))
  val head = RegInit(0.U(log2Ceil(store_queue_size).W))
  val tail = RegInit(0.U(log2Ceil(store_queue_size).W))
  val nextTail = RegInit(1.U(log2Ceil(store_queue_size).W))
  val usage = RegInit(0.U(log2Ceil(store_queue_size + 1).W))

  // Reset logic
    when(io.reset) {
    queue.foreach(_ := 0.U.asTypeOf(new StoreQueueEntry))
    head := 0.U
    tail := 0.U
    usage := 0.U
  }

  // Logic for popping entries from the queue
  when(!io.reset && usage > 0.U) {
    when(queue(head).killed && queue(head).state =/= 1.U) {
      head := head + 1.U
      usage := usage - 1.U
    }
  }

  // Logic for updating state based on D_state_valid
  when(io.D_state_valid) {
    queue(io.D_LSQTag).state := 2.U
  }

//Write Logic for Entry_data1 and Entry_data2  
when(io.LA_EN1) {
  queue(tail) := io.Entry_data1.asTypeOf(new StoreQueueEntry)
  tail := Mux(tail === (store_queue_size.U - 1.U), 0.U, tail + 1.U)
  nextTail := Mux(nextTail === (store_queue_size.U - 1.U), 0.U, nextTail + 1.U)
  usage := usage + 1.U
}
when(io.LA_EN2) {
  queue(nextTail) := io.Entry_data2.asTypeOf(new StoreQueueEntry)
  tail := Mux(tail === (store_queue_size.U - 1.U), 0.U, tail + 1.U)
  nextTail := Mux(nextTail === (store_queue_size.U - 1.U), 0.U, nextTail + 1.U)
  usage := usage + 1.U
}

//Update Logic for Store_Write_valid:
when(io.Store_Write_valid) {
  val entryToUpdate = queue(io.WB_LSQTag)
  entryToUpdate.addressReady := io.WB_addressReady
  entryToUpdate.address := io.WB_address
  entryToUpdate.data := io.WB_data
  entryToUpdate.dataReady := true.B // Assuming dataReady should be set when we write
}

//Update Logic for TryIssue_valid:
    when(io.TryIssue_valid) {
  val entryToUpdate = queue(io.TryIssue_stqPtr)
  entryToUpdate.state := io.TryIssue_state
}

//Logic for Invalidating Entries Based on RR_valid1 and RR_valid2:
when(io.RR_valid1) {
  val entryToInvalidate1 = queue(io.LSQtag1)
  entryToInvalidate1.killed := true.B
}

when(io.RR_valid2) {
  val entryToInvalidate2 = queue(io.LSQtag2)
  entryToInvalidate2.killed := true.B
}

//Logic for Committing Entries Based on Commit_valid1 and Commit_valid2:
    when(io.Commit_valid1) {
  val entryToCommit1 = queue(io.Commit_LSQTag1)
  entryToCommit1.committed := true.B
}

when(io.Commit_valid2) {
  val entryToCommit2 = queue(io.Commit_LSQTag2)
  entryToCommit2.committed := true.B
}

//Logic for Circular Queue Management and Additional Logic for Circular Functionality:
// Logic for updating the head pointer and usage count for the circular buffer
when(queue(head).committed && usage > 0.U) {
  head := Mux(head === (store_queue_size.U - 1.U), 0.U, head + 1.U)
  usage := usage - 1.U
}

// Logic to handle the circular behavior of the tail pointer
when(io.LA_EN1 && usage < store_queue_size.U) {
  tail := Mux(tail === (store_queue_size.U - 1.U), 0.U, tail + 1.U)
  usage := usage + 1.U
}

when(io.LA_EN2 && (usage + 1.U) < store_queue_size.U) {
  val nextPosition = Mux(tail === (store_queue_size.U - 1.U), 0.U, tail + 1.U)
  when(!io.LA_EN1 || nextPosition =/= head) {
    tail := Mux(nextPosition === (store_queue_size.U - 1.U), 0.U, nextPosition + 1.U)
    usage := usage + 1.U
  }
}

// Logic for writing data into the queue
  when(io.LA_EN1) {
    queue(tail) := io.Entry_data1.asTypeOf(new StoreQueueEntry)
    tail := Mux(tail === (store_queue_size.U - 1.U), 0.U, tail + 1.U)
    usage := usage + 1.U
  }
  when(io.LA_EN2) {
    queue(tail) := io.Entry_data2.asTypeOf(new StoreQueueEntry)
    tail := Mux(tail === (store_queue_size.U - 1.U), 0.U, tail + 1.U)
    usage := usage + 1.U
  }

  // Update logic for entries based on input signals
  for (i <- 0 until store_queue_size) {
    when(io.UpdateAddressValid && queue(i).LSQTag === io.UpdateTag) {
      queue(i).address := io.NewAddress
      queue(i).addressReady := true.B
    }
    when(io.UpdateDataValid && queue(i).LSQTag === io.UpdateTag) {
      queue(i).data := io.NewData
      queue(i).dataReady := true.B
    }
  }

  // Outputs
  io.AvailEntryCount := store_queue_size.U - usage
  io.Empty := usage === 0.U
  io.header := head
  val frontEntry = queue(head)
  io.FrontQueue_committed := frontEntry.committed
  io.FrontQueue_state := frontEntry.state
  io.FrontQueue_address := frontEntry.address
  io.FrontQueue_data := frontEntry.data
  io.FrontQueue_subop := frontEntry.Sub_OP

}
