import chisel3._
import chisel3.util._

class ReadyForCommit extends Module {
  val io = IO(new Bundle {
    val Rob_Tag = Input(UInt(4.W))
    val Rob_Header = Input(UInt(4.W))
    val Rob_NextHeader = Input(UInt(4.W))
    val m_RobState = Input(UInt(3.W))
    val m_RollBackTag = Input(UInt(4.W))
    val Rob_Usage = Input(UInt(4.W))
    val Header_isStable = Input(Bool())
    val Header_Function_type = Input(UInt(3.W))
    val Ready = Output(Bool())
  })

  // Define states and function types
  val Rob_Idle = 0.U(3.W)
  val Rob_WaitForResume = 3.U(3.W)
  val LDU = 3.U(3.W)
  val STU = 4.U(3.W)
  val BRU = 1.U(3.W)

  // Define the isOlder submodule
  val isOlderModule = Module(new IsOlder)

  // Connect inputs of isOlder submodule
  isOlderModule.io.tag1 := io.Rob_Tag
  isOlderModule.io.tag2 := io.m_RollBackTag
  isOlderModule.io.header := io.Rob_Header

  // Default all output to 0
  val ready = Wire(Bool())
  ready := false.B

  when(io.m_RobState === Rob_Idle || isOlderModule.io.older) {
    when(io.Rob_Usage === 0.U) {
      ready := false.B
    }.elsewhen(io.Rob_Usage === 1.U) {
      when(io.Rob_Header === io.Rob_Tag) {
        ready := true.B
      }.otherwise {
        ready := false.B
      }
    }.elsewhen(io.Rob_Usage >= 2.U) {
      when(io.Rob_Header === io.Rob_Tag) {
        ready := true.B
      }.otherwise {
        when(!io.Header_isStable && (io.Header_Function_type === LDU || io.Header_Function_type === STU || io.Header_Function_type === BRU)) {
          ready := false.B
        }.otherwise {
          when(io.Rob_NextHeader === io.Rob_Tag) {
            ready := true.B
          }.otherwise {
            ready := false.B
          }
        }
      }
    }
  }.otherwise{
    ready := true.B
  }
  io.Ready := ready
}



class IsOlder extends Module {
  val io = IO(new Bundle {
    val tag1 = Input(UInt(4.W))
    val tag2 = Input(UInt(4.W))
    val header = Input(UInt(4.W))
    val older = Output(Bool())
  })

  // Define comparison logic
  val tag1GeHeader = io.tag1 >= io.header
  val tag2GeHeader = io.tag2 >= io.header

  // XOR to determine which comparison to use
  val comparisonCase = tag1GeHeader ^ tag2GeHeader

  // Compute 'older' based on the comparison case
  when (comparisonCase) {
    io.older := io.tag1 > io.tag2
  } .otherwise {
    io.older := io.tag1 < io.tag2
  }
}

object main extends App {
println((new chisel3.stage.ChiselStage).emitVerilog(new ReadyForCommit))
}