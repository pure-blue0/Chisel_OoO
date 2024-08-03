#ifndef __RCU_HH__
#define __RCU_HH__

#include "../DynInsn.hh"
#include "../Pipeline/Pipe_data.hh"
#include "BusyList.hh"
#include "FreeList.hh"
#include "RegisterFile.hh"
#include "RenameTable.hh"
#include "LoopQueue.hh"
//#include "./obj_dir/VRcuAllocate.h"
namespace Emulator
{
    
struct Rob_entry_t
{
    bool                     valid;
    bool                     done;

    bool                     isStable;
    bool                     isMisPred;
    bool                     isExcp;
    
    //bool                     isRvc;
    Addr_t                   pc;

    funcType_t               Fu;
    uint16_t                 LSQtag;

    IsaRegId_t               isaRd;
    PhyRegId_t               phyRd;
    PhyRegId_t               LphyRd;

    InsnPtr_t                insnPtr;
};


enum rob_state_t{
    Rob_Idle, Rob_Undo, Rob_Redo, Rob_WaitForResume, Rob_FlushBackend
};

struct Snapshot_t
{
    RenameTable<32,true>        IntRenameTable;
    
    BusyList<true>              IntBusylist;

    FreeList<true>              IntFreelist;

    Snapshot_t()
    :   IntRenameTable(""),
        IntBusylist("",0),
        IntFreelist("",0)
    {}
};


class Processor;
class Rcu : Trace::TraceObject
{

public:

    Processor*                  m_Processor;

    const bool                  m_RenameRegister;

    const uint64_t              m_AllocWidth;

    const uint64_t              m_DeallocWidth;

    const bool                  m_SupportRollback;

    LoopQueue<Rob_entry_t>      m_Rob;

    rob_state_t                 m_RobState;

    uint64_t                    m_RollBackTag;

    xlen_t                      m_ExcpCause;

    xlen_t                      m_ExcpTval;

    /* Interger */

    RegisterFile<uint64_t,true> m_IntRegfile;

    RenameTable<32,true>        m_IntRenameTable;
    
    BusyList<true>              m_IntBusylist;

    FreeList<true>              m_IntFreelist;
    //VRcuAllocate                *RcuAllocate;

    /* SnapShot */
    LoopQueue<Snapshot_t>       m_Snapshot;
    uint8_t lsq_count;
    uint8_t pop_num;
    bool                        RN_Release_EN[4];
    uint64_t                    RN_Release_IsaRd[4];
    uint64_t                    FE_Release_phyRd[4];
    uint64_t                    RN_Release_Data[4];

    bool                        FE_Commit_EN[4];
    uint64_t                    FE_Commit_LphyRd[4];

    uint64_t    ROB_Entry_count;
    InsnPkg_t  rob_insnPkg;
    bool write_in;
    bool ROB_Entry_WEN_GROUP[4];

    bool ROB_WB_EN_Group[4];
    bool ROB_WB_Data_isMisPred_Group[4];
    bool ROB_WB_Data_isExcp_Group[4];
    uint8_t ROB_WB_ROBTag_Group[4];
    
    bool ROB_AGU_EN_Group[2];
    bool ROB_AGU_Data_done_Group[2];
    bool ROB_AGU_Data_isExcp_Group[2];
    uint8_t ROB_AGU_ROBTag_Group[2];
    uint64_t ROB_Count;

    bool BusyList_Update_EN[4];
    PhyRegId_t BusyList_Update_PhyRd[4];
public:
    Rcu(
        Processor*          processor,
        const bool          RenameRegister,
        const uint64_t      RobEntryCount,
        const uint64_t      IntRegCount,
        const bool          SupportRollBack,
        const uint64_t      SnapShotNum,
        const uint64_t      AllocWidth,
        const uint64_t      DeallocWidth
    );
    ~Rcu();

    void Reset();

    void Rename(InsnPkg_t& insnPkg);

    void CreateRobEntry(InsnPkg_t& insnPkg, bool ROB_Entry_WEN_GROUP[4]);

    bool isOlder(uint64_t tag1, uint64_t tag2,uint64_t header);

    void TryAllocate(InsnPkg_t& insnPkg, uint64_t& SuccessCount);

    void Allocate(InsnPkg_t& insnPkg,uint64_t allocCount);

    void ResovleDependancy(InsnPkg_t& insnPkg);

    bool ReadyForCommit(uint64_t RobTag);

    void WriteBack(int index,InsnPtr_t& insn, bool& needRedirect,Redirect_t& RedirectReq);

    void AGUFastDetect(uint8_t index,InsnPtr_t& insn);

    void Forwarding(InsnPtr_t& insn);

    void RollBack();

    void ReleaseResource(uint16_t robTag);

    void Evaulate();

    void CommitInsn(InsnPkg_t& insnPkg, Redirect_t& redirectReq, bool& needRedirect);

    void Freelist_Evaluate();
};

} // namespace Emulator










#endif