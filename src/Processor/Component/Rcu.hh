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

    bool                        m_RN_EN_Update;
    uint64_t                    m_RN_IsaRd_Update;
    uint64_t                    m_RN_Data_Update;

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

    void CreateRobEntry(InsnPkg_t& insnPkg, uint64_t allocCount);

    bool isOlder(uint64_t tag1, uint64_t tag2,uint64_t header);

    void TryAllocate(InsnPkg_t& insnPkg, uint64_t& SuccessCount);

    void Allocate(InsnPkg_t& insnPkg,uint64_t allocCount);

    void ResovleDependancy(InsnPkg_t& insnPkg);

    bool ReadyForCommit(uint64_t RobTag);

    void WriteBack(InsnPtr_t& insn, bool& needRedirect,Redirect_t& RedirectReq);

    void AGUFastDetect(InsnPtr_t& insn);

    void Forwarding(InsnPtr_t& insn);

    void RollBack();

    void ReleaseResource(uint16_t robTag);

    void Evaulate();

    void CommitInsn(InsnPkg_t& insnPkg, Redirect_t& redirectReq, bool& needRedirect);

};

} // namespace Emulator










#endif