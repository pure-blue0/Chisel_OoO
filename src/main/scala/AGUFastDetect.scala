import chisel3._
import chisel3.util._

class AGUFastDetect extends Module {
  val io = IO(new Bundle {
    // Inputs
    val RobTag_in = Input(UInt(4.W))
    val Function_type = Input(UInt(3.W))
    val Agu_addr_ready = Input(Bool())
    val Agu_data_ready = Input(Bool())
    val RobState = Input(UInt(3.W))
    val excp_vaild_in = Input(Bool())
    val excp_cause_in = Input(UInt(6.W))
    val excpt_tval_in = Input(UInt(32.W))
    
    // Outputs
    val isStable = Output(Bool())
    val Done = Output(Bool())
    val isExcp = Output(Bool())
    val RobTag = Output(UInt(4.W))
    val RollBackTag = Output(UInt(4.W))
    val excp_vaild_out = Output(Bool())
    val excp_cause_out = Output(UInt(6.W))
    val excpt_tval_out = Output(UInt(32.W))
  })

  // Logic
  io.isStable := true.B // As per the description, isStable is always set to true when a new instruction is detected
  io.RobTag := io.RobTag_in // Directly output the input RobTag
  
  // Determine if the Done signal should be set
  io.Done := io.Function_type === 4.U && io.Agu_addr_ready && io.Agu_data_ready // Assuming STU is represented by '4'
  
  // Exception handling
  when(io.excp_vaild_in) {
    io.isExcp := true.B
    io.Done := true.B
    when(io.RobState =/= 0.U) { // Assuming Rob_Idle is represented by '0'
      io.RobState := 1.U // Assuming Rob_Undo is represented by '1'
      io.RollBackTag := io.RobTag_in
      io.excp_cause_out := io.excp_cause_in
      io.excpt_tval_out := io.excpt_tval_in
    }
  } .otherwise {
    io.isExcp := false.B
    io.excp_vaild_out := false.B
    io.excp_cause_out := 0.U
    io.excpt_tval_out := 0.U
  }
}

