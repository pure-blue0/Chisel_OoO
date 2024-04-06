import chisel3._
import chisel3.util._

// Assuming the enumeration values for function and state types are defined as follows:
object funcType {
  val ALU = 0.U(3.W)
  val BRU = 1.U(3.W)
  val CSR = 2.U(3.W)
  val LDU = 3.U(3.W)
  val STU = 4.U(3.W)
}

object stateType {
  val WaitSend = 0.U(2.W)
  val Inflight = 1.U(2.W)
  val Executed = 2.U(2.W)
}

class Lsq_Allocate extends Module {
  val io = IO(new Bundle {
    val allocCount = Input(UInt(2.W))
    val insn1_Function_type = Input(UInt(3.W))
    val insn2_Function_type = Input(UInt(3.W))
    val load_queue_tail = Input(UInt(4.W))
    val store_queue_tail = Input(UInt(4.W))

    val insn1_LSQTag = Output(UInt(4.W))
    val insn2_LSQTag = Output(UInt(4.W))
    val wEn1_load_queue = Output(Bool())
    val load_insn1_state = Output(UInt(2.W))
    val load_Commited1 = Output(Bool())
    val load_Killed1 = Output(Bool())
    val load_addressReady1 = Output(Bool())
    val load_address1 = Output(UInt(32.W))
    
    val wEn2_load_queue = Output(Bool())
    val load_insn2_state = Output(UInt(2.W))
    val load_Commited2 = Output(Bool())
    val load_Killed2 = Output(Bool())
    val load_addressReady2 = Output(Bool())
    val load_address2 = Output(UInt(32.W))
    
    val wEn1_store_queue = Output(Bool())
    val store_insn1_state = Output(UInt(2.W))
    val store_Commited1 = Output(Bool())
    val store_Killed1 = Output(Bool())
    val store_addressReady1 = Output(Bool())
    val store_address1 = Output(UInt(32.W))
    val store_dataReady1 = Output(Bool())
    val store_data1 = Output(UInt(64.W))
    
    val wEn2_store_queue = Output(Bool())
    val store_insn2_state = Output(UInt(2.W))
    val store_Commited2 = Output(Bool())
    val store_Killed2 = Output(Bool())
    val store_addressReady2 = Output(Bool())
    val store_address2 = Output(UInt(32.W))
    val store_dataReady2 = Output(Bool())
    val store_data2 = Output(UInt(64.W))
  })

  // Initialize all outputs to default values
  io.insn1_LSQTag := 0.U
  io.insn2_LSQTag := 0.U
  io.wEn1_load_queue := false.B
  io.load_insn1_state := stateType.WaitSend
  io.load_Commited1 := false.B
  io.load_Killed1 := false.B
  io.load_addressReady1 := false.B
  io.load_address1 := 0.U
  io.wEn2_load_queue := false.B
  io.load_insn2_state := stateType.WaitSend
  io.load_Commited2 := false.B
  io.load_Killed2 := false.B
  io.load_addressReady2 := false.B
  io.load_address2 := 0.U
  io.wEn1_store_queue := false.B
  io.store_insn1_state := stateType.WaitSend
  io.store_Commited1 := false.B
  io.store_Killed1 := false.B
  io.store_addressReady1 := false.B
  io.store_address1 := 0.U
  io.store_dataReady1 := false.B
  io.store_data1 := 0.U
  io.wEn2_store_queue := false.B
  io.store_insn2_state := stateType.WaitSend
  io.store_Commited2 := false.B
  io.store_Killed2 := false.B
  io.store_addressReady2 := false.B
  io.store_address2 := 0.U
  io.store_dataReady2 := false.B
  io.store_data2 := 0.U

  // Real-world dynamic tail pointer management for load and store queues
  val nextLoadQueueTail = Wire(UInt(log2Ceil(queueSize).W))
  val nextStoreQueueTail = Wire(UInt(log2Ceil(queueSize).W))
  
  nextLoadQueueTail := (io.load_queue_tail + 1.U) % queueSize.U
  nextStoreQueueTail := (io.store_queue_tail + 1.U) % queueSize.U
  
  when(io.allocCount > 0.U) {
    when(io.insn1_Function_type === funcType.LDU) {
      io.wEn1_load_queue := true.B
      io.load_insn1_state := stateType.WaitSend
      io.insn1_LSQTag := nextLoadQueueTail
      // Update next tail pointer considering the modulo operation
      nextLoadQueueTail := (nextLoadQueueTail + 1.U) % queueSize.U
    }.elsewhen(io.insn1_Function_type === funcType.STU) {
      io.wEn1_store_queue := true.B
      io.store_insn1_state := stateType.WaitSend
      io.insn1_LSQTag := nextStoreQueueTail
      nextStoreQueueTail := (nextStoreQueueTail + 1.U) % queueSize.U
    }

  when(io.allocCount === 2.U) {
    when(io.insn2_Function_type === funcType.LDU) {
      io.wEn2_load_queue := true.B
      io.load_insn2_state := stateType.WaitSend
      io.insn2_LSQTag := nextLoadQueueTail // Use the dynamically updated tail
      // Set remaining signals for LDU of insn2 similar to insn1
      nextLoadQueueTail := (nextLoadQueueTail + 1.U) % queueSize.U // Update for next potential use
    }.elsewhen(io.insn2_Function_type === funcType.STU) {
      io.wEn2_store_queue := true.B
      io.store_insn2_state := stateType.WaitSend
      io.insn2_LSQTag := nextStoreQueueTail // Use the dynamically updated tail
      // Set remaining signals for STU of insn2 similar to insn1
      nextStoreQueueTail := (nextStoreQueueTail + 1.U) % queueSize.U // Update for next potential use
    }
  }

}

}
