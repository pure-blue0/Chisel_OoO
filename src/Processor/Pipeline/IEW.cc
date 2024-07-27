#include "IEW.hh"
#include "../Processor.hh"

namespace Emulator
{

IEW::IEW(Processor* processor)
:   BaseStage(processor,"IEW")
{}

IEW::~IEW(){}

void 
IEW::Reset(){
    for(auto it : this->m_IssueStageVec){
        it->Reset();
    }
    for(auto it : this->m_ExceuteStageVec){
        it->Reset();
    }
    for(auto it : this->m_ReadOperandStageVec){
        it->Reset();
    }
    for(auto it : this->m_WriteBackStageVec){
        it->Reset();
    }
    this->m_FlushSyncLatch.reset();
}

void 
IEW::Flush(){
    this->m_FlushSyncLatch.InPort->set(true);
    for(auto it : this->m_IssueStageVec){
        it->Flush();
    }
    for(auto it : this->m_ExceuteStageVec){
        it->Flush();
    }
    for(auto it : this->m_ReadOperandStageVec){
        it->Flush();
    }
    for(auto it : this->m_WriteBackStageVec){
        it->Flush();
    }
}


void 
IEW::Evaluate(){
    this->WriteBack();
    
    this->Execute();
    this->Forwarding();

    this->Issue();
    this->ReadOperand();
}

void 
IEW::Issue(){
    for(auto issuer : this->m_IssueStageVec){
        issuer->Evaluate();
    }
}

void
IEW::ReadOperand(){
    for(auto rfPort : this->m_ReadOperandStageVec){
        rfPort->Evaluate();
    }
}

void 
IEW::Execute(){
    for(auto executer : this->m_ExceuteStageVec){
        executer->Evaluate();
    }
}

void 
IEW::Forwarding(){
    for(auto forwardinger : this->m_WriteBackStageVec){
        forwardinger->Forwarding();
    }
}

void 
IEW::WriteBack(){
    Redirect_t RedirectReq;
    bool needRedirect = false;
    Redirect_message redirect_message;
    redirect_message.valid=0;
    for(auto& writebacker : this->m_WriteBackStageVec){
        writebacker->Evaluate(RedirectReq,needRedirect);
        
        
        
        if(needRedirect){
            this->m_RedirectPort->set(RedirectReq);
            redirect_message.valid=needRedirect;
            redirect_message.target=RedirectReq.target;
        }
    }
    this->m_Processor->getFetch1Ptr()->IEW_Redirect_Reg->InPort->set(redirect_message);
}

void 
IEW::AddFuncUnitToExecStage(std::shared_ptr<BaseFuncUnit> fu){
    this->m_ExceuteStageVec.emplace_back(fu);
}

void 
IEW::AddSchedularToIssueStage(std::shared_ptr<BaseSchedular> schedular){
    this->m_IssueStageVec.emplace_back(schedular);
}

void 
IEW::AddRfReadPortToReadOperandStage(std::shared_ptr<RfReadPort> rfRdPort){
    this->m_ReadOperandStageVec.emplace_back(rfRdPort);
}

void 
IEW::AddWbPortToWriteBackStage(std::shared_ptr<WriteBackPort> wbPort){
    this->m_WriteBackStageVec.emplace_back(wbPort);
}

std::shared_ptr<BaseStage> Create_IEW_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
){
    auto t = std::make_shared<IEW>(processor);
    for(auto& fu : processor->m_FuncUnitVec){
        t->AddFuncUnitToExecStage(fu);
    }
    for(auto& schedular : processor->m_SchedularVec){
        t->AddSchedularToIssueStage(schedular);
    }
    for(auto& rdPort : processor->m_RfReadPortVec){
        t->AddRfReadPortToReadOperandStage(rdPort);
    }
    for(auto& wrPort : processor->m_WriteBackPortVec){
        t->AddWbPortToWriteBackStage(wrPort);
    }
    return t;
}

} // namespace Emulator
