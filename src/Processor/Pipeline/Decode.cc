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
    m_DecodeQueue("DecodeQueue",DecodeQueueSize)
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
        if(!this->m_MustTakenAllInsn || (this->m_DecodeQueue.getAvailEntryCount() >= this->m_FromLastStageInsnCount)){
            StageAck_t ack;
            ack.takenInsnCount = 0;
            for(auto insn : this->m_StageInPort->data){
                if(!this->m_DecodeQueue.full()){
                    insn->State = InsnState_t::State_Decode;
                    if(!insn->Excp.valid){
                        this->DecodeInsn(insn);
                        if(this->m_Processor->m_doSpeculation && insn->Fu == funcType_t::BRU){
                            //如果采用了分支推测，且当前指令是BR,那么将控制流指令的标志由true（在译码时被设置了）改为false
                            //从而告诉处理器这并不是控制流处理器，不用等待分支预测的结果
                            insn->ControlFlowInsn = false;
                        }
                    }
                    this->m_DecodeQueue.Push(insn);
                    ack.takenInsnCount++;
                }else{
                    break;//译码队列满时触发，这里不需要进行重定向，因为相当于没取出数据，等下个周期再从fetch2的缓存队列里发就行了。
                }
            }
            this->m_StageAckOutPort->set(ack);//告诉fetch2 decode了多少指令

            #ifdef TRACE_ON
            std::stringstream insnInfo;
            if(ack.takenInsnCount){
                for(size_t i = 0; i < ack.takenInsnCount; i++){
                    auto t = this->m_StageInPort->data[i];
                    insnInfo << fmt::format("\n\tInsn_{:02} -> Pc({:#x}) , Insn({:#>08x})",i, t->Pc, t->CompressedInsn);
                }
                DPRINTF(ReceiveReq,insnInfo.str());
            }
            #endif
        }else{
            this->m_StageInPort->stall();
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
    RISCV::Decode_Func(*insn.get(),this->m_Processor->m_ExecContext->getPriv());
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
