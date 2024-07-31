import chisel3._
import chisel3.util._

class Lsq_Allocate extends Module {
  val io = IO(new Bundle {
    val allocCount = Input(UInt(2.W))
    val insn1_Function_type = Input(UInt(3.W))
    val insn2_Function_type = Input(UInt(3.W))
    val insn1_subop = Input(UInt(3.W))
    val insn2_subop = Input(UInt(3.W))
    val load_queue_tail = Input(UInt(4.W))
    val store_queue_tail = Input(UInt(4.W))
    val load_queue_nextTail = Input(UInt(4.W))
    val store_queue_nextTail = Input(UInt(4.W))

    val insn1_LSQTag = Output(UInt(4.W))
    val insn2_LSQTag = Output(UInt(4.W))
    val wEn1_load_queue = Output(Bool())
    val load_insn1_state = Output(UInt(2.W))
    val load_committed1 = Output(Bool())
    val load_killed1 = Output(Bool())
    val load_addressReady1 = Output(Bool())
    val load_address1 = Output(UInt(32.W))

    val wEn2_load_queue = Output(Bool())
    val load_insn2_state = Output(UInt(2.W))
    val load_committed2 = Output(Bool())
    val load_killed2 = Output(Bool())
    val load_addressReady2 = Output(Bool())
    val load_address2 = Output(UInt(32.W))

    val wEn1_store_queue = Output(Bool())
    val store_insn1_state = Output(UInt(2.W))
    val store_committed1 = Output(Bool())
    val store_killed1 = Output(Bool())
    val store_addressReady1 = Output(Bool())
    val store_address1 = Output(UInt(32.W))
    val store_dataReady1 = Output(Bool())
    val store_data1 = Output(UInt(64.W))
    val store_subop1 = Output(UInt(3.W))

    val wEn2_store_queue = Output(Bool())
    val store_insn2_state = Output(UInt(2.W))
    val store_committed2 = Output(Bool())
    val store_killed2 = Output(Bool())
    val store_addressReady2 = Output(Bool())
    val store_address2 = Output(UInt(32.W))
    val store_dataReady2 = Output(Bool())
    val store_data2 = Output(UInt(64.W))
    val store_subop2 = Output(UInt(3.W))
  })

  // Signal Initialization
  io.insn1_LSQTag := 0.U
  io.insn2_LSQTag := 0.U
  io.wEn1_load_queue := false.B
  io.wEn2_load_queue := false.B
  io.wEn1_store_queue := false.B
  io.wEn2_store_queue := false.B
  io.load_insn1_state := 0.U
  io.load_committed1 := false.B
  io.load_killed1 := false.B
  io.load_addressReady1 := false.B
  io.load_address1 := 0.U
  io.load_insn2_state := 0.U
  io.load_committed2 := false.B
  io.load_killed2 := false.B
  io.load_addressReady2 := false.B
  io.load_address2 := 0.U
  io.store_insn1_state := 0.U
  io.store_committed1 := false.B
  io.store_killed1 := false.B
  io.store_addressReady1 := false.B
  io.store_address1 := 0.U
  io.store_dataReady1 := false.B
  io.store_data1 := 0.U
  io.store_subop1 := 0.U
  io.store_insn2_state := 0.U
  io.store_committed2 := false.B
  io.store_killed2 := false.B
  io.store_addressReady2 := false.B
  io.store_address2 := 0.U
  io.store_dataReady2 := false.B
  io.store_data2 := 0.U
  io.store_subop2 := 0.U
  // Functional Logic
  when(io.allocCount > 0.U) {
    when(io.insn1_Function_type === 3.U) { // LDU
      io.wEn1_load_queue := true.B
      io.load_insn1_state := "b00".U // Assuming "01" is load_WaitSend
      io.load_committed1 := false.B
      io.load_killed1 := false.B
      io.load_addressReady1 := false.B
      io.load_address1 := 0.U
      io.insn1_LSQTag := io.load_queue_tail
    }.elsewhen(io.insn1_Function_type === 4.U) { // STU
      io.wEn1_store_queue := true.B
      io.store_insn1_state := "b00".U // Assuming "01" is store_WaitSend
      io.store_committed1 := false.B
      io.store_killed1 := false.B
      io.store_addressReady1 := false.B
      io.store_address1 := 0.U
      io.store_dataReady1 := false.B
      io.store_data1 := 0.U
      io.insn1_LSQTag := io.store_queue_tail
      io.store_subop1 := io.insn1_subop
    }

    when(io.allocCount === 2.U) {
      when(io.insn2_Function_type === 3.U) { // LDU
        io.wEn2_load_queue := true.B
        io.load_insn2_state := "b00".U
        io.load_committed2 := false.B
        io.load_killed2 := false.B
        io.load_addressReady2 := false.B
        io.load_address2 := 0.U
        io.insn2_LSQTag := Mux(io.wEn1_load_queue,io.load_queue_nextTail,io.load_queue_tail)
      }.elsewhen(io.insn2_Function_type === 4.U) { // STU
        io.wEn2_store_queue := true.B
        io.store_insn2_state := "b00".U
        io.store_committed2 := false.B
        io.store_killed2 := false.B
        io.store_addressReady2 := false.B
        io.store_address2 := 0.U
        io.store_dataReady2 := false.B
        io.store_data2 := 0.U
        io.store_subop2 :=io.insn2_subop
        io.insn2_LSQTag := Mux(io.wEn1_store_queue,io.store_queue_nextTail,io.store_queue_tail)
      }
    }
  }

  // Default inactive state for not allocated instructions
  when(io.allocCount < 2.U) {
    // Reset states for insn2
    io.wEn2_load_queue := false.B
    io.load_insn2_state := 0.U
    io.load_committed2 := false.B
    io.load_killed2 := false.B
    io.load_addressReady2 := false.B
    io.load_address2 := 0.U
    io.wEn2_store_queue := false.B
    io.store_insn2_state := 0.U
    io.store_committed2 := false.B
    io.store_killed2 := false.B
    io.store_addressReady2 := false.B
    io.store_address2 := 0.U
    io.store_dataReady2 := false.B
    io.store_data2 := 0.U
    io.insn2_LSQTag := 0.U
    io.store_subop2 :=0.U
  }
}
object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new Lsq_Allocate))
}
