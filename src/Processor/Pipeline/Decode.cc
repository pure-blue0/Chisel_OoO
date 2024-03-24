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
                //DPRINTF(ReceiveReq,"enter Pc is[{:#x}] ",insnPkg.size());
                if(insnPkg.size()){
                    for(auto insn1 : insnPkg){
                        insn1->State = InsnState_t::State_Decode;
                        if(!insn1->Excp.valid){
                            this->DecodeInsn(insn1);
                        }
                        this->m_DecodeQueue.Push(insn1);
                    }
                }
            // for(auto insn : this->m_StageInPort->data){
            //     insn->State = InsnState_t::State_Decode;
            //     if(!insn->Excp.valid){
            //         this->DecodeInsn(insn);
            //     }
            //     this->m_DecodeQueue.Push(insn);
                
            // }
            #ifdef TRACE_ON
            std::stringstream insnInfo;
            
            auto t = this->m_StageInPort->data[0];
            insnInfo << fmt::format("\n\tInsn_{:02} -> Pc({:#x}) , Insn({:#>08x})",0, t->Pc, t->CompressedInsn);
            DPRINTF(ReceiveReq,insnInfo.str());
            
            #endif
        }else{
            this->m_Processor->FlushBackWard(InsnState_t::State_Decode);//冲刷flash
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

void 
Decode::Predecode(Emulator::DynInsn& insnEntry,InsnPkg_t& insnPkg){
    bool needRedirect;
    Redirect_t RedirectReq;
    
    if(insnEntry.Excp.valid){//在这里只有前面出现INSTR_ADDR_MISALIGNED才会导致这种可能
        auto insn = this->m_Processor->CreateInsn();
        insn->Pc   = insnEntry.Pc;//就只发送一条异常的地址
        insn->Excp = insnEntry.Excp;
        insnPkg.emplace_back(insn);
    }else{
        uint64_t offset  = insnEntry.Pc & (this->m_iCacheAlignByte-1); //看当前的地址是否对齐，也就是低五位是否为0
       // DASSERT(!(offset && this->m_MisalignValid),"Shift Cacheline when MisAlign valid!");//避免出现上一次的地址发生截断，还不连续的情况
        //如果当前数据的地址并不是对其的，比如0x80000010，偏移了16位，那么头指针也需要偏移16位，然后再开始取指令
        char* dataPtr = insnEntry.InsnByte.data() + offset;
        uint64_t numByte = m_FetchByteWidth - (insnEntry.Pc & (this->m_iCacheAlignByte-1))  ;//32-偏移的16位，然后如果上一次最后的指令发送了截断，那就多处理两bit
                                               
        uint64_t Npc  = insnEntry.Pc ;//偏移到上一行的倒数第二个字节

        while(numByte){
            InsnPtr_t insn          = this->m_Processor->CreateInsn();//调用processor的函数是因为这里只是填加了insn的pc，后面其他stage还要添加信息
            insn->Pc                = Npc;
            insn->Excp              = insnEntry.Excp;
            //判断是否是压缩指令，如果上一次发生截断，这一次的头两个数据只能是剩下的那两byte高位地址数据，那么就把当前数据左移16位，和上一次的拼起来
           // insn->IsRvcInsn         = false;
            insn->UncompressedInsn    = *(uint32_t*)dataPtr;
            insnPkg.emplace_back(insn);
            this->BranchRedirect(insn,needRedirect,RedirectReq);
            
            if(needRedirect){//如果需要重定向，那么后面的数据也就没必要放进来了
                DPRINTF(Redirect,"Pc[{:#x}] -> Predict Mismatch, Redirect to {:#x}",insn->Pc,RedirectReq.target);
                this->m_RedirectPort->set(RedirectReq);
                this->m_Processor->FlushBackWard(InsnState_t::State_Decode);//冲刷flash
                break;
            }
            
            numByte -= 4;
            dataPtr += 4;
            Npc     += 4;
            offset  += 4;
        }   
    }     
}

void
Decode::BranchRedirect(InsnPtr_t& insn,bool& needRedirect,Redirect_t& RedirectReq){
    //默认分支不跳转，后续可扩展
    Pred_t Pred;
    Pred.taken_valid  = false;//说明预测是否有效
    Pred.taken        = false;//预测结果
    Pred.target       = 0;//预测地址
    Pred.target_valid = false;//预测地址是否有效

    needRedirect = false;
    RedirectReq.StageId = InsnState_t::State_Decode;
    RISCV::StaticInsn instruction(insn->UncompressedInsn);//对指令进行解码
    insn->Pred.Taken        = false;
    insn->Pred.Target       = insn->Pc +  4;//pred.target内保存的是下一条地址，具体的地址取决于是否跳转
    if(instruction.opcode() == 0b1101111){ // JAL//默认跳转
        insn->Pred.Taken    = true;
        insn->Pred.Target   = insn->Pc + instruction.ujimm();
        if(!(Pred.taken_valid == true && Pred.taken == true && Pred.target_valid && Pred.target == (insn->Pc + instruction.ujimm()))){
            //如果以上所有条件都成立，说明预测的跳转目标地址与 JAL 指令的跳转目标地址一致，此时不需要重定向
            needRedirect = true;
            RedirectReq.target = insn->Pred.Target;
        }
    }else if(instruction.opcode() == 0b1100111 && instruction.func3() == 0b000){ // JALR//默认不跳转
        if(Pred.taken_valid && Pred.target_valid){
            insn->Pred.Taken  = Pred.taken;
            insn->Pred.Target = Pred.target;
        }
    }else if(instruction.opcode() == 0b1100011){ // BRANCH//默认不跳转
        if(Pred.taken_valid && Pred.taken){
            insn->Pred.Taken = true;
            insn->Pred.Target = insn->Pc + instruction.sbimm();
            if(!(Pred.target_valid && Pred.target == (insn->Pc + instruction.sbimm()))){
                needRedirect = true;
                RedirectReq.target = insn->Pred.Target;
            }
        }
    }
    else{
        if(Pred.taken_valid && Pred.target_valid){
            needRedirect = true;
            RedirectReq.target = insn->Pred.Target; 
        }
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
