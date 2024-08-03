#include "Commit.hh"
#include "../Processor.hh"
#include "typeinfo"
namespace Emulator
{


Commit::Commit(Processor* processor)
:   BaseStage(processor,"Commit")
{
}
    
Commit::~Commit(){}

void 
Commit::Reset(){}

void 
Commit::SendCommitReq(){
    auto rcu = this->m_Processor->getRcuPtr();
    InsnPkg_t insnPkg;
    uint16_t robPtr = rcu->m_Rob.getHeader();
    DASSERT((this->m_ToNextStageInsnCount == rcu->m_DeallocWidth), 
        "Commit[{}] & RCU Deallocate[{}] Width MisMatch!",this->m_ToNextStageInsnCount,rcu->m_DeallocWidth);
    for(size_t i = 0; i < this->m_ToNextStageInsnCount && i < rcu->m_Rob.getUsage(); i++){
        auto& entry = rcu->m_Rob[robPtr];
        if((entry.done | !entry.valid) && rcu->ReadyForCommit(robPtr)){
            //如果指令已完成且有效，并且 RCU 准备好提交该指令
            insnPkg.emplace_back(entry.insnPtr);
        }else{
            break; //Only Support In order Commit For Now
        }
        robPtr = rcu->m_Rob.getNextPtr(robPtr);
    }
    if(insnPkg.size()){
        this->m_StageOutPort->set(insnPkg);
    }
}

void 
Commit::CommitInsnPkg(){
    Redirect_message redirect_message;
    redirect_message.valid=0;
    auto rcu = this->m_Processor->getRcuPtr();
    if(this->m_StageInPort->valid){
        InsnPkg_t& insnPkg = this->m_StageOutPort->data;
        Redirect_t RedirectReq;
        bool       needRedirect;
        for(auto& insn : insnPkg){
            if(insn){
                
                DPRINTF(CommitLog,"RobTag[{}],Pc[{:#x}] -> Write Result Rd[{}] PRd[{}] ({:#x})",
                    insn->RobTag,
                    insn->Pc,
                    insn->IsaRd,
                    insn->PhyRd,
                    insn->RdResult
                );
                if(insn->Fu == funcType_t::STU){
                    DPRINTF(CommitLog,"RobTag[{}],Pc[{:#x}] -> Write Mem Address[{:#x}] Data[{:#x}]",
                        insn->RobTag,
                        insn->Pc,
                        insn->Agu_addr,
                        insn->Agu_data
                    );
                }else{
                    DPRINTF(CommitLog,"RobTag[{}],Pc[{:#x}]",insn->RobTag,insn->Pc);
                }
            }
        }
        rcu->CommitInsn(insnPkg,RedirectReq,needRedirect);//进行实际的提交操作
        redirect_message.valid=needRedirect;
        redirect_message.target=RedirectReq.target;
        if(needRedirect){
            this->m_RedirectPort->set(RedirectReq);
            
        }
    }
    else{
        for(int i=0;i<4;i++){
                rcu->FE_Commit_EN[i]=false;
        }
        
    }
    this->m_Processor->getFetch1Ptr()->Commit_Redirect_Reg->InPort->set(redirect_message);
}


void 
Commit::Evaluate(){
    this->CommitInsnPkg();
    this->SendCommitReq();
}

void
Commit::Flush(){
    
}



std::shared_ptr<BaseStage> Create_Commit_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
){
    return std::make_shared<Commit>(processor);
}


    
} // namespace Emulator
