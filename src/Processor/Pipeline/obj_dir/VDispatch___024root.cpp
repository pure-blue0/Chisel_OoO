// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VDispatch.h for the primary calling header

#include "VDispatch___024root.h"
#include "VDispatch__Syms.h"

//==========

VL_INLINE_OPT void VDispatch___024root___combo__TOP__1(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___combo__TOP__1\n"); );
    // Variables
    CData/*0:0*/ Dispatch__DOT___T_10;
    CData/*0:0*/ Dispatch__DOT___T_29;
    // Body
    Dispatch__DOT___T_10 = (1U & (((vlSelf->io_insn1[2U] 
                                    >> 0x19U) | ((2U 
                                                  == 
                                                  (7U 
                                                   & (vlSelf->io_insn1[1U] 
                                                      >> 0x12U))) 
                                                 & ((9U 
                                                     == 
                                                     (0xfU 
                                                      & (vlSelf->io_insn1[1U] 
                                                         >> 0x15U))) 
                                                    | (7U 
                                                       == 
                                                       (0xfU 
                                                        & (vlSelf->io_insn1[1U] 
                                                           >> 0x15U)))))) 
                                  | ((0U == (7U & (
                                                   vlSelf->io_insn1[1U] 
                                                   >> 0x12U))) 
                                     & (0U == (0x1fU 
                                               & (vlSelf->io_insn1[1U] 
                                                  >> 0xcU))))));
    Dispatch__DOT___T_29 = (1U & (((vlSelf->io_insn2[2U] 
                                    >> 0x19U) | ((2U 
                                                  == 
                                                  (7U 
                                                   & (vlSelf->io_insn2[1U] 
                                                      >> 0x12U))) 
                                                 & ((9U 
                                                     == 
                                                     (0xfU 
                                                      & (vlSelf->io_insn2[1U] 
                                                         >> 0x15U))) 
                                                    | (7U 
                                                       == 
                                                       (0xfU 
                                                        & (vlSelf->io_insn2[1U] 
                                                           >> 0x15U)))))) 
                                  | ((0U == (7U & (
                                                   vlSelf->io_insn2[1U] 
                                                   >> 0x12U))) 
                                     & (0U == (0x1fU 
                                               & (vlSelf->io_insn2[1U] 
                                                  >> 0xcU))))));
    if ((0U < (IData)(vlSelf->io_DispatchCount))) {
        vlSelf->io_Issue_num1 = ((IData)(Dispatch__DOT___T_10)
                                  ? 3U : ((2U == (7U 
                                                  & (vlSelf->io_insn1[1U] 
                                                     >> 0x12U)))
                                           ? 0U : (
                                                   ((3U 
                                                     == 
                                                     (7U 
                                                      & (vlSelf->io_insn1[1U] 
                                                         >> 0x12U))) 
                                                    | (4U 
                                                       == 
                                                       (7U 
                                                        & (vlSelf->io_insn1[1U] 
                                                           >> 0x12U))))
                                                    ? 1U
                                                    : 
                                                   (((0U 
                                                      == 
                                                      (7U 
                                                       & (vlSelf->io_insn1[1U] 
                                                          >> 0x12U))) 
                                                     | (1U 
                                                        == 
                                                        (7U 
                                                         & (vlSelf->io_insn1[1U] 
                                                            >> 0x12U))))
                                                     ? 2U
                                                     : 3U))));
        vlSelf->io_insn1_Out[0U] = ((IData)(Dispatch__DOT___T_10)
                                     ? 0U : vlSelf->io_insn1[0U]);
        vlSelf->io_insn1_Out[1U] = ((IData)(Dispatch__DOT___T_10)
                                     ? 0U : vlSelf->io_insn1[1U]);
        vlSelf->io_insn1_Out[2U] = ((IData)(Dispatch__DOT___T_10)
                                     ? 0U : vlSelf->io_insn1[2U]);
        vlSelf->io_insn1_Out[3U] = ((IData)(Dispatch__DOT___T_10)
                                     ? 0U : vlSelf->io_insn1[3U]);
        vlSelf->io_Insn1_LSQTag = ((IData)(Dispatch__DOT___T_10)
                                    ? 0U : (IData)(vlSelf->io_Insn1_LSQTag_in));
        vlSelf->io_insn1_PhyRs1 = ((IData)(Dispatch__DOT___T_10)
                                    ? 0U : (IData)(vlSelf->io_insn1_PhyRs1_in));
        vlSelf->io_insn1_PhyRs2 = ((IData)(Dispatch__DOT___T_10)
                                    ? 0U : (IData)(vlSelf->io_insn1_PhyRs2_in));
        vlSelf->io_insn1_PhyRd = ((IData)(Dispatch__DOT___T_10)
                                   ? 0U : (IData)(vlSelf->io_insn1_PhyRd_in));
        vlSelf->io_insn1_LPhyRd = ((IData)(Dispatch__DOT___T_10)
                                    ? 0U : (IData)(vlSelf->io_insn1_LPhyRd_in));
        vlSelf->io_Insn1_RobTag = ((IData)(Dispatch__DOT___T_10)
                                    ? 0U : (IData)(vlSelf->io_Insn1_RobTag_in));
    } else {
        vlSelf->io_Issue_num1 = 3U;
        vlSelf->io_insn1_Out[0U] = 0U;
        vlSelf->io_insn1_Out[1U] = 0U;
        vlSelf->io_insn1_Out[2U] = 0U;
        vlSelf->io_insn1_Out[3U] = 0U;
        vlSelf->io_Insn1_LSQTag = 0U;
        vlSelf->io_insn1_PhyRs1 = 0U;
        vlSelf->io_insn1_PhyRs2 = 0U;
        vlSelf->io_insn1_PhyRd = 0U;
        vlSelf->io_insn1_LPhyRd = 0U;
        vlSelf->io_Insn1_RobTag = 0U;
    }
    if ((2U == (IData)(vlSelf->io_DispatchCount))) {
        vlSelf->io_Issue_num2 = ((IData)(Dispatch__DOT___T_29)
                                  ? 3U : ((2U == (7U 
                                                  & (vlSelf->io_insn2[1U] 
                                                     >> 0x12U)))
                                           ? 0U : (
                                                   ((3U 
                                                     == 
                                                     (7U 
                                                      & (vlSelf->io_insn2[1U] 
                                                         >> 0x12U))) 
                                                    | (4U 
                                                       == 
                                                       (7U 
                                                        & (vlSelf->io_insn2[1U] 
                                                           >> 0x12U))))
                                                    ? 1U
                                                    : 
                                                   (((0U 
                                                      == 
                                                      (7U 
                                                       & (vlSelf->io_insn2[1U] 
                                                          >> 0x12U))) 
                                                     | (1U 
                                                        == 
                                                        (7U 
                                                         & (vlSelf->io_insn2[1U] 
                                                            >> 0x12U))))
                                                     ? 2U
                                                     : 3U))));
        vlSelf->io_insn2_Out[0U] = ((IData)(Dispatch__DOT___T_29)
                                     ? 0U : vlSelf->io_insn2[0U]);
        vlSelf->io_insn2_Out[1U] = ((IData)(Dispatch__DOT___T_29)
                                     ? 0U : vlSelf->io_insn2[1U]);
        vlSelf->io_insn2_Out[2U] = ((IData)(Dispatch__DOT___T_29)
                                     ? 0U : vlSelf->io_insn2[2U]);
        vlSelf->io_insn2_Out[3U] = ((IData)(Dispatch__DOT___T_29)
                                     ? 0U : vlSelf->io_insn2[3U]);
        vlSelf->io_Insn2_LSQTag = ((IData)(Dispatch__DOT___T_29)
                                    ? 0U : (IData)(vlSelf->io_Insn2_LSQTag_in));
        vlSelf->io_insn2_PhyRs1 = ((IData)(Dispatch__DOT___T_29)
                                    ? 0U : (IData)(vlSelf->io_insn2_PhyRs1_in));
        vlSelf->io_insn2_PhyRs2 = ((IData)(Dispatch__DOT___T_29)
                                    ? 0U : (IData)(vlSelf->io_insn2_PhyRs2_in));
        vlSelf->io_insn2_PhyRd = ((IData)(Dispatch__DOT___T_29)
                                   ? 0U : (IData)(vlSelf->io_insn2_PhyRd_in));
        vlSelf->io_insn2_LPhyRd = ((IData)(Dispatch__DOT___T_29)
                                    ? 0U : (IData)(vlSelf->io_insn2_LPhyRd_in));
        vlSelf->io_Insn2_RobTag = ((IData)(Dispatch__DOT___T_29)
                                    ? 0U : (IData)(vlSelf->io_Insn2_RobTag_in));
    } else {
        vlSelf->io_Issue_num2 = 3U;
        vlSelf->io_insn2_Out[0U] = 0U;
        vlSelf->io_insn2_Out[1U] = 0U;
        vlSelf->io_insn2_Out[2U] = 0U;
        vlSelf->io_insn2_Out[3U] = 0U;
        vlSelf->io_Insn2_LSQTag = 0U;
        vlSelf->io_insn2_PhyRs1 = 0U;
        vlSelf->io_insn2_PhyRs2 = 0U;
        vlSelf->io_insn2_PhyRd = 0U;
        vlSelf->io_insn2_LPhyRd = 0U;
        vlSelf->io_Insn2_RobTag = 0U;
    }
}

void VDispatch___024root___eval(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___eval\n"); );
    // Body
    VDispatch___024root___combo__TOP__1(vlSelf);
}

QData VDispatch___024root___change_request_1(VDispatch___024root* vlSelf);

VL_INLINE_OPT QData VDispatch___024root___change_request(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___change_request\n"); );
    // Body
    return (VDispatch___024root___change_request_1(vlSelf));
}

VL_INLINE_OPT QData VDispatch___024root___change_request_1(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___change_request_1\n"); );
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VDispatch___024root___eval_debug_assertions(VDispatch___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VDispatch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VDispatch___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_DispatchCount & 0xfcU))) {
        Verilated::overWidthError("io_DispatchCount");}
    if (VL_UNLIKELY((vlSelf->io_Insn1_LSQTag_in & 0xf0U))) {
        Verilated::overWidthError("io_Insn1_LSQTag_in");}
    if (VL_UNLIKELY((vlSelf->io_Insn2_LSQTag_in & 0xf0U))) {
        Verilated::overWidthError("io_Insn2_LSQTag_in");}
    if (VL_UNLIKELY((vlSelf->io_Insn1_RobTag_in & 0xf0U))) {
        Verilated::overWidthError("io_Insn1_RobTag_in");}
    if (VL_UNLIKELY((vlSelf->io_Insn2_RobTag_in & 0xf0U))) {
        Verilated::overWidthError("io_Insn2_RobTag_in");}
    if (VL_UNLIKELY((vlSelf->io_insn1_PhyRs1_in & 0xc0U))) {
        Verilated::overWidthError("io_insn1_PhyRs1_in");}
    if (VL_UNLIKELY((vlSelf->io_insn1_PhyRs2_in & 0xc0U))) {
        Verilated::overWidthError("io_insn1_PhyRs2_in");}
    if (VL_UNLIKELY((vlSelf->io_insn1_PhyRd_in & 0xc0U))) {
        Verilated::overWidthError("io_insn1_PhyRd_in");}
    if (VL_UNLIKELY((vlSelf->io_insn1_LPhyRd_in & 0xc0U))) {
        Verilated::overWidthError("io_insn1_LPhyRd_in");}
    if (VL_UNLIKELY((vlSelf->io_insn2_PhyRs1_in & 0xc0U))) {
        Verilated::overWidthError("io_insn2_PhyRs1_in");}
    if (VL_UNLIKELY((vlSelf->io_insn2_PhyRs2_in & 0xc0U))) {
        Verilated::overWidthError("io_insn2_PhyRs2_in");}
    if (VL_UNLIKELY((vlSelf->io_insn2_PhyRd_in & 0xc0U))) {
        Verilated::overWidthError("io_insn2_PhyRd_in");}
    if (VL_UNLIKELY((vlSelf->io_insn2_LPhyRd_in & 0xc0U))) {
        Verilated::overWidthError("io_insn2_LPhyRd_in");}
}
#endif  // VL_DEBUG
