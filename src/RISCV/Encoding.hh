#ifndef __ENCODING_HH__
#define __ENCODING_HH__ 


namespace RISCV
{

// ----------------------
// Exception Cause Codes
// ----------------------
enum ExcpCause_t{
    INSTR_ADDR_MISALIGNED = 0,
    INSTR_ACCESS_FAULT    = 1,  // Illegal access as governed by PMPs and PMAs
    ILLEGAL_INSTR         = 2,
    BREAKPOINT            = 3,
    LD_ADDR_MISALIGNED    = 4,
    LD_ACCESS_FAULT       = 5,  // Illegal access as governed by PMPs and PMAs
    ST_ADDR_MISALIGNED    = 6,
    ST_ACCESS_FAULT       = 7,  // Illegal access as governed by PMPs and PMAs
    ENV_CALL_UMODE        = 8,  // environment call from user mode
    ENV_CALL_SMODE        = 9,  // environment call from supervisor mode
    ENV_CALL_MMODE        = 11, // environment call from machine mode
    INSTR_PAGE_FAULT      = 12, // Instruction page fault
    LOAD_PAGE_FAULT       = 13, // Load page fault
    STORE_PAGE_FAULT      = 15, // Store page fault
    DEBUG_REQUEST         = 24  // Debug request
};

enum Priv_level_t{
    PRIV_M = 3,
    PRIV_S = 1,
    PRIV_U = 0  
};


// RV32/64G listings:
// Quadrant 0
#define OpcodeLoad      0b0000011
#define OpcodeLoadFp    0b0000111
#define OpcodeCustom0   0b0001011
#define OpcodeMiscMem   0b0001111
#define OpcodeOpImm     0b0010011
#define OpcodeAuipc     0b0010111
#define OpcodeOpImm32   0b0011011
// Quadrant 1
#define OpcodeStore     0b0100011
#define OpcodeStoreFp   0b0100111
#define OpcodeCustom1   0b0101011
#define OpcodeAmo       0b0101111
#define OpcodeOp        0b0110011
#define OpcodeLui       0b0110111
#define OpcodeOp32      0b0111011
// Quadrant 2
#define OpcodeMadd      0b1000011
#define OpcodeMsub      0b1000111
#define OpcodeNmsub     0b1001011
#define OpcodeNmadd     0b1001111
#define OpcodeOpFp      0b1010011
#define OpcodeRsrvd1    0b1010111
#define OpcodeCustom2   0b1011011
// Quadrant 3
#define OpcodeBranch    0b1100011
#define OpcodeJalr      0b1100111
#define OpcodeRsrvd2    0b1101011
#define OpcodeJal       0b1101111
#define OpcodeSystem    0b1110011
#define OpcodeRsrvd3    0b1110111
#define OpcodeCustom3   0b1111011

} // namespace RISCV

#endif	