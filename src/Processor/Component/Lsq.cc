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
        this->m_LoadQueue[insn->LSQTag].addressReady = true;
        this->m_LoadQueue[insn->LSQTag].address  = insn->Agu_addr;
    }else if(insn->Fu == funcType_t::STU)
    {
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
    for(auto& insn : insnPkg){//根据insn的function type统计输入的两条指令立有多少条是load指令，有多少条是store指令
        if(insn->Fu == funcType_t::LDU)      AllocLdqCount++;
        else if(insn->Fu == funcType_t::STU) AllocStqCount++;    
    }
    
    if(this->m_LoadQueue.getAvailEntryCount()<AllocLdqCount)
        SuccessCount=this->m_LoadQueue.getAvailEntryCount();
    else if(this->m_StoreQueue.getAvailEntryCount()<AllocStqCount)
        SuccessCount=this->m_StoreQueue.getAvailEntryCount();
    else SuccessCount = insnPkg.size();

}

void 
Lsq::Allocate(InsnPkg_t& insnPkg,uint64_t allocCount){
    for(size_t i = 0 ; i < allocCount; i++){
        auto& insn = insnPkg[i];
            if(insn->Fu == funcType_t::LDU){
                LDQ_entry_t t; 
                t.state = loadState_t::load_WaitSend;
                t.commited = false;
                t.killed = false;
                
                t.addressReady = false;
                t.address      = 0;
                t.insnPtr = insn;
                insn->LSQTag = this->m_LoadQueue.Allocate();//获取load queue的尾指针，并将尾指针+1
                this->m_LoadQueue[insn->LSQTag] = t;//将数据存入load queue的尾部
            }else if (insn->Fu == funcType_t::STU){
                STQ_entry_t t;
                t.state = storeState_t::store_WaitSend;
                t.commited = false;
                t.addressReady = false;
                t.address = 0;
                t.killed = false;
                t.dataReady = false;;
                t.data = 0 ;
                t.insnPtr = insn;
                insn->LSQTag = this->m_StoreQueue.Allocate();//获取store queue的尾指针，并将尾指针+1
                this->m_StoreQueue[insn->LSQTag] = t;  //将数据存入store queue的尾部
            }
    }
}



void 
Lsq::TryIssueLoad(MemReq_t& memReq,bool& Success){
    Success = false;
    if(!this->m_LoadQueue.empty()){//需要加载的地址都放在了loadqueue里
        uint16_t ldqPtr = this->m_LoadQueue.getHeader();
        //for(size_t i = 0 ; i < this->m_LoadQueue.getUsage(); i++){
            auto& ldqEntry = this->m_LoadQueue[ldqPtr];
            if(ldqEntry.state == loadState_t::load_WaitSend && !ldqEntry.killed){//在dispatch stage就已经改变了load state
                if(ldqEntry.addressReady )
                {
                    Success = true;
                    ldqEntry.state = loadState_t::load_Inflight;
                    memReq.Opcode  = MemOp_t::Load;
                    memReq.Id.TransId = ldqPtr;
                    memReq.Address = ldqEntry.address & ~(this->m_dCacheAlignByte - 1);
                    memReq.Length  = this->m_Processor->m_XLEN / 2;
                    DPRINTF(LoadReq,"RobPtr[{}],Pc[{:#x}] -> Send Load Request : Address[{:#x}]",
                        ldqEntry.insnPtr->RobTag,ldqEntry.insnPtr->Pc,ldqEntry.address);
                }
                //break;
            }
            ldqPtr = this->m_LoadQueue.getNextPtr(ldqPtr);
        //}
    }
}

void 
Lsq::TryIssueStore(MemReq_t& memReq,bool& Success){
    Success = false;
    if(!this->m_StoreQueue.empty()){
        uint16_t stqPtr = this->m_StoreQueue.getHeader();
        auto& stqEntry = this->m_StoreQueue[stqPtr];
        if(stqEntry.state == storeState_t::store_WaitSend){
            if(stqEntry.commited){
                uint64_t offset     = (stqEntry.address & (this->m_dCacheAlignByte - 1));
                Success             = true;
                stqEntry.state      = storeState_t::store_Inflight;
                memReq.Opcode       = MemOp_t::Store;
                memReq.Id.TransId   = stqPtr;
                for(size_t it = offset ; it < 8; it++){//将byte一个个从stqEntry.data中取出来，存入到对应的memreq_data中
                    memReq.Data[it] = (stqEntry.data >> ((it-offset)<<3)) & 0xff;
                }
                memReq.Address      = stqEntry.address & ~(this->m_dCacheAlignByte - 1);
                memReq.Length       = this->m_Processor->m_XLEN / 2;
                switch (stqEntry.insnPtr->SubOp)
                {
                case STU_SB:memReq.ByteMask = ((2 << (1-1)) - 1) << offset;break;
                case STU_SH:memReq.ByteMask = ((2 << (2-1)) - 1) << offset;break;
                case STU_SW:memReq.ByteMask = ((2 << (4-1)) - 1) << offset;break;
                case STU_SD:memReq.ByteMask = ((2 << (8-1)) - 1) << offset;break;
                default:memReq.ByteMask=0;break;
                }
            }
        }
    }
}

void
Lsq::KillLoadEntry(uint16_t LdqTag){
    this->m_LoadQueue[LdqTag].killed = true;
}

void
Lsq::KillStoreEntry(uint16_t StqTag){
    this->m_StoreQueue[StqTag].killed = true;
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
    if(!this->m_StoreQueue.empty()){
        auto& STqEntry = this->m_StoreQueue[this->m_StoreQueue.getHeader()];
        if(STqEntry.killed && STqEntry.state != storeState_t::store_Inflight){
            this->m_StoreQueue.Pop();
        }
    }
    if(!this->m_LoadQueue.empty()){
        auto& LdqEntry = this->m_LoadQueue[this->m_LoadQueue.getHeader()];
        if(LdqEntry.commited){
            this->m_LoadQueue.Pop();
        }else if(LdqEntry.killed && LdqEntry.state != loadState_t::load_Inflight){
            this->m_LoadQueue.Pop();
        }
    }
}

} // namespace Emulator











