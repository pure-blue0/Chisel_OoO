// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VRfReadPort.h for the primary calling header

#include "VRfReadPort___024root.h"
#include "VRfReadPort__Syms.h"

//==========


void VRfReadPort___024root___ctor_var_reset(VRfReadPort___024root* vlSelf);

VRfReadPort___024root::VRfReadPort___024root(const char* _vcname__)
    : VerilatedModule(_vcname__)
 {
    // Reset structure values
    VRfReadPort___024root___ctor_var_reset(this);
}

void VRfReadPort___024root::__Vconfigure(VRfReadPort__Syms* _vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->vlSymsp = _vlSymsp;
}

VRfReadPort___024root::~VRfReadPort___024root() {
}

void VRfReadPort___024root___eval_initial(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___eval_initial\n"); );
}

void VRfReadPort___024root___combo__TOP__1(VRfReadPort___024root* vlSelf);

void VRfReadPort___024root___eval_settle(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___eval_settle\n"); );
    // Body
    VRfReadPort___024root___combo__TOP__1(vlSelf);
}

void VRfReadPort___024root___final(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___final\n"); );
}

void VRfReadPort___024root___ctor_var_reset(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clock = VL_RAND_RESET_I(1);
    vlSelf->reset = VL_RAND_RESET_I(1);
    vlSelf->io_Valid = VL_RAND_RESET_I(1);
    vlSelf->io_Insn_Operand1Ready = VL_RAND_RESET_I(1);
    vlSelf->io_Insn_PhyRs1_data = VL_RAND_RESET_Q(64);
    vlSelf->io_Insn_Operand2Ready = VL_RAND_RESET_I(1);
    vlSelf->io_Insn_PhyRs2_data = VL_RAND_RESET_Q(64);
    vlSelf->io_desIndex_in = VL_RAND_RESET_I(3);
    vlSelf->io_Sub_OP_in = VL_RAND_RESET_I(4);
    vlSelf->io_imm_in = VL_RAND_RESET_Q(64);
    vlSelf->io_pc_in = VL_RAND_RESET_Q(64);
    vlSelf->io_Pred_taken_in = VL_RAND_RESET_I(1);
    vlSelf->io_pred_target_in = VL_RAND_RESET_Q(64);
    vlSelf->io_Function_type_in = VL_RAND_RESET_I(3);
    vlSelf->io_Insn_operand1_in = VL_RAND_RESET_Q(64);
    vlSelf->io_Insn_operand2_in = VL_RAND_RESET_Q(64);
    vlSelf->io_RobTag_in = VL_RAND_RESET_I(4);
    vlSelf->io_LSQTag_in = VL_RAND_RESET_I(4);
    vlSelf->io_Out_valid = VL_RAND_RESET_I(1);
    vlSelf->io_desIndex = VL_RAND_RESET_I(3);
    vlSelf->io_Insn_operand1 = VL_RAND_RESET_Q(64);
    vlSelf->io_Insn_operand2 = VL_RAND_RESET_Q(64);
    vlSelf->io_Sub_OP_out = VL_RAND_RESET_I(4);
    vlSelf->io_desIndex_out = VL_RAND_RESET_I(3);
    vlSelf->io_imm_out = VL_RAND_RESET_Q(64);
    vlSelf->io_pc_out = VL_RAND_RESET_Q(64);
    vlSelf->io_Pred_taken_out = VL_RAND_RESET_I(1);
    vlSelf->io_pred_target_out = VL_RAND_RESET_Q(64);
    vlSelf->io_Function_type_out = VL_RAND_RESET_I(3);
    vlSelf->io_RobTag_out = VL_RAND_RESET_I(4);
    vlSelf->io_LSQTag_out = VL_RAND_RESET_I(4);
}
