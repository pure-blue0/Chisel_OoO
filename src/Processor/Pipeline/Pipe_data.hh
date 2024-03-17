#ifndef __PIPE_DATA_HH__
#define __PIPE_DATA_HH__ 

#include "../../Common/Common.hh"
#include "../../RISCV/riscv.hh"
#include "../DynInsn.hh"

namespace Emulator
{

struct TransId_t{
    ThreadId HartId;
    uint16_t TransId;    
};

enum  MemOp_t{
    Fetch, Load, Store, Amo, FlushAll, Flush
};

struct MemReq_t
{
    TransId_t Id;
    Addr_t    Address;
    MemOp_t   Opcode;
    uint16_t  Length;
    char*     Data;
    uint64_t  ByteMask;
};


struct MemResp_t
{
    TransId_t   Id;
    Addr_t      Address;
    MemOp_t     Opcode;
    uint16_t    Length;
    char*       Data;
    Exception_t Excp;
};

struct Redirect_t
{
    InsnState_t  StageId;
    Addr_t       target;
};

struct StageAck_t
{
    uint64_t     takenInsnCount;
};

typedef std::shared_ptr<DynInsn> InsnPtr_t;

typedef std::vector<InsnPtr_t> InsnPkg_t;


} // namespace Emulator





#endif	