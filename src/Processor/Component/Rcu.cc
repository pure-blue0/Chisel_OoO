#include "Rcu.hh"
#include "../Processor.hh"
#include "../../RISCV/encoding.out.h"

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
{}

Rcu::~Rcu(){}

void
Rcu::Reset(){
    /* Reset Rob */
    this->m_Rob.Reset();
    this->m_RobState = rob_state_t::Rob_Idle;
    this->m_RollBackTag = 0;
    /* Reset Regfile */
    this->m_IntRegfile.Reset();
    /* Reset RenameTable */
    this->m_IntRenameTable.Reset(this->m_RenameRegister);
    /* Reset Busylist */
    this->m_IntBusylist.Reset(this->m_RenameRegister);
    /* Reset FreeList */
    this->m_IntFreelist.Reset();
}


void 
Rcu::CreateRobEntry(InsnPkg_t& insnPkg, uint64_t allocCount){
    for(size_t i = 0; i < allocCount; i++){
        InsnPtr_t insn = insnPkg[i];
        Rob_entry_t newEntry;
        insn->RobTag = this->m_Rob.Allocate();//返回一个可用的rob entry指针
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
    }   
}
 
void
Rcu::Allocate(InsnPkg_t& insnPkg, uint64_t allocCount){
    for(size_t i = 0; i < allocCount; i++){
        InsnPtr_t insn = insnPkg[i];
        insn->PhyRs1 = this->m_IntRenameTable[insn->IsaRs1];
        insn->PhyRs2 = this->m_IntRenameTable[insn->IsaRs2];
        insn->LPhyRd = this->m_IntRenameTable[insn->IsaRd];
        if(insn->IsaRd != 0){
            insn->PhyRd = this->m_IntFreelist.pop();//取出空闲的reg id
            this->m_IntBusylist[insn->PhyRd].allocated = true;//根据reg id 将busylist中对应的entry进行更新
            this->m_IntBusylist[insn->PhyRd].done      = false;
            this->m_IntBusylist[insn->PhyRd].forwarding = false;
            this->m_IntRenameTable[insn->IsaRd]        = insn->PhyRd;//保存rd对应的reg id
        }
    }
    if(allocCount==2){//处理指令2与指令1之间可能存在的依赖
            InsnPtr_t& insn1 = insnPkg[0];
            InsnPtr_t& Insn2 = insnPkg[1];
            if(insn1->IsaRd != 0){
                if(insn1->IsaRd == Insn2->IsaRs1)Insn2->PhyRs1 = insn1->PhyRd;
                if(insn1->IsaRd == Insn2->IsaRs2)Insn2->PhyRs2 = insn1->PhyRd;
                if(insn1->IsaRd == Insn2->IsaRd) Insn2->LPhyRd = insn1->PhyRd;
            }
    }
    
    this->CreateRobEntry(insnPkg,allocCount);//发送创建rob请求
    
}


void 
Rcu::TryAllocate(InsnPkg_t& insnPkg, uint64_t& SuccessCount){ 
    SuccessCount = this->m_Rob.getAvailEntryCount()<this->m_IntFreelist.getAvailEntryCount()?this->m_Rob.getAvailEntryCount():this->m_IntFreelist.getAvailEntryCount();
    SuccessCount =insnPkg.size()<SuccessCount?insnPkg.size():SuccessCount; 
};

bool Rcu::ReadyForCommit(uint64_t RobTag){
    uint64_t RobPtr = this->m_Rob.getHeader();
    if(this->m_RobState == rob_state_t::Rob_Idle ||  this->m_Rob.isOlder(RobTag,this->m_RollBackTag)){
        for(size_t i = 0; i < this->m_DeallocWidth && i < this->m_Rob.getUsage(); i++){
            if(RobPtr == RobTag){
                return true;
            }else{
                auto RobEntry = this->m_Rob[RobPtr];
                if( !RobEntry.isStable && (RobEntry.Fu == funcType_t::LDU || 
                    RobEntry.Fu == funcType_t::STU || RobEntry.Fu == funcType_t::BRU))
                {
                    return false;
                }
                RobPtr = this->m_Rob.getNextPtr(RobPtr);
            }
        }
    }else{
        if(this->m_RobState == rob_state_t::Rob_WaitForResume && this->m_RollBackTag == RobTag){//上一周期的rob中进行了回滚操作
            return true;
        }
    }
    return false;
}

void Rcu::WriteBack(InsnPtr_t& insn, bool& needRedirect,Redirect_t& RedirectReq){
    needRedirect = false;
    if(!this->m_Rob.empty() && (this->m_Rob.isOlder(insn->RobTag,this->m_Rob.getLastest()) || insn->RobTag == this->m_Rob.getLastest())){
        this->m_Rob[insn->RobTag].done = true;
        this->m_Rob[insn->RobTag].isStable = true;
        if(!insn->Excp.valid){
            if(insn->Fu == funcType_t::BRU){
                if(insn->BruMisPred){
                    this->m_Rob[insn->RobTag].isMisPred = true;  
                    if(this->m_RobState == rob_state_t::Rob_Idle || 
                        (this->m_Rob.isOlder(insn->RobTag,this->m_RollBackTag) ||
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
            this->m_Rob[insn->RobTag].isExcp = true;
            if(this->m_RobState == rob_state_t::Rob_Idle || this->m_Rob.isOlder(insn->RobTag,this->m_RollBackTag))
            {
                this->m_RobState = rob_state_t::Rob_Undo;
                this->m_RollBackTag = insn->RobTag;
                this->m_ExcpCause   = insn->Excp.Cause;
                this->m_ExcpTval    = insn->Excp.Tval;
            }
        }
    }
}

void 
Rcu::AGUFastDetect(InsnPtr_t& insn){
    this->m_Rob[insn->RobTag].isStable = true;
    DPRINTF(WriteBack,"RobTag[{}],Pc[{:#x}] -> Scan AGU result, Exception [{}]",
                    insn->RobTag,insn->Pc,insn->Excp.valid);
    if(insn->Fu == funcType_t::STU && insn->Agu_addr_ready && insn->Agu_data_ready)
    {
        this->m_Rob[insn->RobTag].done = true;
    }
    else this->m_Rob[insn->RobTag].done = false;
    if(insn->Excp.valid){
        this->m_Rob[insn->RobTag].done   = true;
        this->m_Rob[insn->RobTag].isExcp = true;
        if(this->m_RobState != rob_state_t::Rob_Idle )
        {
            this->m_RobState = rob_state_t::Rob_Undo;
            this->m_RollBackTag = insn->RobTag;
            this->m_ExcpCause   = insn->Excp.Cause;
            this->m_ExcpTval    = insn->Excp.Tval;
        }
    }
}

void 
Rcu::ReleaseResource(uint16_t robTag){
    auto& entry = this->m_Rob[robTag];
    if(entry.phyRd != 0){
        this->m_IntFreelist.push(entry.phyRd);
        this->m_IntBusylist[entry.phyRd].forwarding = false;
        this->m_IntBusylist[entry.phyRd].done       = false;
        this->m_IntBusylist[entry.phyRd].allocated  = false;
        this->m_IntRenameTable[entry.isaRd] = entry.LphyRd;
        DPRINTF(RollBack,"RobTag[{}],Pc[{:#x}], Free phyRegister : Rd[{}], PRd[{}], LPRd[{}]",
            robTag,
            entry.pc,
            entry.isaRd,
            entry.phyRd,
            entry.LphyRd
        );
    }
    if(entry.Fu == funcType_t::LDU){
        this->m_Processor->getLsqPtr()->KillLoadEntry(entry.LSQtag);
    }else if(entry.Fu == funcType_t::STU){
        this->m_Processor->getLsqPtr()->KillStoreEntry(entry.LSQtag);
    }
}

void 
Rcu::RollBack(){
    uint16_t RobPtr = this->m_Rob.getLastest();
    for(size_t i = 0 ; i < this->m_AllocWidth && 
        this->m_Rob.isOlder(this->m_RollBackTag,RobPtr) || 
        RobPtr == this->m_RollBackTag;i++)//如果当前的robptr比回滚标记更旧，或者robptr恰好等于回滚标记，那么就需要进行回滚处理。
    {
        Rob_entry_t entry = this->m_Rob[RobPtr];
        if(entry.valid){
            if(RobPtr == this->m_RollBackTag){//如果当前robptr恰好就是要回滚的那个
                this->m_RobState = rob_state_t::Rob_WaitForResume;//设置状态为等待恢复
                if(entry.isExcp){//如果存在异常，则释放资源（物理寄存器，lsq，更新renametable）
                    this->ReleaseResource(RobPtr);
                }
                DPRINTF(RollBack,"RobTag[{}],Pc[{:#x}] -> RollBack Finish, Wait For Resume",RobPtr,entry.pc);
                break;
            }
            entry.valid = false;//将条目设置为无效，表示没有在使用了
            this->ReleaseResource(RobPtr);
            DPRINTF(RollBack,"RobTag[{}],Pc[{:#x}]",RobPtr,entry.pc);
        }
        this->m_Rob.RollBack();//更新rob尾指针
        RobPtr = this->m_Rob.getLastest();
    }
}

void 
Rcu::Evaulate(){
    if(this->m_RobState == rob_state_t::Rob_Undo){
        this->RollBack();
    }
    if(this->m_RobState == rob_state_t::Rob_FlushBackend){
        this->m_RobState = rob_state_t::Rob_Idle;
    }
}


void 
Rcu::CommitInsn(InsnPkg_t& insnPkg, Redirect_t& redirectReq, bool& needRedirect){
    needRedirect = false;
    for(size_t i = 0; i < insnPkg.size(); i++){
        uint16_t robPtr = this->m_Rob.getHeader();
        auto& robEntry = this->m_Rob[robPtr];
        if(robEntry.valid){
            DASSERT(robEntry.done,"Commit Insn When not Ready!")
            if(!robEntry.isExcp){
                if(robEntry.LphyRd != 0){
                    this->m_Processor->m_ExecContext->WriteIntReg(robEntry.isaRd,this->m_IntRegfile[robEntry.phyRd]);
                    this->m_IntFreelist.push(robEntry.LphyRd);
                    this->m_IntBusylist[robEntry.LphyRd].done = false;
                    this->m_IntBusylist[robEntry.LphyRd].allocated = false;
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Deallocate Last PRd[{}]",robPtr,robEntry.pc,robEntry.LphyRd);
                    
                }
                if(robEntry.Fu == funcType_t::LDU){
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit a Load LSQTag[{}]", 
                    robPtr,robEntry.pc,robEntry.LSQtag);
                    this->m_Processor->getLsqPtr()->CommitLoadEntry(robEntry.LSQtag);

                }else if(robEntry.Fu == funcType_t::STU){
                    DPRINTF(Commit,"RobTag[{}],Pc[{:#x}] -> Commit a Store LSQTag[{}]: Address[{:#x}] Data[{:#x}]",
                        robPtr,robEntry.pc,robEntry.LSQtag,robEntry.insnPtr->Agu_addr,robEntry.insnPtr->Agu_data);
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
void 
Rcu::Forwarding(InsnPtr_t& insn){
    this->m_IntRegfile[insn->PhyRd] = insn->RdResult;
    this->m_IntBusylist[insn->PhyRd].done = true;
}

} // namespace Emulator
