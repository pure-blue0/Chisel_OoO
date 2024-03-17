#include "Lsq.hh"
#include "../Processor.hh"

namespace Emulator
{
    
Lsq::Lsq(
    Processor*          processor,
    const uint16_t      dCacheAlignByte,
    const uint64_t      LDQEntryCount,
    const uint64_t      STQEntryCount
):  TraceObject("LSQ"),
    m_Processor(processor),
    m_dCacheAlignByte(dCacheAlignByte),
    m_LoadQueue("LSQ_LoadQueue",LDQEntryCount),
    m_StoreQueue("LSQ_StoreQueue",STQEntryCount)
{
    DASSERT((dCacheAlignByte >= 8),"dCache AlignByte Must >= 8 ! ");
}

Lsq::~Lsq(){}

void
Lsq::Reset(){
    this->m_LoadQueue.Reset();
    this->m_StoreQueue.Reset();
}


void 
Lsq::WriteBack(InsnPtr_t& insn){
    if(insn->Fu == funcType_t::LDU){
        this->m_LoadQueue[insn->LSQTag].isVirtual = false;
        this->m_LoadQueue[insn->LSQTag].addressReady = true;
        this->m_LoadQueue[insn->LSQTag].address  = insn->Agu_addr;
    }else if(insn->Fu == funcType_t::STU)
    {
        this->m_StoreQueue[insn->LSQTag].isVirtual = false;
        this->m_StoreQueue[insn->LSQTag].addressReady = true;
        this->m_StoreQueue[insn->LSQTag].address  = insn->Agu_addr;
        this->m_StoreQueue[insn->LSQTag].data  = insn->Agu_data;
    }
    
}

void 
Lsq::TryAllocate(InsnPkg_t& insnPkg, uint64_t& SuccessCount){
    SuccessCount = 0;
    uint16_t AllocLdqCount = 0;
    uint16_t AllocStqCount = 0;
    for(auto& insn : insnPkg){
        if(insn){
            
            if(insn->Fu == funcType_t::LDU){
                if(this->m_LoadQueue.getAvailEntryCount() > AllocLdqCount){
                    AllocLdqCount++;
                }else{
                    break;
                }
            }else if(insn->Fu == funcType_t::STU){
                if(this->m_StoreQueue.getAvailEntryCount() > AllocStqCount){
                    AllocStqCount++;
                }else{
                    break;
                }
            }
        }
        SuccessCount++;
    }
}

void 
Lsq::Allocate(InsnPkg_t& insnPkg,uint64_t allocCount){
    for(size_t i = 0 ; i < allocCount; i++){
        auto& insn = insnPkg[i];
        if(insn){
            if(insn->Fu == funcType_t::LDU){
                insn->LSQTag = this->m_LoadQueue.Allocate();
                this->CreateLdqEntry(insn);
            }else if (insn->Fu == funcType_t::STU){
                insn->LSQTag = this->m_StoreQueue.Allocate();
                this->CreateStqEntry(insn);
            }
        }
    }
}

void
Lsq::CreateLdqEntry(InsnPtr_t& insn){
    LDQ_entry_t t; 
    t.state = loadState_t::load_WaitSend;
    t.commited = false;
    t.killed = false;
    t.hasDependancy = !this->m_StoreQueue.empty();
    t.oldestStqTag = this->m_StoreQueue.getLastPtr(this->m_StoreQueue.getTail());
    /* Load Address CAM */
    t.addressReady = false;
    t.isVirtual    = false;;
    t.address      = 0;
    t.op           = insn->SubOp;


    t.insnPtr = insn;
    this->m_LoadQueue[insn->LSQTag] = t;

}

void
Lsq::CreateStqEntry(InsnPtr_t& insn){
    STQ_entry_t t;
    t.state = storeState_t::store_WaitSend;
    t.commited = false;
    t.addressReady = false;
    t.isVirtual = false;
    t.address = 0;
    t.op = insn->SubOp;
    t.dataReady = false;;
    t.data = 0 ;
    t.insnPtr = insn;
    this->m_StoreQueue[insn->LSQTag] = t;   
}


void 
Lsq::TryIssueLoad(MemReq_t& memReq,bool& Success){
    Success = false;
    if(!this->m_LoadQueue.empty()){//需要加载的地址都放在了loadqueue里
        uint16_t ldqPtr = this->m_LoadQueue.getHeader();
        for(size_t i = 0 ; i < this->m_LoadQueue.getUsage(); i++){
            auto& ldqEntry = this->m_LoadQueue[ldqPtr];
            if(ldqEntry.state == loadState_t::load_WaitSend && !ldqEntry.killed){//在dispatch stage就已经改变了load state
                if(ldqEntry.addressReady && !ldqEntry.hasDependancy)
                {
                    Success = true;
                    ldqEntry.state = loadState_t::load_Inflight;
                    memReq.Opcode  = MemOp_t::Load;
                    memReq.Id.HartId = this->m_Processor->getThreadId();
                    memReq.Id.TransId = ldqPtr;
                    memReq.Address = ldqEntry.address & ~(this->m_dCacheAlignByte - 1);
                    memReq.Length  = this->m_Processor->m_XLEN / 8;
                    DPRINTF(LoadReq,"RobPtr[{}],Pc[{:#x}] -> Send Load Request : Address[{:#x}]",
                        ldqEntry.insnPtr->RobTag,
                        ldqEntry.insnPtr->Pc,
                        ldqEntry.address
                    );
                }
                break;
            }
            ldqPtr = this->m_LoadQueue.getNextPtr(ldqPtr);
        }
    }
}

void 
Lsq::TryIssueStore(MemReq_t& memReq,bool& Success){
    Success = false;
    if(!this->m_StoreQueue.empty()){
        uint16_t stqPtr = this->m_StoreQueue.getHeader();
        for(size_t i = 0 ; i < this->m_StoreQueue.getUsage(); i++){
            auto& stqEntry = this->m_StoreQueue[stqPtr];
            if(stqEntry.state == storeState_t::store_WaitSend){
                if(stqEntry.commited){
                    uint64_t offset     = (stqEntry.address & (this->m_dCacheAlignByte - 1));
                    Success             = true;
                    stqEntry.state      = storeState_t::store_Inflight;
                    memReq.Opcode       = MemOp_t::Store;
                    memReq.Id.HartId    = this->m_Processor->getThreadId();
                    memReq.Id.TransId   = stqPtr;
                    for(size_t it = offset ; it < 8; it++){
                        memReq.Data[it] = (stqEntry.data >> ((it-offset)<<3)) & 0xff;
                    }
                    memReq.Address      = stqEntry.address & ~(this->m_dCacheAlignByte - 1);
                    memReq.Length       = this->m_Processor->m_XLEN / 8;
                    switch (stqEntry.op)
                    {
                    case STU_SB:
                        memReq.ByteMask = ((2 << (1-1)) - 1) << offset;
                        break;
                    case STU_SH:
                        memReq.ByteMask = ((2 << (2-1)) - 1) << offset;
                        break;
                    case STU_SW:
                        memReq.ByteMask = ((2 << (4-1)) - 1) << offset;
                        break;
                    case STU_SD:
                        memReq.ByteMask = ((2 << (8-1)) - 1) << offset;
                        break;
                    default:
                        break;
                    }
                    DPRINTF(StoreReq,"RobPtr[{}],Pc[{:#x}] -> Send Store Request : Address[{:#x} Data[{:#x}]]",
                        stqEntry.insnPtr->RobTag,
                        stqEntry.insnPtr->Pc,
                        stqEntry.address,
                        stqEntry.data
                    );
                }
                break;
            }
            stqPtr = this->m_StoreQueue.getNextPtr(stqPtr);
        }
    }
}

void
Lsq::KillLoadEntry(uint16_t LdqTag){
    this->m_LoadQueue[LdqTag].killed = true;
}

void
Lsq::KillStoreEntry(uint16_t StqTag){
    this->m_StoreQueue.RollBack();
}

void 
Lsq::CommitLoadEntry(uint16_t LdqTag){
    this->m_LoadQueue[LdqTag].commited = true;
}

void 
Lsq::CommitStoreEntry(uint16_t StqTag){
    this->m_StoreQueue[StqTag].commited = true;
}

void
Lsq::Evaulate(){
    while(!this->m_StoreQueue.empty() && this->m_StoreQueue.front().state == storeState_t::store_Executed){
        uint16_t ldqPtr = this->m_LoadQueue.getHeader();
        for(size_t i = 0; i < this->m_LoadQueue.getUsage(); i++){
            auto& ldqEntry = this->m_LoadQueue[ldqPtr];
            if(ldqEntry.hasDependancy && ldqEntry.oldestStqTag == this->m_StoreQueue.getHeader()){
                ldqEntry.hasDependancy = false;
                DPRINTF(MemoryDependancy,"RobTag[{}],Pc[{:#x}] -> Clear Dependancy RobTag[{}],Pc[{:#x}]",
                    this->m_StoreQueue.front().insnPtr->RobTag,
                    this->m_StoreQueue.front().insnPtr->Pc,
                    ldqEntry.insnPtr->RobTag,
                    ldqEntry.insnPtr->Pc
                );
            }
            ldqPtr = this->m_LoadQueue.getNextPtr(ldqPtr);
        }
        this->m_StoreQueue.Pop();
    }
    while(!this->m_LoadQueue.empty()){
        auto& LdqEntry = this->m_LoadQueue[this->m_LoadQueue.getHeader()];
        if(LdqEntry.commited){
            this->m_LoadQueue.Pop();
        }else if(LdqEntry.killed && LdqEntry.state != loadState_t::load_Inflight){
            this->m_LoadQueue.Pop();
        }else{
            break;
        }
    }
}

} // namespace Emulator











