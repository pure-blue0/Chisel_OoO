#include "Decode.hh"
#include "../../RISCV/Decoder.hh"
#include "../Processor.hh"
#include "../Component/Rcu.hh"

// #include <verilated.h>          // Defines common routines
// //#include "./obj_dir/VBranchRedirect.h"
// #include "./obj_dir/VDecodeQueue.h"
// #include "./obj_dir/VPredecode.h"
// #include "./obj_dir/VDecode_Func.h"
namespace Emulator
{
// uint32_t error_cnt = 0;
// VPredecode *predecode = new VPredecode ;
// VDecode_Func *Decode_Func1 = new VDecode_Func;
// VDecode_Func *Decode_Func2 = new VDecode_Func;
// VDecodeQueue *Decode_Queue = new VDecodeQueue ; 

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
    uint8_t avail_count;

    bool empty,one_entry_flag;
    InsnPkg_t insnPkg;
    InsnPtr_t insn1          = this->m_Processor->CreateInsn();
    InsnPtr_t insn2          = this->m_Processor->CreateInsn();

    
    uint8_t send_header_ptr,next_send_header_ptr;


    //求读指针 send_header_ptr 和 next_send_header_ptr
    send_header_ptr = (header_ptr + pop_count) % DecodeQueue_Size;
    next_send_header_ptr = (send_header_ptr + 1) % DecodeQueue_Size;
    
    //求 empty(队列是否为空) 和 one_entry_flag(在本周期弹出条目后是否只剩一个有效条目)
    empty = (usage_count-pop_count)==0 ? true : false;
    one_entry_flag = (usage_count-pop_count)==1 ? true : false;
    
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
        header_ptr = (header_ptr + pop_count) % DecodeQueue_Size;

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
        
    }
    avail_count=DecodeQueue_Size-usage_count;
    r_avail_count=avail_count;
    insnPkg.emplace_back(insn1);
    insnPkg.emplace_back(insn2);
    this->m_StageOutPort->set(insnPkg);

    
    // DecodeQueue_entry &insn1_in =  *insn1_WriteIn.get();
    // DecodeQueue_entry &insn2_in =  *insn2_WriteIn.get();
    // Decode_Queue->io_decode_flush_flag = decodeQueue_flush;
    // Decode_Queue ->io_decoded_insn1_data_valid = insn1_in.data_valid;
    // Decode_Queue->io_decoded_insn1_Pred_Taken = insn1_in.Pred.Taken;
    // Decode_Queue->io_decoded_insn1_Pred_Target = insn1_in.Pred.Target;
    // Decode_Queue ->io_decoded_insn1_Excp_Tval = insn1_in.Excp.Tval;
    // Decode_Queue ->io_decoded_insn1_Excp_Cause = insn1_in.Excp.Cause;
    // Decode_Queue ->io_decoded_insn1_Excp_valid = insn1_in.Excp.valid;
    // Decode_Queue ->io_decoded_insn1_Sub_OP = insn1_in.SubOp;
    // Decode_Queue ->io_decoded_insn1_Function_type = insn1_in.Fu;
    // Decode_Queue ->io_decoded_insn1_ControlFlowInsn = insn1_in.ControlFlowInsn;
    // Decode_Queue ->io_decoded_insn1_IsaRd = insn1_in.IsaRd;
    // Decode_Queue ->io_decoded_insn1_Operand2Ready = insn1_in.Operand2Ready;
    // Decode_Queue ->io_decoded_insn1_IsaRs2 = insn1_in.IsaRs2;
    // Decode_Queue ->io_decoded_insn1_Operand1Ready = insn1_in.Operand1Ready;
    // Decode_Queue ->io_decoded_insn1_IsaRs1 = insn1_in.IsaRs1;
    // Decode_Queue ->io_decoded_insn1_imm = insn1_in.imm;
    // Decode_Queue ->io_decoded_insn1_Operand2 = insn1_in.Operand2;
    // Decode_Queue ->io_decoded_insn1_Operand1 = insn1_in.Operand1;
    // Decode_Queue ->io_decoded_insn1_insn = insn1_in.UncompressedInsn;
    // Decode_Queue ->io_decoded_insn1_pc = insn1_in.Pc;

    
    // Decode_Queue ->io_decoded_insn2_data_valid = insn2_in.data_valid;
    // Decode_Queue->io_decoded_insn2_Pred_Taken = insn2_in.Pred.Taken;
    // Decode_Queue->io_decoded_insn2_Pred_Target = insn2_in.Pred.Target;
    // Decode_Queue ->io_decoded_insn2_Excp_Tval = insn2_in.Excp.Tval;
    // Decode_Queue ->io_decoded_insn2_Excp_Cause = insn2_in.Excp.Cause;
    // Decode_Queue ->io_decoded_insn2_Excp_valid = insn2_in.Excp.valid;
    // Decode_Queue ->io_decoded_insn2_Sub_OP = insn2_in.SubOp;
    // Decode_Queue ->io_decoded_insn2_Function_type = insn2_in.Fu;
    // Decode_Queue ->io_decoded_insn2_ControlFlowInsn = insn2_in.ControlFlowInsn;
    // Decode_Queue ->io_decoded_insn2_IsaRd = insn2_in.IsaRd;
    // Decode_Queue ->io_decoded_insn2_Operand2Ready = insn2_in.Operand2Ready;
    // Decode_Queue ->io_decoded_insn2_IsaRs2 = insn2_in.IsaRs2;
    // Decode_Queue ->io_decoded_insn2_Operand1Ready = insn2_in.Operand1Ready;
    // Decode_Queue ->io_decoded_insn2_IsaRs1 = insn2_in.IsaRs1;
    // Decode_Queue ->io_decoded_insn2_imm = insn2_in.imm;
    // Decode_Queue ->io_decoded_insn2_Operand2 = insn2_in.Operand2;
    // Decode_Queue ->io_decoded_insn2_Operand1 = insn2_in.Operand1;
    // Decode_Queue ->io_decoded_insn2_insn = insn2_in.UncompressedInsn;
    // Decode_Queue ->io_decoded_insn2_pc = insn2_in.Pc;
    // Decode_Queue->io_isStalled = isStalled;
    // Decode_Queue ->io_WEN1 = WEN1;
    // Decode_Queue ->io_WEN2 = WEN2;

    // Decode_Queue ->io_pop_count = pop_count;
    
    // Decode_Queue->clock = 0;
    // Decode_Queue->eval();


    // InsnPkg_t insnPkg;
    // InsnPtr_t insn1          = this->m_Processor->CreateInsn();
    // InsnPtr_t insn2          = this->m_Processor->CreateInsn();
    // //output
    // insn1->data_valid = Decode_Queue->io_entry_out1_data_valid;
    // insn1->Excp.valid = Decode_Queue->io_entry_out1_Excp_valid;
    // insn1->Excp.Cause = Decode_Queue->io_entry_out1_Excp_Cause;
    // insn1->Excp.Tval = Decode_Queue->io_entry_out1_Excp_Tval;
    // insn1->Pred.Taken = Decode_Queue->io_entry_out1_Pred_Taken;
    // insn1->Pred.Target = Decode_Queue->io_entry_out1_Pred_Target;
    // insn1->SubOp = Decode_Queue->io_entry_out1_Sub_OP;
    // insn1->Fu = Emulator::funcType_t(Decode_Queue->io_entry_out1_Function_type);
    // insn1->ControlFlowInsn = Decode_Queue->io_entry_out1_ControlFlowInsn;
    // insn1->IsaRd = Decode_Queue->io_entry_out1_IsaRd;
    // insn1->Operand2Ready = Decode_Queue->io_entry_out1_Operand2Ready;
    // insn1->IsaRs2 = Decode_Queue->io_entry_out1_IsaRs2;
    // insn1->Operand1Ready = Decode_Queue->io_entry_out1_Operand1Ready;
    // insn1->IsaRs1 = Decode_Queue->io_entry_out1_IsaRs1;
    // insn1->imm = Decode_Queue->io_entry_out1_imm;
    // insn1->Operand2 = Decode_Queue->io_entry_out1_Operand2;
    // insn1->Operand1 = Decode_Queue->io_entry_out1_Operand1;
    // insn1->UncompressedInsn = Decode_Queue->io_entry_out1_insn;
    // insn1->Pc = Decode_Queue->io_entry_out1_pc;


    // insn2->data_valid = Decode_Queue->io_entry_out2_data_valid;
    // insn2->Excp.valid = Decode_Queue->io_entry_out2_Excp_valid;
    // insn2->Excp.Cause = Decode_Queue->io_entry_out2_Excp_Cause;
    // insn2->Excp.Tval = Decode_Queue->io_entry_out2_Excp_Tval;
    // insn2->Pred.Taken = Decode_Queue->io_entry_out2_Pred_Taken;
    // insn2->Pred.Target = Decode_Queue->io_entry_out2_Pred_Target;
    // insn2->SubOp = Decode_Queue->io_entry_out2_Sub_OP;
    // insn2->Fu = Emulator::funcType_t(Decode_Queue->io_entry_out2_Function_type);
    // insn2->ControlFlowInsn = Decode_Queue->io_entry_out2_ControlFlowInsn;
    // insn2->IsaRd = Decode_Queue->io_entry_out2_IsaRd;
    // insn2->Operand2Ready = Decode_Queue->io_entry_out2_Operand2Ready;
    // insn2->IsaRs2 = Decode_Queue->io_entry_out2_IsaRs2;
    // insn2->Operand1Ready = Decode_Queue->io_entry_out2_Operand1Ready;
    // insn2->IsaRs1 = Decode_Queue->io_entry_out2_IsaRs1;
    // insn2->imm = Decode_Queue->io_entry_out2_imm;
    // insn2->Operand2 = Decode_Queue->io_entry_out2_Operand2;
    // insn2->Operand1 = Decode_Queue->io_entry_out2_Operand1;
    // insn2->UncompressedInsn = Decode_Queue->io_entry_out2_insn;
    // insn2->Pc = Decode_Queue->io_entry_out2_pc;


    // Decode_Queue->clock = 1;
    // Decode_Queue->eval();
    // r_avail_count = Decode_Queue ->io_available_entries;

    // insnPkg.emplace_back(insn1);
    // insnPkg.emplace_back(insn2);
    // this->m_StageOutPort->set(insnPkg);


    // bool insn1_error = ((insn1->data_valid != Decode_Queue->io_entry_out1_data_valid) 
    //                     ||((insn1->data_valid && Decode_Queue->io_entry_out1_data_valid)
    //                     && ((insn1->Excp.valid != Decode_Queue->io_entry_out1_Excp_valid)
    //                     || (insn1->Excp.Cause != Decode_Queue->io_entry_out1_Excp_Cause)
    //                     || (insn1->Excp.Tval != Decode_Queue->io_entry_out1_Excp_Tval)
    //                     || (insn1->Pred.Taken != Decode_Queue->io_entry_out1_Pred_Taken)
    //                     || (insn1->Pred.Target != Decode_Queue->io_entry_out1_Pred_Target)
    //                     || (insn1->SubOp != Decode_Queue->io_entry_out1_Sub_OP)
    //                     || (insn1->Fu != Emulator::funcType_t(Decode_Queue->io_entry_out1_Function_type))
    //                     || (insn1->ControlFlowInsn != Decode_Queue->io_entry_out1_ControlFlowInsn)
    //                     || (insn1->IsaRd != Decode_Queue->io_entry_out1_IsaRd)
    //                     || (insn1->Operand2Ready != Decode_Queue->io_entry_out1_Operand2Ready)
    //                     || (insn1->IsaRs2 != Decode_Queue->io_entry_out1_IsaRs2)
    //                     || (insn1->Operand1Ready != Decode_Queue->io_entry_out1_Operand1Ready)
    //                     || (insn1->IsaRs1 != Decode_Queue->io_entry_out1_IsaRs1)
    //                     || (insn1->imm != Decode_Queue->io_entry_out1_imm)
    //                     || (insn1->Operand2 != Decode_Queue->io_entry_out1_Operand2)
    //                     || (insn1->Operand1 != Decode_Queue->io_entry_out1_Operand1)
    //                     || (insn1->UncompressedInsn != Decode_Queue->io_entry_out1_insn)
    //                     || (insn1->Pc != Decode_Queue->io_entry_out1_pc))));

    // bool insn2_error = ((insn2->data_valid != Decode_Queue->io_entry_out2_data_valid) 
    //                 || ((insn2->data_valid && Decode_Queue->io_entry_out2_data_valid) 
    //                 && ((insn2->Excp.valid != Decode_Queue->io_entry_out2_Excp_valid)
    //                 || (insn2->Excp.Cause != Decode_Queue->io_entry_out2_Excp_Cause)
    //                 || (insn2->Excp.Tval != Decode_Queue->io_entry_out2_Excp_Tval)
    //                 || (insn2->Pred.Taken != Decode_Queue->io_entry_out2_Pred_Taken)
    //                 || (insn2->Pred.Target != Decode_Queue->io_entry_out2_Pred_Target)
    //                 || (insn2->SubOp != Decode_Queue->io_entry_out2_Sub_OP)
    //                 || (insn2->Fu != Emulator::funcType_t(Decode_Queue->io_entry_out2_Function_type))
    //                 || (insn2->ControlFlowInsn != Decode_Queue->io_entry_out2_ControlFlowInsn)
    //                 || (insn2->IsaRd != Decode_Queue->io_entry_out2_IsaRd)
    //                 || (insn2->Operand2Ready != Decode_Queue->io_entry_out2_Operand2Ready)
    //                 || (insn2->IsaRs2 != Decode_Queue->io_entry_out2_IsaRs2)
    //                 || (insn2->Operand1Ready != Decode_Queue->io_entry_out2_Operand1Ready)
    //                 || (insn2->IsaRs1 != Decode_Queue->io_entry_out2_IsaRs1)
    //                 || (insn2->imm != Decode_Queue->io_entry_out2_imm)
    //                 || (insn2->Operand2 != Decode_Queue->io_entry_out2_Operand2)
    //                 || (insn2->Operand1 != Decode_Queue->io_entry_out2_Operand1)
    //                 || (insn2->UncompressedInsn != Decode_Queue->io_entry_out2_insn)
    //                 || (insn2->Pc != Decode_Queue->io_entry_out2_pc))));
    // bool other_error = (r_avail_count != (decodeQueue_flush?0x40:Decode_Queue ->io_available_entries-WEN1-WEN2+pop_count));
    // if((insn1_error || insn2_error || other_error) && error_cnt < 100) {
    //         DPRINTF(temptest, "WEN1:{:x} WEN2:{:x} isStalled:{:x} decodeQueue_flush:{:x} pop_count:{:x}",WEN1,WEN2,isStalled,decodeQueue_flush,pop_count);
    //     if(insn1_error) {
    //         // DPRINTF(temptest, "Expected Values: data_valid:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x} Pred.Taken:{:#x} Pred.Target:{:#x} SubOp:{:#x} Fu:{:#x} ControlFlowInsn:{:#x} IsaRd:{:#x} Operand2Ready:{:#x} IsaRs2:{:#x} Operand1Ready:{:#x} IsaRs1:{:#x} imm:{:#x} Operand2:{:#x} Operand1:{:#x} UncompressedInsn:{:#x} Pc:{:#x}",
    //         //     insn1->data_valid, insn1->Excp.valid, insn1->Excp.Cause, insn1->Excp.Tval, insn1->Pred.Taken, insn1->Pred.Target, insn1->SubOp, insn1->Fu, insn1->ControlFlowInsn, insn1->IsaRd, insn1->Operand2Ready, insn1->IsaRs2, insn1->Operand1Ready, insn1->IsaRs1, insn1->imm, insn1->Operand2, insn1->Operand1, insn1->UncompressedInsn, insn1->Pc);
    //         // DPRINTF(temptest, "insn1 Error Detected:  data_valid:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x} Pred.Taken:{:#x} Pred.Target:{:#x} SubOp:{:#x} Fu:{:#x} ControlFlowInsn:{:#x} IsaRd:{:#x} Operand2Ready:{:#x} IsaRs2:{:#x} Operand1Ready:{:#x} IsaRs1:{:#x} imm:{:#x} Operand2:{:#x} Operand1:{:#x} UncompressedInsn:{:#x} Pc:{:#x}",
    //         //     Decode_Queue->io_entry_out1_data_valid, Decode_Queue->io_entry_out1_Excp_valid, Decode_Queue->io_entry_out1_Excp_Cause, Decode_Queue->io_entry_out1_Excp_Tval, Decode_Queue->io_entry_out1_Pred_Taken, Decode_Queue->io_entry_out1_Pred_Target, Decode_Queue->io_entry_out1_Sub_OP, Decode_Queue->io_entry_out1_Function_type, Decode_Queue->io_entry_out1_ControlFlowInsn, Decode_Queue->io_entry_out1_IsaRd, Decode_Queue->io_entry_out1_Operand2Ready, Decode_Queue->io_entry_out1_IsaRs2, Decode_Queue->io_entry_out1_Operand1Ready, Decode_Queue->io_entry_out1_IsaRs1, Decode_Queue->io_entry_out1_imm, Decode_Queue->io_entry_out1_Operand2, Decode_Queue->io_entry_out1_Operand1, Decode_Queue->io_entry_out1_insn, Decode_Queue->io_entry_out1_pc);
    //         DPRINTF(temptest, "Expected Values:     {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x}",
    //             insn1->data_valid, insn1->Excp.valid, insn1->Excp.Cause, insn1->Excp.Tval, insn1->Pred.Taken, insn1->Pred.Target, insn1->SubOp, insn1->Fu, insn1->ControlFlowInsn, insn1->IsaRd, insn1->Operand2Ready, insn1->IsaRs2, insn1->Operand1Ready, insn1->IsaRs1, insn1->imm, insn1->Operand2, insn1->Operand1, insn1->UncompressedInsn, insn1->Pc);
    //         DPRINTF(temptest, "insn1 Error Detected:{:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x}",
    //             Decode_Queue->io_entry_out1_data_valid, Decode_Queue->io_entry_out1_Excp_valid, Decode_Queue->io_entry_out1_Excp_Cause, Decode_Queue->io_entry_out1_Excp_Tval, Decode_Queue->io_entry_out1_Pred_Taken, Decode_Queue->io_entry_out1_Pred_Target, Decode_Queue->io_entry_out1_Sub_OP, Decode_Queue->io_entry_out1_Function_type, Decode_Queue->io_entry_out1_ControlFlowInsn, Decode_Queue->io_entry_out1_IsaRd, Decode_Queue->io_entry_out1_Operand2Ready, Decode_Queue->io_entry_out1_IsaRs2, Decode_Queue->io_entry_out1_Operand1Ready, Decode_Queue->io_entry_out1_IsaRs1, Decode_Queue->io_entry_out1_imm, Decode_Queue->io_entry_out1_Operand2, Decode_Queue->io_entry_out1_Operand1, Decode_Queue->io_entry_out1_insn, Decode_Queue->io_entry_out1_pc);
    //     }

    //     if(insn2_error) {
    //         // DPRINTF(temptest, "Expected Values: data_valid:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x} Pred.Taken:{:#x} Pred.Target:{:#x} SubOp:{:#x} Fu:{:#x} ControlFlowInsn:{:#x} IsaRd:{:#x} Operand2Ready:{:#x} IsaRs2:{:#x} Operand1Ready:{:#x} IsaRs1:{:#x} imm:{:#x} Operand2:{:#x} Operand1:{:#x} UncompressedInsn:{:#x} Pc:{:#x}",
    //         //     insn2->data_valid, insn2->Excp.valid, insn2->Excp.Cause, insn2->Excp.Tval, insn2->Pred.Taken, insn2->Pred.Target, insn2->SubOp, insn2->Fu, insn2->ControlFlowInsn, insn2->IsaRd, insn2->Operand2Ready, insn2->IsaRs2, insn2->Operand1Ready, insn2->IsaRs1, insn2->imm, insn2->Operand2, insn2->Operand1, insn2->UncompressedInsn, insn2->Pc);
    //         // DPRINTF(temptest, "insn2 Error Detected: data_valid:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x} Pred.Taken:{:#x} Pred.Target:{:#x} SubOp:{:#x} Fu:{:#x} ControlFlowInsn:{:#x} IsaRd:{:#x} Operand2Ready:{:#x} IsaRs2:{:#x} Operand1Ready:{:#x} IsaRs1:{:#x} imm:{:#x} Operand2:{:#x} Operand1:{:#x} UncompressedInsn:{:#x} Pc:{:#x}",
    //         //     Decode_Queue->io_entry_out2_data_valid, Decode_Queue->io_entry_out2_Excp_valid, Decode_Queue->io_entry_out2_Excp_Cause, Decode_Queue->io_entry_out2_Excp_Tval, Decode_Queue->io_entry_out2_Pred_Taken, Decode_Queue->io_entry_out2_Pred_Target, Decode_Queue->io_entry_out2_Sub_OP, Decode_Queue->io_entry_out2_Function_type, Decode_Queue->io_entry_out2_ControlFlowInsn, Decode_Queue->io_entry_out2_IsaRd, Decode_Queue->io_entry_out2_Operand2Ready, Decode_Queue->io_entry_out2_IsaRs2, Decode_Queue->io_entry_out2_Operand1Ready, Decode_Queue->io_entry_out2_IsaRs1, Decode_Queue->io_entry_out2_imm, Decode_Queue->io_entry_out2_Operand2, Decode_Queue->io_entry_out2_Operand1, Decode_Queue->io_entry_out2_insn, Decode_Queue->io_entry_out2_pc);
    //         DPRINTF(temptest, "Expected Values:     {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x}",
    //             insn2->data_valid, insn2->Excp.valid, insn2->Excp.Cause, insn2->Excp.Tval, insn2->Pred.Taken, insn2->Pred.Target, insn2->SubOp, insn2->Fu, insn2->ControlFlowInsn, insn2->IsaRd, insn2->Operand2Ready, insn2->IsaRs2, insn2->Operand1Ready, insn2->IsaRs1, insn2->imm, insn2->Operand2, insn2->Operand1, insn2->UncompressedInsn, insn2->Pc);
    //         DPRINTF(temptest, "insn2 Error Detected:{:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x}",
    //             Decode_Queue->io_entry_out2_data_valid, Decode_Queue->io_entry_out2_Excp_valid, Decode_Queue->io_entry_out2_Excp_Cause, Decode_Queue->io_entry_out2_Excp_Tval, Decode_Queue->io_entry_out2_Pred_Taken, Decode_Queue->io_entry_out2_Pred_Target, Decode_Queue->io_entry_out2_Sub_OP, Decode_Queue->io_entry_out2_Function_type, Decode_Queue->io_entry_out2_ControlFlowInsn, Decode_Queue->io_entry_out2_IsaRd, Decode_Queue->io_entry_out2_Operand2Ready, Decode_Queue->io_entry_out2_IsaRs2, Decode_Queue->io_entry_out2_Operand1Ready, Decode_Queue->io_entry_out2_IsaRs1, Decode_Queue->io_entry_out2_imm, Decode_Queue->io_entry_out2_Operand2, Decode_Queue->io_entry_out2_Operand1, Decode_Queue->io_entry_out2_insn, Decode_Queue->io_entry_out2_pc);
    //     }
    //     if(other_error){
    //         DPRINTF(temptest, "r_avail_count:{:#x}",r_avail_count);
    //         DPRINTF(temptest, "io_available_entries:{:#x}",decodeQueue_flush?0x40:Decode_Queue ->io_available_entries-WEN1-WEN2+pop_count);
    //     }
    //     DPRINTF(temptest, "--------------");
    //     error_cnt += 1;
    // }

}

void 
Decode::DecodeInsn(DecodeInsn_t& insn){
    RISCV::Decode_Func(*insn.get());
//     Emulator::DynInsn& inst_ptr = *insn.get();
//     VDecode_Func *Decode_Func ;
//     Decode_Func = new VDecode_Func;
//     Decode_Func->io_Pc = inst_ptr.Pc;
//     Decode_Func ->io_Insn = inst_ptr.UncompressedInsn;
//     Decode_Func->io_predecode_excp_valid = inst_ptr.Excp.valid;
//     Decode_Func->io_predecode_excp_cause = inst_ptr.Excp.Cause;
//     Decode_Func->io_predecode_excpt_tval = inst_ptr.Excp.Tval;
//     Decode_Func->eval();
    
//     inst_ptr.IsaRs1 = Decode_Func ->io_IsaRs1;
//     inst_ptr.Operand1 = Decode_Func->io_Operand1;
//     inst_ptr.Operand1Ready = Decode_Func ->io_Operand1Ready;
//     inst_ptr.IsaRs2 = Decode_Func ->io_IsaRs2;
//     inst_ptr.Operand2 = Decode_Func->io_Operand2;
//     inst_ptr.Operand2Ready= Decode_Func ->io_Operand2Ready;
//     inst_ptr.IsaRd = Decode_Func ->io_IsaRd;
//     inst_ptr.ControlFlowInsn = Decode_Func ->io_ControlFlowInsn;
//     inst_ptr.Fu = Emulator::funcType_t(Decode_Func ->io_Function_type);
//     inst_ptr.SubOp = Decode_Func ->io_Sub_OP;
//     inst_ptr.imm = int32_t(Decode_Func ->io_imm);
//     inst_ptr.Excp.valid = Decode_Func ->io_excp_valid;
//     inst_ptr.Excp.Cause = Decode_Func ->io_excp_cause;
//     inst_ptr.Excp.Tval = Decode_Func ->io_excpt_tval;
    // bool error_occur = ((inst_ptr.IsaRs1 != Decode_Func->io_IsaRs1)
    // || (inst_ptr.Operand1 != Decode_Func->io_Operand1)
    // || (inst_ptr.Operand1Ready != Decode_Func->io_Operand1Ready)
    // || (inst_ptr.IsaRs2 != Decode_Func->io_IsaRs2)
    // || (inst_ptr.Operand2 != Decode_Func->io_Operand2)
    // || (inst_ptr.Operand2Ready != Decode_Func->io_Operand2Ready)
    // || (inst_ptr.IsaRd != Decode_Func->io_IsaRd)
    // || (inst_ptr.ControlFlowInsn != Decode_Func->io_ControlFlowInsn)
    // || (inst_ptr.Fu != Emulator::funcType_t(Decode_Func->io_Function_type))
    // || (inst_ptr.SubOp != Decode_Func->io_Sub_OP)
    // || (inst_ptr.imm != int32_t( Decode_Func->io_imm))
    // || (inst_ptr.Excp.valid != Decode_Func->io_excp_valid)
    // || (inst_ptr.Excp.Cause != Decode_Func->io_excp_cause)
    // || (inst_ptr.Excp.Tval != Decode_Func->io_excpt_tval));
    // if (error_occur && error_cnt < 100) {
    // DPRINTF(temptest, "True: {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} ",
    //     inst_ptr.IsaRs1,inst_ptr.Operand1, inst_ptr.Operand1Ready, inst_ptr.IsaRs2, inst_ptr.Operand2, inst_ptr.Operand2Ready, inst_ptr.IsaRd, inst_ptr.ControlFlowInsn, inst_ptr.Fu, inst_ptr.SubOp, inst_ptr.imm, inst_ptr.Excp.valid, inst_ptr.Excp.Cause, inst_ptr.Excp.Tval);
    // DPRINTF(temptest, "False:{:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} {:#x} ",
    //     Decode_Func->io_IsaRs1, Decode_Func->io_Operand1, Decode_Func->io_Operand1Ready, Decode_Func->io_IsaRs2, Decode_Func->io_Operand2, Decode_Func->io_Operand2Ready, Decode_Func->io_IsaRd, Decode_Func->io_ControlFlowInsn, Decode_Func->io_Function_type, Decode_Func->io_Sub_OP, int32_t(Decode_Func->io_imm), Decode_Func->io_excp_valid, Decode_Func->io_excp_cause, Decode_Func->io_excpt_tval);

    // DPRINTF(temptest, "--------------");
    // error_cnt += 1;
    // }
}

void 
Decode::Predecode(Emulator::DynInsn& insnEntry,DecodeInsn_t& insn1,DecodeInsn_t& insn2,Redirect_message& redirect_message,bool& flush_flag){
    redirect_message.valid=0;
    redirect_message.target=0;
    Redirect_message redirect_message1,redirect_message2;
    redirect_message1.valid=0;
    redirect_message1.target=0;
    redirect_message2.valid=0;
    redirect_message2.target=0;

    uint64_t offset  = insnEntry.Address & (this->m_iCacheAlignByte-1); //看当前的地址是否对齐
    char* dataPtr = insnEntry.InsnByte.data();  
    if(offset==0){
    //insn1->send_valid        = false;
    insn1->data_valid        = true;
    insn1->Pc                = insnEntry.Address;
    insn1->Excp              = insnEntry.Excp;
    insn1->UncompressedInsn  = *(uint32_t*)dataPtr;
    this->BranchRedirect(insn1,redirect_message1);
    }
    else{
        insn1->data_valid        = false;
    }
    
    if(!(offset==0 && redirect_message1.valid)){
    // insn2->send_valid        = false;
        insn2->data_valid        = true;
        insn2->Pc                = insn1->data_valid?insnEntry.Address+4:insnEntry.Address;//
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


    // predecode->io_Address = insnEntry.Address;
    // predecode->io_data = *(uint64_t*)insnEntry.InsnByte.data();
    // predecode->io_excp_valid = insnEntry.Excp.valid;
    // predecode->io_excp_cause = insnEntry.Excp.Cause;
    // predecode->io_excpt_tval = insnEntry.Excp.Tval;
    // predecode->eval();
    // insn1->data_valid        = predecode->io_insn_valid1;
    // insn1->Pc                = predecode->io_pc1;
    // insn1->Excp.valid        = predecode->io_excp_valid1;
    // insn1->Excp.Cause        = predecode->io_excp_cause1;
    // insn1->Excp.Tval         = predecode->io_excpt_tval1;
    // insn1->UncompressedInsn  = predecode->io_insn1;

    // insn2->data_valid        = predecode->io_insn_valid2;
    // insn2->Pc                = predecode->io_pc2;
    // insn2->Excp.valid        = predecode->io_excp_valid2;
    // insn2->Excp.Cause        = predecode->io_excp_cause2;
    // insn2->Excp.Tval        = predecode->io_excpt_tval2;
    // insn2->UncompressedInsn  = predecode->io_insn2;

    // redirect_message.valid = predecode->io_fetch_flush;
    // redirect_message.target = predecode->io_Redirect_Target;

    // flush_flag = predecode->io_fetch_flush;


    // bool error_occur = ((insn1->data_valid != predecode->io_insn_valid1)
    //                 || ((insn1->data_valid && predecode->io_insn_valid1)
    //                 && ((insn1->Pc != predecode->io_pc1)
    //                 || (insn1->Excp.valid != predecode->io_excp_valid1)
    //                 || (insn1->Excp.Cause != predecode->io_excp_cause1)
    //                 || (insn1->Excp.Tval != predecode->io_excpt_tval1)
    //                 || (insn1->UncompressedInsn != predecode->io_insn1)))
    //                 || (insn2->data_valid != predecode->io_insn_valid2)
    //                 || ((insn2->data_valid && predecode->io_insn_valid2)
    //                 && ((insn2->Pc != predecode->io_pc2)
    //                 || (insn2->Excp.valid != predecode->io_excp_valid2)
    //                 || (insn2->Excp.Cause != predecode->io_excp_cause2)
    //                 || (insn2->Excp.Tval != predecode->io_excpt_tval2)
    //                 || (insn2->UncompressedInsn != predecode->io_insn2)))
    //                 || (redirect_message.valid != predecode->io_fetch_flush)
    //                 || (redirect_message.target != predecode->io_Redirect_Target)
    //                 || (flush_flag != predecode->io_fetch_flush));

    // if(error_occur && error_cnt < 100){
    // DPRINTF(temptest,"True: data_valid1: {:#x} Pc1: {:#x} Excp.valid1: {:#x} Excp.Cause1: {:#x} Excp.Tval1: {:#x} UncompressedInsn1: {:#x}",
    // insn1->data_valid, insn1->Pc, insn1->Excp.valid, insn1->Excp.Cause, insn1->Excp.Tval, insn1->UncompressedInsn);
    // DPRINTF(temptest,"False: io_insn_valid1: {:#x} io_pc1: {:#x} io_excp_valid1: {:#x} io_excp_cause1: {:#x} io_excpt_tval1: {:#x} io_insn1: {:#x}",
    // predecode->io_insn_valid1, predecode->io_pc1, predecode->io_excp_valid1, predecode->io_excp_cause1, predecode->io_excpt_tval1, predecode->io_insn1);
    // DPRINTF(temptest,"True: data_valid2: {:#x} Pc2: {:#x} Excp.valid2: {:#x} Excp.Cause2: {:#x} Excp.Tval2: {:#x} UncompressedInsn2: {:#x}",
    // insn2->data_valid, insn2->Pc, insn2->Excp.valid, insn2->Excp.Cause, insn2->Excp.Tval, insn2->UncompressedInsn);
    // DPRINTF(temptest,"False: io_insn_valid2: {:#x} io_pc2: {:#x} io_excp_valid2: {:#x} io_excp_cause2: {:#x} io_excpt_tval2: {:#x} io_insn2: {:#x}",
    // predecode->io_insn_valid2, predecode->io_pc2, predecode->io_excp_valid2, predecode->io_excp_cause2, predecode->io_excpt_tval2, predecode->io_insn2);
    // DPRINTF(temptest,"True: valid: {:#x} target: {:#x}",
    // redirect_message.valid, redirect_message.target);
    // DPRINTF(temptest,"False: io_fetch_flush: {:#x} io_Redirect_Target: {:#x}",
    // predecode->io_fetch_flush, predecode->io_Redirect_Target);
    // DPRINTF(temptest,"True: flush_flag: {:#x}", flush_flag);
    // DPRINTF(temptest,"False: io_fetch_flush: {:#x}", predecode->io_fetch_flush);
    // DPRINTF(temptest,"--------------");
    // error_cnt+=1;
    // }


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
    // VBranchRedirect *BranchRedirect;
    // BranchRedirect = new VBranchRedirect;
    // BranchRedirect->io_Insn = insn->UncompressedInsn;
    // BranchRedirect->io_Insn_PC = insn->Pc;
    // insn->Pred.Taken = BranchRedirect->io_Pred_Taken;
    // insn->Pred.Target = BranchRedirect->io_Pred_Target;
    // BranchRedirect->eval();
    // RedirectReq.StageId = InsnState_t(BranchRedirect->io_Redirect_ID);
    // RedirectReq.target = BranchRedirect->io_Redirect_Target;
    // needRedirect = BranchRedirect->io_needRedirect;
    // bool error_occur = ((RedirectReq.StageId != InsnState_t(BranchRedirect->io_Redirect_ID)) 
    //             || (RedirectReq.target != BranchRedirect->io_Redirect_Target))
    //             && (needRedirect != BranchRedirect->io_needRedirect);
    // if(error_occur && error_cnt < 100){
    //     DPRINTF(temptest,"True: operand1:{:#x} operand2:{:#x} operand3:{:#x}",
    //         RedirectReq.StageId,RedirectReq.target,needRedirect);
    //     DPRINTF(temptest,"Flase: operand1:{:#x} operand2:{:#x} operand3:{:#x}",
    //     InsnState_t(BranchRedirect->io_Redirect_ID),BranchRedirect->io_Redirect_Target,BranchRedirect->io_needRedirect);
    //     DPRINTF(temptest,"--------------");
    //     error_cnt+=1;
    // }
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
