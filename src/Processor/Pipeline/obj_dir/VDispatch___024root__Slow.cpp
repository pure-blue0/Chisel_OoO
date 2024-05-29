// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VDispatch.h for the primary calling header

#include "VDispatch___024root.h"
#include "VDispatch__Syms.h"

//==========


void VDispatch___024root___ctor_var_reset(VDispatch___024root* vlSelf);

VDispatch___024root::VDispatch___024root(const char* _vcname__)
    : VerilatedModule(_vcname__)
 {
    // Reset structure values
    VDispatch___024root___ctor_var_reset(this);
}

void VDispatch___024root::__Vconfigure(VDispatch__Syms* _vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->vlSymsp = _vlSymsp;
}

VDispatch___024root::~VDispatch___024root() {
}

void VDispatch___024root___eval_initial(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___eval_initial\n"); );
}

void VDispatch___024root___combo__TOP__1(VDispatch___024root* vlSelf);

void VDispatch___024root___eval_settle(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___eval_settle\n"); );
    // Body
    VDispatch___024root___combo__TOP__1(vlSelf);
}

void VDispatch___024root___final(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___final\n"); );
}

void VDispatch___024root___ctor_var_reset(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clock = VL_RAND_RESET_I(1);
    vlSelf->reset = VL_RAND_RESET_I(1);
    vlSelf->io_DispatchCount = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(128, vlSelf->io_insn1);
    VL_RAND_RESET_W(128, vlSelf->io_insn2);
    vlSelf->io_Insn1_LSQTag_in = VL_RAND_RESET_I(4);
    vlSelf->io_Insn2_LSQTag_in = VL_RAND_RESET_I(4);
    vlSelf->io_Insn1_RobTag_in = VL_RAND_RESET_I(4);
    vlSelf->io_Insn2_RobTag_in = VL_RAND_RESET_I(4);
    vlSelf->io_insn1_PhyRs1_in = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_PhyRs2_in = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_PhyRd_in = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_LPhyRd_in = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRs1_in = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRs2_in = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRd_in = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_LPhyRd_in = VL_RAND_RESET_I(6);
    vlSelf->io_Issue_num1 = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(128, vlSelf->io_insn1_Out);
    vlSelf->io_Insn1_LSQTag = VL_RAND_RESET_I(4);
    vlSelf->io_insn1_PhyRs1 = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_PhyRs2 = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_PhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_LPhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_Insn1_RobTag = VL_RAND_RESET_I(4);
    vlSelf->io_Issue_num2 = VL_RAND_RESET_I(2);
    VL_RAND_RESET_W(128, vlSelf->io_insn2_Out);
    vlSelf->io_Insn2_LSQTag = VL_RAND_RESET_I(4);
    vlSelf->io_insn2_PhyRs1 = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRs2 = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_LPhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_Insn2_RobTag = VL_RAND_RESET_I(4);
}
