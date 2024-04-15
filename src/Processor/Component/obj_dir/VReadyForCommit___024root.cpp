// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VReadyForCommit.h for the primary calling header

#include "VReadyForCommit___024root.h"
#include "VReadyForCommit__Syms.h"

//==========

VL_INLINE_OPT void VReadyForCommit___024root___combo__TOP__1(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___combo__TOP__1\n"); );
    // Variables
    CData/*0:0*/ ReadyForCommit__DOT___T_4;
    // Body
    ReadyForCommit__DOT___T_4 = ((IData)(vlSelf->io_Rob_Header) 
                                 == (IData)(vlSelf->io_Rob_Tag));
    vlSelf->io_Ready = (1U & ((~ ((0U == (IData)(vlSelf->io_m_RobState)) 
                                  | ((((IData)(vlSelf->io_Rob_Tag) 
                                       >= (IData)(vlSelf->io_Rob_Header)) 
                                      ^ ((IData)(vlSelf->io_m_RollBackTag) 
                                         >= (IData)(vlSelf->io_Rob_Header)))
                                      ? ((IData)(vlSelf->io_Rob_Tag) 
                                         > (IData)(vlSelf->io_m_RollBackTag))
                                      : ((IData)(vlSelf->io_Rob_Tag) 
                                         < (IData)(vlSelf->io_m_RollBackTag))))) 
                              | ((0U != (IData)(vlSelf->io_Rob_Usage)) 
                                 & ((1U == (IData)(vlSelf->io_Rob_Usage))
                                     ? (IData)(ReadyForCommit__DOT___T_4)
                                     : ((2U <= (IData)(vlSelf->io_Rob_Usage)) 
                                        & ((IData)(ReadyForCommit__DOT___T_4) 
                                           | ((~ ((~ (IData)(vlSelf->io_Header_isStable)) 
                                                  & (((3U 
                                                       == (IData)(vlSelf->io_Header_Function_type)) 
                                                      | (4U 
                                                         == (IData)(vlSelf->io_Header_Function_type))) 
                                                     | (1U 
                                                        == (IData)(vlSelf->io_Header_Function_type))))) 
                                              & ((IData)(vlSelf->io_Rob_NextHeader) 
                                                 == (IData)(vlSelf->io_Rob_Tag)))))))));
}

void VReadyForCommit___024root___eval(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___eval\n"); );
    // Body
    VReadyForCommit___024root___combo__TOP__1(vlSelf);
}

QData VReadyForCommit___024root___change_request_1(VReadyForCommit___024root* vlSelf);

VL_INLINE_OPT QData VReadyForCommit___024root___change_request(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___change_request\n"); );
    // Body
    return (VReadyForCommit___024root___change_request_1(vlSelf));
}

VL_INLINE_OPT QData VReadyForCommit___024root___change_request_1(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___change_request_1\n"); );
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VReadyForCommit___024root___eval_debug_assertions(VReadyForCommit___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VReadyForCommit__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VReadyForCommit___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_Rob_Tag & 0xf0U))) {
        Verilated::overWidthError("io_Rob_Tag");}
    if (VL_UNLIKELY((vlSelf->io_Rob_Header & 0xf0U))) {
        Verilated::overWidthError("io_Rob_Header");}
    if (VL_UNLIKELY((vlSelf->io_Rob_NextHeader & 0xf0U))) {
        Verilated::overWidthError("io_Rob_NextHeader");}
    if (VL_UNLIKELY((vlSelf->io_m_RobState & 0xf8U))) {
        Verilated::overWidthError("io_m_RobState");}
    if (VL_UNLIKELY((vlSelf->io_m_RollBackTag & 0xf0U))) {
        Verilated::overWidthError("io_m_RollBackTag");}
    if (VL_UNLIKELY((vlSelf->io_Rob_Usage & 0xf0U))) {
        Verilated::overWidthError("io_Rob_Usage");}
    if (VL_UNLIKELY((vlSelf->io_Header_isStable & 0xfeU))) {
        Verilated::overWidthError("io_Header_isStable");}
    if (VL_UNLIKELY((vlSelf->io_Header_Function_type 
                     & 0xf8U))) {
        Verilated::overWidthError("io_Header_Function_type");}
}
#endif  // VL_DEBUG
