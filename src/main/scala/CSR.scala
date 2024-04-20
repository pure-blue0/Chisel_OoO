import chisel3._
import chisel3.util._

class CSR extends Module {
  val io = IO(new Bundle {
    val CSR_Read_Address = Input(UInt(32.W))
    val CSR_Write_Address = Input(UInt(32.W))
    val CSR_Write_Data = Input(UInt(64.W))
    val CommitInsn_valid = Input(Bool())
    val CSR_MEPC_data = Input(UInt(64.W))
    val CSR_MCAUSE_data = Input(UInt(64.W))
    val CSR_MTVAL_data = Input(UInt(64.W))
    val InstretUp = Input(UInt(2.W))
    val CSR_Data = Output(UInt(64.W))
  })

  // Define all CSR registers
  val misa = RegInit(0x301L.U(64.W))
  val mvendorid = RegInit(0xf11L.U(64.W))
  val marchid = RegInit(0xf12L.U(64.W))
  val mimpid = RegInit(0xf13L.U(64.W))
  val mhartid = RegInit(0xf14L.U(64.W))
  val medeleg = RegInit(0x302L.U(64.W))
  val mideleg = RegInit(0x303L.U(64.W))
  val mie = RegInit(0x304L.U(64.W))
  val mtvec = RegInit(0x305L.U(64.W))
  val mcounteren = RegInit(0x306L.U(64.W))
  val mstatus = RegInit(0x300L.U(64.W))
  val mepc = RegInit(0x341L.U(64.W))
  val mcause = RegInit(0x342L.U(64.W))
  val mtval = RegInit(0x343L.U(64.W))
  val mip = RegInit(0x344L.U(64.W))
  val mscratch = RegInit(0x340L.U(64.W))
  val mtime = RegInit(0x701L.U(64.W))
  val mtimecmp = RegInit(0x741L.U(64.W))
  val mcycle = RegInit(0xb00L.U(64.W))
  val minstret = RegInit(0xb02L.U(64.W))
  val mcountinhibit = RegInit(0x320L.U(64.W))

  // Increment mcycle register every clock cycle
  mcycle := mcycle + 1.U

  // Handle CSR write operations
  when(io.CommitInsn_valid) {
    switch(io.CSR_Write_Address) {
      is(mepc) { mepc := io.CSR_MEPC_data }
      is(mcause) { mcause := io.CSR_MCAUSE_data }
      is(mtval) { mtval := io.CSR_MTVAL_data }
      is(mcycle) { mcycle := io.CSR_Write_Data }
      is(minstret) { minstret := io.CSR_Write_Data + io.InstretUp }
      // Add cases for other CSRs
    }
  }

  // Handle CSR reads
  io.CSR_Data := MuxLookup(io.CSR_Read_Address, 0.U, Array(
    misa -> misa,
    mvendorid -> mvendorid,
    marchid -> marchid,
    mimpid -> mimpid,
    mhartid -> mhartid,
    medeleg -> medeleg,
    mideleg -> mideleg,
    mie -> mie,
    mtvec -> mtvec,
    mcounteren -> mcounteren,
    mstatus -> mstatus,
    mepc -> mepc,
    mcause -> mcause,
    mtval -> mtval,
    mip -> mip,
    mscratch -> mscratch,
    mtime -> mtime,
    mtimecmp -> mtimecmp,
    mcycle -> mcycle,
    minstret -> minstret,
    mcountinhibit -> mcountinhibit
  ))

  // Reset logic
  when(reset.asBool()) {
    misa := 0x301L.U
    mvendorid := 0xf11L.U
    marchid := 0xf12L.U
    mimpid := 0xf13L.U
    mhartid := 0xf14L.U
    medeleg := 0x302L.U
    mideleg := 0x303L.U
    mie := 0x304L.U
    mtvec := 0x305L.U
    mcounteren := 0x306L.U
    mstatus := 0x300L.U
    mepc := 0x341L.U
    mcause := 0x342L.U
    mtval := 0x343L.U
    mip := 0x344L.U
    mscratch := 0x340L.U
    mtime := 0x701L.U
    mtimecmp := 0x741L.U
    mcycle := 0xb00L.U
    minstret := 0xb02L.U
    mcountinhibit := 0x320L.U
  }
}

