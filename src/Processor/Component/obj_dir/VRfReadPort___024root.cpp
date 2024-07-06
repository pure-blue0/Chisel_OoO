// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VRfReadPort.h for the primary calling header

#include "VRfReadPort___024root.h"
#include "VRfReadPort__Syms.h"

//==========

VL_INLINE_OPT void VRfReadPort___024root___combo__TOP__1(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___combo__TOP__1\n"); );
    // Body
    vlSelf->io_Out_valid = vlSelf->io_Valid;
    if (vlSelf->io_Valid) {
        vlSelf->io_desIndex = vlSelf->io_desIndex_in;
        vlSelf->io_Sub_OP_out = vlSelf->io_Sub_OP_in;
        vlSelf->io_desIndex_out = vlSelf->io_desIndex_in;
        vlSelf->io_imm_out = vlSelf->io_imm_in;
        vlSelf->io_pc_out = vlSelf->io_pc_in;
        vlSelf->io_Pred_taken_out = ((IData)(vlSelf->io_Pred_taken_in) 
                                     & 1U);
        vlSelf->io_pred_target_out = vlSelf->io_pred_target_in;
        vlSelf->io_Function_type_out = vlSelf->io_Function_type_in;
        vlSelf->io_RobTag_out = vlSelf->io_RobTag_in;
        vlSelf->io_LSQTag_out = vlSelf->io_LSQTag_in;
        vlSelf->io_Insn_operand1 = ((IData)(vlSelf->io_Insn_Operand1Ready)
                                     ? vlSelf->io_Insn_operand1_in
                                     : vlSelf->io_Insn_PhyRs1_data);
        vlSelf->io_Insn_operand2 = ((IData)(vlSelf->io_Insn_Operand2Ready)
                                     ? vlSelf->io_Insn_operand2_in
                                     : vlSelf->io_Insn_PhyRs2_data);
    } else {
        vlSelf->io_desIndex = 0U;
        vlSelf->io_Sub_OP_out = 0U;
        vlSelf->io_desIndex_out = 0U;
        vlSelf->io_imm_out = 0ULL;
        vlSelf->io_pc_out = 0ULL;
        vlSelf->io_Pred_taken_out = 0U;
        vlSelf->io_pred_target_out = 0ULL;
        vlSelf->io_Function_type_out = 0U;
        vlSelf->io_RobTag_out = 0U;
        vlSelf->io_LSQTag_out = 0U;
        vlSelf->io_Insn_operand1 = 0ULL;
        vlSelf->io_Insn_operand2 = 0ULL;
    }
}

void VRfReadPort___024root___eval(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___eval\n"); );
    // Body
    VRfReadPort___024root___combo__TOP__1(vlSelf);
}

QData VRfReadPort___024root___change_request_1(VRfReadPort___024root* vlSelf);

VL_INLINE_OPT QData VRfReadPort___024root___change_request(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___change_request\n"); );
    // Body
    return (VRfReadPort___024root___change_request_1(vlSelf));
}

VL_INLINE_OPT QData VRfReadPort___024root___change_request_1(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___change_request_1\n"); );
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VRfReadPort___024root___eval_debug_assertions(VRfReadPort___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRfReadPort__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRfReadPort___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_Valid & 0xfeU))) {
        Verilated::overWidthError("io_Valid");}
    if (VL_UNLIKELY((vlSelf->io_Insn_Operand1Ready 
                     & 0xfeU))) {
        Verilated::overWidthError("io_Insn_Operand1Ready");}
    if (VL_UNLIKELY((vlSelf->io_Insn_Operand2Ready 
                     & 0xfeU))) {
        Verilated::overWidthError("io_Insn_Operand2Ready");}
    if (VL_UNLIKELY((vlSelf->io_desIndex_in & 0xf8U))) {
        Verilated::overWidthError("io_desIndex_in");}
    if (VL_UNLIKELY((vlSelf->io_Sub_OP_in & 0xf0U))) {
        Verilated::overWidthError("io_Sub_OP_in");}
    if (VL_UNLIKELY((vlSelf->io_Pred_taken_in & 0xfeU))) {
        Verilated::overWidthError("io_Pred_taken_in");}
    if (VL_UNLIKELY((vlSelf->io_Function_type_in & 0xf8U))) {
        Verilated::overWidthError("io_Function_type_in");}
    if (VL_UNLIKELY((vlSelf->io_RobTag_in & 0xf0U))) {
        Verilated::overWidthError("io_RobTag_in");}
    if (VL_UNLIKELY((vlSelf->io_LSQTag_in & 0xf0U))) {
        Verilated::overWidthError("io_LSQTag_in");}
}
#endif  // VL_DEBUG
