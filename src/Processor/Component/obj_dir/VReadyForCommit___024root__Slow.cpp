// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VReadyForCommit.h for the primary calling header

#include "VReadyForCommit___024root.h"
#include "VReadyForCommit__Syms.h"

//==========


void VReadyForCommit___024root___ctor_var_reset(VReadyForCommit___024root* vlSelf);

VReadyForCommit___024root::VReadyForCommit___024root(const char* _vcname__)
    : VerilatedModule(_vcname__)
 {
    // Reset structure values
    VReadyForCommit___024root___ctor_var_reset(this);
}

void VReadyForCommit___024root::__Vconfigure(VReadyForCommit__Syms* _vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->vlSymsp = _vlSymsp;
}

VReadyForCommit___024root::~VReadyForCommit___024root() {
}

void VReadyForCommit___024root___eval_initial(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___eval_initial\n"); );
}

void VReadyForCommit___024root___combo__TOP__1(VReadyForCommit___024root* vlSelf);

void VReadyForCommit___024root___eval_settle(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___eval_settle\n"); );
    // Body
    VReadyForCommit___024root___combo__TOP__1(vlSelf);
}

void VReadyForCommit___024root___final(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___final\n"); );
}

void VReadyForCommit___024root___ctor_var_reset(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clock = VL_RAND_RESET_I(1);
    vlSelf->reset = VL_RAND_RESET_I(1);
    vlSelf->io_Rob_Tag = VL_RAND_RESET_I(4);
    vlSelf->io_Rob_Header = VL_RAND_RESET_I(4);
    vlSelf->io_Rob_NextHeader = VL_RAND_RESET_I(4);
    vlSelf->io_m_RobState = VL_RAND_RESET_I(3);
    vlSelf->io_m_RollBackTag = VL_RAND_RESET_I(4);
    vlSelf->io_Rob_Usage = VL_RAND_RESET_I(4);
    vlSelf->io_Header_isStable = VL_RAND_RESET_I(1);
    vlSelf->io_Header_Function_type = VL_RAND_RESET_I(3);
    vlSelf->io_Ready = VL_RAND_RESET_I(1);
}
