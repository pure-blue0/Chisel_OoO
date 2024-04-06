import chisel3._
import chisel3.util._

class ReadyForCommit extends Module {
  val io = IO(new Bundle {
    val robTag = Input(UInt(4.W))
    val robHeader = Input(UInt(4.W))
    val robNextHeader = Input(UInt(4.W))
    val mRobState = Input(UInt(3.W))
    val mRollBackTag = Input(UInt(4.W))
    val robUsage = Input(UInt(4.W))
    val headerIsStable = Input(Bool())
    val headerFunctionType = Input(UInt(3.W))
    val nextHeaderIsStable = Input(Bool())
    val nextHeaderFunctionType = Input(UInt(3.W))
    val ready = Output(Bool())
  })

  // Rob states and function types as Enums
  val robIdle :: robUndo :: robRedo :: robWaitForResume :: robFlushBackend :: Nil = Enum(5)
  val alu :: bru :: csr :: ldu :: stu :: Nil = Enum(5)

  // Initial Ready signal as false
  io.ready := false.B

  // Check conditions based on the provided functionality
  when(io.mRobState === robIdle || (io.robTag < io.mRollBackTag && io.robUsage > 0.U)) {
    when(io.robTag === io.robHeader || io.robTag === io.robNextHeader) {
      io.ready := true.B
    } .elsewhen(!(io.headerIsStable && (io.headerFunctionType === ldu || io.headerFunctionType === stu || io.headerFunctionType === bru)) || !(io.nextHeaderIsStable && (io.nextHeaderFunctionType === ldu || io.nextHeaderFunctionType === stu || io.nextHeaderFunctionType === bru))) {
      io.ready := false.B
    }
  } .elsewhen(io.mRobState === robWaitForResume && io.robTag === io.mRollBackTag) {
    io.ready := true.B
  }

}

// Generate the Verilog code
object ReadyForCommit extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new ReadyForCommit())
}
