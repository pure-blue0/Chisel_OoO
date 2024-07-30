#include "Decode.hh"
#include "../../RISCV/Decoder.hh"
#include "../Processor.hh"
#include "../Component/Rcu.hh"

// #include <verilated.h>          // Defines common routines
// #include "./obj_dir/VPredecode.h"
// #include "./obj_dir/VBranchRedirect.h"
// #include "./obj_dir/VDecodeQueue.h"
namespace Emulator
{
// size_t error_cnt = 0;   
// VDecodeQueue *Decode_Queue = new VDecodeQueue;   

Decode::Decode( 
    Processor*          processor            ,
    const   uint64_t    DecodeQueueSize      
):  BaseStage(processor,"Decode"),
    m_DecodeQueue("DecodeQueue",DecodeQueueSize),
    m_iCacheAlignByte(16),
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
    DecodeInsn_t insn3          = this->m_Processor->CreateDecodeInsn();
    DecodeInsn_t insn4          = this->m_Processor->CreateDecodeInsn();
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
    if(Predcode_EN)this->Predecode(*this->m_StageInPort->data[0].get(),insn1,insn2,insn3,insn4,redirect_message,flush_flag); 
    else{
        insn1->data_valid=false;
        insn2->data_valid=false;
        insn3->data_valid=false;
        insn4->data_valid=false;
    }
    this->m_Processor->getFetch1Ptr()->Decode_Redirect_Reg->InPort->set(redirect_message);
    if(flush_flag){
        //DPRINTF(Redirect,"Pc[{:#x}] -> Predict Mismatch, Redirect to {:#x}",insn1->Pc,RedirectReq.target);
        this->m_Processor->FlushBackWard(InsnState_t::State_Decode);//冲刷fetch1_flush
    }   
    //-----------------------------------------------------------------//
    this->DecodeInsn(insn1);
    this->DecodeInsn(insn2);
    this->DecodeInsn(insn3);
    this->DecodeInsn(insn4);
    //-----------------------------------------------------------------//
    this->DecodeQueue_Evaluate(reset_n,this->decodeQueue_flush,this->m_StageAckInPort->data.takenInsnCount,insn1->data_valid,insn1,insn2->data_valid,insn2,insn3->data_valid,insn3,insn4->data_valid,insn4,this->m_StageOutPort->isStalled(),avail_count);
}


void 
Decode::Reset(){
    this->m_FlushSyncLatch.reset();
    this->m_DecodeQueue.Reset();
    // Decode_Queue->clock = 0;
    // Decode_Queue->reset = 0;
    // Decode_Queue->eval();
    // Decode_Queue->reset = 1;
    // Decode_Queue->clock = 1;
    // Decode_Queue->reset = 0;
}

void 
Decode::FlushAction(){
    this->decodeQueue_flush=true;
    // Decode_Queue->clock = 0;
    // Decode_Queue->reset = 0;
    // Decode_Queue->eval();
    // Decode_Queue->reset = 1;
    // Decode_Queue->clock = 1;
    // Decode_Queue->reset = 0;
}

void
Decode::ReleaseAction(){  
}


// void insn_package(uint32_t io_decoded_insn[12], DecodeInsn_t insn_WriteIn){
//     uint32_t temp = 0;
//     io_decoded_insn[0] = insn_WriteIn->Pc;  
//     io_decoded_insn[1] = insn_WriteIn->UncompressedInsn;
//     io_decoded_insn[2] = insn_WriteIn->Operand1 & 0xFFFFFFFF;
//     io_decoded_insn[3] = (insn_WriteIn->Operand1 >> 32) & 0xFFFFFFFF;
//     io_decoded_insn[4] = insn_WriteIn->Operand2 & 0xFFFFFFFF;
//     io_decoded_insn[5] = (insn_WriteIn->Operand2 >> 32) & 0xFFFFFFFF;
//     io_decoded_insn[6] = insn_WriteIn->imm & 0xFFFFFFFF;

//     temp |= (insn_WriteIn->IsaRs1 & 0x1F);                    // [4:0]
//     temp |= (insn_WriteIn->Operand1Ready ? 1 : 0) << 5;      // 1 bit
//     temp |= (insn_WriteIn->IsaRs2 & 0x1F) << 6;               // [4:0]
//     temp |= (insn_WriteIn->Operand2Ready ? 1 : 0) << 11;     // 1 bit
//     temp |= (insn_WriteIn->IsaRd & 0x1F) << 12;               // [4:0]
//     temp |= (insn_WriteIn->ControlFlowInsn ? 1 : 0) << 17;   // 1 bit
//     temp |= (insn_WriteIn->Fu & 0x07) << 18;                // [2:0]
//     temp |= (insn_WriteIn->SubOp & 0x0F) << 21;             // [3:0]
//     temp |= (insn_WriteIn->Excp.valid ? 1 : 0) << 25;        // 1 bit
//     temp |= (insn_WriteIn->Excp.Cause & 0x3F) << 26;         // [5:0]
//     io_decoded_insn[7] = temp;   

//     io_decoded_insn[8] = insn_WriteIn->Excp.Tval;             // [31:0]
//     io_decoded_insn[9] = insn_WriteIn->Pred.Target;             // [31:0]
//     temp = 0;
//     temp |= (insn_WriteIn->Pred.Taken ? 1 : 0) ;            // 1 bit
//     temp |= (insn_WriteIn->data_valid ? 1 : 0) << 1;            // 1 bit
//     io_decoded_insn[10] = temp;

// }


// void insn_depackage(uint32_t io_entry_out[12], InsnPtr_t &insn) {
//     insn->Pc = io_entry_out[0];
//     insn->UncompressedInsn = io_entry_out[1];
//     insn->Operand1 = (static_cast<uint64_t>(io_entry_out[3]) << 32) | (io_entry_out[2]);
//     insn->Operand2 = (static_cast<uint64_t>(io_entry_out[5]) << 32) | (io_entry_out[4]);
//     insn->imm = io_entry_out[6];

//     uint32_t temp = io_entry_out[7];
//     insn->IsaRs1 = temp & 0x1F;                          // [4:0]
//     insn->Operand1Ready = (temp >> 5) & 0x01;          // 1 bit
//     insn->IsaRs2 = (temp >> 6) & 0x1F;                  // [4:0]
//     insn->Operand2Ready = (temp >> 11) & 0x01;         // 1 bit
//     insn->IsaRd = (temp >> 12) & 0x1F;                  // [4:0]
//     insn->ControlFlowInsn = (temp >> 17) & 0x01;       // 1 bit
//     insn->Fu = Emulator::funcType_t((temp >> 18) & 0x07);                     // [2:0]
//     insn->SubOp = (temp >> 21) & 0x0F;                  // [3:0]
//     insn->Excp.valid = (temp >> 25) & 0x01;            // 1 bit
//     insn->Excp.Cause = (temp >> 26) & 0x3F;             // [5:0]

//     insn->Excp.Tval = io_entry_out[8];                   // [31:0]
//     insn->Pred.Target = io_entry_out[9];                 // [31:0]
    
//     temp = io_entry_out[10];
//     insn->Pred.Taken = temp & 0x01;                     // 1 bit
//     insn->data_valid = (temp >> 1) & 0x01;              // 1 bit
// }


void Decode::DecodeQueue_Evaluate(bool reset_n,bool decodeQueue_flush,uint8_t pop_count,bool WEN1,DecodeInsn_t insn1_WriteIn,bool WEN2,DecodeInsn_t insn2_WriteIn,bool WEN3,DecodeInsn_t insn3_WriteIn,bool WEN4,DecodeInsn_t insn4_WriteIn,bool isStalled,uint8_t& r_avail_count){
    static DecodeQueue_entry DecodeQueue[DecodeQueue_Size];
    static uint8_t header_ptr,tail_ptr;
    static uint8_t usage_count;
    static uint8_t avail_count;

    bool empty,send_two_entry_flag,send_three_entry_flag,send_four_entry_flag;
    InsnPkg_t insnPkg;
    InsnPtr_t insn1          = this->m_Processor->CreateInsn();
    InsnPtr_t insn2          = this->m_Processor->CreateInsn();
    InsnPtr_t insn3          = this->m_Processor->CreateInsn();
    InsnPtr_t insn4          = this->m_Processor->CreateInsn();
    
    size_t ISSUEWIDTH = 4;
    InsnPtr_t insn[ISSUEWIDTH]; //= {this->m_Processor->CreateInsn(),this->m_Processor->CreateInsn(),this->m_Processor->CreateInsn(),this->m_Processor->CreateInsn()};
    for(size_t i = 0;i < ISSUEWIDTH;i++){
        insn[i] = this->m_Processor->CreateInsn();
    }

    if(!reset_n||decodeQueue_flush){
        for(int i=0;i<DecodeQueue_Size;i++){
        DecodeQueue->reset();
        }
        header_ptr=0;
        tail_ptr=0;
        usage_count=0;
        avail_count=DecodeQueue_Size;
        for(size_t i = 0;i < ISSUEWIDTH;i++){
            insn[i]->data_valid = false;
        }
    }
    else{
        size_t sub_temp = usage_count-pop_count>0?usage_count-pop_count:0;
        for(size_t i = 0;i < ISSUEWIDTH;i++){
            if(!isStalled && i<sub_temp){
                size_t header_ptr_next = (header_ptr+pop_count+i)%DecodeQueue_Size;
                insn[i]->data_valid = DecodeQueue[header_ptr_next].data_valid;
                insn[i]->Pc = DecodeQueue[header_ptr_next].Pc;
                insn[i]->Fu = DecodeQueue[header_ptr_next].Fu;
                insn[i]->SubOp = DecodeQueue[header_ptr_next].SubOp;
                insn[i]->UncompressedInsn = DecodeQueue[header_ptr_next].UncompressedInsn;
                insn[i]->ControlFlowInsn = DecodeQueue[header_ptr_next].ControlFlowInsn;
                insn[i]->IsaRs1 = DecodeQueue[header_ptr_next].IsaRs1;
                insn[i]->IsaRs2 = DecodeQueue[header_ptr_next].IsaRs2;
                insn[i]->IsaRd = DecodeQueue[header_ptr_next].IsaRd;
                insn[i]->Operand1Ready = DecodeQueue[header_ptr_next].Operand1Ready;
                insn[i]->Operand2Ready = DecodeQueue[header_ptr_next].Operand2Ready;
                insn[i]->Operand1 = DecodeQueue[header_ptr_next].Operand1;
                insn[i]->Operand2 = DecodeQueue[header_ptr_next].Operand2;
                insn[i]->imm = DecodeQueue[header_ptr_next].imm;
                insn[i]->Excp = DecodeQueue[header_ptr_next].Excp;
                insn[i]->Pred = DecodeQueue[header_ptr_next].Pred;
            }else{
                insn[i]->data_valid = false;
            }
            insnPkg.emplace_back(insn[i]);
        }

        if(WEN1){
            DecodeQueue[tail_ptr]=*insn1_WriteIn.get();
        }
        if(WEN2){
            DecodeQueue[(tail_ptr+WEN1)%DecodeQueue_Size]=*insn2_WriteIn.get();
        }
        if(WEN3){
            DecodeQueue[(tail_ptr+WEN1+WEN2)%DecodeQueue_Size]=*insn3_WriteIn.get();
        }
        if(WEN4){
            DecodeQueue[(tail_ptr+WEN1+WEN2+WEN3)%DecodeQueue_Size]=*insn4_WriteIn.get();    
        }
        header_ptr = (header_ptr+pop_count)%DecodeQueue_Size;
        tail_ptr = (tail_ptr + WEN1 + WEN2 + WEN3 + WEN4)%DecodeQueue_Size;
        usage_count = usage_count - pop_count + WEN1 + WEN2 + WEN3 + WEN4;      
        avail_count=DecodeQueue_Size-usage_count; 
    }
    r_avail_count=avail_count;
    this->m_StageOutPort->set(insnPkg); 

    // Inputs
    // InsnPkg_t insnPkg;
    // InsnPtr_t insn1_temp          = this->m_Processor->CreateInsn();
    // InsnPtr_t insn2_temp          = this->m_Processor->CreateInsn();
    // InsnPtr_t insn3_temp          = this->m_Processor->CreateInsn();
    // InsnPtr_t insn4_temp          = this->m_Processor->CreateInsn();
    // Decode_Queue->io_reset_n = reset_n;
    // Decode_Queue->io_decode_flush_flag = decodeQueue_flush;
    // insn_package(Decode_Queue->io_decoded_insn1,insn1_WriteIn);
    // insn_package(Decode_Queue->io_decoded_insn2,insn2_WriteIn);
    // insn_package(Decode_Queue->io_decoded_insn3,insn3_WriteIn);
    // insn_package(Decode_Queue->io_decoded_insn4,insn4_WriteIn);
    // Decode_Queue->io_WEN1 = WEN1;
    // Decode_Queue->io_WEN2 = WEN2;
    // Decode_Queue->io_WEN3 = WEN3;
    // Decode_Queue->io_WEN4 = WEN4;
    // Decode_Queue->io_isStalled = isStalled;
    // Decode_Queue->io_pop_count = pop_count;
    // Decode_Queue->clock = 0;
    // Decode_Queue->eval();
    // //outputs
    // insn_depackage(Decode_Queue->io_entry_out1,insn1_temp);
    // insn_depackage(Decode_Queue->io_entry_out2,insn2_temp);
    // insn_depackage(Decode_Queue->io_entry_out3,insn3_temp);
    // insn_depackage(Decode_Queue->io_entry_out4,insn4_temp);

    // Decode_Queue->clock = 1;
    // Decode_Queue->eval();
    // r_avail_count = Decode_Queue->io_available_entries;
    // insnPkg.emplace_back(insn1_temp);
    // insnPkg.emplace_back(insn2_temp);
    // insnPkg.emplace_back(insn3_temp);
    // insnPkg.emplace_back(insn4_temp);

    // this->m_StageOutPort->set(insnPkg); 



    // // Monitor code output
    // bool insn1_error_occur = (insn[0]->data_valid != insn1_temp->data_valid) ||
    //                     ((insn[0]->data_valid && insn1_temp->data_valid) &&
    //                     ((insn[0]->Pc != insn1_temp->Pc) ||
    //                     (insn[0]->Fu != insn1_temp->Fu) ||
    //                     (insn[0]->SubOp != insn1_temp->SubOp) ||
    //                     (insn[0]->UncompressedInsn != insn1_temp->UncompressedInsn) ||
    //                     (insn[0]->ControlFlowInsn != insn1_temp->ControlFlowInsn) ||
    //                     (insn[0]->IsaRs1 != insn1_temp->IsaRs1) ||
    //                     (insn[0]->IsaRs2 != insn1_temp->IsaRs2) ||
    //                     (insn[0]->IsaRd != insn1_temp->IsaRd) ||
    //                     (insn[0]->Operand1Ready != insn1_temp->Operand1Ready) ||
    //                     (insn[0]->Operand2Ready != insn1_temp->Operand2Ready) ||
    //                     (insn[0]->Operand1 != insn1_temp->Operand1) ||
    //                     (insn[0]->Operand2 != insn1_temp->Operand2) ||
    //                     (insn[0]->imm != insn1_temp->imm) ||
    //                     (insn[0]->Excp.valid != insn1_temp->Excp.valid) ||
    //                     (insn[0]->Excp.Cause != insn1_temp->Excp.Cause) ||
    //                     (insn[0]->Excp.Tval != insn1_temp->Excp.Tval) ||
    //                     (insn[0]->Pred.Taken != insn1_temp->Pred.Taken) ||
    //                     (insn[0]->Pred.Target != insn1_temp->Pred.Target)));

    // bool insn2_error_occur = (insn[1]->data_valid != insn2_temp->data_valid) ||
    //                     ((insn[1]->data_valid && insn2_temp->data_valid) &&
    //                     ((insn[1]->Pc != insn2_temp->Pc) ||
    //                     (insn[1]->Fu != insn2_temp->Fu) ||
    //                     (insn[1]->SubOp != insn2_temp->SubOp) ||
    //                     (insn[1]->UncompressedInsn != insn2_temp->UncompressedInsn) ||
    //                     (insn[1]->ControlFlowInsn != insn2_temp->ControlFlowInsn) ||
    //                     (insn[1]->IsaRs1 != insn2_temp->IsaRs1) ||
    //                     (insn[1]->IsaRs2 != insn2_temp->IsaRs2) ||
    //                     (insn[1]->IsaRd != insn2_temp->IsaRd) ||
    //                     (insn[1]->Operand1Ready != insn2_temp->Operand1Ready) ||
    //                     (insn[1]->Operand2Ready != insn2_temp->Operand2Ready) ||
    //                     (insn[1]->Operand1 != insn2_temp->Operand1) ||
    //                     (insn[1]->Operand2 != insn2_temp->Operand2) ||
    //                     (insn[1]->imm != insn2_temp->imm) ||
    //                     (insn[1]->Excp.valid != insn2_temp->Excp.valid) ||
    //                     (insn[1]->Excp.Cause != insn2_temp->Excp.Cause) ||
    //                     (insn[1]->Excp.Tval != insn2_temp->Excp.Tval) ||
    //                     (insn[1]->Pred.Taken != insn2_temp->Pred.Taken) ||
    //                     (insn[1]->Pred.Target != insn2_temp->Pred.Target)));

    // bool insn3_error_occur = (insn[2]->data_valid != insn3_temp->data_valid) ||
    //                     ((insn[2]->data_valid && insn3_temp->data_valid) &&
    //                     ((insn[2]->Pc != insn3_temp->Pc) ||
    //                     (insn[2]->Fu != insn3_temp->Fu) ||
    //                     (insn[2]->SubOp != insn3_temp->SubOp) ||
    //                     (insn[2]->UncompressedInsn != insn3_temp->UncompressedInsn) ||
    //                     (insn[2]->ControlFlowInsn != insn3_temp->ControlFlowInsn) ||
    //                     (insn[2]->IsaRs1 != insn3_temp->IsaRs1) ||
    //                     (insn[2]->IsaRs2 != insn3_temp->IsaRs2) ||
    //                     (insn[2]->IsaRd != insn3_temp->IsaRd) ||
    //                     (insn[2]->Operand1Ready != insn3_temp->Operand1Ready) ||
    //                     (insn[2]->Operand2Ready != insn3_temp->Operand2Ready) ||
    //                     (insn[2]->Operand1 != insn3_temp->Operand1) ||
    //                     (insn[2]->Operand2 != insn3_temp->Operand2) ||
    //                     (insn[2]->imm != insn3_temp->imm) ||
    //                     (insn[2]->Excp.valid != insn3_temp->Excp.valid) ||
    //                     (insn[2]->Excp.Cause != insn3_temp->Excp.Cause) ||
    //                     (insn[2]->Excp.Tval != insn3_temp->Excp.Tval) ||
    //                     (insn[2]->Pred.Taken != insn3_temp->Pred.Taken) ||
    //                     (insn[2]->Pred.Target != insn3_temp->Pred.Target)));

    // bool insn4_error_occur = (insn[3]->data_valid != insn4_temp->data_valid) ||
    //                     ((insn[3]->data_valid && insn4_temp->data_valid) &&
    //                     ((insn[3]->Pc != insn4_temp->Pc) ||
    //                     (insn[3]->Fu != insn4_temp->Fu) ||
    //                     (insn[3]->SubOp != insn4_temp->SubOp) ||
    //                     (insn[3]->UncompressedInsn != insn4_temp->UncompressedInsn) ||
    //                     (insn[3]->ControlFlowInsn != insn4_temp->ControlFlowInsn) ||
    //                     (insn[3]->IsaRs1 != insn4_temp->IsaRs1) ||
    //                     (insn[3]->IsaRs2 != insn4_temp->IsaRs2) ||
    //                     (insn[3]->IsaRd != insn4_temp->IsaRd) ||
    //                     (insn[3]->Operand1Ready != insn4_temp->Operand1Ready) ||
    //                     (insn[3]->Operand2Ready != insn4_temp->Operand2Ready) ||
    //                     (insn[3]->Operand1 != insn4_temp->Operand1) ||
    //                     (insn[3]->Operand2 != insn4_temp->Operand2) ||
    //                     (insn[3]->imm != insn4_temp->imm) ||
    //                     (insn[3]->Excp.valid != insn4_temp->Excp.valid) ||
    //                     (insn[3]->Excp.Cause != insn4_temp->Excp.Cause) ||
    //                     (insn[3]->Excp.Tval != insn4_temp->Excp.Tval) ||
    //                     (insn[3]->Pred.Taken != insn4_temp->Pred.Taken) ||
    //                     (insn[3]->Pred.Target != insn4_temp->Pred.Target)));

    // bool avail_count_error_occur = (r_avail_count != Decode_Queue->io_available_entries);
    // bool error_occur = insn1_error_occur || insn2_error_occur || insn3_error_occur || insn4_error_occur || avail_count_error_occur;
    // if (error_occur && error_cnt < 100) {
    //     DPRINTF(temptest, "reset_n:{:x}, decodeQueue_flush:{:x}, WEN1:{:x}, WEN2:{:x}, WEN3:{:x}, WEN4:{:x}, isStalled:{:x}, pop_count:{:x}",
    //     reset_n, decodeQueue_flush, WEN1, WEN2, WEN3, WEN4, isStalled, pop_count);
    //     if(insn1_error_occur){
    //         DPRINTF(temptest, "Error in insn[0]:    data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //             insn[0]->data_valid, insn[0]->Pc, insn[0]->Fu, insn[0]->SubOp, insn[0]->UncompressedInsn, insn[0]->ControlFlowInsn, 
    //             insn[0]->IsaRs1, insn[0]->IsaRs2, insn[0]->IsaRd, insn[0]->Operand1Ready, insn[0]->Operand2Ready, insn[0]->Operand1, 
    //             insn[0]->Operand2, insn[0]->imm, insn[0]->Excp.valid, insn[0]->Excp.Cause, insn[0]->Excp.Tval, insn[0]->Pred.Taken, insn[0]->Pred.Target);
    //         DPRINTF(temptest, "Expected insn1_temp: data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //                 insn1_temp->data_valid, insn1_temp->Pc, insn1_temp->Fu, insn1_temp->SubOp, insn1_temp->UncompressedInsn, insn1_temp->ControlFlowInsn, 
    //                 insn1_temp->IsaRs1, insn1_temp->IsaRs2, insn1_temp->IsaRd, insn1_temp->Operand1Ready, insn1_temp->Operand2Ready, insn1_temp->Operand1, 
    //                 insn1_temp->Operand2, insn1_temp->imm, insn1_temp->Excp.valid, insn1_temp->Excp.Cause, insn1_temp->Excp.Tval, insn1_temp->Pred.Taken, insn1_temp->Pred.Target);
    //     }
    //     if(insn2_error_occur){
    //         DPRINTF(temptest, "Error in insn[1]:    data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //                 insn[1]->data_valid, insn[1]->Pc, insn[1]->Fu, insn[1]->SubOp, insn[1]->UncompressedInsn, insn[1]->ControlFlowInsn, 
    //                 insn[1]->IsaRs1, insn[1]->IsaRs2, insn[1]->IsaRd, insn[1]->Operand1Ready, insn[1]->Operand2Ready, insn[1]->Operand1, 
    //                 insn[1]->Operand2, insn[1]->imm, insn[1]->Excp.valid, insn[1]->Excp.Cause, insn[1]->Excp.Tval, insn[1]->Pred.Taken, insn[1]->Pred.Target);
    //         DPRINTF(temptest, "Expected insn2_temp: data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //                 insn2_temp->data_valid, insn2_temp->Pc, insn2_temp->Fu, insn2_temp->SubOp, insn2_temp->UncompressedInsn, insn2_temp->ControlFlowInsn, 
    //                 insn2_temp->IsaRs1, insn2_temp->IsaRs2, insn2_temp->IsaRd, insn2_temp->Operand1Ready, insn2_temp->Operand2Ready, insn2_temp->Operand1, 
    //                 insn2_temp->Operand2, insn2_temp->imm, insn2_temp->Excp.valid, insn2_temp->Excp.Cause, insn2_temp->Excp.Tval, insn2_temp->Pred.Taken, insn2_temp->Pred.Target);
    //     }

    //     if(insn3_error_occur){
    //         DPRINTF(temptest, "Error in insn[2]:    data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //                 insn[2]->data_valid, insn[2]->Pc, insn[2]->Fu, insn[2]->SubOp, insn[2]->UncompressedInsn, insn[2]->ControlFlowInsn, 
    //                 insn[2]->IsaRs1, insn[2]->IsaRs2, insn[2]->IsaRd, insn[2]->Operand1Ready, insn[2]->Operand2Ready, insn[2]->Operand1, 
    //                 insn[2]->Operand2, insn[2]->imm, insn[2]->Excp.valid, insn[2]->Excp.Cause, insn[2]->Excp.Tval, insn[2]->Pred.Taken, insn[2]->Pred.Target);
    //         DPRINTF(temptest, "Expected insn3_temp: data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //                 insn3_temp->data_valid, insn3_temp->Pc, insn3_temp->Fu, insn3_temp->SubOp, insn3_temp->UncompressedInsn, insn3_temp->ControlFlowInsn, 
    //                 insn3_temp->IsaRs1, insn3_temp->IsaRs2, insn3_temp->IsaRd, insn3_temp->Operand1Ready, insn3_temp->Operand2Ready, insn3_temp->Operand1, 
    //                 insn3_temp->Operand2, insn3_temp->imm, insn3_temp->Excp.valid, insn3_temp->Excp.Cause, insn3_temp->Excp.Tval, insn3_temp->Pred.Taken, insn3_temp->Pred.Target);
    //     }

    //     if(insn4_error_occur){
    //         DPRINTF(temptest, "Error in insn[3]:    data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //                 insn[3]->data_valid, insn[3]->Pc, insn[3]->Fu, insn[3]->SubOp, insn[3]->UncompressedInsn, insn[3]->ControlFlowInsn, 
    //                 insn[3]->IsaRs1, insn[3]->IsaRs2, insn[3]->IsaRd, insn[3]->Operand1Ready, insn[3]->Operand2Ready, insn[3]->Operand1, 
    //                 insn[3]->Operand2, insn[3]->imm, insn[3]->Excp.valid, insn[3]->Excp.Cause, insn[3]->Excp.Tval, insn[3]->Pred.Taken, insn[3]->Pred.Target);
    //         DPRINTF(temptest, "Expected insn4_temp: data_valid: {:x}, Pc: {:#x}, Fu: {:#x}, SubOp: {:#x}, UncompressedInsn: {:#x}, ControlFlowInsn: {:#x}, IsaRs1: {:#x}, IsaRs2: {:#x}, IsaRd: {:#x}, Operand1Ready: {:#x}, Operand2Ready: {:#x}, Operand1: {:#x}, Operand2: {:#x}, imm: {:#x}, Excp.valid: {:#x}, Excp.Cause: {:#x}, Excp.Tval: {:#x}, Pred.Taken: {:#x}, Pred.Target: {:#x}",
    //                 insn4_temp->data_valid, insn4_temp->Pc, insn4_temp->Fu, insn4_temp->SubOp, insn4_temp->UncompressedInsn, insn4_temp->ControlFlowInsn, 
    //                 insn4_temp->IsaRs1, insn4_temp->IsaRs2, insn4_temp->IsaRd, insn4_temp->Operand1Ready, insn4_temp->Operand2Ready, insn4_temp->Operand1, 
    //                 insn4_temp->Operand2, insn4_temp->imm, insn4_temp->Excp.valid, insn4_temp->Excp.Cause, insn4_temp->Excp.Tval, insn4_temp->Pred.Taken, insn4_temp->Pred.Target);
    //     }
    //     //if(avail_count_error_occur){
    //         DPRINTF(temptest,"Decode_Queue->io_available_entries:{:x} r_avail_count:{:x} header_ptr: {:x} tail_ptr: {:x}",Decode_Queue->io_available_entries,r_avail_count,header_ptr,tail_ptr);
    //     //}
    //     DPRINTF(temptest, "--------------");
    //     error_cnt += 1;
        
    // }

}

void 
Decode::DecodeInsn(DecodeInsn_t& insn){
    RISCV::Decode_Func(*insn.get());
}

void 
Decode::Predecode(Emulator::DynInsn& insnEntry,DecodeInsn_t& insn1,DecodeInsn_t& insn2,DecodeInsn_t& insn3,DecodeInsn_t& insn4,Redirect_message& redirect_message,bool& flush_flag){
    redirect_message.valid=0;
    redirect_message.target=0;
    Redirect_message redirect_message1,redirect_message2,redirect_message3,redirect_message4;
    uint64_t offset  = insnEntry.Address & (this->m_iCacheAlignByte-1); //看当前的地址是否对齐
    char* dataPtr = insnEntry.InsnByte.data() + offset;  

    insn1->Pc                = insnEntry.Address;
    insn1->Excp              = insnEntry.Excp;
    insn1->UncompressedInsn  = *(uint32_t*)dataPtr;
    this->BranchRedirect(insn1,redirect_message1);

 
    insn2->Pc                = (insnEntry.Address+4);
    insn2->Excp              = insnEntry.Excp;
    insn2->UncompressedInsn  = *(uint32_t*)(dataPtr+4);
    this->BranchRedirect(insn2,redirect_message2);


    insn3->Pc                = (insnEntry.Address+8);
    insn3->Excp              = insnEntry.Excp;
    insn3->UncompressedInsn  = *(uint32_t*)(dataPtr+8);
    this->BranchRedirect(insn3,redirect_message3);


    insn4->Pc                = (insnEntry.Address+12);
    insn4->Excp              = insnEntry.Excp;
    insn4->UncompressedInsn  = *(uint32_t*)(dataPtr+12);
    this->BranchRedirect(insn4,redirect_message4);
    
    insn1->data_valid=true;
    insn2->data_valid=((offset==8)||(offset==4)||((offset==0)))&&!redirect_message1.valid;
    insn3->data_valid=((offset==4)||((offset==0)))&&!redirect_message1.valid&&!redirect_message2.valid;
    insn4->data_valid=(!offset)&&!redirect_message1.valid&&!redirect_message2.valid&&!redirect_message3.valid;
    
    flush_flag=redirect_message1.valid|redirect_message2.valid|redirect_message3.valid|redirect_message4.valid;
    if(redirect_message1.valid)redirect_message=redirect_message1;
    else if(redirect_message2.valid)redirect_message=redirect_message2;
    else if(redirect_message3.valid)redirect_message=redirect_message3;
    else if(redirect_message4.valid)redirect_message=redirect_message4;


        // VPredecode *Predecode = new VPredecode;
    // Redirect_message redirect_message1,redirect_message2,redirect_message3,redirect_message4;
    
    // //Inputs
    // Predecode->io_Address = insnEntry.Address;
    // for(int j=0;j<4;j++){
    //     Predecode->io_data[j] = *(uint32_t*)(insnEntry.InsnByte.data()+j*4);
    // }
    // Predecode->io_excp_valid = insnEntry.Excp.valid;
    // Predecode->io_excp_cause = insnEntry.Excp.Cause;
    // Predecode->io_excpt_tval = insnEntry.Excp.Tval;
    
    // Predecode->eval();
    
    // //outputs
    // insn1->data_valid = Predecode->io_insn_valid1;
    // insn1->Pc = Predecode->io_pc1;
    // insn1->UncompressedInsn = Predecode->io_insn1;
    // insn1->Pred.Taken = Predecode->io_Perd_Taken1;
    // insn1->Pred.Target = Predecode->io_Pred_Target1;
    // redirect_message1.valid = Predecode->io_Perd_Taken1;
    // redirect_message1.target = Predecode->io_Pred_Target1;
    // insn1->Excp.valid = Predecode->io_excp_valid1;
    // insn1->Excp.Cause = Predecode->io_excp_cause1;
    // insn1->Excp.Tval = Predecode->io_excpt_tval1;

    // insn2->data_valid = Predecode->io_insn_valid2;
    // insn2->Pc = Predecode->io_pc2;
    // insn2->UncompressedInsn = Predecode->io_insn2;
    // insn2->Pred.Taken = Predecode->io_Perd_Taken2;
    // insn2->Pred.Target = Predecode->io_Pred_Target2;
    // redirect_message2.valid = Predecode->io_Perd_Taken2;
    // redirect_message2.target = Predecode->io_Pred_Target2;
    // insn2->Excp.valid = Predecode->io_excp_valid2;
    // insn2->Excp.Cause = Predecode->io_excp_cause2;
    // insn2->Excp.Tval = Predecode->io_excpt_tval2;

    // insn3->data_valid = Predecode->io_insn_valid3;
    // insn3->Pc = Predecode->io_pc3;
    // insn3->UncompressedInsn = Predecode->io_insn3;
    // insn3->Pred.Taken = Predecode->io_Perd_Taken3;
    // insn3->Pred.Target = Predecode->io_Pred_Target3;
    // redirect_message3.valid = Predecode->io_Perd_Taken3;
    // redirect_message3.target = Predecode->io_Pred_Target3;
    // insn3->Excp.valid = Predecode->io_excp_valid3;
    // insn3->Excp.Cause = Predecode->io_excp_cause3;
    // insn3->Excp.Tval = Predecode->io_excpt_tval3;

    // insn4->data_valid = Predecode->io_insn_valid4;
    // insn4->Pc = Predecode->io_pc4;
    // insn4->UncompressedInsn = Predecode->io_insn4;
    // insn4->Pred.Taken = Predecode->io_Perd_Taken4;
    // insn4->Pred.Target = Predecode->io_Pred_Target4;
    // redirect_message4.valid = Predecode->io_Perd_Taken4;
    // redirect_message4.target = Predecode->io_Pred_Target4;
    // insn4->Excp.valid = Predecode->io_excp_valid4;
    // insn4->Excp.Cause = Predecode->io_excp_cause4;
    // insn4->Excp.Tval = Predecode->io_excpt_tval4;

    // redirect_message.valid = Predecode->io_Redirect_valid;
    // redirect_message.target = Predecode->io_Redirect_Target;
    // flush_flag = Predecode->io_fetch_flush;

    // // Monitor code for output validation
    // bool error_occur_insn1 = (insn1->data_valid != Predecode->io_insn_valid1) ||
    //                         ((insn1->data_valid && Predecode->io_insn_valid1) &&
    //                         ((insn1->Pc != Predecode->io_pc1) ||
    //                         (insn1->UncompressedInsn != Predecode->io_insn1) ||
    //                         (redirect_message1.valid != Predecode->io_Perd_Taken1) ||
    //                         ((redirect_message1.valid && Predecode->io_Perd_Taken1) && 
    //                         (redirect_message1.target != Predecode->io_Pred_Target1)) ||
    //                         (insn1->Excp.valid != Predecode->io_excp_valid1) ||
    //                         (insn1->Excp.Cause != Predecode->io_excp_cause1) ||
    //                         (insn1->Excp.Tval != Predecode->io_excpt_tval1)));



    // bool error_occur_insn2 = (insn2->data_valid != Predecode->io_insn_valid2) ||
    //                         ((insn2->data_valid && Predecode->io_insn_valid2) &&
    //                         ((insn2->Pc != Predecode->io_pc2) ||
    //                         (insn2->UncompressedInsn != Predecode->io_insn2) ||
    //                         (redirect_message2.valid != Predecode->io_Perd_Taken2) ||
    //                         ((redirect_message2.valid && Predecode->io_Perd_Taken2) && 
    //                         (redirect_message2.target != Predecode->io_Pred_Target2)) ||
    //                         (insn2->Excp.valid != Predecode->io_excp_valid2) ||
    //                         (insn2->Excp.Cause != Predecode->io_excp_cause2) ||
    //                         (insn2->Excp.Tval != Predecode->io_excpt_tval2)));



    // bool error_occur_insn3 = (insn3->data_valid != Predecode->io_insn_valid3) ||
    //                         ((insn3->data_valid && Predecode->io_insn_valid3) &&
    //                         ((insn3->Pc != Predecode->io_pc3) ||
    //                         (insn3->UncompressedInsn != Predecode->io_insn3) ||
    //                         (redirect_message3.valid != Predecode->io_Perd_Taken3) ||
    //                         ((redirect_message3.valid && Predecode->io_Perd_Taken3) && 
    //                         (redirect_message3.target != Predecode->io_Pred_Target3)) ||
    //                         (insn3->Excp.valid != Predecode->io_excp_valid3) ||
    //                         (insn3->Excp.Cause != Predecode->io_excp_cause3) ||
    //                         (insn3->Excp.Tval != Predecode->io_excpt_tval3)));



    // bool error_occur_insn4 = (insn4->data_valid != Predecode->io_insn_valid4) ||
    //                         ((insn4->data_valid && Predecode->io_insn_valid4) &&    
    //                         ((insn4->Pc != Predecode->io_pc4) ||
    //                         (insn4->UncompressedInsn != Predecode->io_insn4) ||
    //                         (redirect_message4.valid != Predecode->io_Perd_Taken4) ||
    //                         ((redirect_message4.valid && Predecode->io_Perd_Taken4) && 
    //                         (redirect_message4.target != Predecode->io_Pred_Target4)) ||
    //                         (insn4->Excp.valid != Predecode->io_excp_valid4) ||
    //                         (insn4->Excp.Cause != Predecode->io_excp_cause4) ||
    //                         (insn4->Excp.Tval != Predecode->io_excpt_tval4)));

    // // Check for redirect_message validation
    // bool error_occur_redirect = (redirect_message.valid != Predecode->io_Redirect_valid) ||
    //                             ((redirect_message.valid && Predecode->io_Redirect_valid) &&
    //                             (redirect_message.target != Predecode->io_Redirect_Target)) ||
    //                             (flush_flag != Predecode->io_fetch_flush);
    // bool error_occur = error_occur_insn1 || error_occur_insn2 || error_occur_insn3 || error_occur_insn4 || error_occur_redirect;
    // if(error_occur && error_cnt < 100) {
    //     if (error_occur_insn1)  {
    //         DPRINTF(temptest, "Insn1 Error: data_valid:{:#x} Pc:{:#x} UncompressedInsn:{:#x} "
    //             "Pred_Taken:{:#x} Target:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x}",
    //             insn1->data_valid, insn1->Pc, insn1->UncompressedInsn,
    //             redirect_message1.valid, redirect_message1.target,
    //             insn1->Excp.valid, insn1->Excp.Cause, insn1->Excp.Tval);
            
    //         DPRINTF(temptest, "Expected:    io_insn_valid1:{:#x} io_pc1:{:#x} io_insn1:{:#x} "
    //             "io_Perd_Taken1:{:#x} io_Pred_Target1:{:#x} io_excp_valid1:{:#x} "
    //             "io_excp_cause1:{:#x} io_excpt_tval1:{:#x}",
    //             Predecode->io_insn_valid1, Predecode->io_pc1, Predecode->io_insn1,
    //             Predecode->io_Perd_Taken1, Predecode->io_Pred_Target1,
    //             Predecode->io_excp_valid1, Predecode->io_excp_cause1, Predecode->io_excpt_tval1);
    //             }
    //     if (error_occur_insn2) {
    //         DPRINTF(temptest, "Insn2 Error: data_valid:{:#x} Pc:{:#x} UncompressedInsn:{:#x} "
    //             "Pred_Taken:{:#x} Target:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x}",
    //             insn2->data_valid, insn2->Pc, insn2->UncompressedInsn,
    //             redirect_message2.valid, redirect_message2.target,
    //             insn2->Excp.valid, insn2->Excp.Cause, insn2->Excp.Tval);
            
    //         DPRINTF(temptest, "Expected:    io_insn_valid2:{:#x} io_pc2:{:#x} io_insn2:{:#x} "
    //             "io_Perd_Taken2:{:#x} io_Pred_Target2:{:#x} io_excp_valid2:{:#x} "
    //             "io_excp_cause2:{:#x} io_excpt_tval2:{:#x}",
    //             Predecode->io_insn_valid2, Predecode->io_pc2, Predecode->io_insn2,
    //             Predecode->io_Perd_Taken2, Predecode->io_Pred_Target2,
    //             Predecode->io_excp_valid2, Predecode->io_excp_cause2, Predecode->io_excpt_tval2);
    //     }
    //     if (error_occur_insn3) {
    //         DPRINTF(temptest, "Insn3 Error: data_valid:{:#x} Pc:{:#x} UncompressedInsn:{:#x} "
    //             "Pred_Taken:{:#x} Target:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x}",
    //             insn3->data_valid, insn3->Pc, insn3->UncompressedInsn,
    //             redirect_message3.valid, redirect_message3.target,
    //             insn3->Excp.valid, insn3->Excp.Cause, insn3->Excp.Tval);
            
    //         DPRINTF(temptest, "Expected:    io_insn_valid3:{:#x} io_pc3:{:#x} io_insn3:{:#x} "
    //             "io_Perd_Taken3:{:#x} io_Pred_Target3:{:#x} io_excp_valid3:{:#x} "
    //             "io_excp_cause3:{:#x} io_excpt_tval3:{:#x}",
    //             Predecode->io_insn_valid3, Predecode->io_pc3, Predecode->io_insn3,
    //             Predecode->io_Perd_Taken3, Predecode->io_Pred_Target3,
    //             Predecode->io_excp_valid3, Predecode->io_excp_cause3, Predecode->io_excpt_tval3);
    //     }
    //     if (error_occur_insn4) {
    //         DPRINTF(temptest, "Insn4 Error: data_valid:{:#x} Pc:{:#x} UncompressedInsn:{:#x} "
    //             "Pred_Taken:{:#x} Target:{:#x} Excp.valid:{:#x} Excp.Cause:{:#x} Excp.Tval:{:#x}",
    //             insn4->data_valid, insn4->Pc, insn4->UncompressedInsn,
    //             redirect_message4.valid, redirect_message4.target,
    //             insn4->Excp.valid, insn4->Excp.Cause, insn4->Excp.Tval);
            
    //         DPRINTF(temptest, "Expected:    io_insn_valid4:{:#x} io_pc4:{:#x} io_insn4:{:#x} "
    //             "io_Perd_Taken4:{:#x} io_Pred_Target4:{:#x} io_excp_valid4:{:#x} "
    //             "io_excp_cause4:{:#x} io_excpt_tval4:{:#x}",
    //             Predecode->io_insn_valid4, Predecode->io_pc4, Predecode->io_insn4,
    //             Predecode->io_Perd_Taken4, Predecode->io_Pred_Target4,
    //             Predecode->io_excp_valid4, Predecode->io_excp_cause4, Predecode->io_excpt_tval4);
    //     }
    //     if (error_occur_redirect) {
    //         DPRINTF(temptest, "Error in redirect_message: valid:{:#x} target:{:#x} flush_flag:{:#x}",
    //                 redirect_message.valid, redirect_message.target, flush_flag);
    //         DPRINTF(temptest, "Expected: io_Redirect_valid:{:#x} io_Redirect_Target:{:#x} io_fetch_flush:{:#x}",
    //                 Predecode->io_Redirect_valid, Predecode->io_Redirect_Target, Predecode->io_fetch_flush);
    //     }
    //     error_cnt += 1;
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

    // BranchRedirect->eval();
    // // insn->Pred.Taken = BranchRedirect->io_Pred_Taken;
    // // insn->Pred.Target = BranchRedirect->io_Pred_Target;
    // // redirect_message.valid = BranchRedirect->io_Pred_Taken;
    // // redirect_message.target = BranchRedirect->io_Redirect_Target;

    // bool error_occur = (redirect_message.valid != BranchRedirect->io_Pred_Taken) ||
    //             ((redirect_message.valid && BranchRedirect->io_Pred_Taken) && 
    //             (redirect_message.target != BranchRedirect->io_Redirect_Target));
    // if(error_occur && error_cnt < 100){
    //     DPRINTF(temptest,"True: operand2:{:#x} operand3:{:#x}",
    //         redirect_message.target ,redirect_message.valid );
    //     DPRINTF(temptest,"Flase:operand2:{:#x} operand3:{:#x}",
    //     BranchRedirect->io_Redirect_Target,BranchRedirect->io_Pred_Taken);
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
