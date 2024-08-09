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
        this->FE_Commit_EN[i]=false;
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
    }
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

void Rcu::CreateRobEntry(InsnPkg_t& insnPkg, bool ROB_Entry_WEN[4]){
    uint64_t allocCount=ROB_Entry_WEN[0]+ROB_Entry_WEN[1]+ROB_Entry_WEN[2]+ROB_Entry_WEN[3];
    for(size_t i = 0; i < allocCount; i++){
        InsnPtr_t insn = insnPkg[i];
        Rob_entry_t newEntry;
        
        bool isNop   = (insn->Fu == funcType_t::ALU) && (insn->IsaRd == 0);//nop
        bool isFence = (insn->Fu == funcType_t::CSR) && (insn->SubOp == 9);//fence
        bool isMret  = (insn->Fu == funcType_t::CSR) && (insn->SubOp == 7);//mret
        newEntry.valid              = true;
        newEntry.done               = isNop | insn->Excp.valid | isFence | isMret;//如果是这些情况则done
        newEntry.isStable           = newEntry.done;
        newEntry.isMisPred          = false;
        newEntry.isExcp             = insn->Excp.valid;
        newEntry.pc                 = insn->Pc;
        newEntry.Fu                 = insn->Fu;
        newEntry.LSQtag             = insn->LSQTag;
        newEntry.isaRd              = insn->IsaRd;
        newEntry.phyRd              = insn->PhyRd;
        newEntry.LphyRd             = insn->LPhyRd;

        if(insn->Excp.valid){
            this->m_RobState        = rob_state_t::Rob_Undo;
            this->m_RollBackTag     = insn->RobTag;
            this->m_ExcpCause       = insn->Excp.Cause;
            this->m_ExcpTval        = insn->Excp.Tval;
            this->m_Processor->FlushBackWard(InsnState_t::State_Issue);//发生异常就把issue前的stage都插入气泡
               
        }else{
            if(insn->ControlFlowInsn){
                this->m_RobState        = rob_state_t::Rob_Undo;
                this->m_RollBackTag     = insn->RobTag;   
                if(isMret){
                    this->m_RobState        = rob_state_t::Rob_Idle;
                }
            }
        }
        
        this->m_Rob[insn->RobTag] = newEntry;
        // DPRINTF(temptest,"{:#x}:{:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x}  ", 
        //     this->m_Rob.getLastest(),this->m_Rob[this->m_Rob.getLastest()].valid,this->m_Rob[this->m_Rob.getLastest()].done,this->m_Rob[this->m_Rob.getLastest()].Fu,
        //     this->m_Rob[this->m_Rob.getLastest()].isaRd,this->m_Rob[this->m_Rob.getLastest()].isExcp,this->m_Rob[this->m_Rob.getLastest()].isMisPred,
        //     this->m_Rob[this->m_Rob.getLastest()].isStable,this->m_Rob[this->m_Rob.getLastest()].LphyRd,this->m_Rob[this->m_Rob.getLastest()].LSQtag,
        //     this->m_Rob[this->m_Rob.getLastest()].pc,this->m_Rob[this->m_Rob.getLastest()].phyRd,this->m_Rob[this->m_Rob.getLastest()].valid);
    }   
    //update tail ptr
    for(size_t i = 0; i < allocCount; i++)this->m_Rob.Allocate();

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
                this->m_IntRenameTable[this->RN_Release_IsaRd[i]]=this->RN_Release_Data[i];
            }
        }
    }
    
    //-----------------------------------------------------------------

    for(int i=0;i<allocCount;i++)
    {
        this->rob_insnPkg.emplace_back(insnPkg[i]);
    }

    for(int i=0;i<4;i++){
        if(this->BusyList_Update_EN[i]){
            this->m_IntBusylist[this->BusyList_Update_PhyRd[i]].allocated = true;//根据reg id 将busylist中对应的entry进行更新
            this->m_IntBusylist[this->BusyList_Update_PhyRd[i]].done      = false;
            this->m_IntBusylist[this->BusyList_Update_PhyRd[i]].forwarding = false;
        }
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
    uint64_t RobPtr = this->m_Rob.getHeader();
    if(this->m_RobState == rob_state_t::Rob_Idle ||  this->isOlder(RobTag,this->m_RollBackTag,this->m_Rob.getHeader())){
        if(this->m_Rob.getUsage()==0)return false;
        else if(this->m_Rob.getUsage()==1){
            if(RobPtr == RobTag)return true;
            else return false;
        }
        else if(this->m_Rob.getUsage()>=2){
            if(RobPtr == RobTag)return true;
            else{
                auto RobEntry = this->m_Rob[RobPtr];
                if( !RobEntry.isStable && (RobEntry.Fu == funcType_t::LDU || 
                    RobEntry.Fu == funcType_t::STU || RobEntry.Fu == funcType_t::BRU))
                {
                    return false;
                }
                else {
                    RobPtr = this->m_Rob.getNextHeader();//get rob next head ptr
                    if(RobPtr == RobTag)return true;
                    else return false;
                }
            }
        }
    }else{
        if(this->m_RobState == rob_state_t::Rob_WaitForResume && this->m_RollBackTag == RobTag){//上一周期的rob中进行了回滚操作
            return true;
        }
    }
    return false;
}

void Rcu::WriteBack(int index,InsnPtr_t& insn, bool& needRedirect,Redirect_t& RedirectReq){
    needRedirect = false;
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
                        RedirectReq.StageId = InsnState_t::State_Issue;
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
                this->m_IntRegfile[insn->PhyRd] = insn->RdResult;
                this->m_IntBusylist[insn->PhyRd].done = true;
                this->m_IntBusylist[insn->PhyRd].forwarding = false;
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
    bool Rob_Release_EN[4]={false,false,false,false},Resource_Release_EN[4]={false,false,false,false};
    uint64_t Resource_Release_Tag[4];
    uint16_t RobPtr = this->m_Rob.getLastest();//获取ROB尾指针
    for(size_t i = 0 ; i < this->m_AllocWidth && 
        this->isOlder(this->m_RollBackTag,RobPtr,this->m_Rob.getHeader()) || 
        RobPtr == this->m_RollBackTag;i++)//如果当前的robptr比回滚标记更旧，或者robptr恰好等于回滚标记，那么就需要进行回滚处理。
    {
        Rob_entry_t entry = this->m_Rob[RobPtr];
        if(entry.valid){
            if(RobPtr == this->m_RollBackTag){//如果当前robptr恰好就是要回滚的那个
                this->m_RobState = rob_state_t::Rob_WaitForResume;//设置状态为等待恢复
                if(entry.isExcp){//如果存在异常，则释放资源（物理寄存器，lsq，更新renametable）
                    
                    Resource_Release_EN[i]=true;
                    Resource_Release_Tag[i]=RobPtr;
                }
                DPRINTF(RollBack,"RobTag[{}],Pc[{:#x}] -> RollBack Finish, Wait For Resume",RobPtr,entry.pc);
                break;
            }
           else{
             entry.valid = false;//将条目设置为无效，表示没有在使用了
             
             Resource_Release_EN[i]=true;
             Resource_Release_Tag[i]=RobPtr;
           }
            DPRINTF(RollBack,"RobTag[{}],Pc[{:#x}]",RobPtr,entry.pc);
        }
        this->m_Rob.RollBack();//更新rob尾指针
        RobPtr = this->m_Rob.getLastest();
    }
    for(int i=0;i<4;i++){
        if(Rob_Release_EN[i]){
            this->ReleaseResource(Resource_Release_Tag[i]);
            if(this->m_Rob[Resource_Release_Tag[i]].Fu == funcType_t::LDU){
                this->m_Processor->getLsqPtr()->KillLoadEntry_flag[i]=true;
                this->m_Processor->getLsqPtr()->KillLoadEntry_Tag[i]=(this->m_Rob[Resource_Release_Tag[i]].LSQtag);
            }else if(this->m_Rob[Resource_Release_Tag[i]].Fu == funcType_t::STU){
                this->m_Processor->getLsqPtr()->KillStoreEntry_flag[i]=true;
                this->m_Processor->getLsqPtr()->KillStoreEntry_Tag[i]=(this->m_Rob[Resource_Release_Tag[i]].LSQtag);
            }
            if(this->m_Rob[Resource_Release_Tag[i]].phyRd != 0){
                this->RN_Release_EN[i]=true;
                this->RN_Release_IsaRd[i]=this->m_Rob[Resource_Release_Tag[i]].isaRd;
                this->RN_Release_Data[i]=this->m_Rob[Resource_Release_Tag[i]].LphyRd;
                this->FE_Release_phyRd[i]=this->m_Rob[Resource_Release_Tag[i]].phyRd;
            }
            else{
                this->RN_Release_EN[i]=false;
            }
        }
    }
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
                this->m_IntFreelist.push(this->FE_Release_phyRd[i]);
            }
            if(this->FE_Commit_EN[i]){
                this->m_IntFreelist.push(this->FE_Commit_LphyRd[i]);
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
void 
Rcu::Evaulate(){
    
    this->CreateRobEntry(this->rob_insnPkg,this->ROB_Entry_WEN);
    this->rob_insnPkg.clear();

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
    this->ROB_AGU_EN=false;
    
   
    if(this->m_RobState == rob_state_t::Rob_Undo){
        this->RollBack();
    }
    if(this->m_RobState == rob_state_t::Rob_FlushBackend){
        this->m_RobState = rob_state_t::Rob_Idle;
    }
    this->Freelist_Evaluate(true);
}


void Rcu::CommitInsn(InsnPkg_t& insnPkg, Redirect_t& redirectReq, bool& needRedirect){
    needRedirect = false;
    for(int i=0;i<4;i++){
        this->FE_Commit_EN[i]=false;
        this->FE_Commit_LphyRd[i]=0;
    }
    for(size_t i = 0; i < insnPkg.size(); i++){
        uint16_t robPtr = this->m_Rob.getHeader();
        auto& robEntry = this->m_Rob[robPtr];
        if(robEntry.valid){
            DASSERT(robEntry.done,"Commit Insn When not Ready!")
            if(!robEntry.isExcp){
                if(robEntry.LphyRd != 0){
                    this->m_Processor->m_ExecContext->WriteIntReg(robEntry.isaRd,this->m_IntRegfile[robEntry.phyRd]);
                    //this->m_IntFreelist.push(robEntry.LphyRd);
                    this->FE_Commit_EN[i]=true;
                    this->FE_Commit_LphyRd[i]=robEntry.LphyRd;
                    this->m_IntBusylist[robEntry.LphyRd].done = false;
                    this->m_IntBusylist[robEntry.LphyRd].forwarding = false;
                    this->m_IntBusylist[robEntry.LphyRd].allocated = false;
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Deallocate Last PRd[{}]",robPtr,robEntry.pc,robEntry.LphyRd);
                }
                if(robEntry.Fu == funcType_t::LDU){
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit a Load LSQTag[{}]", 
                    robPtr,robEntry.pc,robEntry.LSQtag);
                    this->m_Processor->getLsqPtr()->CommitLoadEntry(robEntry.LSQtag);
                    
                }else if(robEntry.Fu == funcType_t::STU){
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit a Store LSQTag[{}]: Address[{:#x}] Data[{:#x}]",
                        robPtr,robEntry.pc,robEntry.LSQtag);
                    this->m_Processor->getLsqPtr()->CommitStoreEntry(robEntry.LSQtag);
                }
            }
            //首先检查乱序处理器的状态是否为等待恢复状态，并且当前处理的指令是否为最近的回滚指令。
            //如果条件成立，说明当前正在处理的指令是最近一次发生异常需要回滚的指令。
            if(this->m_RobState == rob_state_t::Rob_WaitForResume && robPtr == this->m_RollBackTag){
                this->m_RobState = rob_state_t::Rob_FlushBackend;
                this->m_Processor->FlushForward(InsnState_t::State_Dispatch);//将issue_flush,commit_flush置1
                if(robEntry.isExcp){
                    needRedirect = true;
                    redirectReq.StageId = InsnState_t::State_Commit;
                    this->m_Processor->m_ExecContext->WriteCsr(CSR_MEPC,robEntry.pc);
                    this->m_Processor->m_ExecContext->WriteCsr(CSR_MCAUSE,this->m_ExcpCause);
                    this->m_Processor->m_ExecContext->WriteCsr(CSR_MTVAL,this->m_ExcpTval);
                    this->m_Processor->m_ExecContext->ReadCsr(CSR_MTVEC,redirectReq.target);
                    this->m_Processor->FlushBackWard(InsnState_t::State_Issue);//将fetch_flush,decode_flushdispatch_flush都置1
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit an Exception, Redirect to {:#x}!",robPtr,robEntry.pc,redirectReq.target);
                }
                DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Resume Execution!",robPtr,robEntry.pc);
            }
            this->m_Processor->m_ExecContext->InstretInc();//处理指令计数+1
        }
        this->m_Rob.Pop();   //弹出处理完毕的指令                   
    }

}
// //RAW
// //insn1 
// insnPkg[0]->PhyRs1 = this->m_IntRenameTable[insnPkg[0]->IsaRs1];
// insnPkg[0]->PhyRs2 = this->m_IntRenameTable[insnPkg[0]->IsaRs2];

// //insn2
// if(insnPkg[1]->IsaRs1==insnPkg[0]->IsaRd) insnPkg[1]->PhyRs1=insnPkg[0]->PhyRd;
// else insnPkg[1]->PhyRs1=this->m_IntRenameTable[insnPkg[1]->IsaRs1];
// if(insnPkg[1]->IsaRs2==insnPkg[0]->IsaRd) insnPkg[1]->PhyRs2=insnPkg[0]->PhyRd;
// else insnPkg[1]->PhyRs2=this->m_IntRenameTable[insnPkg[1]->IsaRs2];
// //insn3
// if(insnPkg[2]->IsaRs1==insnPkg[1]->IsaRd) insnPkg[2]->PhyRs1=insnPkg[1]->PhyRd;
// else if(insnPkg[2]->IsaRs1==insnPkg[0]->IsaRd) insnPkg[2]->PhyRs1=insnPkg[0]->PhyRd;
// else insnPkg[2]->PhyRs1=this->m_IntRenameTable[insnPkg[2]->IsaRs1];

// if(insnPkg[2]->IsaRs2==insnPkg[1]->IsaRd) insnPkg[2]->PhyRs2=insnPkg[1]->PhyRd;
// else if(insnPkg[2]->IsaRs2==insnPkg[0]->IsaRd) insnPkg[2]->PhyRs2=insnPkg[0]->PhyRd;
// else insnPkg[2]->PhyRs2=this->m_IntRenameTable[insnPkg[2]->IsaRs2];
// //insn4
// if(insnPkg[3]->IsaRs1==insnPkg[2]->IsaRd) insnPkg[3]->PhyRs1=insnPkg[2]->PhyRd;
// else if(insnPkg[3]->IsaRs1==insnPkg[1]->IsaRd) insnPkg[3]->PhyRs1=insnPkg[1]->PhyRd;
// else if(insnPkg[3]->IsaRs1==insnPkg[0]->IsaRd) insnPkg[3]->PhyRs1=insnPkg[0]->PhyRd;
// else insnPkg[3]->PhyRs1=this->m_IntRenameTable[insnPkg[3]->IsaRs1];

// if(insnPkg[3]->IsaRs2==insnPkg[2]->IsaRd) insnPkg[3]->PhyRs2=insnPkg[2]->PhyRd;
// else if(insnPkg[3]->IsaRs2==insnPkg[1]->IsaRd) insnPkg[3]->PhyRs2=insnPkg[1]->PhyRd;
// else if(insnPkg[3]->IsaRs2==insnPkg[0]->IsaRd) insnPkg[3]->PhyRs2=insnPkg[0]->PhyRd;
// else insnPkg[3]->PhyRs2=this->m_IntRenameTable[insnPkg[3]->IsaRs2];
// //WAW-对ROB
// //insn1
// insnPkg[0]->LPhyRd = this->m_IntRenameTable[insnPkg[0]->IsaRd];
// //insn2
// if(insnPkg[1]->IsaRd==insnPkg[0]->IsaRd) insnPkg[1]->LPhyRd=insnPkg[0]->PhyRd;
// else insnPkg[1]->LPhyRd=this->m_IntRenameTable[insnPkg[1]->IsaRd];
// //insn3
// if(insnPkg[2]->IsaRd==insnPkg[1]->IsaRd) insnPkg[2]->LPhyRd=insnPkg[1]->PhyRd;
// else if(insnPkg[2]->IsaRd==insnPkg[0]->IsaRd) insnPkg[2]->LPhyRd=insnPkg[0]->PhyRd;
// else insnPkg[2]->LPhyRd=this->m_IntRenameTable[insnPkg[2]->IsaRd];
// //insn4
// if(insnPkg[3]->IsaRd==insnPkg[2]->IsaRd) insnPkg[3]->LPhyRd=insnPkg[2]->PhyRd;
// else if(insnPkg[3]->IsaRd==insnPkg[1]->IsaRd) insnPkg[3]->LPhyRd=insnPkg[1]->PhyRd;
// else if(insnPkg[3]->IsaRd==insnPkg[0]->IsaRd) insnPkg[3]->LPhyRd=insnPkg[0]->PhyRd;
// else insnPkg[3]->LPhyRd=this->m_IntRenameTable[insnPkg[3]->IsaRd];

// void Rcu::CreateRobEntry(InsnPkg_t& insnPkg, uint64_t allocCount){
//         VCreateRobEntry *CreateRobEntry;
//     CreateRobEntry=new VCreateRobEntry;//创建对象
//     CreateRobEntry->io_Rob_Tail=0;
//     CreateRobEntry->io_Rob_NextTail=0;
//     //连接输入
//      if(allocCount==1){
//         CreateRobEntry->io_Rob_Tail= this->m_Rob.Allocate();
//     }
//     else if(allocCount==2){
//         CreateRobEntry->io_Rob_Tail= this->m_Rob.Allocate();
//         CreateRobEntry->io_Rob_NextTail= this->m_Rob.Allocate();
//     }
//     CreateRobEntry->io_allocCount=allocCount;
//     CreateRobEntry->io_insn1_Function_type=insnPkg[0]->Fu;
//     CreateRobEntry->io_insn1_Sub_OP=insnPkg[0]->SubOp;
//     CreateRobEntry->io_insn1_excp_vaild=insnPkg[0]->Excp.valid;
//     CreateRobEntry->io_insn1_excp_cause=insnPkg[0]->Excp.Cause;
//     CreateRobEntry->io_insn1_excpt_tval=insnPkg[0]->Excp.Tval;
//     CreateRobEntry->io_insn1_Pc=insnPkg[0]->Pc;
//     CreateRobEntry->io_insn1_LSQTag=insnPkg[0]->LSQTag;
//     CreateRobEntry->io_insn1_IsaRd=insnPkg[0]->IsaRd;
//     CreateRobEntry->io_insn1_PhyRd=insnPkg[0]->PhyRd;
//     CreateRobEntry->io_insn1_LPhyRd=insnPkg[0]->LPhyRd;
//     CreateRobEntry->io_insn1_ControlFlowInsn=insnPkg[0]->ControlFlowInsn;
//    
//    
//     CreateRobEntry->io_insn2_Function_type=insnPkg[1]->Fu;
//     CreateRobEntry->io_insn2_Sub_OP=insnPkg[1]->SubOp;
//     CreateRobEntry->io_insn2_excp_vaild=insnPkg[1]->Excp.valid;
//     CreateRobEntry->io_insn2_excp_cause=insnPkg[1]->Excp.Cause;
//     CreateRobEntry->io_insn2_excpt_tval=insnPkg[1]->Excp.Tval;
//     CreateRobEntry->io_insn2_Pc=insnPkg[1]->Pc;
//     CreateRobEntry->io_insn2_LSQTag=insnPkg[1]->LSQTag;
//     CreateRobEntry->io_insn2_IsaRd=insnPkg[1]->IsaRd;
//     CreateRobEntry->io_insn2_PhyRd=insnPkg[1]->PhyRd;
//     CreateRobEntry->io_insn2_LPhyRd=insnPkg[1]->LPhyRd;
//     CreateRobEntry->io_insn2_ControlFlowInsn=insnPkg[1]->ControlFlowInsn;
//    
//    
//     CreateRobEntry->io_m_RobState=this->m_RobState;
//     CreateRobEntry->io_m_RollBackTag=this->m_RollBackTag;
//     CreateRobEntry->io_m_ExcpCause=this->m_ExcpCause;
//     CreateRobEntry->io_m_ExcpTval=this->m_ExcpTval;    
//     CreateRobEntry->io_m_ExcpTval=this->m_ExcpTval;    
//     //连接输出
//     CreateRobEntry->eval();
//
//
//     Rob_entry_t newEntry1;
//     newEntry1.valid=CreateRobEntry->io_insn1_Entry_valid;
//     newEntry1.done=CreateRobEntry->io_insn1_Entry_done;
//     newEntry1.isStable=CreateRobEntry->io_insn1_Entry_isStable;
//     newEntry1.isMisPred=CreateRobEntry->io_insn1_Entry_isMisPred;
//     newEntry1.isExcp=CreateRobEntry->io_insn1_Entry_isExcp;
//     newEntry1.pc=CreateRobEntry->io_insn1_Entry_pc;
//     newEntry1.Fu=funcType_t(CreateRobEntry->io_insn1_Entry_Fu);
//     newEntry1.LSQtag=CreateRobEntry->io_insn1_Entry_LSQtag;
//     newEntry1.isaRd=CreateRobEntry->io_insn1_Entry_isaRd;
//     newEntry1.phyRd=CreateRobEntry->io_insn1_Entry_phyRd;
//     newEntry1.LphyRd=CreateRobEntry->io_insn1_Entry_LphyRd;
//     if(allocCount>0)insnPkg[0]->RobTag=CreateRobEntry->io_insn1_RobTag;
//    
//    
//     Rob_entry_t newEntry2;
//     newEntry2.valid=CreateRobEntry->io_insn2_Entry_valid;
//     newEntry2.done=CreateRobEntry->io_insn2_Entry_done;
//     newEntry2.isStable=CreateRobEntry->io_insn2_Entry_isStable;
//     newEntry2.isMisPred=CreateRobEntry->io_insn2_Entry_isMisPred;
//     newEntry2.isExcp=CreateRobEntry->io_insn2_Entry_isExcp;
//     newEntry2.pc=CreateRobEntry->io_insn2_Entry_pc;
//     newEntry2.Fu=funcType_t(CreateRobEntry->io_insn2_Entry_Fu);
//     newEntry2.LSQtag=CreateRobEntry->io_insn2_Entry_LSQtag;
//     newEntry2.isaRd=CreateRobEntry->io_insn2_Entry_isaRd;
//     newEntry2.phyRd=CreateRobEntry->io_insn2_Entry_phyRd;
//     newEntry2.LphyRd=CreateRobEntry->io_insn2_Entry_LphyRd;
//     if(allocCount==2)insnPkg[1]->RobTag=CreateRobEntry->io_insn2_RobTag;
//     this->m_RobState=rob_state_t(CreateRobEntry->io_m_RobState_new);
//     this->m_RollBackTag=CreateRobEntry->io_m_RollBackTag_new;
//     this->m_ExcpCause=CreateRobEntry->io_m_ExcpCause_new;
//     this->m_ExcpTval=CreateRobEntry->io_m_ExcpTval_new; 
//     //模拟mRob收到有效信号后将数据写入
//     if(CreateRobEntry->io_ROB_EN1){
//         this->m_Rob[CreateRobEntry->io_insn1_RobTag] = newEntry1;
//         DPRINTF(temptest,"{:#x}:{:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x}  ", 
//             CreateRobEntry->io_insn1_RobTag,this->m_Rob[CreateRobEntry->io_insn1_RobTag].valid,this->m_Rob[CreateRobEntry->io_insn1_RobTag].done,this->m_Rob[CreateRobEntry->io_insn1_RobTag].Fu,
//             this->m_Rob[CreateRobEntry->io_insn1_RobTag].isaRd,this->m_Rob[CreateRobEntry->io_insn1_RobTag].isExcp,this->m_Rob[CreateRobEntry->io_insn1_RobTag].isMisPred,
//             this->m_Rob[CreateRobEntry->io_insn1_RobTag].isStable,this->m_Rob[CreateRobEntry->io_insn1_RobTag].LphyRd,this->m_Rob[CreateRobEntry->io_insn1_RobTag].LSQtag,
//             this->m_Rob[CreateRobEntry->io_insn1_RobTag].pc,this->m_Rob[CreateRobEntry->io_insn1_RobTag].phyRd,this->m_Rob[CreateRobEntry->io_insn1_RobTag].valid);        
//     }
//     if(CreateRobEntry->io_ROB_EN2){
//         this->m_Rob[CreateRobEntry->io_insn2_RobTag] = newEntry2;
//         DPRINTF(temptest,"{:#x}:{:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x}  ", 
//             CreateRobEntry->io_insn2_RobTag,this->m_Rob[CreateRobEntry->io_insn2_RobTag].valid,this->m_Rob[CreateRobEntry->io_insn2_RobTag].done,this->m_Rob[CreateRobEntry->io_insn2_RobTag].Fu,
//             this->m_Rob[CreateRobEntry->io_insn2_RobTag].isaRd,this->m_Rob[CreateRobEntry->io_insn2_RobTag].isExcp,this->m_Rob[CreateRobEntry->io_insn2_RobTag].isMisPred,
//             this->m_Rob[CreateRobEntry->io_insn2_RobTag].isStable,this->m_Rob[CreateRobEntry->io_insn2_RobTag].LphyRd,this->m_Rob[CreateRobEntry->io_insn2_RobTag].LSQtag,
//             this->m_Rob[CreateRobEntry->io_insn2_RobTag].pc,this->m_Rob[CreateRobEntry->io_insn2_RobTag].phyRd,this->m_Rob[CreateRobEntry->io_insn2_RobTag].valid);
//      } 
//      } 
//     delete CreateRobEntry;
// }


// void
// Rcu::Allocate(InsnPkg_t& insnPkg, uint64_t allocCount){
//     //连接输入
//     RcuAllocate->io_allocCount=allocCount;
//     RcuAllocate->io_EN_Update=this->m_RN_EN_Update;
//     RcuAllocate->io_IsaRd_Update=this->m_RN_IsaRd_Update;
//     RcuAllocate->io_Data_Update=this->m_RN_Data_Update;
//     RcuAllocate->io_IntFreelist_phyrd1=this->m_IntFreelist.front();
//     RcuAllocate->io_IntFreelist_phyrd2=this->m_IntFreelist.next_front();
//     RcuAllocate->io_insn1_IsaRs1=insnPkg[0]->IsaRs1;
//     RcuAllocate->io_insn1_IsaRs2=insnPkg[0]->IsaRs2;
//     RcuAllocate->io_insn1_IsaRd=insnPkg[0]->IsaRd;
//     RcuAllocate->io_insn2_IsaRs1=insnPkg[1]->IsaRs1;
//     RcuAllocate->io_insn2_IsaRs2=insnPkg[1]->IsaRs2;
//     RcuAllocate->io_insn2_IsaRd=insnPkg[1]->IsaRd;
//     //模拟时钟上升沿
//     RcuAllocate->clock=0;
//     RcuAllocate->eval();
//    
//     //连接输出
//     if(allocCount>0){
//         insnPkg[0]->PhyRs1=RcuAllocate->io_insn1_PhyRs1;
//         insnPkg[0]->PhyRs2=RcuAllocate->io_insn1_PhyRs2;
//         insnPkg[0]->PhyRd=RcuAllocate->io_insn1_PhyRd;
//         insnPkg[0]->LPhyRd=RcuAllocate->io_insn1_LPhyRd;
//     }
//     if(allocCount==2){
//         insnPkg[1]->PhyRs1=RcuAllocate->io_insn2_PhyRs1;
//         insnPkg[1]->PhyRs2=RcuAllocate->io_insn2_PhyRs2;
//         insnPkg[1]->PhyRd=RcuAllocate->io_insn2_PhyRd;
//         insnPkg[1]->LPhyRd=RcuAllocate->io_insn2_LPhyRd;
//     }
//
//     if(RcuAllocate->io_WEN1_IntBusylist){
//         this->m_IntBusylist[insnPkg[0]->PhyRd].allocated = true;
//         this->m_IntBusylist[insnPkg[0]->PhyRd].done      = false;
//         this->m_IntBusylist[insnPkg[0]->PhyRd].forwarding = false;
//     }
//     if(RcuAllocate->io_WEN2_IntBusylist){
//         this->m_IntBusylist[insnPkg[1]->PhyRd].allocated = true;
//         this->m_IntBusylist[insnPkg[1]->PhyRd].done      = false;
//         this->m_IntBusylist[insnPkg[1]->PhyRd].forwarding = false;
//     }
//     if(RcuAllocate->io_Freelist_pop_num==1){
//         this->m_IntFreelist.pop_front();
//     }else if(RcuAllocate->io_Freelist_pop_num==2){
//         this->m_IntFreelist.pop_front();
//         this->m_IntFreelist.pop_front();
//     }
//     // for(size_t i = 0; i < allocCount; i++){
//     //     InsnPtr_t insn = insnPkg[i];
//     //     insn->PhyRs1 = this->m_IntRenameTable[insn->IsaRs1];
//     //     insn->PhyRs2 = this->m_IntRenameTable[insn->IsaRs2];
//     //     insn->LPhyRd = this->m_IntRenameTable[insn->IsaRd];
//     //     if(insn->IsaRd != 0){
//     //         this->m_IntFreelist.pop();//取出空闲的reg id
//     //         this->m_IntBusylist[insn->PhyRd].allocated = true;//根据reg id 将busylist中对应的entry进行更新
//     //         this->m_IntBusylist[insn->PhyRd].done      = false;
//     //         this->m_IntBusylist[insn->PhyRd].forwarding = false;
//     //         this->m_IntRenameTable[insn->IsaRd]        = insn->PhyRd;//保存rd对应的reg id
//     //     }
//     // }
//     // if(allocCount==2){//处理指令2与指令1之间可能存在的依赖
//     //         InsnPtr_t& insn1 = insnPkg[0];
//     //         InsnPtr_t& Insn2 = insnPkg[1];
//     //         if(insn1->IsaRd != 0){
//     //             if(insn1->IsaRd == Insn2->IsaRs1)Insn2->PhyRs1 = insn1->PhyRd;
//     //             if(insn1->IsaRd == Insn2->IsaRs2)Insn2->PhyRs2 = insn1->PhyRd;
//     //             if(insn1->IsaRd == Insn2->IsaRd) Insn2->LPhyRd = insn1->PhyRd;
//     //         }
//     // }
//     // if(allocCount==1)
//     // {
//     //     if(insnPkg[0]->PhyRs1!=RcuAllocate->io_insn1_PhyRs1||insnPkg[0]->PhyRs2!=RcuAllocate->io_insn1_PhyRs2||
//     //         insnPkg[0]->PhyRd!=RcuAllocate->io_insn1_PhyRd||insnPkg[0]->LPhyRd!=RcuAllocate->io_insn1_LPhyRd)
//     //     {   
//     //         DPRINTF(temptest,"Input:I1 {:} {:} {:} {:} {:}",
//     //         RcuAllocate->io_EN_Update,RcuAllocate->io_IsaRd_Update=this->m_RN_IsaRd_Update,RcuAllocate->io_Data_Update,
//     //         RcuAllocate->io_IntFreelist_phyrd1,RcuAllocate->io_IntFreelist_phyrd2);
//     //         DPRINTF(temptest,"Input:I1 {:} {:} {:} | {:} {:} {:} I2 {:} {:} {:} | {:} {:} {:}",
//     //         insnPkg[0]->IsaRs1,insnPkg[0]->IsaRs2,insnPkg[0]->IsaRd,
//     //         RcuAllocate->io_insn1_IsaRs1,RcuAllocate->io_insn1_IsaRs2,RcuAllocate->io_insn1_IsaRd,
//     //         insnPkg[1]->IsaRs1,insnPkg[1]->IsaRs2,insnPkg[1]->IsaRd,
//     //         RcuAllocate->io_insn2_IsaRs1,RcuAllocate->io_insn2_IsaRs2,RcuAllocate->io_insn2_IsaRd);
//     //         DPRINTF(temptest,"V:n {:} I1 {:} {:} {:} {:} I2 {:} {:} {:} {:}",
//     //         allocCount,RcuAllocate->io_insn1_PhyRs1,RcuAllocate->io_insn1_PhyRs2,RcuAllocate->io_insn1_PhyRd,
//     //         RcuAllocate->io_insn1_LPhyRd,RcuAllocate->io_insn2_PhyRs1,RcuAllocate->io_insn2_PhyRs2,
//     //         RcuAllocate->io_insn2_PhyRd,RcuAllocate->io_insn2_LPhyRd);
//     //         DPRINTF(temptest,"O:n {:} I1 {:} {:} {:} {:} I2 {:} {:} {:} {:}",
//     //         allocCount,insnPkg[0]->PhyRs1,insnPkg[0]->PhyRs2,insnPkg[0]->PhyRd,insnPkg[0]->LPhyRd,
//     //         insnPkg[1]->PhyRs1,insnPkg[1]->PhyRs2,insnPkg[1]->PhyRd,insnPkg[1]->LPhyRd);
//     //         exit(1);
//     //     }
//     // }
//     // if(allocCount==2)
//     // {
//     //     if(insnPkg[0]->PhyRs1!=RcuAllocate->io_insn1_PhyRs1||insnPkg[0]->PhyRs2!=RcuAllocate->io_insn1_PhyRs2||
//     //         insnPkg[0]->PhyRd!=RcuAllocate->io_insn1_PhyRd||insnPkg[0]->LPhyRd!=RcuAllocate->io_insn1_LPhyRd||
//     //         insnPkg[1]->PhyRs1!=RcuAllocate->io_insn2_PhyRs1||insnPkg[1]->PhyRs2!=RcuAllocate->io_insn2_PhyRs2||
//     //         insnPkg[1]->PhyRd!=RcuAllocate->io_insn2_PhyRd||insnPkg[1]->LPhyRd!=RcuAllocate->io_insn2_LPhyRd)
//     //     {   
//     //         DPRINTF(temptest,"Input:I1 {:} {:} {:} {:} {:}",
//     //         RcuAllocate->io_EN_Update,RcuAllocate->io_IsaRd_Update=this->m_RN_IsaRd_Update,RcuAllocate->io_Data_Update,
//     //         RcuAllocate->io_IntFreelist_phyrd1,RcuAllocate->io_IntFreelist_phyrd2);
//     //         DPRINTF(temptest,"Input:I1 {:} {:} {:} | {:} {:} {:} I2 {:} {:} {:} | {:} {:} {:}",
//     //         insnPkg[0]->IsaRs1,insnPkg[0]->IsaRs2,insnPkg[0]->IsaRd,
//     //         RcuAllocate->io_insn1_IsaRs1,RcuAllocate->io_insn1_IsaRs2,RcuAllocate->io_insn1_IsaRd,
//     //         insnPkg[1]->IsaRs1,insnPkg[1]->IsaRs2,insnPkg[1]->IsaRd,
//     //         RcuAllocate->io_insn2_IsaRs1,RcuAllocate->io_insn2_IsaRs2,RcuAllocate->io_insn2_IsaRd);
//     //         DPRINTF(temptest,"V:n {:} I1 {:} {:} {:} {:} I2 {:} {:} {:} {:}",
//     //         allocCount,RcuAllocate->io_insn1_PhyRs1,RcuAllocate->io_insn1_PhyRs2,RcuAllocate->io_insn1_PhyRd,
//     //         RcuAllocate->io_insn1_LPhyRd,RcuAllocate->io_insn2_PhyRs1,RcuAllocate->io_insn2_PhyRs2,
//     //         RcuAllocate->io_insn2_PhyRd,RcuAllocate->io_insn2_LPhyRd);
//     //         DPRINTF(temptest,"O:n {:} I1 {:} {:} {:} {:} I2 {:} {:} {:} {:}",
//     //         allocCount,insnPkg[0]->PhyRs1,insnPkg[0]->PhyRs2,insnPkg[0]->PhyRd,insnPkg[0]->LPhyRd,
//     //         insnPkg[1]->PhyRs1,insnPkg[1]->PhyRs2,insnPkg[1]->PhyRd,insnPkg[1]->LPhyRd);
//     //         exit(1);
//     //     }
//     // }    
//     RcuAllocate->clock=1;
//     RcuAllocate->eval();
//     this->m_RN_EN_Update=0;//wait next updata  
//     this->CreateRobEntry(insnPkg,allocCount);//发送创建rob请求   
// }

// bool Rcu::ReadyForCommit(uint64_t RobTag){
//     VReadyForCommit *ReadyForCommit;
//     ReadyForCommit=new VReadyForCommit;//创建对象
//     //连接输入
//     ReadyForCommit->io_Rob_Tag=RobTag;
//     ReadyForCommit->io_Rob_Header=this->m_Rob.getHeader();
//     ReadyForCommit->io_Rob_NextHeader=this->m_Rob.getNextHeader();
//     ReadyForCommit->io_m_RobState=this->m_RobState;
//     ReadyForCommit->io_m_RollBackTag=this->m_RollBackTag;
//     ReadyForCommit->io_Rob_Usage=this->m_Rob.getUsage();
//     ReadyForCommit->io_Header_isStable=this->m_Rob[this->m_Rob.getHeader()].isStable;
//     ReadyForCommit->io_Header_Function_type=this->m_Rob[this->m_Rob.getHeader()].Fu;    
//     //连接输出
//     ReadyForCommit->eval();
//     return ReadyForCommit->io_Ready;
//     // DPRINTF(temptest,"Verilog: I1 {:#x} I2 {:#x} I3 {:#x} I4 {:#x} I5 {:#x} I6 {:#x} I7 {:#x} I8 {:#x} Out {:#x}",
//     //             ReadyForCommit->io_Rob_Tag,ReadyForCommit->io_Rob_Header,ReadyForCommit->io_Rob_NextHeader,ReadyForCommit->io_m_RobState,
//     //             ReadyForCommit->io_m_RollBackTag,ReadyForCommit->io_Rob_Usage,ReadyForCommit->io_Header_isStable,ReadyForCommit->io_Header_Function_type,a); 
// }


// void 
// Rcu::ResovleDependancy(InsnPkg_t& insnPkg){//解决依赖的解释见daily learning
//     for(size_t i = 0 ; i < insnPkg.size(); i++){
//         InsnPtr_t& insn = insnPkg[i];
//         if(insn->IsaRd != 0){
//             for(size_t j = i + 1; j < insnPkg.size(); j++){//如果后面的指令的rs1或rs2等于当前指令的rd，且数据类型一致
//                 InsnPtr_t& laterInsn = insnPkg[j];         //那么需要将后续指令的rs1或rs2也替换为当前rd映射的物理寄存器
//                 if(laterInsn){
//                     if(insn->IsaRd == laterInsn->IsaRs1){
//                         laterInsn->PhyRs1 = insn->PhyRd;
//                     }
//                     if(insn->IsaRd == laterInsn->IsaRs2){
//                         laterInsn->PhyRs2 = insn->PhyRd;
//                     } 
//                     if(insn->IsaRd == laterInsn->IsaRd){
//                         laterInsn->LPhyRd = insn->PhyRd;
//                     } 
//                 }
//             }
//         }
//     }
// }

// void 
// Rcu::TryAllocate(InsnPkg_t& insnPkg, uint64_t& SuccessCount){
    
//     SuccessCount = 0;
//     uint64_t allocRobCount = 0;
//     uint64_t allocRegCount = 0;
//     for(auto insn : insnPkg){
//         if(insn){
//             if(this->m_Rob.getAvailEntryCount() > allocRobCount){
//                 allocRobCount++;
//             }else{
//                 break;
//             }
//             if(insn->IsaRd != 0){
//                 if((this->m_IntFreelist.getAvailEntryCount() > allocRegCount)){
//                     allocRegCount++;
//                 }else{
//                     break;
//                 }
//             }
//         }
//         SuccessCount++;
//     }
// };

// void 
// Rcu::Rename(InsnPkg_t& insnPkg){
//     for(auto& insn : insnPkg){
//         insn->PhyRs1 = this->m_IntRenameTable[insn->IsaRs1];
//         insn->PhyRs2 = this->m_IntRenameTable[insn->IsaRs2];
//         insn->LPhyRd = this->m_IntRenameTable[insn->IsaRd];
//     }
// }
// void 
// Rcu::Forwarding(InsnPtr_t& insn){
//     this->m_IntRegfile[insn->PhyRd] = insn->RdResult;
//     this->m_IntBusylist[insn->PhyRd].done = true;
// }

} // namespace Emulator
