// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Valu.h for the primary calling header

#include "Valu___024root.h"
#include "Valu__Syms.h"

//==========

VL_INLINE_OPT void Valu___024root___combo__TOP__1(Valu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___combo__TOP__1\n"); );
    // Variables
    IData/*31:0*/ ALU__DOT___io_Insn_rdresult_T_25;
    IData/*31:0*/ ALU__DOT___io_Insn_rdresult_T_34;
    IData/*31:0*/ ALU__DOT___io_Insn_rdresult_T_48;
    VlWide<4>/*127:0*/ __Vtemp2;
    VlWide<4>/*127:0*/ __Vtemp3;
    VlWide<4>/*127:0*/ __Vtemp4;
    VlWide<4>/*127:0*/ __Vtemp5;
    VlWide<4>/*127:0*/ __Vtemp6;
    QData/*63:0*/ ALU__DOT___GEN_9;
    // Body
    ALU__DOT___io_Insn_rdresult_T_25 = ((IData)(vlSelf->io_Insn_operand1) 
                                        + (IData)(vlSelf->io_Insn_operand2));
    ALU__DOT___io_Insn_rdresult_T_34 = ((IData)(vlSelf->io_Insn_operand1) 
                                        - (IData)(vlSelf->io_Insn_operand2));
    ALU__DOT___io_Insn_rdresult_T_48 = VL_SHIFTRS_III(32,32,5, (IData)(vlSelf->io_Insn_operand1), 
                                                      (0x1fU 
                                                       & (IData)(vlSelf->io_Insn_operand2)));
    ALU__DOT___GEN_9 = ((5U == (IData)(vlSelf->io_Sub_OP_out))
                         ? (vlSelf->io_Insn_operand1 
                            ^ vlSelf->io_Insn_operand2)
                         : ((6U == (IData)(vlSelf->io_Sub_OP_out))
                             ? (vlSelf->io_Insn_operand1 
                                >> (0x3fU & (IData)(vlSelf->io_Insn_operand2)))
                             : ((7U == (IData)(vlSelf->io_Sub_OP_out))
                                 ? VL_SHIFTRS_QQI(64,64,6, vlSelf->io_Insn_operand1, 
                                                  (0x3fU 
                                                   & (IData)(vlSelf->io_Insn_operand2)))
                                 : ((8U == (IData)(vlSelf->io_Sub_OP_out))
                                     ? (vlSelf->io_Insn_operand1 
                                        | vlSelf->io_Insn_operand2)
                                     : ((9U == (IData)(vlSelf->io_Sub_OP_out))
                                         ? (vlSelf->io_Insn_operand1 
                                            & vlSelf->io_Insn_operand2)
                                         : ((0xaU == (IData)(vlSelf->io_Sub_OP_out))
                                             ? (((QData)((IData)(
                                                                 (- (IData)(
                                                                            (ALU__DOT___io_Insn_rdresult_T_25 
                                                                             >> 0x1fU))))) 
                                                 << 0x20U) 
                                                | (QData)((IData)(ALU__DOT___io_Insn_rdresult_T_25)))
                                             : ((0xbU 
                                                 == (IData)(vlSelf->io_Sub_OP_out))
                                                 ? 
                                                (((QData)((IData)(
                                                                  (- (IData)(
                                                                             (ALU__DOT___io_Insn_rdresult_T_34 
                                                                              >> 0x1fU))))) 
                                                  << 0x20U) 
                                                 | (QData)((IData)(ALU__DOT___io_Insn_rdresult_T_34)))
                                                 : 
                                                ((0xcU 
                                                  == (IData)(vlSelf->io_Sub_OP_out))
                                                  ? 
                                                 (0x7fffffffffffffffULL 
                                                  & ((QData)((IData)(vlSelf->io_Insn_operand1)) 
                                                     << 
                                                     (0x1fU 
                                                      & (IData)(vlSelf->io_Insn_operand2))))
                                                  : 
                                                 ((0xdU 
                                                   == (IData)(vlSelf->io_Sub_OP_out))
                                                   ? (QData)((IData)(
                                                                     ((IData)(vlSelf->io_Insn_operand1) 
                                                                      >> 
                                                                      (0x1fU 
                                                                       & (IData)(vlSelf->io_Insn_operand2)))))
                                                   : 
                                                  ((0xeU 
                                                    == (IData)(vlSelf->io_Sub_OP_out))
                                                    ? 
                                                   (((QData)((IData)(
                                                                     (- (IData)(
                                                                                (ALU__DOT___io_Insn_rdresult_T_48 
                                                                                >> 0x1fU))))) 
                                                     << 0x20U) 
                                                    | (QData)((IData)(ALU__DOT___io_Insn_rdresult_T_48)))
                                                    : 0ULL))))))))));
    VL_EXTEND_WQ(127,64, __Vtemp2, (vlSelf->io_Insn_operand1 
                                    + vlSelf->io_Insn_operand2));
    VL_EXTEND_WQ(127,64, __Vtemp3, (vlSelf->io_Insn_operand1 
                                    - vlSelf->io_Insn_operand2));
    VL_EXTEND_WQ(127,64, __Vtemp4, vlSelf->io_Insn_operand1);
    VL_SHIFTL_WWI(127,127,6, __Vtemp5, __Vtemp4, (0x3fU 
                                                  & (IData)(vlSelf->io_Insn_operand2)));
    VL_EXTEND_WQ(127,64, __Vtemp6, ((3U == (IData)(vlSelf->io_Sub_OP_out))
                                     ? (QData)((IData)(
                                                       VL_LTS_IQQ(1,64,64, vlSelf->io_Insn_operand1, vlSelf->io_Insn_operand2)))
                                     : ((4U == (IData)(vlSelf->io_Sub_OP_out))
                                         ? (QData)((IData)(
                                                           (vlSelf->io_Insn_operand1 
                                                            < vlSelf->io_Insn_operand2)))
                                         : ALU__DOT___GEN_9)));
    vlSelf->io_Insn_rdresult = (((QData)((IData)(((0U 
                                                   == (IData)(vlSelf->io_Sub_OP_out))
                                                   ? 
                                                  __Vtemp2[1U]
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelf->io_Sub_OP_out))
                                                    ? 
                                                   __Vtemp3[1U]
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelf->io_Sub_OP_out))
                                                     ? 
                                                    __Vtemp5[1U]
                                                     : 
                                                    __Vtemp6[1U]))))) 
                                 << 0x20U) | (QData)((IData)(
                                                             ((0U 
                                                               == (IData)(vlSelf->io_Sub_OP_out))
                                                               ? 
                                                              __Vtemp2[0U]
                                                               : 
                                                              ((1U 
                                                                == (IData)(vlSelf->io_Sub_OP_out))
                                                                ? 
                                                               __Vtemp3[0U]
                                                                : 
                                                               ((2U 
                                                                 == (IData)(vlSelf->io_Sub_OP_out))
                                                                 ? 
                                                                __Vtemp5[0U]
                                                                 : 
                                                                __Vtemp6[0U]))))));
}

void Valu___024root___eval(Valu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval\n"); );
    // Body
    Valu___024root___combo__TOP__1(vlSelf);
}

QData Valu___024root___change_request_1(Valu___024root* vlSelf);

VL_INLINE_OPT QData Valu___024root___change_request(Valu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___change_request\n"); );
    // Body
    return (Valu___024root___change_request_1(vlSelf));
}

VL_INLINE_OPT QData Valu___024root___change_request_1(Valu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___change_request_1\n"); );
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void Valu___024root___eval_debug_assertions(Valu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Valu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_Sub_OP_out & 0xf0U))) {
        Verilated::overWidthError("io_Sub_OP_out");}
}
#endif  // VL_DEBUG
