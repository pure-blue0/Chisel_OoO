// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VRfReadPort.h for the primary calling header

#ifndef VERILATED_VRFREADPORT___024ROOT_H_
#define VERILATED_VRFREADPORT___024ROOT_H_  // guard

#include "verilated_heavy.h"

//==========

class VRfReadPort__Syms;

//----------

VL_MODULE(VRfReadPort___024root) {
  public:

    // PORTS
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_Valid,0,0);
    VL_IN8(io_Insn_Operand1Ready,0,0);
    VL_IN8(io_Insn_Operand2Ready,0,0);
    VL_IN8(io_desIndex_in,2,0);
    VL_IN8(io_Sub_OP_in,3,0);
    VL_IN8(io_Pred_taken_in,0,0);
    VL_IN8(io_Function_type_in,2,0);
    VL_IN8(io_RobTag_in,3,0);
    VL_IN8(io_LSQTag_in,3,0);
    VL_OUT8(io_Out_valid,0,0);
    VL_OUT8(io_desIndex,2,0);
    VL_OUT8(io_Sub_OP_out,3,0);
    VL_OUT8(io_desIndex_out,2,0);
    VL_OUT8(io_Pred_taken_out,0,0);
    VL_OUT8(io_Function_type_out,2,0);
    VL_OUT8(io_RobTag_out,3,0);
    VL_OUT8(io_LSQTag_out,3,0);
    VL_IN64(io_Insn_PhyRs1_data,63,0);
    VL_IN64(io_Insn_PhyRs2_data,63,0);
    VL_IN64(io_imm_in,63,0);
    VL_IN64(io_pc_in,63,0);
    VL_IN64(io_pred_target_in,63,0);
    VL_IN64(io_Insn_operand1_in,63,0);
    VL_IN64(io_Insn_operand2_in,63,0);
    VL_OUT64(io_Insn_operand1,63,0);
    VL_OUT64(io_Insn_operand2,63,0);
    VL_OUT64(io_imm_out,63,0);
    VL_OUT64(io_pc_out,63,0);
    VL_OUT64(io_pred_target_out,63,0);

    // INTERNAL VARIABLES
    VRfReadPort__Syms* vlSymsp;  // Symbol table

    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VRfReadPort___024root);  ///< Copying not allowed
  public:
    VRfReadPort___024root(const char* name);
    ~VRfReadPort___024root();

    // INTERNAL METHODS
    void __Vconfigure(VRfReadPort__Syms* symsp, bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
