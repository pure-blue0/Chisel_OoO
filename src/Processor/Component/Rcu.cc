#include "Rcu.hh"
#include "../Processor.hh"
#include "../../RISCV/encoding.out.h"
//#include "./obj_dir/VCreateRobEntry.h"
//#include "./obj_dir/VRcuAllocate.h"
namespace Emulator
{
    
Rcu::Rcu(
    Processor*          processor,
    const bool          RenameRegister,
    const uint64_t      RobEntryCount,
    const uint64_t      IntRegCount,
    const bool          SupportRollBack,
    const uint64_t      SnapShotNum,
    const uint64_t      AllocWidth,
    const uint64_t      DeallocWidth
):  TraceObject("Rcu"),
    m_Processor(processor),
    m_RenameRegister(RenameRegister),
    m_Rob("Rob",RobEntryCount),
    m_IntRegfile("IntRegfile",IntRegCount),
    m_IntRenameTable("IntRenameTable"),
    m_IntBusylist("IntBusylist",IntRegCount),
    m_IntFreelist("m_IntFreelist",IntRegCount),
    m_Snapshot("Snapshot",SnapShotNum),
    m_SupportRollback(SupportRollBack),
    m_AllocWidth(AllocWidth),
    m_DeallocWidth(DeallocWidth)
{
    this->ROB_Count=RobEntryCount;
    this->Freelist_size=IntRegCount;
   // RcuAllocate=new VRcuAllocate;//创建对象
}

Rcu::~Rcu(){
    //delete this->RcuAllocate;
}

void
Rcu::Reset(){
    /* Reset Rob */
    this->m_Rob.Reset();
    this->m_RobState = rob_state_t::Rob_Idle;
    this->m_RollBackTag = 0;
    this->m_ExcpCause=0;
    this->m_ExcpTval=0;
    this->freelist_pop_num=0;
    for(int i=0;i<4;i++){
        this->RN_Release_EN[i]=false;
    }
    /* Reset Regfile */
    this->m_IntRegfile.Reset();
    /* Reset RenameTable */
    this->m_IntRenameTable.Reset(this->m_RenameRegister);
    /* Reset Busylist */
    this->m_IntBusylist.Reset(this->m_RenameRegister);
    /* Reset FreeList */
    this->m_IntFreelist.Reset();
    this->FreeList_Header[0]=this->m_IntFreelist.front();
    this->FreeList_Header[1]=this->m_IntFreelist.next_front();
    this->FreeList_Header[2]=this->m_IntFreelist.third_front();
    this->FreeList_Header[3]=this->m_IntFreelist.forth_front();
    this->FreeList_AvailEntryCount=this->Freelist_size;

    for(int i=0;i<4;i++){
        this->ROB_WB_EN_Group[i]=false;
        this->ROB_WB_ROBTag_Group[i]=0;
        this->ROB_WB_Data_isExcp_Group[i]=false;
        this->ROB_WB_Data_isMisPred_Group[i]=false;
        this->ROB_Entry_WEN[i]=false;
        this->BusyList_Forward_Update_EN[i]=false;
        this->BusyList_Forward_Update_PhyRd[i]=0;
        this->BusyList_Forward_Update_Rdresult[i]=0;

        this->RegandBusylist_WB_EN[i]=false;
        this->Regfile_Busylist_WB_PhyRd[i]=0;
        this->IntRegfile_WB_RdResult[i]=0;

        this->FreeBusyList_Commit_EN[i]=false;
        this->FreeBusyList_Commit_LphyRd[i]=0;
    }
    this->ROB_POP_Num=0;
    this->RollBack_num=0;
}

bool Rcu::isOlder(uint64_t tag1, uint64_t tag2,uint64_t header){//当tag1更先入队的话，则输出true

        // VIsOlder *IsOlder;
        // IsOlder=new VIsOlder;//创建对象
        // //连接输入
        // IsOlder->io_tag1=tag1;
        // IsOlder->io_tag2=tag2;
        // IsOlder->io_header=header;
        // //连接输出
        // IsOlder->eval();
        // return IsOlder->io_older;
        // delete IsOlder;//删除创建的对象
        bool tag1GeHeader = tag1 >= header;
        bool tag2GeHeader = tag2 >= header;
        bool older=(tag1GeHeader ^ tag2GeHeader) ? tag1 > tag2 : tag1 < tag2; 
        return  older;
}

void Rcu::CreateRobEntry(InsnPkg_t& insnPkg, bool ROB_Entry_WEN[4],uint8_t& allocCount,Rob_entry_t* newEntry){
    allocCount=ROB_Entry_WEN[0]+ROB_Entry_WEN[1]+ROB_Entry_WEN[2]+ROB_Entry_WEN[3];
    bool FlushBackWard[4]={false,false,false,false};
    for(size_t i = 0; i < allocCount; i++){
        InsnPtr_t insn = insnPkg[i];
        bool isNop   = (insn->Fu == funcType_t::ALU) && (insn->IsaRd == 0);//nop
        bool isFence = (insn->Fu == funcType_t::CSR) && (insn->SubOp == 9);//fence
        bool isMret  = (insn->Fu == funcType_t::CSR) && (insn->SubOp == 7);//mret
        newEntry[i].valid              = true;
        newEntry[i].done               = isNop | insn->Excp.valid | isFence | isMret;//如果是这些情况则done
        newEntry[i].isStable           = newEntry[i].done;
        newEntry[i].isMisPred          = false;
        newEntry[i].isExcp             = insn->Excp.valid;
        newEntry[i].pc                 = insn->Pc;
        newEntry[i].Fu                 = insn->Fu;
        newEntry[i].LSQtag             = insn->LSQTag;
        newEntry[i].isaRd              = insn->IsaRd;
        newEntry[i].phyRd              = insn->PhyRd;
        newEntry[i].LphyRd             = insn->LPhyRd;

        if(insn->Excp.valid){
            this->m_RobState        = rob_state_t::Rob_Undo;
            this->m_RollBackTag     = insn->RobTag;
            this->m_ExcpCause       = insn->Excp.Cause;
            this->m_ExcpTval        = insn->Excp.Tval;
            FlushBackWard[i] = true;
        }else{
            if(insn->ControlFlowInsn){
                this->m_RobState        = rob_state_t::Rob_Undo;
                this->m_RollBackTag     = insn->RobTag;   
                if(isMret){
                    this->m_RobState        = rob_state_t::Rob_Idle;
                }
            }
        }
        // DPRINTF(temptest,"{:#x}:{:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x}  ", 
        //     this->m_Rob.getLastest(),this->m_Rob[this->m_Rob.getLastest()].valid,this->m_Rob[this->m_Rob.getLastest()].done,this->m_Rob[this->m_Rob.getLastest()].Fu,
        //     this->m_Rob[this->m_Rob.getLastest()].isaRd,this->m_Rob[this->m_Rob.getLastest()].isExcp,this->m_Rob[this->m_Rob.getLastest()].isMisPred,
        //     this->m_Rob[this->m_Rob.getLastest()].isStable,this->m_Rob[this->m_Rob.getLastest()].LphyRd,this->m_Rob[this->m_Rob.getLastest()].LSQtag,
        //     this->m_Rob[this->m_Rob.getLastest()].pc,this->m_Rob[this->m_Rob.getLastest()].phyRd,this->m_Rob[this->m_Rob.getLastest()].valid);
    }   
    bool FlushBackWard_flag=FlushBackWard[0]||FlushBackWard[1]||FlushBackWard[2]||FlushBackWard[3];
    //--------------------------------------------------
    if(FlushBackWard_flag)this->m_Processor->FlushBackWard(InsnState_t::State_Issue);//发生异常就把issue前的stage都插入气泡
}

void Rcu::Allocate(bool reset_n,InsnPkg_t& insnPkg, uint64_t allocCount){
    uint8_t freelist_usecount_temp=0;
    for(int i=0;i<4;i++){this->BusyList_Update_EN[i]=false;}
    uint64_t m_Rob_Tail=this->m_Rob.getTail();
    //----------------------------Composition----------------------------------
    for(size_t i = 0; i < allocCount; i++){
        InsnPtr_t& insn = insnPkg[i];
        if(insn->IsaRd != 0){
            insn->PhyRd = this->FreeList_Header[freelist_usecount_temp];//取出空闲的reg id
            this->BusyList_Update_EN[i]=true;
            this->BusyList_Update_PhyRd[i]=insn->PhyRd;
            freelist_usecount_temp++;
        }
    }
    for(size_t i = 0; i < allocCount; i++){
        insnPkg[i]->RobTag=(m_Rob_Tail+i)%this->ROB_Count;
    }

    for(int i=0;i<4;i++){
        insnPkg[i]->PhyRs1 = this->m_IntRenameTable[insnPkg[i]->IsaRs1];
        insnPkg[i]->PhyRs2 = this->m_IntRenameTable[insnPkg[i]->IsaRs2];
        insnPkg[i]->LPhyRd = this->m_IntRenameTable[insnPkg[i]->IsaRd];
        for(int j=0;j<i;j++){
            if(insnPkg[i]->IsaRs1==insnPkg[j]->IsaRd)insnPkg[i]->PhyRs1=insnPkg[j]->PhyRd;//RAW
            if(insnPkg[i]->IsaRs2==insnPkg[j]->IsaRd)insnPkg[i]->PhyRs2=insnPkg[j]->PhyRd;//RAW
            if(insnPkg[i]->IsaRd==insnPkg[j]->IsaRd) insnPkg[i]->LPhyRd=insnPkg[j]->PhyRd;//WAW
        }
    }
    
    this->ROB_Entry_WEN[0]=allocCount?true:false;
    this->ROB_Entry_WEN[1]=allocCount>1?true:false;
    this->ROB_Entry_WEN[2]=allocCount>2?true:false;
    this->ROB_Entry_WEN[3]=allocCount>3?true:false;

    this->freelist_pop_num=this->BusyList_Update_EN[0]+this->BusyList_Update_EN[1]+this->BusyList_Update_EN[2]+this->BusyList_Update_EN[3];
    //----------------------------Sequence----------------------------------
    if(!reset_n){
        this->m_IntRenameTable.Reset(this->m_RenameRegister);
    }
    else{
        for(size_t i = 0; i < allocCount; i++){
        if(insnPkg[i]->IsaRd != 0)this->m_IntRenameTable[insnPkg[i]->IsaRd]=insnPkg[i]->PhyRd;
        }

        for(int i=0;i<4;i++)
        {   //Resource release
            if(this->RN_Release_EN[i]){
                this->m_IntRenameTable[this->RN_Release_IsaRd[i]]=this->RN_Release_LphyRd[i];
            }
        }
    }
    
    //-----------------------------------------------------------------

    for(int i=0;i<allocCount;i++)
    {
        this->rob_insnPkg.emplace_back(insnPkg[i]);
    }

    
}

void Rcu::TryAllocate(InsnPkg_t& insnPkg, uint64_t& SuccessCount){ 
    // VRcu_TryAllocate *Rcu_TryAllocate;
    // Rcu_TryAllocate=new VRcu_TryAllocate;//创建对象
    // //连接输入
    // Rcu_TryAllocate->io_insn_num=insnPkg.size();
    // Rcu_TryAllocate->io_Rob_unuse_count=this->m_Rob.getAvailEntryCount();
    // Rcu_TryAllocate->io_IntFreelist_unuse_count=this->m_IntFreelist.getAvailEntryCount();
    // //连接输出
    // Rcu_TryAllocate->eval();
    // SuccessCount=Rcu_TryAllocate->io_Rcu_success_count;
    // delete Rcu_TryAllocate;//删除创建的对象
    
    uint8_t avail_count=insnPkg[0]->data_valid+insnPkg[1]->data_valid+insnPkg[2]->data_valid+insnPkg[3]->data_valid;
    SuccessCount = this->m_Rob.getAvailEntryCount()<this->FreeList_AvailEntryCount?this->m_Rob.getAvailEntryCount():this->FreeList_AvailEntryCount;
    SuccessCount =avail_count<SuccessCount?avail_count:SuccessCount; 
}

bool Rcu::ReadyForCommit(uint64_t RobTag){
    uint8_t mid_result1[4]={0,0,0,0};
    uint64_t RobHeaderPtr[4]={this->m_Rob.getHeader(),this->m_Rob.getNextPtr(this->m_Rob.getHeader()),
                        this->m_Rob.getNextPtr(this->m_Rob.getNextPtr(this->m_Rob.getHeader())),
                        this->m_Rob.getNextPtr(this->m_Rob.getNextPtr(this->m_Rob.getNextPtr(this->m_Rob.getHeader())))};
    for(size_t i = 0; i < this->m_DeallocWidth ; i++){
        if(RobHeaderPtr[i] == RobTag){
                mid_result1[i]=2;
        }
        else{
            auto RobEntry = this->m_Rob[RobHeaderPtr[i]];
            if( !RobEntry.isStable && (RobEntry.Fu == funcType_t::LDU || 
                RobEntry.Fu == funcType_t::STU || RobEntry.Fu == funcType_t::BRU))
            {
                mid_result1[i]=1;
            }
            else mid_result1[i]=0;
        }
        if(i>=this->m_Rob.getUsage())mid_result1[i]=0;
    }         
    bool mid_result2=false;
    if(!mid_result1[0])mid_result2=mid_result1[0]-1;
    else if(!mid_result1[1])mid_result2=mid_result1[1]-1;
    else if(!mid_result1[2])mid_result2=mid_result1[2]-1;
    else if(!mid_result1[3])mid_result2=mid_result1[3]-1;
    else mid_result2=0;

    uint8_t ReadyForCommit=false;
    if(this->m_RobState == rob_state_t::Rob_Idle ||  this->isOlder(RobTag,this->m_RollBackTag,this->m_Rob.getHeader())) ReadyForCommit=bool(mid_result2);
    else if(this->m_RobState == rob_state_t::Rob_WaitForResume && this->m_RollBackTag == RobTag) ReadyForCommit=true;
    else ReadyForCommit=false;
    
    return ReadyForCommit;
}

void Rcu::WriteBack(int index,InsnPtr_t& insn, bool& needRedirect,Redirect_t& RedirectReq){
    needRedirect = false;
     RegandBusylist_WB_EN[index]=false;
     this->ROB_WB_EN_Group[index]=false;
    if(!this->m_Rob.empty() && (this->isOlder(insn->RobTag,this->m_Rob.getLastest(),this->m_Rob.getHeader()) || insn->RobTag == this->m_Rob.getLastest())){

        this->ROB_WB_EN_Group[index]=true;
        this->ROB_WB_ROBTag_Group[index]=insn->RobTag;
        this->ROB_WB_Data_isExcp_Group[index]=insn->Excp.valid;
        this->ROB_WB_Data_isMisPred_Group[index]=insn->BruMisPred;
        if(!insn->Excp.valid){
            if(insn->Fu == funcType_t::BRU){
                if(insn->BruMisPred){
                      
                    if(this->m_RobState == rob_state_t::Rob_Idle || 
                        (this->isOlder(insn->RobTag,this->m_RollBackTag,this->m_Rob.getHeader()) ||
                        insn->RobTag == this->m_RollBackTag)
                    ){
                        this->m_RobState = rob_state_t::Rob_Undo;
                        this->m_RollBackTag = insn->RobTag;
                        
                        this->m_Processor->FlushBackWard(InsnState_t::State_Issue);//刷新fetch1，decode，dispatch
                        needRedirect = true;
                        RedirectReq.target  = insn->BruTarget;
                    }
                }else{
                    if(this->m_RobState == rob_state_t::Rob_WaitForResume && insn->RobTag == this->m_RollBackTag)//已经回滚完了，然后再下一个周期的wbstage恢复为正常状态，可以继续处理其他异常
                    {
                        this->m_RobState = rob_state_t::Rob_Idle;
                    }
                }
            }
            if(insn->IsaRd != 0){
                this->RegandBusylist_WB_EN[index]=true;
                this->Regfile_Busylist_WB_PhyRd[index]=insn->PhyRd;
                this->IntRegfile_WB_RdResult[index]=insn->RdResult;
                
            }
        }else{
            if(this->m_RobState == rob_state_t::Rob_Idle || this->isOlder(insn->RobTag,this->m_RollBackTag,this->m_Rob.getHeader()))
            {
                this->m_RobState = rob_state_t::Rob_Undo;
                this->m_RollBackTag = insn->RobTag;
                this->m_ExcpCause   = insn->Excp.Cause;
                this->m_ExcpTval    = insn->Excp.Tval;
            }
        }
    }
}

void Rcu::AGUFastDetect(InsnPtr_t& insn){
   
    DPRINTF(WriteBack,"RobTag[{}],Pc[{:#x}] -> Scan AGU result, Exception [{}]",
                    insn->RobTag,insn->Pc,insn->Excp.valid);
    if(insn->Fu == funcType_t::STU && insn->Agu_addr_ready && insn->Agu_data_ready)
    {
                this->ROB_AGU_Data_done= true;
    }
    else this->ROB_AGU_Data_done = false;
    if(insn->Excp.valid){
         this->ROB_AGU_Data_done   = true;
        if(this->m_RobState != rob_state_t::Rob_Idle )
        {
            this->m_RobState = rob_state_t::Rob_Undo;
            this->m_RollBackTag = insn->RobTag;
            this->m_ExcpCause   = insn->Excp.Cause;
            this->m_ExcpTval    = insn->Excp.Tval;
        }
    }
    
    this->ROB_AGU_ROBTag=insn->RobTag;
    this->ROB_AGU_Data_isExcp=insn->Excp.valid;
}

void 
Rcu::ReleaseResource(uint16_t robTag){
    auto& entry = this->m_Rob[robTag];
    if(entry.phyRd != 0){
        this->m_IntBusylist[entry.phyRd].forwarding = false;
        this->m_IntBusylist[entry.phyRd].done       = false;
        this->m_IntBusylist[entry.phyRd].allocated  = false;
        DPRINTF(RollBack,"RobTag[{}],Pc[{:#x}], Free phyRegister : Rd[{}], PRd[{}], LPRd[{}]",
            robTag,entry.pc,entry.isaRd,entry.phyRd,entry.LphyRd
        );
    }
 
}

void Rcu::RollBack(){
    uint64_t ROB_LastestPtr[4]={this->m_Rob.getLastest(),this->m_Rob.getLastPtr(this->m_Rob.getLastest()),
                             this->m_Rob.getLastPtr(this->m_Rob.getLastPtr(this->m_Rob.getLastest())),
                             this->m_Rob.getLastPtr(this->m_Rob.getLastPtr(this->m_Rob.getLastPtr(this->m_Rob.getLastest())))};
    
    bool ROB_Lastest_valid[4]={this->m_Rob[ROB_LastestPtr[0]].valid,this->m_Rob[ROB_LastestPtr[1]].valid,
                                this->m_Rob[ROB_LastestPtr[2]].valid,this->m_Rob[ROB_LastestPtr[3]].valid};
    bool ROB_Lastest_isExcp[4]={this->m_Rob[ROB_LastestPtr[0]].isExcp,this->m_Rob[ROB_LastestPtr[1]].isExcp,
                                this->m_Rob[ROB_LastestPtr[2]].isExcp,this->m_Rob[ROB_LastestPtr[3]].isExcp};                            
    bool Resource_Release_EN[4]={false,false,false,false};
    bool Rollback_flag[4]={false,false,false,false};
    bool stop_flag[5]={false,false,false,false,false};

    if(this->m_RobState == rob_state_t::Rob_Undo){
        for(size_t i = 0 ; i < this->m_AllocWidth ;i++)
        {   
            if(stop_flag[i]||(!this->isOlder(this->m_RollBackTag, ROB_LastestPtr[i], this->m_Rob.getHeader()) &&
                ROB_LastestPtr[i] != this->m_RollBackTag)) stop_flag[i+1]=true;
            else{
                if(ROB_Lastest_valid[i]){
                    if(ROB_LastestPtr[i] == this->m_RollBackTag){
                        this->m_RobState = rob_state_t::Rob_WaitForResume;
                        if(ROB_Lastest_isExcp[i]){
                            Resource_Release_EN[i]=true;
                        }
                        stop_flag[i+1]=true;
                    }
                    if(!stop_flag[i+1]){
                        Resource_Release_EN[i]=true;
                    }
                }
                if(!stop_flag[i+1])Rollback_flag[i]=true;
            }
        }
    }
    
    for(int i=0;i<4;i++){
        if(Resource_Release_EN[i]){
            if(this->m_Rob[ROB_LastestPtr[i]].Fu == funcType_t::LDU){
                this->m_Processor->getLsqPtr()->KillLsqEntry_flag[i]=1;
            }else if(this->m_Rob[ROB_LastestPtr[i]].Fu == funcType_t::STU){
                this->m_Processor->getLsqPtr()->KillLsqEntry_flag[i]=2;
            }
            else this->m_Processor->getLsqPtr()->KillLsqEntry_flag[i]=0;
            this->m_Processor->getLsqPtr()->KillLsqEntry_Tag[i]=(this->m_Rob[ROB_LastestPtr[i]].LSQtag);
            
            if(this->m_Rob[ROB_LastestPtr[i]].phyRd != 0){
                this->RN_Release_EN[i]=true;
                this->RN_Release_IsaRd[i]=this->m_Rob[ROB_LastestPtr[i]].isaRd;
                this->RN_Release_LphyRd[i]=this->m_Rob[ROB_LastestPtr[i]].LphyRd;
                this->FreeBusy_Release_phyRd[i]=this->m_Rob[ROB_LastestPtr[i]].phyRd;
            }
            else this->RN_Release_EN[i]=false;
        }
    }
    this->RollBack_num=Rollback_flag[0]+Rollback_flag[1]+Rollback_flag[2]+Rollback_flag[3];
}
void Rcu::Freelist_Evaluate(bool reset_n){
    //-------------Sequence-----------------------
    if(!reset_n){
        this->m_IntFreelist.Reset();
        this->FreeList_AvailEntryCount=this->Freelist_size;
    }
    else{
        for(int i=0;i<this->freelist_pop_num;i++){
            this->m_IntFreelist.pop();
        }
        for(int i=0;i<4;i++)
        {   
            if(this->RN_Release_EN[i]){
                this->m_IntFreelist.push(this->FreeBusy_Release_phyRd[i]);
            }
            if(this->FreeBusyList_Commit_EN[i]){
                this->m_IntFreelist.push(this->FreeBusyList_Commit_LphyRd[i]);
            }
        }
        this->FreeList_AvailEntryCount=this->m_IntFreelist.getAvailEntryCount();
    }
    //-------------Composition-----------------------
    this->FreeList_Header[0]=this->m_IntFreelist.front();
    this->FreeList_Header[1]=this->m_IntFreelist.next_front();
    this->FreeList_Header[2]=this->m_IntFreelist.third_front();
    this->FreeList_Header[3]=this->m_IntFreelist.forth_front();
}
void Rcu::Busylist_Evaluate(bool reset_n){
    if(!reset_n){
        this->m_IntBusylist.Reset(this->m_RenameRegister);
    }
    else{
        for(int i=0;i<4;i++){
            //From RCU Allocate
            if(this->BusyList_Update_EN[i]){
                this->m_IntBusylist[this->BusyList_Update_PhyRd[i]].allocated = true;//根据reg id 将busylist中对应的entry进行更新
                this->m_IntBusylist[this->BusyList_Update_PhyRd[i]].forwarding = false;
                this->m_IntBusylist[this->BusyList_Update_PhyRd[i]].done      = false;
            }
            //From Forwarding
            if(this->BusyList_Forward_Update_EN[i]){
                this->m_IntBusylist[this->BusyList_Forward_Update_PhyRd[i]].forwarding = true;
                this->m_IntBusylist[this->BusyList_Forward_Update_PhyRd[i]].done = true;//说明当前指令的rd是被前递了的
            }
            //From WriteBack
            if(this->RegandBusylist_WB_EN[i]){
                this->m_IntBusylist[this->Regfile_Busylist_WB_PhyRd[i]].forwarding = false;
                this->m_IntBusylist[this->Regfile_Busylist_WB_PhyRd[i]].done = true;
            }    
            //From Commit
            if(this->FreeBusyList_Commit_EN[i]){
                this->m_IntBusylist[this->FreeBusyList_Commit_LphyRd[i]].allocated = false;
                this->m_IntBusylist[this->FreeBusyList_Commit_LphyRd[i]].forwarding = false;
                this->m_IntBusylist[this->FreeBusyList_Commit_LphyRd[i]].done = false;
            }
            //From RollBack
            if(this->RN_Release_EN[i]){
                this->m_IntBusylist[this->FreeBusy_Release_phyRd[i]].allocated  = false;
                this->m_IntBusylist[this->FreeBusy_Release_phyRd[i]].forwarding = false;
                this->m_IntBusylist[this->FreeBusy_Release_phyRd[i]].done       = false;
                
            }   
        }
    }
}
void Rcu::IntRegfile_Evaluate(bool reset_n){
    for(int i=0;i<4;i++){
        if(this->BusyList_Forward_Update_EN[i]){
            this->m_IntRegfile[this->BusyList_Forward_Update_PhyRd[i]] = this->BusyList_Forward_Update_Rdresult[i];
        }
        if(this->RegandBusylist_WB_EN[i]){
            this->m_IntRegfile[this->Regfile_Busylist_WB_PhyRd[i]] = this->IntRegfile_WB_RdResult[i];
        }  
    }
}

void Rcu::Evaulate(){
    uint8_t allocCount;
    Rob_entry_t newEntry[4];
    this->CreateRobEntry(this->rob_insnPkg,this->ROB_Entry_WEN,allocCount,newEntry);
    
    for(size_t i = 0; i < allocCount; i++){
        this->m_Rob[this->rob_insnPkg[i]->RobTag] = newEntry[i];
        this->m_Rob.Allocate();
    }
    this->RollBack();
    if(this->m_RobState == rob_state_t::Rob_FlushBackend){
        this->m_RobState = rob_state_t::Rob_Idle;
    }

    //----------------------------Sequence----------------------------------
    //ROB sequence
    for(int i=0;i<4;i++){
        if(this->ROB_WB_EN_Group[i]){
            this->m_Rob[this->ROB_WB_ROBTag_Group[i]].done = true;
            this->m_Rob[this->ROB_WB_ROBTag_Group[i]].isStable = true;
            this->m_Rob[this->ROB_WB_ROBTag_Group[i]].isExcp = this->ROB_WB_Data_isExcp_Group[i];
            this->m_Rob[this->ROB_WB_ROBTag_Group[i]].isMisPred = this->ROB_WB_Data_isMisPred_Group[i];
        }
    }
    if(this->ROB_AGU_EN){
        this->m_Rob[this->ROB_AGU_ROBTag].isStable=true;
        this->m_Rob[this->ROB_AGU_ROBTag].done=this->ROB_AGU_Data_done;
        this->m_Rob[this->ROB_AGU_ROBTag].isExcp=this->ROB_AGU_Data_isExcp;
    }
    for(int i=0;i<this->RollBack_num;i++){
        this->m_Rob.RollBack();//更新rob尾指针
    } 
 

    //--------------------------------------
    this->rob_insnPkg.clear();
    this->ROB_AGU_EN=false;
}


void Rcu::CommitInsn(uint8_t commit_insn_num, Redirect_t& redirectReq, bool& needRedirect){
    bool flush_forward=false,flush_backward=false;
    bool insn_upper[4]={false,false,false,false};
    needRedirect = false;
    for(int i=0;i<4;i++){
        this->FreeBusyList_Commit_EN[i]=false;
        this->FreeBusyList_Commit_LphyRd[i]=0;
        this->m_Processor->getLsqPtr()->Commit_Style_Group[i]=false;
        this->m_Processor->getLsqPtr()->Commit_LSQTag[i]=0;
    }

    uint16_t robPtr = this->m_Rob.getHeader();
    for(size_t i = 0; i < commit_insn_num; i++){
        auto& robEntry = this->m_Rob[robPtr];
        if(robEntry.valid){
            DASSERT(robEntry.done,"Commit Insn When not Ready!")
            if(!robEntry.isExcp){
                if(robEntry.LphyRd != 0){
                    //this->m_Processor->m_ExecContext->WriteIntReg(robEntry.isaRd,this->m_IntRegfile[robEntry.phyRd]);
                    this->FreeBusyList_Commit_EN[i]=true;
                    this->FreeBusyList_Commit_LphyRd[i]=robEntry.LphyRd;

                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Deallocate Last PRd[{}]",robPtr,robEntry.pc,robEntry.LphyRd);
                }
                if(robEntry.Fu == funcType_t::LDU){
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit a Load LSQTag[{}]",robPtr,robEntry.pc,robEntry.LSQtag);
                    this->m_Processor->getLsqPtr()->Commit_Style_Group[i]=1;
                }else if(robEntry.Fu == funcType_t::STU){
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit a Store LSQTag[{}]: Address[{:#x}] Data[{:#x}]",robPtr,robEntry.pc,robEntry.LSQtag);
                    this->m_Processor->getLsqPtr()->Commit_Style_Group[i]=2;
                }
                this->m_Processor->getLsqPtr()->Commit_LSQTag[i]=robEntry.LSQtag;
            }
            //首先检查乱序处理器的状态是否为等待恢复状态，并且当前处理的指令是否为最近的回滚指令。
            //如果条件成立，说明当前正在处理的指令是最近一次发生异常需要回滚的指令。
            if(this->m_RobState == rob_state_t::Rob_WaitForResume && robPtr == this->m_RollBackTag){
                this->m_RobState = rob_state_t::Rob_FlushBackend;
                flush_forward=true;
                if(robEntry.isExcp){
                    needRedirect = true;
                    flush_backward=true;
                    redirectReq.StageId = InsnState_t::State_Commit;
                    this->m_Processor->m_ExecContext->WriteCsr(CSR_MEPC,robEntry.pc);
                    this->m_Processor->m_ExecContext->WriteCsr(CSR_MCAUSE,this->m_ExcpCause);
                    this->m_Processor->m_ExecContext->WriteCsr(CSR_MTVAL,this->m_ExcpTval);
                    this->m_Processor->m_ExecContext->ReadCsr(CSR_MTVEC,redirectReq.target);
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit an Exception, Redirect to {:#x}!",robPtr,robEntry.pc,redirectReq.target);
                }
                DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Resume Execution!",robPtr,robEntry.pc);
            }
            insn_upper[i]=true;
        }
        robPtr=this->m_Rob.getNextPtr(robPtr);    
    }
    this->ROB_POP_Num=commit_insn_num;

    uint8_t insn_count=insn_upper[0]+insn_upper[1]+insn_upper[2]+insn_upper[3];
    
    //----------------------------------------------------------
    if(flush_forward)this->m_Processor->FlushForward(InsnState_t::State_Dispatch);//将issue_flush,commit_flush置1
    if(flush_backward)this->m_Processor->FlushBackWard(InsnState_t::State_Issue);//将fetch_flush,decode_flushdispatch_flush都置1
    for(int i=0;i<insn_count;i++)this->m_Processor->m_ExecContext->InstretInc();//处理指令计数+1
    for(int i=0;i<this->ROB_POP_Num;i++){

        this->m_Rob.Pop();   //弹出处理完毕的指令     
    }
}


} // namespace Emulator
