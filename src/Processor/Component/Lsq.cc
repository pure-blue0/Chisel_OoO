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
    this->storeQueue_pop=false;
    for(int i;i<4;i++){
        this->KillLsqEntry_flag[i]=0;
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
    uint8_t LoadQueue_unuse_count=this->m_LoadQueue.getAvailEntryCount();
    uint8_t StoreQueue_unuse_count=this->m_StoreQueue.getAvailEntryCount();
    uint8_t avail_count=insnPkg[0]->data_valid+insnPkg[1]->data_valid+insnPkg[2]->data_valid+insnPkg[3]->data_valid;
    uint8_t match_num[4]={0,0,0,0};
    bool success_dispatch_flag[4]={false,false,false,false},stop_flag[4]={false,false,false,false};
    for(int i=0;i<avail_count;i++){
        InsnPtr_t insn = insnPkg[i];
        if(stop_flag[i]){
            stop_flag[i+1]=true;
        }
        else{
            if(insn->Fu == funcType_t::LDU){
                uint8_t Load_Allocate_count=0;
                for(int j=0;j<i;j++){
                    Load_Allocate_count=Load_Allocate_count+(match_num[j]==1);
                }
                if(LoadQueue_unuse_count > Load_Allocate_count){
                    success_dispatch_flag[i]=true;
                    match_num[i]=1;
                }else{
                    stop_flag[i+1]=true;
                }
            }else if(insn->Fu == funcType_t::STU){
                uint8_t Store_Allocate_count=0;
                for(int j=0;j<i;j++){
                    Store_Allocate_count=Store_Allocate_count+(match_num[j]==2);
                }
                if(StoreQueue_unuse_count > Store_Allocate_count){
                    success_dispatch_flag[i]=true;
                    match_num[i]=2;
                }else{
                    stop_flag[i+1]=true;
                }
            }
            else success_dispatch_flag[i]=true;
        }
        
    }
    
    SuccessCount=success_dispatch_flag[0]+success_dispatch_flag[1]+success_dispatch_flag[2]+success_dispatch_flag[3];

    // SuccessCount = 0;
    // uint16_t AllocLdqCount = 0;
    // uint16_t AllocStqCount = 0;
    // uint8_t avail_count=insnPkg[0]->data_valid+insnPkg[1]->data_valid+insnPkg[2]->data_valid+insnPkg[3]->data_valid;
    // for(auto& insn : insnPkg){//根据insn的function type统计输入的两条指令立有多少条是load指令，有多少条是store指令
    //     if(insn->Fu == funcType_t::LDU)      AllocLdqCount++;
    //     else if(insn->Fu == funcType_t::STU) AllocStqCount++;    
    // }
    // uint8_t stop_flag[4];

    // uint8_t AllocLdqCount[4],AllocStqCount[4];

    // if(this->m_LoadQueue.getAvailEntryCount()<AllocLdqCount)
    //     SuccessCount=this->m_LoadQueue.getAvailEntryCount();
    // else if(this->m_StoreQueue.getAvailEntryCount()<AllocStqCount)
    //     SuccessCount=this->m_StoreQueue.getAvailEntryCount();
    // else SuccessCount = avail_count;
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
    
    for(size_t i = 0 ; i < allocCount; i++){
        auto& insn = insnPkg[i];

        if(insn->Fu == funcType_t::LDU){
            this->LSQ_Style_Group[i]=1;
            uint8_t AllocatedIn_load_count=0;
            for(int j=0;j<i;j++){
                AllocatedIn_load_count=AllocatedIn_load_count+(insnPkg[j]->Fu==funcType_t::LDU);
            }
            insnPkg[i]->LSQTag=(m_LoadQueue_tail+AllocatedIn_load_count)%this->m_LoadQueue_count;
        }else if (insn->Fu == funcType_t::STU){
            this->LSQ_Style_Group[i]=2;

            uint8_t AllocatedIn_store_count=0;
            for(int j=0;j<i;j++){
                AllocatedIn_store_count=AllocatedIn_store_count+(insnPkg[j]->Fu==funcType_t::STU);
            }
            insnPkg[i]->LSQTag=(m_StoreQueue_tail+AllocatedIn_store_count)%this->m_StoreQueue_count;
        }
        else{
            this->LSQ_Style_Group[i]=0;
        }
        this->lsq_data[i].RobTag=insnPkg[i]->RobTag;
        this->lsq_data[i].IsaRd=insnPkg[i]->IsaRd;
        this->lsq_data[i].PhyRd=insnPkg[i]->PhyRd;
        this->lsq_data[i].Fu=insnPkg[i]->Fu;
        this->lsq_data[i].SubOp=insnPkg[i]->SubOp;
    }  
}

void 
Lsq::TryIssueLoad(MemReq_t& memReq,bool& Success){
    memReq.Length  = this->m_Processor->m_XLEN / 2;//spec中没写，验证时把这一行留着。

    bool stop_flag[4]={false,false,false,false};//根据最大循环次数改变数量，C++里是 n+1，HDL里就是n
    Success = false;
    this->load_state_update_EN=false;
    uint8_t Try_num=this->m_LoadQueue.getUsage()<MAX_Try_issue_num?this->m_LoadQueue.getUsage():MAX_Try_issue_num;
    
    uint16_t ldqPtr = this->m_LoadQueue.getHeader();
    for(size_t i = 0 ; i < Try_num; i++){
        if(stop_flag[i]){
            stop_flag[i+1]=true;
        }
        else{
            auto ldqEntry = this->m_LoadQueue[ldqPtr];
            if(ldqEntry.state == loadState_t::load_WaitSend && !ldqEntry.killed){
                if(ldqEntry.addressReady )
                {
                    Success = true;
                    this->load_state_update_EN=true;
                    this->load_state_update_ptr=ldqPtr;
                    memReq.Opcode  = MemOp_t::Load;
                    memReq.Id.TransId = ldqPtr;
                    memReq.Address = ldqEntry.address & ~(this->m_dCacheAlignByte - 1);  
                }
                stop_flag[i+1]=true;//<==>break;
            }
            ldqPtr = this->m_LoadQueue.getNextPtr(ldqPtr);
        }
    }
}

void 
Lsq::TryIssueStore(MemReq_t& memReq,bool& Success){
    memReq.Length       = this->m_Processor->m_XLEN / 2;//spec中没写，验证时把这一行留着。
    Success = false;
    this->store_state_update_EN=false;
    if(!this->m_StoreQueue.empty()){
        uint16_t stqPtr = this->m_StoreQueue.getHeader();
        auto stqEntry = this->m_StoreQueue[stqPtr];
        if(stqEntry.state == storeState_t::store_WaitSend){
            if(stqEntry.commited){
                this->store_state_update_EN=true;
                uint64_t offset     = (stqEntry.address & (this->m_dCacheAlignByte - 1));
                memReq.Address      = stqEntry.address & ~(this->m_dCacheAlignByte - 1);
                Success             = true;
                memReq.Opcode       = MemOp_t::Store;
                memReq.Id.TransId   = stqPtr;
                for(size_t it = offset ; it < 8; it++){
                    memReq.Data[it] = (stqEntry.data >> ((it-offset)<<3)) & 0xff;
                }
                
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
            LDQ_entry_t load_entry;
            load_entry.state = loadState_t::load_WaitSend;
            load_entry.commited = false;
            load_entry.killed = false;
            load_entry.addressReady = false;
            load_entry.address      = 0;
            load_entry.RobTag=this->lsq_data[i].RobTag;
            load_entry.IsaRd=this->lsq_data[i].IsaRd;
            load_entry.PhyRd=this->lsq_data[i].PhyRd;
            load_entry.Fu=this->lsq_data[i].Fu;
            load_entry.SubOp=this->lsq_data[i].SubOp;
            this->m_LoadQueue[this->m_LoadQueue.Allocate()] = load_entry;//将数据存入load queue的尾部 
        }
        else if(this->LSQ_Style_Group[i]==2){
            STQ_entry_t store_entry;
            store_entry.state = storeState_t::store_WaitSend;
            store_entry.commited = false;
            store_entry.addressReady = false;
            store_entry.address = 0;
            store_entry.killed = false;
            store_entry.dataReady = false;;
            store_entry.data = 0 ;
            store_entry.SubOp=this->lsq_data[i].SubOp;
            this->m_StoreQueue[this->m_StoreQueue.Allocate()] = store_entry;//将数据存入load queue的尾部
        }

        if(this->KillLsqEntry_flag[i]==1)this->m_LoadQueue[this->KillLsqEntry_Tag[i]].killed = true;
        else if(this->KillLsqEntry_flag[i]==2)this->m_StoreQueue[this->KillLsqEntry_Tag[i]].killed = true;
        
        if(this->Commit_Style_Group[i]==1)this->m_LoadQueue[this->Commit_LSQTag[i]].commited = true;
        else if(this->Commit_Style_Group[i]==2)this->m_StoreQueue[this->Commit_LSQTag[i]].commited = true;
    }
    //from LSU
    if(this->LSU_Style== 1){
        this->m_LoadQueue[this->LSU_LSQTag].addressReady = true;
        this->m_LoadQueue[this->LSU_LSQTag].address  = this->LSU_Agu_addr;
    }else if(this->LSU_Style== 2)
    {
        this->m_StoreQueue[this->LSU_LSQTag].addressReady = true;
        this->m_StoreQueue[this->LSU_LSQTag].dataReady = true;
        this->m_StoreQueue[this->LSU_LSQTag].address  = this->LSU_Agu_addr;
        this->m_StoreQueue[this->LSU_LSQTag].data  = this->LSU_Agu_data;
    }
    //from TryIssueStore and TryIssueLoad
    if(this->load_state_update_EN){
        this->m_LoadQueue[this->load_state_update_ptr].state=loadState_t::load_Inflight;
    }
    if(this->store_state_update_EN){
        this->m_StoreQueue[this->m_StoreQueue.getHeader()].state=storeState_t::store_Inflight;       
    }
    //From  LSU mem
    if(this->MEM_load_state_update_EN){
        this->m_LoadQueue[this->MEM_lsq_state_update_ptr].state=loadState_t::load_Executed;
    }
        
    if(!this->m_StoreQueue.empty()){
        auto& STqEntry = this->m_StoreQueue[this->m_StoreQueue.getHeader()];
        if(this->storeQueue_pop){
            this->m_StoreQueue.Pop();
        }
        else if(STqEntry.killed && STqEntry.state != storeState_t::store_Inflight){
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











