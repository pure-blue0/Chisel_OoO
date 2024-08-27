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
    InsnPtr_t insn[4];
    for(size_t i = 0;i < 4;i++){
        insn[i] = this->m_Processor->CreateInsn();
        insn[i]->data_valid=false;
    }
    
    auto rcu = this->m_Processor->getRcuPtr();
    InsnPkg_t insnPkg;
    uint16_t robPtr = rcu->m_Rob.getHeader();
    DASSERT((this->m_ToNextStageInsnCount == rcu->m_DeallocWidth), 
        "Commit[{}] & RCU Deallocate[{}] Width MisMatch!",this->m_ToNextStageInsnCount,rcu->m_DeallocWidth);
    for(size_t i = 0; i < 4 && i < rcu->m_Rob.getUsage(); i++){
        auto entry = rcu->m_Rob[robPtr];
        if((entry.done | !entry.valid) && rcu->ReadyForCommit(robPtr)){
            insn[i]->data_valid=true;
            insn[i]->RobTag=robPtr;
            //如果指令已完成且有效，并且 RCU 准备好提交该指令
            
        }else{
            break; //Only Support In order Commit For Now
        }
        robPtr = rcu->m_Rob.getNextPtr(robPtr);
    }
    for(size_t i = 0;i < 4;i++){
        insnPkg.emplace_back(insn[i]);
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
        uint8_t commit_insn_num=insnPkg[0]->data_valid+insnPkg[1]->data_valid+insnPkg[2]->data_valid+insnPkg[3]->data_valid;
        rcu->CommitInsn(commit_insn_num,RedirectReq,needRedirect);//进行实际的提交操作
        redirect_message.valid=needRedirect;
        redirect_message.target=RedirectReq.target;

    }
    else{
        for(int i=0;i<4;i++){
                rcu->FreeBusyList_Commit_EN[i]=false;
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
