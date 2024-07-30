#ifndef __LSQ_HH__
#define __LSQ_HH__


#include "../../Trace/TraceObject.hh"
#include "../../Common/Common.hh"
#include "../Pipeline/Pipe_data.hh"
#include "Rcu.hh"
#include "LoopQueue.hh"


namespace Emulator
{

enum loadState_t{
    load_WaitSend,load_Inflight,load_Executed
};

struct LDQ_entry_t
{
    loadState_t              state;
    bool                     killed;
    bool                     commited;
    bool                     hasDependancy;
    uint16_t                 oldestStqTag;
    /* Load Address CAM */
    bool                     addressReady;
    Addr_t                   address;
    
    InsnPtr_t                insnPtr;

    uint64_t     RobTag;
    funcType_t   Fu;
    IsaRegId_t   IsaRd;
    PhyRegId_t   PhyRd;
};

enum storeState_t{
    store_WaitSend,store_Inflight,store_Executed
};
struct STQ_entry_t
{
    storeState_t             state;
    bool                     killed;
    bool                     commited;
    /* Store Address CAM */
    bool                     addressReady;
    Addr_t                   address;
    bool                     dataReady;
    xReg_t                   data;
    uint8_t                  SubOp;
    InsnPtr_t                insnPtr;           
};



enum fence_state_t{
    Fence_None, Fence_Active
};

class Processor;
class Lsq : public Trace::TraceObject
{
private:
    Processor*                  m_Processor;

public:

    const uint16_t              m_dCacheAlignByte;

    LoopQueue<STQ_entry_t>      m_StoreQueue;

    LoopQueue<LDQ_entry_t>      m_LoadQueue;
    uint64_t m_LoadQueue_count;
    uint64_t m_StoreQueue_count;

public:
    bool KillLoadEntry_flag;
    uint16_t KillLoadEntry_Tag;
    bool KillStoreEntry_flag;
    uint16_t KillStoreEntry_Tag;

    bool Load_WEN_Group[4];
    bool Store_WEN_Group[4];
    LDQ_entry_t load_entry[4];
    STQ_entry_t store_entry[4];

    uint8_t     LSQ_Style_Group[4];
    InsnPtr_t   send_lsq_insnPtr[4]; 
    Lsq(
        Processor*          processor,
        const uint16_t      dCacheAlignByte,
        const uint64_t      LDQEntryCount,
        const uint64_t      STQEntryCount
    );
    
    ~Lsq();

    void Reset();

    void Evaulate();

    void WriteBack(InsnPtr_t& Insn);
    
    void TryAllocate(InsnPkg_t& insnPkg, uint64_t& SuccessCount);

    void Allocate(InsnPkg_t& insnPkg,uint64_t allocCount);

    void TryIssueLoad(MemReq_t& memReq,bool& Success);

    void TryIssueStore(MemReq_t& memReq,bool& Success);

    void KillLoadEntry(uint16_t LdqTag);
 
    void KillStoreEntry(uint16_t StqTag);

    void CommitLoadEntry(uint16_t LdqTag);
 
    void CommitStoreEntry(uint16_t StqTag);

};

    
} // namespace Emulator


#endif