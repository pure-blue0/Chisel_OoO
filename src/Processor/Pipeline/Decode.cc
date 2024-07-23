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
    Redirect_message redirect_message;
    redirect_message.valid=0;
    static uint8_t avail_count;
    bool flush_flag=false;

    bool reset_n=true;
    DecodeInsn_t insn1          = this->m_Processor->CreateDecodeInsn();
    DecodeInsn_t insn2          = this->m_Processor->CreateDecodeInsn();
    //-----------------------------------------------------------------//
    bool Predcode_EN=false;
    if(this->m_StageInPort->valid){
        if((*this->m_StageInPort->data[0].get()).data_valid)
        {
            if(avail_count >= this->m_FromLastStageInsnCount){
                Predcode_EN=true;
            }
        }
    }
     //-----------------------------------------------------------------//
    if(Predcode_EN)this->Predecode(*this->m_StageInPort->data[0].get(),insn1,insn2,redirect_message,flush_flag); 
    else{
        insn1->data_valid=false;
        insn2->data_valid=false;
    }
    this->m_Processor->getFetch1Ptr()->Decode_Redirect_Reg->InPort->set(redirect_message);
    if(flush_flag){
        //DPRINTF(Redirect,"Pc[{:#x}] -> Predict Mismatch, Redirect to {:#x}",insn1->Pc,RedirectReq.target);
        this->m_Processor->FlushBackWard(InsnState_t::State_Decode);//冲刷fetch1_flush
    }   
    //-----------------------------------------------------------------//
    this->DecodeInsn(insn1);
    this->DecodeInsn(insn2);
    //-----------------------------------------------------------------//
    this->DecodeQueue_Evaluate(reset_n,this->decodeQueue_flush,this->m_StageAckInPort->data.takenInsnCount,insn1->data_valid,insn1,insn2->data_valid,insn2,this->m_StageOutPort->isStalled(),avail_count);
}


void 
Decode::Reset(){
    this->m_FlushSyncLatch.reset();
    this->m_DecodeQueue.Reset();

}

void 
Decode::FlushAction(){
    this->decodeQueue_flush=true;
}

void
Decode::ReleaseAction(){  
}


void Decode::DecodeQueue_Evaluate(bool reset_n,bool decodeQueue_flush,uint8_t pop_count,bool WEN1,DecodeInsn_t insn1_WriteIn,bool WEN2,DecodeInsn_t insn2_WriteIn,bool isStalled,uint8_t& r_avail_count){
    static DecodeQueue_entry DecodeQueue[DecodeQueue_Size];
    static uint8_t header_ptr,tail_ptr;
    static uint8_t usage_count;
    static uint8_t avail_count;

    bool empty,one_entry_flag;
    InsnPkg_t insnPkg;
    InsnPtr_t insn1          = this->m_Processor->CreateInsn();
    InsnPtr_t insn2          = this->m_Processor->CreateInsn();

    
    uint8_t send_header_ptr,next_send_header_ptr;


    if(pop_count==2){
        
        if(header_ptr==(DecodeQueue_Size-1)){
            send_header_ptr=1;
            next_send_header_ptr=2;
        }
        else if(header_ptr==58){
            send_header_ptr=0;
            next_send_header_ptr=1;
        }
        else {
            send_header_ptr=header_ptr+2;
            if(send_header_ptr==(DecodeQueue_Size-1)){
                next_send_header_ptr=0;
            }
            else next_send_header_ptr=send_header_ptr+1;
        }
    }
    else if(pop_count==1){
        
        if(header_ptr==(DecodeQueue_Size-1)){
            send_header_ptr=0;
            next_send_header_ptr=1;
        }
        else {
            send_header_ptr=header_ptr+1;
            if(send_header_ptr==(DecodeQueue_Size-1)){
                next_send_header_ptr=0;
            }
            else next_send_header_ptr=send_header_ptr+1;
        }
    }
    else {
        send_header_ptr=header_ptr;
        if(send_header_ptr==(DecodeQueue_Size-1)){
                next_send_header_ptr=0;
        }
        else next_send_header_ptr=send_header_ptr+1;
    }
    
    if((usage_count-pop_count)==0) {
        empty=true;
        one_entry_flag=false;
    }
    else if((usage_count-pop_count)==1) {
        empty=false;
        one_entry_flag=true;
    }
    else{
        empty=false;
        one_entry_flag=false;
    }
    
    if(!reset_n||decodeQueue_flush){
        for(int i=0;i<DecodeQueue_Size;i++){
        DecodeQueue->reset();
        }
        header_ptr=0;
        tail_ptr=0;
        usage_count=0;
        avail_count=DecodeQueue_Size;
        insn1->data_valid=false;
        insn2->data_valid=false;
    }
    else{
        
        if(!(empty)&&!isStalled){
            insn1->data_valid = DecodeQueue[send_header_ptr].data_valid;
            insn1->Pc = DecodeQueue[send_header_ptr].Pc;
            insn1->Fu = DecodeQueue[send_header_ptr].Fu;
            insn1->SubOp = DecodeQueue[send_header_ptr].SubOp;
            insn1->UncompressedInsn = DecodeQueue[send_header_ptr].UncompressedInsn;
            insn1->ControlFlowInsn = DecodeQueue[send_header_ptr].ControlFlowInsn;
            insn1->IsaRs1 = DecodeQueue[send_header_ptr].IsaRs1;
            insn1->IsaRs2 = DecodeQueue[send_header_ptr].IsaRs2;
            insn1->IsaRd = DecodeQueue[send_header_ptr].IsaRd;
            insn1->Operand1Ready = DecodeQueue[send_header_ptr].Operand1Ready;
            insn1->Operand2Ready = DecodeQueue[send_header_ptr].Operand2Ready;
            insn1->Operand1 = DecodeQueue[send_header_ptr].Operand1;
            insn1->Operand2 = DecodeQueue[send_header_ptr].Operand2;
            insn1->imm = DecodeQueue[send_header_ptr].imm;
            insn1->Excp = DecodeQueue[send_header_ptr].Excp;
            insn1->Pred = DecodeQueue[send_header_ptr].Pred;
            
            if(!one_entry_flag){
                insn2->data_valid = DecodeQueue[next_send_header_ptr].data_valid;
                insn2->Pc = DecodeQueue[next_send_header_ptr].Pc;
                insn2->Fu = DecodeQueue[next_send_header_ptr].Fu;
                insn2->SubOp = DecodeQueue[next_send_header_ptr].SubOp;
                insn2->UncompressedInsn = DecodeQueue[next_send_header_ptr].UncompressedInsn;
                insn2->ControlFlowInsn = DecodeQueue[next_send_header_ptr].ControlFlowInsn;
                insn2->IsaRs1 = DecodeQueue[next_send_header_ptr].IsaRs1;
                insn2->IsaRs2 = DecodeQueue[next_send_header_ptr].IsaRs2;
                insn2->IsaRd = DecodeQueue[next_send_header_ptr].IsaRd;
                insn2->Operand1Ready = DecodeQueue[next_send_header_ptr].Operand1Ready;
                insn2->Operand2Ready = DecodeQueue[next_send_header_ptr].Operand2Ready;
                insn2->Operand1 = DecodeQueue[next_send_header_ptr].Operand1;
                insn2->Operand2 = DecodeQueue[next_send_header_ptr].Operand2;
                insn2->imm = DecodeQueue[next_send_header_ptr].imm;
                insn2->Excp = DecodeQueue[next_send_header_ptr].Excp;
                insn2->Pred = DecodeQueue[next_send_header_ptr].Pred;
            }
            else{
                insn2->data_valid =false;
            }
        }
        else{
            insn1->data_valid=false;
            insn2->data_valid=false;
        }
        for(size_t i = 0; i < pop_count ; i++){
            header_ptr++;
            if(header_ptr==DecodeQueue_Size)header_ptr=0;
            
        }
        usage_count-=pop_count;
        if(WEN1){
            DecodeQueue[tail_ptr]=*insn1_WriteIn.get();
            tail_ptr++;
            if(tail_ptr==DecodeQueue_Size){tail_ptr=0;}
            usage_count++;
        }
        if(WEN2){
            DecodeQueue[tail_ptr]=*insn2_WriteIn.get();
            tail_ptr++;
            if(tail_ptr==DecodeQueue_Size){tail_ptr=0;}
            usage_count++;
            
        }
        avail_count=DecodeQueue_Size-usage_count;
    }
    
    r_avail_count=avail_count;
    insnPkg.emplace_back(insn1);
    insnPkg.emplace_back(insn2);
    this->m_StageOutPort->set(insnPkg);

}

void 
Decode::DecodeInsn(DecodeInsn_t& insn){
    RISCV::Decode_Func(*insn.get());
}

void 
Decode::Predecode(Emulator::DynInsn& insnEntry,DecodeInsn_t& insn1,DecodeInsn_t& insn2,Redirect_message& redirect_message,bool& flush_flag){
    redirect_message.valid=0;
    redirect_message.target=0;
    Redirect_message redirect_message1,redirect_message2;
    uint64_t offset  = insnEntry.Address & (this->m_iCacheAlignByte-1); //看当前的地址是否对齐
    char* dataPtr = insnEntry.InsnByte.data() + offset;  
    //insn1->send_valid        = false;
    insn1->data_valid        = true;
    insn1->Pc                = insnEntry.Address;
    insn1->Excp              = insnEntry.Excp;
    insn1->UncompressedInsn  = *(uint32_t*)dataPtr;
    this->BranchRedirect(insn1,redirect_message1);
    
    if(!offset&&!redirect_message1.valid){
       // insn2->send_valid        = false;
        insn2->data_valid        = true;
        insn2->Pc                = insnEntry.Address+4;
        insn2->Excp              = insnEntry.Excp;
        insn2->UncompressedInsn    = *(uint32_t*)(dataPtr+4);
        
        this->BranchRedirect(insn2,redirect_message2);
        
    }
    else{
        insn2->data_valid        = false;
    }
    flush_flag=redirect_message1.valid|redirect_message2.valid;
    if(redirect_message2.valid)redirect_message=redirect_message2;
    else if(redirect_message1.valid)redirect_message=redirect_message1;
}

void
Decode::BranchRedirect(DecodeInsn_t& insn,Redirect_message& redirect_message){
    //采用静态预测，后续可扩展
    redirect_message.valid = false;
    //RedirectReq.StageId = InsnState_t::State_Decode;
    RISCV::StaticInsn instruction(insn->UncompressedInsn);
    insn->Pred.Taken        = false;
    insn->Pred.Target       = insn->Pc +  4;//pred.target内保存的是下一条地址，具体的地址取决于是否跳转
    if(instruction.opcode() == 0b1101111){ // JAL//默认跳转
        insn->Pred.Taken    = true;
        insn->Pred.Target   = insn->Pc + instruction.ujimm();//instruction.ujimm()表示的是riscv j-type指令立即数
        redirect_message.valid = true;
        redirect_message.target = insn->Pred.Target;
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
