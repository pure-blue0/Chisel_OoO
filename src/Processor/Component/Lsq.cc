#include "Lsq.hh"
#include "../Processor.hh"
//#include "./obj_dir/VLsq_Allocate.h"
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
    this->m_LoadQueue_count=LDQEntryCount;
    this->m_StoreQueue_count=STQEntryCount;
}

Lsq::~Lsq(){}

void
Lsq::Reset(){
    this->m_LoadQueue.Reset();
    this->m_StoreQueue.Reset();
    this->KillLoadEntry_flag=false;
    this->KillStoreEntry_flag=false;
    for(int i;i<4;i++){
        this->Load_WEN_Group[i]=false;
        this->Store_WEN_Group[i]=false;
        this->LSQ_Style_Group[i]=0;
    }
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
    uint8_t avail_count=insnPkg[0]->data_valid+insnPkg[1]->data_valid+insnPkg[2]->data_valid+insnPkg[3]->data_valid;
    for(auto& insn : insnPkg){//根据insn的function type统计输入的两条指令立有多少条是load指令，有多少条是store指令
        if(insn->Fu == funcType_t::LDU)      AllocLdqCount++;
        else if(insn->Fu == funcType_t::STU) AllocStqCount++;    
    }

    if(this->m_LoadQueue.getAvailEntryCount()<AllocLdqCount)
        SuccessCount=this->m_LoadQueue.getAvailEntryCount();
    else if(this->m_StoreQueue.getAvailEntryCount()<AllocStqCount)
        SuccessCount=this->m_StoreQueue.getAvailEntryCount();
    else SuccessCount = avail_count;
}
// void Lsq::Allocate(InsnPkg_t& insnPkg,uint64_t allocCount){
//     VLsq_Allocate *Lsq_Allocate=new VLsq_Allocate;//创建对象
//     //连接输入
//     Lsq_Allocate->io_allocCount=allocCount;
//     Lsq_Allocate->io_insn1_Function_type=insnPkg[0]->Fu;
//     Lsq_Allocate->io_insn2_Function_type=insnPkg[1]->Fu;
//     Lsq_Allocate->io_insn1_subop=insnPkg[0]->SubOp;
//     Lsq_Allocate->io_insn2_subop=insnPkg[1]->SubOp;
//     Lsq_Allocate->io_load_queue_tail=this->m_LoadQueue.getTail();
//     Lsq_Allocate->io_store_queue_tail=this->m_StoreQueue.getTail();
//     Lsq_Allocate->io_load_queue_nextTail=this->m_LoadQueue.getNextTail();
//     Lsq_Allocate->io_store_queue_nextTail=this->m_StoreQueue.getNextTail();
//     //连接输出
//     Lsq_Allocate->eval();
//  
//     LDQ_entry_t ldq_entry1; 
//     ldq_entry1.state=loadState_t(Lsq_Allocate->io_load_insn1_state);
//     ldq_entry1.commited=Lsq_Allocate->io_load_committed1;
//     ldq_entry1.killed=Lsq_Allocate->io_load_killed1;
//     ldq_entry1.addressReady=Lsq_Allocate->io_load_addressReady1;
//     ldq_entry1.address=Lsq_Allocate->io_load_address1;
//     ldq_entry1.insnPtr = insnPkg[0];
//   
//     LDQ_entry_t ldq_entry2; 
//     ldq_entry2.state=loadState_t(Lsq_Allocate->io_load_insn2_state);
//     ldq_entry2.commited=Lsq_Allocate->io_load_committed2;
//     ldq_entry2.killed=Lsq_Allocate->io_load_killed2;
//     ldq_entry2.addressReady=Lsq_Allocate->io_load_addressReady2;
//     ldq_entry2.address=Lsq_Allocate->io_load_address2;
//     ldq_entry2.insnPtr = insnPkg[1];
//
//     STQ_entry_t stq_entry1;
//     stq_entry1.state=storeState_t(Lsq_Allocate->io_store_insn1_state);
//     stq_entry1.commited=Lsq_Allocate->io_store_committed1;
//     stq_entry1.killed=Lsq_Allocate->io_store_killed1;
//     stq_entry1.addressReady=Lsq_Allocate->io_store_addressReady1;
//     stq_entry1.address=Lsq_Allocate->io_store_address1;
//     stq_entry1.dataReady=Lsq_Allocate->io_store_dataReady1;
//     stq_entry1.data=Lsq_Allocate->io_store_data1;
//     stq_entry1.SubOp=Lsq_Allocate->io_store_subop1;
//   
//     STQ_entry_t stq_entry2;
//     stq_entry2.state=storeState_t(Lsq_Allocate->io_store_insn2_state);
//     stq_entry2.commited=Lsq_Allocate->io_store_committed2;
//     stq_entry2.killed=Lsq_Allocate->io_store_killed2;
//     stq_entry2.addressReady=Lsq_Allocate->io_store_addressReady2;
//     stq_entry2.address=Lsq_Allocate->io_store_address2;
//     stq_entry2.dataReady=Lsq_Allocate->io_store_dataReady2;
//     stq_entry2.data=Lsq_Allocate->io_store_data2;
//     stq_entry2.SubOp=Lsq_Allocate->io_store_subop2;
//     //以下代码模拟load/store queue 收到en信号后的操作
//     if(Lsq_Allocate->io_wEn1_load_queue){
//         insnPkg[0]->LSQTag=Lsq_Allocate->io_insn1_LSQTag;
//         this->m_LoadQueue[Lsq_Allocate->io_insn1_LSQTag]=ldq_entry1;
//         this->m_LoadQueue.TailInc();
//     }
//     if(Lsq_Allocate->io_wEn2_load_queue){
//         insnPkg[1]->LSQTag=Lsq_Allocate->io_insn2_LSQTag;
//         this->m_LoadQueue[Lsq_Allocate->io_insn2_LSQTag]=ldq_entry2;
//         this->m_LoadQueue.TailInc();
//     }
//     if(Lsq_Allocate->io_wEn1_store_queue){
//         insnPkg[0]->LSQTag=Lsq_Allocate->io_insn1_LSQTag;
//         this->m_StoreQueue[Lsq_Allocate->io_insn1_LSQTag]=stq_entry1;
//         this->m_StoreQueue.TailInc();
//     }
//     if(Lsq_Allocate->io_wEn2_store_queue){
//         insnPkg[1]->LSQTag=Lsq_Allocate->io_insn2_LSQTag;
//         this->m_StoreQueue[Lsq_Allocate->io_insn2_LSQTag]=stq_entry2;
//         this->m_StoreQueue.TailInc();
//     }
//     delete Lsq_Allocate;//删除创建的对象
//     //debug code
//     /*
//     bool a=false;
//     bool b=false;
//     bool c=false;
//     bool d=false;
//     for(size_t i = 0 ; i < allocCount; i++){
//         auto& insn = insnPkg[i];        
//         if(insn->Fu == funcType_t::LDU){
//             LDQ_entry_t t; 
//             t.state = loadState_t::load_WaitSend;
//             t.commited = false;
//             t.killed = false;
//             t.addressReady = false;
//             t.address      = 0;
//             t.insnPtr = insn;
//             // insn->LSQTag = this->m_LoadQueue.Allocate();//获取load queue的尾指针，并将尾指针+1
//             //this->m_LoadQueue[insn->LSQTag] = t;//将数据存入load queue的尾部
//             //DPRINTF(temptest,"write in load");
//             if(i){
//                 b=1;
//                 if(ldq_entry2.state!=t.state||ldq_entry2.killed!=t.killed||ldq_entry2.addressReady!=t.address||
//                    ldq_entry2.insnPtr!=t.insnPtr||ldq_entry2.state!=t.state)
//                 {
//                     DPRINTF(temptest,"error load2");
//                     DPRINTF(temptest,"V: {:} {:} {:} {:}",
//                     ldq_entry2.state,ldq_entry2.killed,ldq_entry2.addressReady,ldq_entry2.address);
//                     DPRINTF(temptest,"O: {:} {:} {:} {:} ",
//                     t.state,t.killed,t.addressReady,t.address);
//                     exit(1);
//                 }
//             }
//             else{
//                 if(ldq_entry1.state!=t.state||ldq_entry1.killed!=t.killed||ldq_entry1.addressReady!=t.address||
//                    ldq_entry1.insnPtr!=t.insnPtr||ldq_entry1.state!=t.state)
//                 {
//                     DPRINTF(temptest,"error load1");
//                     DPRINTF(temptest,"V: {:} {:} {:} {:} ",
//                     ldq_entry1.state,ldq_entry1.killed,ldq_entry1.addressReady,ldq_entry1.address);
//                     DPRINTF(temptest,"O: {:} {:} {:} {:} ",
//                     t.state,t.killed,t.addressReady,t.address);
//                     exit(1);
//                 }
//                 a=1;                
//             }
//         }else if (insn->Fu == funcType_t::STU){
//             STQ_entry_t t;
//             t.state = storeState_t::store_WaitSend;
//             t.commited = false;
//             t.addressReady = false;
//             t.address = 0;
//             t.killed = false;
//             t.dataReady = false;;
//             t.data = 0 ;
//             t.SubOp = insn->SubOp;
//             //insn->LSQTag = this->m_StoreQueue.Allocate();//获取store queue的尾指针，并将尾指针+1
//             //this->m_StoreQueue[insn->LSQTag] = t;  //将数据存入store queue的尾部
//             //DPRINTF(temptest,"write in store");
//             if(i){
//                 d=1;
//             }
//             else{
//                 c=1;
//             }
//         }
//     }
//     if(allocCount==1){
//         if(a!=Lsq_Allocate->io_wEn1_load_queue||b!=Lsq_Allocate->io_wEn2_load_queue||c!=Lsq_Allocate->io_wEn1_store_queue
//         ||d!=Lsq_Allocate->io_wEn2_store_queue||insnPkg[0]->LSQTag!=Lsq_Allocate->io_insn1_LSQTag){
//             DPRINTF(temptest,"Input: {:} O: {:} {:} V: {:} {:}",
//             allocCount,insnPkg[0]->Fu,insnPkg[1]->Fu,Lsq_Allocate->io_insn1_Function_type,Lsq_Allocate->io_insn2_Function_type);
//             DPRINTF(temptest,"Input: LOAD: {:} {:} STORE: {:} {:}",
//             Lsq_Allocate->io_load_queue_tail,Lsq_Allocate->io_load_queue_nextTail,Lsq_Allocate->io_store_queue_tail,Lsq_Allocate->io_store_queue_nextTail);
//             DPRINTF(temptest,"O: LOAD1 {:#x} {:} LOAD2 {:#x} {:} STORE1 {:#x} {:} STORE2 {:#x} {:}",
//             a,insnPkg[0]->LSQTag,b,insnPkg[1]->LSQTag,
//             c,insnPkg[0]->LSQTag,d,insnPkg[1]->LSQTag);
//             DPRINTF(temptest,"V: LOAD1 {:#x} {:} LOAD2 {:#x} {:} STORE1 {:#x} {:} STORE2 {:#x} {:}",
//             Lsq_Allocate->io_wEn1_load_queue,Lsq_Allocate->io_insn1_LSQTag,Lsq_Allocate->io_wEn2_load_queue,Lsq_Allocate->io_insn2_LSQTag,
//             Lsq_Allocate->io_wEn1_store_queue,Lsq_Allocate->io_insn1_LSQTag,Lsq_Allocate->io_wEn2_store_queue,Lsq_Allocate->io_insn2_LSQTag);
//             exit(1);
//         }
//     }
//     else if(allocCount==2){
//         if(a!=Lsq_Allocate->io_wEn1_load_queue||b!=Lsq_Allocate->io_wEn2_load_queue||c!=Lsq_Allocate->io_wEn1_store_queue
//         ||d!=Lsq_Allocate->io_wEn2_store_queue||insnPkg[0]->LSQTag!=Lsq_Allocate->io_insn1_LSQTag||insnPkg[1]->LSQTag!=Lsq_Allocate->io_insn2_LSQTag){
//             DPRINTF(temptest,"Input: {:} O: {:} {:} V: {:} {:}",
//             allocCount,insnPkg[0]->Fu,insnPkg[1]->Fu,Lsq_Allocate->io_insn1_Function_type,Lsq_Allocate->io_insn2_Function_type);
//             DPRINTF(temptest,"Input: LOAD: {:} {:} STORE: {:} {:}",
//             Lsq_Allocate->io_load_queue_tail,Lsq_Allocate->io_load_queue_nextTail,Lsq_Allocate->io_store_queue_tail,Lsq_Allocate->io_store_queue_nextTail);
//             DPRINTF(temptest,"O: LOAD1 {:#x} {:} LOAD2 {:#x} {:} STORE1 {:#x} {:} STORE2 {:#x} {:}",
//             a,insnPkg[0]->LSQTag,b,insnPkg[1]->LSQTag,
//             c,insnPkg[0]->LSQTag,d,insnPkg[1]->LSQTag);
//             DPRINTF(temptest,"V: LOAD1 {:#x} {:} LOAD2 {:#x} {:} STORE1 {:#x} {:} STORE2 {:#x} {:}",
//             Lsq_Allocate->io_wEn1_load_queue,Lsq_Allocate->io_insn1_LSQTag,Lsq_Allocate->io_wEn2_load_queue,Lsq_Allocate->io_insn2_LSQTag,
//             Lsq_Allocate->io_wEn1_store_queue,Lsq_Allocate->io_insn1_LSQTag,Lsq_Allocate->io_wEn2_store_queue,Lsq_Allocate->io_insn2_LSQTag);
//             exit(1);
//         }
//     }
//     */   
// }

void 
Lsq::Allocate(InsnPkg_t& insnPkg,uint64_t allocCount){
    uint64_t m_LoadQueue_tail=this->m_LoadQueue.getTail();
    uint64_t m_StoreQueue_tail=this->m_StoreQueue.getTail();
    
    insnPkg[0]->LSQTag=insnPkg[0]->Fu==funcType_t::LDU?m_LoadQueue_tail:(
                    insnPkg[0]->Fu==funcType_t::STU?m_StoreQueue_tail:0);

    insnPkg[1]->LSQTag=insnPkg[1]->Fu==funcType_t::LDU?(m_LoadQueue_tail+(insnPkg[0]->Fu==funcType_t::LDU))%this->m_LoadQueue_count:(
                    insnPkg[1]->Fu==funcType_t::STU?(m_StoreQueue_tail+(insnPkg[0]->Fu==funcType_t::STU))%this->m_StoreQueue_count:0);

    insnPkg[2]->LSQTag=insnPkg[2]->Fu==funcType_t::LDU?(m_LoadQueue_tail+(insnPkg[0]->Fu==funcType_t::LDU)+(insnPkg[1]->Fu==funcType_t::LDU))%this->m_LoadQueue_count:(
                    insnPkg[2]->Fu==funcType_t::STU?(m_StoreQueue_tail+(insnPkg[0]->Fu==funcType_t::STU)+(insnPkg[1]->Fu==funcType_t::STU))%this->m_StoreQueue_count:0);

    insnPkg[3]->LSQTag=insnPkg[3]->Fu==funcType_t::LDU?(m_LoadQueue_tail+(insnPkg[0]->Fu==funcType_t::LDU)+(insnPkg[1]->Fu==funcType_t::LDU)+(insnPkg[2]->Fu==funcType_t::LDU))%this->m_LoadQueue_count:(
                    insnPkg[3]->Fu==funcType_t::STU?(m_StoreQueue_tail+(insnPkg[0]->Fu==funcType_t::STU)+(insnPkg[1]->Fu==funcType_t::STU)+(insnPkg[2]->Fu==funcType_t::STU))%this->m_StoreQueue_count:0);
    for(size_t i = 0 ; i < allocCount; i++){
        auto& insn = insnPkg[i];
        if(insn->Fu == funcType_t::LDU){
            this->LSQ_Style_Group[i]=1;
            this->load_entry[i].RobTag=insnPkg[i]->RobTag;
            this->load_entry[i].IsaRd=insnPkg[i]->IsaRd;
            this->load_entry[i].PhyRd=insnPkg[i]->PhyRd;
            this->load_entry[i].Fu=insnPkg[i]->Fu;
        }else if (insn->Fu == funcType_t::STU){
            this->LSQ_Style_Group[i]=2;
            this->store_entry[i].SubOp=insnPkg[i]->SubOp;
        }
        else{
            this->LSQ_Style_Group[i]=0;
        }
    }  

    for(int i=0;i<4;i++){
        this->send_lsq_insnPtr[i]=insnPkg[i];
    }
}

void 
Lsq::TryIssueLoad(MemReq_t& memReq,bool& Success){
    Success = false;
    if(!this->m_LoadQueue.empty()){//需要加载的地址都放在了loadqueue里
        uint16_t ldqPtr = this->m_LoadQueue.getHeader();
        for(size_t i = 0 ; i < this->m_LoadQueue.getUsage(); i++){
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
                    // DPRINTF(LoadReq,"RobPtr[{}],Pc[{:#x}] -> Send Load Request : Address[{:#x}]",
                    //     ldqEntry.insnPtr->RobTag,ldqEntry.insnPtr->Pc,ldqEntry.address);
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
                switch (stqEntry.SubOp)
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
    
    for(int i=0;i<4;i++){
        if(this->LSQ_Style_Group[i]==1){
            this->load_entry[i].state = loadState_t::load_WaitSend;
            this->load_entry[i].commited = false;
            this->load_entry[i].killed = false;
            this->load_entry[i].insnPtr=this->send_lsq_insnPtr[i];
            this->load_entry[i].addressReady = false;
            this->load_entry[i].address      = 0;
            this->m_LoadQueue[this->m_LoadQueue.Allocate()] = this->load_entry[i];//将数据存入load queue的尾部 
        }
        if(this->LSQ_Style_Group[i]==2){
            this->store_entry[i].state = storeState_t::store_WaitSend;
        
            this->store_entry[i].commited = false;
            this->store_entry[i].addressReady = false;
            this->store_entry[i].address = 0;
            this->store_entry[i].killed = false;
            this->store_entry[i].dataReady = false;;
            this->store_entry[i].data = 0 ;
            this->m_StoreQueue[this->m_StoreQueue.Allocate()] = this->store_entry[i];//将数据存入load queue的尾部
        }
    }
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











