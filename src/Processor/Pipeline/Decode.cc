#include "Decode.hh"
#include "../../RISCV/Decoder.hh"
#include "../Processor.hh"
#include "../Component/Rcu.hh"
namespace Emulator
{
    

Decode::Decode( 
    Processor*          processor            ,
    const   uint64_t    DecodeQueueSize      
):  BaseStage(processor,"Decode"),
    m_DecodeQueue("DecodeQueue",DecodeQueueSize),
    m_FetchByteWidth(8),
    m_iCacheAlignByte(8),
    m_PredSync(4)
{}

Decode::~Decode(){}

void 
Decode::Evaluate(){
    this->ReleaseAction();

    this->ReceiveReq();
    this->SendReq();

}


void 
Decode::Reset(){
    this->m_FlushSyncLatch.reset();
    this->m_DecodeQueue.Reset();
}

void 
Decode::FlushAction(){
    this->m_DecodeQueue.Reset();
}

void
Decode::ReleaseAction(){
    if(this->m_StageAckInPort->valid){
        for(size_t i = 0; i < this->m_StageAckInPort->data.takenInsnCount ; i++){
            this->m_DecodeQueue.Pop();
        }
    }
}

void 
Decode::ReceiveReq(){
    if(this->m_StageInPort->valid){
        DASSERT((this->m_DecodeQueue.getSize() >= this->m_FromLastStageInsnCount), "Can Never Taken All Insn");
        if(this->m_DecodeQueue.getAvailEntryCount() >= this->m_FromLastStageInsnCount){
            InsnPkg_t insnPkg;       
                for(auto insn : this->m_StageInPort->data){
                    this->Predecode(*insn.get(),insnPkg); 
                }
                if(insnPkg.size()){
                    for(auto insn1 : insnPkg){
                        insn1->State = InsnState_t::State_Decode;
                        if(!insn1->Excp.valid){
                            this->DecodeInsn(insn1);
                        }
                        this->m_DecodeQueue.Push(insn1);
                    }
                }
            #ifdef TRACE_ON
            std::stringstream insnInfo;
            
            auto t = this->m_StageInPort->data[0];
            insnInfo << fmt::format("\n\tInsn_{:02} -> Pc({:#x}) , Insn({:#>08x})",0, t->Pc, t->CompressedInsn);
            DPRINTF(ReceiveReq,insnInfo.str());
            
            #endif
        }
    }
}

void 
Decode::SendReq(){
    if(!this->m_DecodeQueue.empty() && !this->m_StageOutPort->isStalled()){
        InsnPkg_t insnPkg;
        uint64_t  dqPtr = this->m_DecodeQueue.getHeader();
        for(size_t i = 0 ; (i < this->m_ToNextStageInsnCount) &&
                           (i < this->m_DecodeQueue.getUsage()); i++)
        {
            insnPkg.emplace_back(this->m_DecodeQueue[dqPtr]);
            dqPtr = this->m_DecodeQueue.getNextPtr(dqPtr);
        }
        this->m_StageOutPort->set(insnPkg);
    }
}

void 
Decode::DecodeInsn(InsnPtr_t& insn){
    RISCV::Decode_Func(*insn.get());
}

void 
Decode::Predecode(Emulator::DynInsn& insnEntry,InsnPkg_t& insnPkg){
    bool needRedirect;
    Redirect_t RedirectReq;
    uint64_t offset  = insnEntry.Address & (this->m_iCacheAlignByte-1); //看当前的地址是否对齐
    char* dataPtr = insnEntry.InsnByte.data() + offset;
    uint64_t numByte = m_FetchByteWidth - offset  ;
                                            
    uint64_t Npc  = insnEntry.Address ;
    
    while(numByte&&!needRedirect){//循环处理指令包进行预解码
        InsnPtr_t insn          = this->m_Processor->CreateInsn();
        insn->Pc                = Npc;
        insn->Excp              = insnEntry.Excp;
        insn->UncompressedInsn    = *(uint32_t*)dataPtr;
        insnPkg.emplace_back(insn);//将预解码的数据放入到insnpkg
        this->BranchRedirect(insn,needRedirect,RedirectReq);
        
        if(needRedirect){//如果需要重定向，那么后面的数据也就没必要放进来了
            DPRINTF(Redirect,"Pc[{:#x}] -> Predict Mismatch, Redirect to {:#x}",insn->Pc,RedirectReq.target);
            this->m_RedirectPort->set(RedirectReq);//发送重定向请求
            this->m_Processor->FlushBackWard(InsnState_t::State_Decode);//冲刷fetch1_flush
        }   
        numByte -= 4;
        dataPtr += 4;
        Npc     += 4;
    }      
}

void
Decode::BranchRedirect(InsnPtr_t& insn,bool& needRedirect,Redirect_t& RedirectReq){
    //采用静态预测，后续可扩展
    needRedirect = false;
    RedirectReq.StageId = InsnState_t::State_Decode;
    RISCV::StaticInsn instruction(insn->UncompressedInsn);
    insn->Pred.Taken        = false;
    insn->Pred.Target       = insn->Pc +  4;//pred.target内保存的是下一条地址，具体的地址取决于是否跳转
    if(instruction.opcode() == 0b1101111){ // JAL//默认跳转
        insn->Pred.Taken    = true;
        insn->Pred.Target   = insn->Pc + instruction.ujimm();//instruction.ujimm()表示的是riscv j-type指令立即数
        needRedirect = true;
        RedirectReq.target = insn->Pred.Target;
    }
}



std::shared_ptr<BaseStage> 
Create_Decode_Instance(
        Processor*                       processor         ,
        const YAML::Node&                StageConfig    
){
    const uint64_t DecodeQueueSize      = StageConfig["DecodeQueueSize"].as<uint64_t>() ;
    return std::shared_ptr<BaseStage>(
        new Decode(processor,DecodeQueueSize)
    );
}


} // namespace Emulator
