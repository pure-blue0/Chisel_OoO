#ifndef __LSQ_HH__
#define __LSQ_HH__


#include "../../Trace/TraceObject.hh"
#include "../../Common/Common.hh"
#include "../Pipeline/Pipe_data.hh"
#include "Rcu.hh"
#include "LoopQueue.hh"

#define MAX_Try_issue_num 3 //max 4
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

    uint64_t     RobTag;
    funcType_t   Fu;
    IsaRegId_t   IsaRd;
    PhyRegId_t   PhyRd;
    uint8_t      SubOp;
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
};

struct LSQ_entry_t
{
    uint64_t     RobTag;
    funcType_t   Fu;
    IsaRegId_t   IsaRd;
    PhyRegId_t   PhyRd;
    uint8_t      SubOp;
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
    bool KillLoadEntry_flag[4];
    uint16_t KillLoadEntry_Tag[4];
    bool KillStoreEntry_flag[4];
    uint16_t KillStoreEntry_Tag[4];

    bool store_state_update_EN;
    bool load_state_update_EN;
    uint8_t load_state_update_ptr;

    bool MEM_load_state_update_EN;
    bool MEM_store_state_update_EN;
    uint8_t MEM_lsq_state_update_ptr;
    
    bool storeQueue_pop;

    bool Load_WEN_Group[4];
    bool Store_WEN_Group[4];

    LSQ_entry_t lsq_data[4];

    uint8_t LSU_Style;
    Addr_t LSU_Agu_addr;
    xReg_t LSU_Agu_data;
    uint64_t LSU_LSQTag;

    uint8_t     LSQ_Style_Group[4];
    InsnPtr_t   send_lsq_insnPtr[4]; 

    uint8_t     Commit_Style_Group[4];
    uint8_t     Commit_LSQTag[4];
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