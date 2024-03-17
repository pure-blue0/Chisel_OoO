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
    bool                     isVirtual;
    Addr_t                   address;
    uint8_t                  op;


    InsnPtr_t                insnPtr;
};

enum storeState_t{
    store_WaitSend,store_Inflight,store_Executed
};
struct STQ_entry_t
{
    storeState_t             state;
    bool                     commited;
    /* Store Address CAM */
    bool                     addressReady;
    bool                     isVirtual;
    Addr_t                   address;
    uint8_t                  op;

    bool                     dataReady;
    xReg_t                   data;

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

public:
    
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

    void CreateLdqEntry(InsnPtr_t& insn);

    void CreateStqEntry(InsnPtr_t& insn);

    void TryIssueLoad(MemReq_t& memReq,bool& Success);

    void TryIssueStore(MemReq_t& memReq,bool& Success);

    void KillLoadEntry(uint16_t LdqTag);
 
    void KillStoreEntry(uint16_t StqTag);

    void CommitLoadEntry(uint16_t LdqTag);
 
    void CommitStoreEntry(uint16_t StqTag);

};

    
} // namespace Emulator


#endif