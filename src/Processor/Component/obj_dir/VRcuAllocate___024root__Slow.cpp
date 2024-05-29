// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VRcuAllocate.h for the primary calling header

#include "VRcuAllocate___024root.h"
#include "VRcuAllocate__Syms.h"

//==========


void VRcuAllocate___024root___ctor_var_reset(VRcuAllocate___024root* vlSelf);

VRcuAllocate___024root::VRcuAllocate___024root(const char* _vcname__)
    : VerilatedModule(_vcname__)
 {
    // Reset structure values
    VRcuAllocate___024root___ctor_var_reset(this);
}

void VRcuAllocate___024root::__Vconfigure(VRcuAllocate__Syms* _vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->vlSymsp = _vlSymsp;
}

VRcuAllocate___024root::~VRcuAllocate___024root() {
}

void VRcuAllocate___024root___settle__TOP__2(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___settle__TOP__2\n"); );
    // Variables
    CData/*5:0*/ RcuAllocate__DOT___GEN_40;
    CData/*5:0*/ RcuAllocate__DOT___GEN_41;
    CData/*5:0*/ RcuAllocate__DOT___GEN_42;
    CData/*5:0*/ RcuAllocate__DOT___GEN_43;
    CData/*5:0*/ RcuAllocate__DOT___GEN_44;
    CData/*5:0*/ RcuAllocate__DOT___GEN_45;
    CData/*5:0*/ RcuAllocate__DOT___GEN_46;
    CData/*5:0*/ RcuAllocate__DOT___GEN_47;
    CData/*5:0*/ RcuAllocate__DOT___GEN_48;
    CData/*5:0*/ RcuAllocate__DOT___GEN_49;
    CData/*5:0*/ RcuAllocate__DOT___GEN_50;
    CData/*5:0*/ RcuAllocate__DOT___GEN_51;
    CData/*5:0*/ RcuAllocate__DOT___GEN_52;
    CData/*5:0*/ RcuAllocate__DOT___GEN_53;
    CData/*5:0*/ RcuAllocate__DOT___GEN_54;
    CData/*5:0*/ RcuAllocate__DOT___GEN_55;
    CData/*5:0*/ RcuAllocate__DOT___GEN_56;
    CData/*5:0*/ RcuAllocate__DOT___GEN_57;
    CData/*5:0*/ RcuAllocate__DOT___GEN_58;
    CData/*5:0*/ RcuAllocate__DOT___GEN_59;
    CData/*5:0*/ RcuAllocate__DOT___GEN_60;
    CData/*5:0*/ RcuAllocate__DOT___GEN_61;
    CData/*5:0*/ RcuAllocate__DOT___GEN_62;
    CData/*5:0*/ RcuAllocate__DOT___GEN_63;
    CData/*5:0*/ RcuAllocate__DOT___GEN_64;
    CData/*5:0*/ RcuAllocate__DOT___GEN_65;
    CData/*5:0*/ RcuAllocate__DOT___GEN_66;
    CData/*5:0*/ RcuAllocate__DOT___GEN_67;
    CData/*5:0*/ RcuAllocate__DOT___GEN_68;
    CData/*5:0*/ RcuAllocate__DOT___GEN_69;
    CData/*5:0*/ RcuAllocate__DOT___GEN_70;
    CData/*5:0*/ RcuAllocate__DOT___GEN_71;
    CData/*5:0*/ RcuAllocate__DOT___GEN_101;
    CData/*5:0*/ RcuAllocate__DOT___GEN_141;
    CData/*5:0*/ RcuAllocate__DOT___GEN_181;
    CData/*5:0*/ RcuAllocate__DOT___GEN_348;
    CData/*5:0*/ RcuAllocate__DOT___GEN_388;
    CData/*5:0*/ RcuAllocate__DOT___GEN_428;
    // Body
    vlSelf->io_WEN2_IntBusylist = ((2U == (IData)(vlSelf->io_allocCount)) 
                                   & (0U != (IData)(vlSelf->io_insn2_IsaRd)));
    vlSelf->io_WEN1_IntBusylist = ((0U < (IData)(vlSelf->io_allocCount)) 
                                   & (0U != (IData)(vlSelf->io_insn1_IsaRd)));
    vlSelf->io_insn1_PhyRd = ((0U < (IData)(vlSelf->io_allocCount))
                               ? ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                   ? (IData)(vlSelf->io_IntFreelist_phyrd1)
                                   : 0U) : 0U);
    if (vlSelf->io_EN_Update) {
        RcuAllocate__DOT___GEN_40 = ((0U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0));
        RcuAllocate__DOT___GEN_41 = ((1U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1));
        RcuAllocate__DOT___GEN_42 = ((2U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2));
        RcuAllocate__DOT___GEN_43 = ((3U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3));
        RcuAllocate__DOT___GEN_44 = ((4U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4));
        RcuAllocate__DOT___GEN_45 = ((5U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5));
        RcuAllocate__DOT___GEN_46 = ((6U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6));
        RcuAllocate__DOT___GEN_47 = ((7U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7));
        RcuAllocate__DOT___GEN_48 = ((8U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8));
        RcuAllocate__DOT___GEN_49 = ((9U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9));
        RcuAllocate__DOT___GEN_50 = ((0xaU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10));
        RcuAllocate__DOT___GEN_51 = ((0xbU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_11));
        RcuAllocate__DOT___GEN_52 = ((0xcU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_12));
        RcuAllocate__DOT___GEN_53 = ((0xdU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_13));
        RcuAllocate__DOT___GEN_54 = ((0xeU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_14));
        RcuAllocate__DOT___GEN_55 = ((0xfU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_15));
        RcuAllocate__DOT___GEN_56 = ((0x10U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_16));
        RcuAllocate__DOT___GEN_57 = ((0x11U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_17));
        RcuAllocate__DOT___GEN_58 = ((0x12U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_18));
        RcuAllocate__DOT___GEN_59 = ((0x13U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_19));
        RcuAllocate__DOT___GEN_60 = ((0x14U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_20));
        RcuAllocate__DOT___GEN_61 = ((0x15U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_21));
        RcuAllocate__DOT___GEN_62 = ((0x16U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22));
        RcuAllocate__DOT___GEN_63 = ((0x17U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23));
        RcuAllocate__DOT___GEN_64 = ((0x18U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24));
        RcuAllocate__DOT___GEN_65 = ((0x19U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25));
        RcuAllocate__DOT___GEN_66 = ((0x1aU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26));
        RcuAllocate__DOT___GEN_67 = ((0x1bU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27));
        RcuAllocate__DOT___GEN_68 = ((0x1cU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28));
        RcuAllocate__DOT___GEN_69 = ((0x1dU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29));
        RcuAllocate__DOT___GEN_70 = ((0x1eU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30));
        RcuAllocate__DOT___GEN_71 = ((0x1fU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31));
    } else {
        RcuAllocate__DOT___GEN_40 = vlSelf->RcuAllocate__DOT__IntRenameTable_0;
        RcuAllocate__DOT___GEN_41 = vlSelf->RcuAllocate__DOT__IntRenameTable_1;
        RcuAllocate__DOT___GEN_42 = vlSelf->RcuAllocate__DOT__IntRenameTable_2;
        RcuAllocate__DOT___GEN_43 = vlSelf->RcuAllocate__DOT__IntRenameTable_3;
        RcuAllocate__DOT___GEN_44 = vlSelf->RcuAllocate__DOT__IntRenameTable_4;
        RcuAllocate__DOT___GEN_45 = vlSelf->RcuAllocate__DOT__IntRenameTable_5;
        RcuAllocate__DOT___GEN_46 = vlSelf->RcuAllocate__DOT__IntRenameTable_6;
        RcuAllocate__DOT___GEN_47 = vlSelf->RcuAllocate__DOT__IntRenameTable_7;
        RcuAllocate__DOT___GEN_48 = vlSelf->RcuAllocate__DOT__IntRenameTable_8;
        RcuAllocate__DOT___GEN_49 = vlSelf->RcuAllocate__DOT__IntRenameTable_9;
        RcuAllocate__DOT___GEN_50 = vlSelf->RcuAllocate__DOT__IntRenameTable_10;
        RcuAllocate__DOT___GEN_51 = vlSelf->RcuAllocate__DOT__IntRenameTable_11;
        RcuAllocate__DOT___GEN_52 = vlSelf->RcuAllocate__DOT__IntRenameTable_12;
        RcuAllocate__DOT___GEN_53 = vlSelf->RcuAllocate__DOT__IntRenameTable_13;
        RcuAllocate__DOT___GEN_54 = vlSelf->RcuAllocate__DOT__IntRenameTable_14;
        RcuAllocate__DOT___GEN_55 = vlSelf->RcuAllocate__DOT__IntRenameTable_15;
        RcuAllocate__DOT___GEN_56 = vlSelf->RcuAllocate__DOT__IntRenameTable_16;
        RcuAllocate__DOT___GEN_57 = vlSelf->RcuAllocate__DOT__IntRenameTable_17;
        RcuAllocate__DOT___GEN_58 = vlSelf->RcuAllocate__DOT__IntRenameTable_18;
        RcuAllocate__DOT___GEN_59 = vlSelf->RcuAllocate__DOT__IntRenameTable_19;
        RcuAllocate__DOT___GEN_60 = vlSelf->RcuAllocate__DOT__IntRenameTable_20;
        RcuAllocate__DOT___GEN_61 = vlSelf->RcuAllocate__DOT__IntRenameTable_21;
        RcuAllocate__DOT___GEN_62 = vlSelf->RcuAllocate__DOT__IntRenameTable_22;
        RcuAllocate__DOT___GEN_63 = vlSelf->RcuAllocate__DOT__IntRenameTable_23;
        RcuAllocate__DOT___GEN_64 = vlSelf->RcuAllocate__DOT__IntRenameTable_24;
        RcuAllocate__DOT___GEN_65 = vlSelf->RcuAllocate__DOT__IntRenameTable_25;
        RcuAllocate__DOT___GEN_66 = vlSelf->RcuAllocate__DOT__IntRenameTable_26;
        RcuAllocate__DOT___GEN_67 = vlSelf->RcuAllocate__DOT__IntRenameTable_27;
        RcuAllocate__DOT___GEN_68 = vlSelf->RcuAllocate__DOT__IntRenameTable_28;
        RcuAllocate__DOT___GEN_69 = vlSelf->RcuAllocate__DOT__IntRenameTable_29;
        RcuAllocate__DOT___GEN_70 = vlSelf->RcuAllocate__DOT__IntRenameTable_30;
        RcuAllocate__DOT___GEN_71 = vlSelf->RcuAllocate__DOT__IntRenameTable_31;
    }
    RcuAllocate__DOT___GEN_101 = ((0x15U == (IData)(vlSelf->io_insn1_IsaRs1))
                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_21)
                                   : ((0x14U == (IData)(vlSelf->io_insn1_IsaRs1))
                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_20)
                                       : ((0x13U == (IData)(vlSelf->io_insn1_IsaRs1))
                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_19)
                                           : ((0x12U 
                                               == (IData)(vlSelf->io_insn1_IsaRs1))
                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_18)
                                               : ((0x11U 
                                                   == (IData)(vlSelf->io_insn1_IsaRs1))
                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_17)
                                                   : 
                                                  ((0x10U 
                                                    == (IData)(vlSelf->io_insn1_IsaRs1))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_16)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlSelf->io_insn1_IsaRs1))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_15)
                                                     : 
                                                    ((0xeU 
                                                      == (IData)(vlSelf->io_insn1_IsaRs1))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_14)
                                                      : 
                                                     ((0xdU 
                                                       == (IData)(vlSelf->io_insn1_IsaRs1))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_13)
                                                       : 
                                                      ((0xcU 
                                                        == (IData)(vlSelf->io_insn1_IsaRs1))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_12)
                                                        : 
                                                       ((0xbU 
                                                         == (IData)(vlSelf->io_insn1_IsaRs1))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_11)
                                                         : 
                                                        ((0xaU 
                                                          == (IData)(vlSelf->io_insn1_IsaRs1))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10)
                                                          : 
                                                         ((9U 
                                                           == (IData)(vlSelf->io_insn1_IsaRs1))
                                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9)
                                                           : 
                                                          ((8U 
                                                            == (IData)(vlSelf->io_insn1_IsaRs1))
                                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8)
                                                            : 
                                                           ((7U 
                                                             == (IData)(vlSelf->io_insn1_IsaRs1))
                                                             ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7)
                                                             : 
                                                            ((6U 
                                                              == (IData)(vlSelf->io_insn1_IsaRs1))
                                                              ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6)
                                                              : 
                                                             ((5U 
                                                               == (IData)(vlSelf->io_insn1_IsaRs1))
                                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5)
                                                               : 
                                                              ((4U 
                                                                == (IData)(vlSelf->io_insn1_IsaRs1))
                                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4)
                                                                : 
                                                               ((3U 
                                                                 == (IData)(vlSelf->io_insn1_IsaRs1))
                                                                 ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3)
                                                                 : 
                                                                ((2U 
                                                                  == (IData)(vlSelf->io_insn1_IsaRs1))
                                                                  ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2)
                                                                  : 
                                                                 ((1U 
                                                                   == (IData)(vlSelf->io_insn1_IsaRs1))
                                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1)
                                                                   : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0))))))))))))))))))))));
    RcuAllocate__DOT___GEN_141 = ((0x15U == (IData)(vlSelf->io_insn1_IsaRs2))
                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_21)
                                   : ((0x14U == (IData)(vlSelf->io_insn1_IsaRs2))
                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_20)
                                       : ((0x13U == (IData)(vlSelf->io_insn1_IsaRs2))
                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_19)
                                           : ((0x12U 
                                               == (IData)(vlSelf->io_insn1_IsaRs2))
                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_18)
                                               : ((0x11U 
                                                   == (IData)(vlSelf->io_insn1_IsaRs2))
                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_17)
                                                   : 
                                                  ((0x10U 
                                                    == (IData)(vlSelf->io_insn1_IsaRs2))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_16)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlSelf->io_insn1_IsaRs2))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_15)
                                                     : 
                                                    ((0xeU 
                                                      == (IData)(vlSelf->io_insn1_IsaRs2))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_14)
                                                      : 
                                                     ((0xdU 
                                                       == (IData)(vlSelf->io_insn1_IsaRs2))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_13)
                                                       : 
                                                      ((0xcU 
                                                        == (IData)(vlSelf->io_insn1_IsaRs2))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_12)
                                                        : 
                                                       ((0xbU 
                                                         == (IData)(vlSelf->io_insn1_IsaRs2))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_11)
                                                         : 
                                                        ((0xaU 
                                                          == (IData)(vlSelf->io_insn1_IsaRs2))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10)
                                                          : 
                                                         ((9U 
                                                           == (IData)(vlSelf->io_insn1_IsaRs2))
                                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9)
                                                           : 
                                                          ((8U 
                                                            == (IData)(vlSelf->io_insn1_IsaRs2))
                                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8)
                                                            : 
                                                           ((7U 
                                                             == (IData)(vlSelf->io_insn1_IsaRs2))
                                                             ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7)
                                                             : 
                                                            ((6U 
                                                              == (IData)(vlSelf->io_insn1_IsaRs2))
                                                              ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6)
                                                              : 
                                                             ((5U 
                                                               == (IData)(vlSelf->io_insn1_IsaRs2))
                                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5)
                                                               : 
                                                              ((4U 
                                                                == (IData)(vlSelf->io_insn1_IsaRs2))
                                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4)
                                                                : 
                                                               ((3U 
                                                                 == (IData)(vlSelf->io_insn1_IsaRs2))
                                                                 ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3)
                                                                 : 
                                                                ((2U 
                                                                  == (IData)(vlSelf->io_insn1_IsaRs2))
                                                                  ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2)
                                                                  : 
                                                                 ((1U 
                                                                   == (IData)(vlSelf->io_insn1_IsaRs2))
                                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1)
                                                                   : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0))))))))))))))))))))));
    RcuAllocate__DOT___GEN_181 = ((0x15U == (IData)(vlSelf->io_insn1_IsaRd))
                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_21)
                                   : ((0x14U == (IData)(vlSelf->io_insn1_IsaRd))
                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_20)
                                       : ((0x13U == (IData)(vlSelf->io_insn1_IsaRd))
                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_19)
                                           : ((0x12U 
                                               == (IData)(vlSelf->io_insn1_IsaRd))
                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_18)
                                               : ((0x11U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_17)
                                                   : 
                                                  ((0x10U 
                                                    == (IData)(vlSelf->io_insn1_IsaRd))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_16)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlSelf->io_insn1_IsaRd))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_15)
                                                     : 
                                                    ((0xeU 
                                                      == (IData)(vlSelf->io_insn1_IsaRd))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_14)
                                                      : 
                                                     ((0xdU 
                                                       == (IData)(vlSelf->io_insn1_IsaRd))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_13)
                                                       : 
                                                      ((0xcU 
                                                        == (IData)(vlSelf->io_insn1_IsaRd))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_12)
                                                        : 
                                                       ((0xbU 
                                                         == (IData)(vlSelf->io_insn1_IsaRd))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_11)
                                                         : 
                                                        ((0xaU 
                                                          == (IData)(vlSelf->io_insn1_IsaRd))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10)
                                                          : 
                                                         ((9U 
                                                           == (IData)(vlSelf->io_insn1_IsaRd))
                                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9)
                                                           : 
                                                          ((8U 
                                                            == (IData)(vlSelf->io_insn1_IsaRd))
                                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8)
                                                            : 
                                                           ((7U 
                                                             == (IData)(vlSelf->io_insn1_IsaRd))
                                                             ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7)
                                                             : 
                                                            ((6U 
                                                              == (IData)(vlSelf->io_insn1_IsaRd))
                                                              ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6)
                                                              : 
                                                             ((5U 
                                                               == (IData)(vlSelf->io_insn1_IsaRd))
                                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5)
                                                               : 
                                                              ((4U 
                                                                == (IData)(vlSelf->io_insn1_IsaRd))
                                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4)
                                                                : 
                                                               ((3U 
                                                                 == (IData)(vlSelf->io_insn1_IsaRd))
                                                                 ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3)
                                                                 : 
                                                                ((2U 
                                                                  == (IData)(vlSelf->io_insn1_IsaRd))
                                                                  ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2)
                                                                  : 
                                                                 ((1U 
                                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1)
                                                                   : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0))))))))))))))))))))));
    RcuAllocate__DOT___GEN_348 = ((0x15U == (IData)(vlSelf->io_insn2_IsaRs1))
                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_21)
                                   : ((0x14U == (IData)(vlSelf->io_insn2_IsaRs1))
                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_20)
                                       : ((0x13U == (IData)(vlSelf->io_insn2_IsaRs1))
                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_19)
                                           : ((0x12U 
                                               == (IData)(vlSelf->io_insn2_IsaRs1))
                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_18)
                                               : ((0x11U 
                                                   == (IData)(vlSelf->io_insn2_IsaRs1))
                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_17)
                                                   : 
                                                  ((0x10U 
                                                    == (IData)(vlSelf->io_insn2_IsaRs1))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_16)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlSelf->io_insn2_IsaRs1))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_15)
                                                     : 
                                                    ((0xeU 
                                                      == (IData)(vlSelf->io_insn2_IsaRs1))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_14)
                                                      : 
                                                     ((0xdU 
                                                       == (IData)(vlSelf->io_insn2_IsaRs1))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_13)
                                                       : 
                                                      ((0xcU 
                                                        == (IData)(vlSelf->io_insn2_IsaRs1))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_12)
                                                        : 
                                                       ((0xbU 
                                                         == (IData)(vlSelf->io_insn2_IsaRs1))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_11)
                                                         : 
                                                        ((0xaU 
                                                          == (IData)(vlSelf->io_insn2_IsaRs1))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10)
                                                          : 
                                                         ((9U 
                                                           == (IData)(vlSelf->io_insn2_IsaRs1))
                                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9)
                                                           : 
                                                          ((8U 
                                                            == (IData)(vlSelf->io_insn2_IsaRs1))
                                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8)
                                                            : 
                                                           ((7U 
                                                             == (IData)(vlSelf->io_insn2_IsaRs1))
                                                             ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7)
                                                             : 
                                                            ((6U 
                                                              == (IData)(vlSelf->io_insn2_IsaRs1))
                                                              ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6)
                                                              : 
                                                             ((5U 
                                                               == (IData)(vlSelf->io_insn2_IsaRs1))
                                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5)
                                                               : 
                                                              ((4U 
                                                                == (IData)(vlSelf->io_insn2_IsaRs1))
                                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4)
                                                                : 
                                                               ((3U 
                                                                 == (IData)(vlSelf->io_insn2_IsaRs1))
                                                                 ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3)
                                                                 : 
                                                                ((2U 
                                                                  == (IData)(vlSelf->io_insn2_IsaRs1))
                                                                  ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2)
                                                                  : 
                                                                 ((1U 
                                                                   == (IData)(vlSelf->io_insn2_IsaRs1))
                                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1)
                                                                   : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0))))))))))))))))))))));
    RcuAllocate__DOT___GEN_388 = ((0x15U == (IData)(vlSelf->io_insn2_IsaRs2))
                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_21)
                                   : ((0x14U == (IData)(vlSelf->io_insn2_IsaRs2))
                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_20)
                                       : ((0x13U == (IData)(vlSelf->io_insn2_IsaRs2))
                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_19)
                                           : ((0x12U 
                                               == (IData)(vlSelf->io_insn2_IsaRs2))
                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_18)
                                               : ((0x11U 
                                                   == (IData)(vlSelf->io_insn2_IsaRs2))
                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_17)
                                                   : 
                                                  ((0x10U 
                                                    == (IData)(vlSelf->io_insn2_IsaRs2))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_16)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlSelf->io_insn2_IsaRs2))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_15)
                                                     : 
                                                    ((0xeU 
                                                      == (IData)(vlSelf->io_insn2_IsaRs2))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_14)
                                                      : 
                                                     ((0xdU 
                                                       == (IData)(vlSelf->io_insn2_IsaRs2))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_13)
                                                       : 
                                                      ((0xcU 
                                                        == (IData)(vlSelf->io_insn2_IsaRs2))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_12)
                                                        : 
                                                       ((0xbU 
                                                         == (IData)(vlSelf->io_insn2_IsaRs2))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_11)
                                                         : 
                                                        ((0xaU 
                                                          == (IData)(vlSelf->io_insn2_IsaRs2))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10)
                                                          : 
                                                         ((9U 
                                                           == (IData)(vlSelf->io_insn2_IsaRs2))
                                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9)
                                                           : 
                                                          ((8U 
                                                            == (IData)(vlSelf->io_insn2_IsaRs2))
                                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8)
                                                            : 
                                                           ((7U 
                                                             == (IData)(vlSelf->io_insn2_IsaRs2))
                                                             ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7)
                                                             : 
                                                            ((6U 
                                                              == (IData)(vlSelf->io_insn2_IsaRs2))
                                                              ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6)
                                                              : 
                                                             ((5U 
                                                               == (IData)(vlSelf->io_insn2_IsaRs2))
                                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5)
                                                               : 
                                                              ((4U 
                                                                == (IData)(vlSelf->io_insn2_IsaRs2))
                                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4)
                                                                : 
                                                               ((3U 
                                                                 == (IData)(vlSelf->io_insn2_IsaRs2))
                                                                 ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3)
                                                                 : 
                                                                ((2U 
                                                                  == (IData)(vlSelf->io_insn2_IsaRs2))
                                                                  ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2)
                                                                  : 
                                                                 ((1U 
                                                                   == (IData)(vlSelf->io_insn2_IsaRs2))
                                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1)
                                                                   : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0))))))))))))))))))))));
    RcuAllocate__DOT___GEN_428 = ((0x15U == (IData)(vlSelf->io_insn2_IsaRd))
                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_21)
                                   : ((0x14U == (IData)(vlSelf->io_insn2_IsaRd))
                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_20)
                                       : ((0x13U == (IData)(vlSelf->io_insn2_IsaRd))
                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_19)
                                           : ((0x12U 
                                               == (IData)(vlSelf->io_insn2_IsaRd))
                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_18)
                                               : ((0x11U 
                                                   == (IData)(vlSelf->io_insn2_IsaRd))
                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_17)
                                                   : 
                                                  ((0x10U 
                                                    == (IData)(vlSelf->io_insn2_IsaRd))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_16)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlSelf->io_insn2_IsaRd))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_15)
                                                     : 
                                                    ((0xeU 
                                                      == (IData)(vlSelf->io_insn2_IsaRd))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_14)
                                                      : 
                                                     ((0xdU 
                                                       == (IData)(vlSelf->io_insn2_IsaRd))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_13)
                                                       : 
                                                      ((0xcU 
                                                        == (IData)(vlSelf->io_insn2_IsaRd))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_12)
                                                        : 
                                                       ((0xbU 
                                                         == (IData)(vlSelf->io_insn2_IsaRd))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_11)
                                                         : 
                                                        ((0xaU 
                                                          == (IData)(vlSelf->io_insn2_IsaRd))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10)
                                                          : 
                                                         ((9U 
                                                           == (IData)(vlSelf->io_insn2_IsaRd))
                                                           ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9)
                                                           : 
                                                          ((8U 
                                                            == (IData)(vlSelf->io_insn2_IsaRd))
                                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8)
                                                            : 
                                                           ((7U 
                                                             == (IData)(vlSelf->io_insn2_IsaRd))
                                                             ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7)
                                                             : 
                                                            ((6U 
                                                              == (IData)(vlSelf->io_insn2_IsaRd))
                                                              ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6)
                                                              : 
                                                             ((5U 
                                                               == (IData)(vlSelf->io_insn2_IsaRd))
                                                               ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5)
                                                               : 
                                                              ((4U 
                                                                == (IData)(vlSelf->io_insn2_IsaRd))
                                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4)
                                                                : 
                                                               ((3U 
                                                                 == (IData)(vlSelf->io_insn2_IsaRd))
                                                                 ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3)
                                                                 : 
                                                                ((2U 
                                                                  == (IData)(vlSelf->io_insn2_IsaRd))
                                                                  ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2)
                                                                  : 
                                                                 ((1U 
                                                                   == (IData)(vlSelf->io_insn2_IsaRd))
                                                                   ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1)
                                                                   : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0))))))))))))))))))))));
    vlSelf->io_Freelist_pop_num = (((IData)(vlSelf->io_WEN1_IntBusylist) 
                                    & (IData)(vlSelf->io_WEN2_IntBusylist))
                                    ? 2U : ((IData)(vlSelf->io_WEN1_IntBusylist) 
                                            | (IData)(vlSelf->io_WEN2_IntBusylist)));
    vlSelf->io_insn2_PhyRd = ((2U == (IData)(vlSelf->io_allocCount))
                               ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                                   ? ((IData)(vlSelf->io_WEN1_IntBusylist)
                                       ? (IData)(vlSelf->io_IntFreelist_phyrd2)
                                       : (IData)(vlSelf->io_IntFreelist_phyrd1))
                                   : 0U) : 0U);
    if ((0U < (IData)(vlSelf->io_allocCount))) {
        vlSelf->RcuAllocate__DOT___GEN_287 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_40))
                                               : (IData)(RcuAllocate__DOT___GEN_40));
        vlSelf->RcuAllocate__DOT___GEN_288 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((1U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_41))
                                               : (IData)(RcuAllocate__DOT___GEN_41));
        vlSelf->RcuAllocate__DOT___GEN_289 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((2U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_42))
                                               : (IData)(RcuAllocate__DOT___GEN_42));
        vlSelf->RcuAllocate__DOT___GEN_290 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((3U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_43))
                                               : (IData)(RcuAllocate__DOT___GEN_43));
        vlSelf->RcuAllocate__DOT___GEN_291 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((4U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_44))
                                               : (IData)(RcuAllocate__DOT___GEN_44));
        vlSelf->RcuAllocate__DOT___GEN_292 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((5U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_45))
                                               : (IData)(RcuAllocate__DOT___GEN_45));
        vlSelf->RcuAllocate__DOT___GEN_293 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((6U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_46))
                                               : (IData)(RcuAllocate__DOT___GEN_46));
        vlSelf->RcuAllocate__DOT___GEN_294 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((7U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_47))
                                               : (IData)(RcuAllocate__DOT___GEN_47));
        vlSelf->RcuAllocate__DOT___GEN_295 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((8U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_48))
                                               : (IData)(RcuAllocate__DOT___GEN_48));
        vlSelf->RcuAllocate__DOT___GEN_296 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((9U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_49))
                                               : (IData)(RcuAllocate__DOT___GEN_49));
        vlSelf->RcuAllocate__DOT___GEN_297 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0xaU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_50))
                                               : (IData)(RcuAllocate__DOT___GEN_50));
        vlSelf->RcuAllocate__DOT___GEN_298 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0xbU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_51))
                                               : (IData)(RcuAllocate__DOT___GEN_51));
        vlSelf->RcuAllocate__DOT___GEN_299 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0xcU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_52))
                                               : (IData)(RcuAllocate__DOT___GEN_52));
        vlSelf->RcuAllocate__DOT___GEN_300 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0xdU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_53))
                                               : (IData)(RcuAllocate__DOT___GEN_53));
        vlSelf->RcuAllocate__DOT___GEN_301 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0xeU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_54))
                                               : (IData)(RcuAllocate__DOT___GEN_54));
        vlSelf->RcuAllocate__DOT___GEN_302 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0xfU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_55))
                                               : (IData)(RcuAllocate__DOT___GEN_55));
        vlSelf->RcuAllocate__DOT___GEN_303 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x10U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_56))
                                               : (IData)(RcuAllocate__DOT___GEN_56));
        vlSelf->RcuAllocate__DOT___GEN_304 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x11U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_57))
                                               : (IData)(RcuAllocate__DOT___GEN_57));
        vlSelf->RcuAllocate__DOT___GEN_305 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x12U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_58))
                                               : (IData)(RcuAllocate__DOT___GEN_58));
        vlSelf->RcuAllocate__DOT___GEN_306 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x13U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_59))
                                               : (IData)(RcuAllocate__DOT___GEN_59));
        vlSelf->RcuAllocate__DOT___GEN_307 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x14U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_60))
                                               : (IData)(RcuAllocate__DOT___GEN_60));
        vlSelf->RcuAllocate__DOT___GEN_308 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x15U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_61))
                                               : (IData)(RcuAllocate__DOT___GEN_61));
        vlSelf->RcuAllocate__DOT___GEN_309 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x16U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_62))
                                               : (IData)(RcuAllocate__DOT___GEN_62));
        vlSelf->RcuAllocate__DOT___GEN_310 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x17U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_63))
                                               : (IData)(RcuAllocate__DOT___GEN_63));
        vlSelf->RcuAllocate__DOT___GEN_311 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x18U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_64))
                                               : (IData)(RcuAllocate__DOT___GEN_64));
        vlSelf->RcuAllocate__DOT___GEN_312 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x19U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_65))
                                               : (IData)(RcuAllocate__DOT___GEN_65));
        vlSelf->RcuAllocate__DOT___GEN_313 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1aU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_66))
                                               : (IData)(RcuAllocate__DOT___GEN_66));
        vlSelf->RcuAllocate__DOT___GEN_314 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1bU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_67))
                                               : (IData)(RcuAllocate__DOT___GEN_67));
        vlSelf->RcuAllocate__DOT___GEN_315 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1cU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_68))
                                               : (IData)(RcuAllocate__DOT___GEN_68));
        vlSelf->RcuAllocate__DOT___GEN_316 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1dU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_69))
                                               : (IData)(RcuAllocate__DOT___GEN_69));
        vlSelf->RcuAllocate__DOT___GEN_317 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1eU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_70))
                                               : (IData)(RcuAllocate__DOT___GEN_70));
        vlSelf->RcuAllocate__DOT___GEN_318 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1fU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_71))
                                               : (IData)(RcuAllocate__DOT___GEN_71));
        vlSelf->io_insn1_PhyRs1 = ((0x1fU == (IData)(vlSelf->io_insn1_IsaRs1))
                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31)
                                    : ((0x1eU == (IData)(vlSelf->io_insn1_IsaRs1))
                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30)
                                        : ((0x1dU == (IData)(vlSelf->io_insn1_IsaRs1))
                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29)
                                            : ((0x1cU 
                                                == (IData)(vlSelf->io_insn1_IsaRs1))
                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28)
                                                : (
                                                   (0x1bU 
                                                    == (IData)(vlSelf->io_insn1_IsaRs1))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27)
                                                    : 
                                                   ((0x1aU 
                                                     == (IData)(vlSelf->io_insn1_IsaRs1))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26)
                                                     : 
                                                    ((0x19U 
                                                      == (IData)(vlSelf->io_insn1_IsaRs1))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25)
                                                      : 
                                                     ((0x18U 
                                                       == (IData)(vlSelf->io_insn1_IsaRs1))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24)
                                                       : 
                                                      ((0x17U 
                                                        == (IData)(vlSelf->io_insn1_IsaRs1))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23)
                                                        : 
                                                       ((0x16U 
                                                         == (IData)(vlSelf->io_insn1_IsaRs1))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22)
                                                         : (IData)(RcuAllocate__DOT___GEN_101)))))))))));
        vlSelf->io_insn1_PhyRs2 = ((0x1fU == (IData)(vlSelf->io_insn1_IsaRs2))
                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31)
                                    : ((0x1eU == (IData)(vlSelf->io_insn1_IsaRs2))
                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30)
                                        : ((0x1dU == (IData)(vlSelf->io_insn1_IsaRs2))
                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29)
                                            : ((0x1cU 
                                                == (IData)(vlSelf->io_insn1_IsaRs2))
                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28)
                                                : (
                                                   (0x1bU 
                                                    == (IData)(vlSelf->io_insn1_IsaRs2))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27)
                                                    : 
                                                   ((0x1aU 
                                                     == (IData)(vlSelf->io_insn1_IsaRs2))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26)
                                                     : 
                                                    ((0x19U 
                                                      == (IData)(vlSelf->io_insn1_IsaRs2))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25)
                                                      : 
                                                     ((0x18U 
                                                       == (IData)(vlSelf->io_insn1_IsaRs2))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24)
                                                       : 
                                                      ((0x17U 
                                                        == (IData)(vlSelf->io_insn1_IsaRs2))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23)
                                                        : 
                                                       ((0x16U 
                                                         == (IData)(vlSelf->io_insn1_IsaRs2))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22)
                                                         : (IData)(RcuAllocate__DOT___GEN_141)))))))))));
        vlSelf->io_insn1_LPhyRd = ((0x1fU == (IData)(vlSelf->io_insn1_IsaRd))
                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31)
                                    : ((0x1eU == (IData)(vlSelf->io_insn1_IsaRd))
                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30)
                                        : ((0x1dU == (IData)(vlSelf->io_insn1_IsaRd))
                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29)
                                            : ((0x1cU 
                                                == (IData)(vlSelf->io_insn1_IsaRd))
                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28)
                                                : (
                                                   (0x1bU 
                                                    == (IData)(vlSelf->io_insn1_IsaRd))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27)
                                                    : 
                                                   ((0x1aU 
                                                     == (IData)(vlSelf->io_insn1_IsaRd))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26)
                                                     : 
                                                    ((0x19U 
                                                      == (IData)(vlSelf->io_insn1_IsaRd))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25)
                                                      : 
                                                     ((0x18U 
                                                       == (IData)(vlSelf->io_insn1_IsaRd))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24)
                                                       : 
                                                      ((0x17U 
                                                        == (IData)(vlSelf->io_insn1_IsaRd))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23)
                                                        : 
                                                       ((0x16U 
                                                         == (IData)(vlSelf->io_insn1_IsaRd))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22)
                                                         : (IData)(RcuAllocate__DOT___GEN_181)))))))))));
    } else {
        vlSelf->RcuAllocate__DOT___GEN_287 = RcuAllocate__DOT___GEN_40;
        vlSelf->RcuAllocate__DOT___GEN_288 = RcuAllocate__DOT___GEN_41;
        vlSelf->RcuAllocate__DOT___GEN_289 = RcuAllocate__DOT___GEN_42;
        vlSelf->RcuAllocate__DOT___GEN_290 = RcuAllocate__DOT___GEN_43;
        vlSelf->RcuAllocate__DOT___GEN_291 = RcuAllocate__DOT___GEN_44;
        vlSelf->RcuAllocate__DOT___GEN_292 = RcuAllocate__DOT___GEN_45;
        vlSelf->RcuAllocate__DOT___GEN_293 = RcuAllocate__DOT___GEN_46;
        vlSelf->RcuAllocate__DOT___GEN_294 = RcuAllocate__DOT___GEN_47;
        vlSelf->RcuAllocate__DOT___GEN_295 = RcuAllocate__DOT___GEN_48;
        vlSelf->RcuAllocate__DOT___GEN_296 = RcuAllocate__DOT___GEN_49;
        vlSelf->RcuAllocate__DOT___GEN_297 = RcuAllocate__DOT___GEN_50;
        vlSelf->RcuAllocate__DOT___GEN_298 = RcuAllocate__DOT___GEN_51;
        vlSelf->RcuAllocate__DOT___GEN_299 = RcuAllocate__DOT___GEN_52;
        vlSelf->RcuAllocate__DOT___GEN_300 = RcuAllocate__DOT___GEN_53;
        vlSelf->RcuAllocate__DOT___GEN_301 = RcuAllocate__DOT___GEN_54;
        vlSelf->RcuAllocate__DOT___GEN_302 = RcuAllocate__DOT___GEN_55;
        vlSelf->RcuAllocate__DOT___GEN_303 = RcuAllocate__DOT___GEN_56;
        vlSelf->RcuAllocate__DOT___GEN_304 = RcuAllocate__DOT___GEN_57;
        vlSelf->RcuAllocate__DOT___GEN_305 = RcuAllocate__DOT___GEN_58;
        vlSelf->RcuAllocate__DOT___GEN_306 = RcuAllocate__DOT___GEN_59;
        vlSelf->RcuAllocate__DOT___GEN_307 = RcuAllocate__DOT___GEN_60;
        vlSelf->RcuAllocate__DOT___GEN_308 = RcuAllocate__DOT___GEN_61;
        vlSelf->RcuAllocate__DOT___GEN_309 = RcuAllocate__DOT___GEN_62;
        vlSelf->RcuAllocate__DOT___GEN_310 = RcuAllocate__DOT___GEN_63;
        vlSelf->RcuAllocate__DOT___GEN_311 = RcuAllocate__DOT___GEN_64;
        vlSelf->RcuAllocate__DOT___GEN_312 = RcuAllocate__DOT___GEN_65;
        vlSelf->RcuAllocate__DOT___GEN_313 = RcuAllocate__DOT___GEN_66;
        vlSelf->RcuAllocate__DOT___GEN_314 = RcuAllocate__DOT___GEN_67;
        vlSelf->RcuAllocate__DOT___GEN_315 = RcuAllocate__DOT___GEN_68;
        vlSelf->RcuAllocate__DOT___GEN_316 = RcuAllocate__DOT___GEN_69;
        vlSelf->RcuAllocate__DOT___GEN_317 = RcuAllocate__DOT___GEN_70;
        vlSelf->RcuAllocate__DOT___GEN_318 = RcuAllocate__DOT___GEN_71;
        vlSelf->io_insn1_PhyRs1 = 0U;
        vlSelf->io_insn1_PhyRs2 = 0U;
        vlSelf->io_insn1_LPhyRd = 0U;
    }
    if ((2U == (IData)(vlSelf->io_allocCount))) {
        vlSelf->io_insn2_PhyRs1 = (((0U != (IData)(vlSelf->io_insn1_IsaRd)) 
                                    & ((IData)(vlSelf->io_insn1_IsaRd) 
                                       == (IData)(vlSelf->io_insn2_IsaRs1)))
                                    ? (IData)(vlSelf->io_insn1_PhyRd)
                                    : ((0x1fU == (IData)(vlSelf->io_insn2_IsaRs1))
                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31)
                                        : ((0x1eU == (IData)(vlSelf->io_insn2_IsaRs1))
                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30)
                                            : ((0x1dU 
                                                == (IData)(vlSelf->io_insn2_IsaRs1))
                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29)
                                                : (
                                                   (0x1cU 
                                                    == (IData)(vlSelf->io_insn2_IsaRs1))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28)
                                                    : 
                                                   ((0x1bU 
                                                     == (IData)(vlSelf->io_insn2_IsaRs1))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27)
                                                     : 
                                                    ((0x1aU 
                                                      == (IData)(vlSelf->io_insn2_IsaRs1))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26)
                                                      : 
                                                     ((0x19U 
                                                       == (IData)(vlSelf->io_insn2_IsaRs1))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25)
                                                       : 
                                                      ((0x18U 
                                                        == (IData)(vlSelf->io_insn2_IsaRs1))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24)
                                                        : 
                                                       ((0x17U 
                                                         == (IData)(vlSelf->io_insn2_IsaRs1))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23)
                                                         : 
                                                        ((0x16U 
                                                          == (IData)(vlSelf->io_insn2_IsaRs1))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22)
                                                          : (IData)(RcuAllocate__DOT___GEN_348))))))))))));
        vlSelf->io_insn2_PhyRs2 = (((0U != (IData)(vlSelf->io_insn1_IsaRd)) 
                                    & ((IData)(vlSelf->io_insn1_IsaRd) 
                                       == (IData)(vlSelf->io_insn2_IsaRs2)))
                                    ? (IData)(vlSelf->io_insn1_PhyRd)
                                    : ((0x1fU == (IData)(vlSelf->io_insn2_IsaRs2))
                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31)
                                        : ((0x1eU == (IData)(vlSelf->io_insn2_IsaRs2))
                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30)
                                            : ((0x1dU 
                                                == (IData)(vlSelf->io_insn2_IsaRs2))
                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29)
                                                : (
                                                   (0x1cU 
                                                    == (IData)(vlSelf->io_insn2_IsaRs2))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28)
                                                    : 
                                                   ((0x1bU 
                                                     == (IData)(vlSelf->io_insn2_IsaRs2))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27)
                                                     : 
                                                    ((0x1aU 
                                                      == (IData)(vlSelf->io_insn2_IsaRs2))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26)
                                                      : 
                                                     ((0x19U 
                                                       == (IData)(vlSelf->io_insn2_IsaRs2))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25)
                                                       : 
                                                      ((0x18U 
                                                        == (IData)(vlSelf->io_insn2_IsaRs2))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24)
                                                        : 
                                                       ((0x17U 
                                                         == (IData)(vlSelf->io_insn2_IsaRs2))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23)
                                                         : 
                                                        ((0x16U 
                                                          == (IData)(vlSelf->io_insn2_IsaRs2))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22)
                                                          : (IData)(RcuAllocate__DOT___GEN_388))))))))))));
        vlSelf->io_insn2_LPhyRd = (((0U != (IData)(vlSelf->io_insn1_IsaRd)) 
                                    & ((IData)(vlSelf->io_insn1_IsaRd) 
                                       == (IData)(vlSelf->io_insn2_IsaRd)))
                                    ? (IData)(vlSelf->io_insn1_PhyRd)
                                    : ((0x1fU == (IData)(vlSelf->io_insn2_IsaRd))
                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31)
                                        : ((0x1eU == (IData)(vlSelf->io_insn2_IsaRd))
                                            ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30)
                                            : ((0x1dU 
                                                == (IData)(vlSelf->io_insn2_IsaRd))
                                                ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29)
                                                : (
                                                   (0x1cU 
                                                    == (IData)(vlSelf->io_insn2_IsaRd))
                                                    ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28)
                                                    : 
                                                   ((0x1bU 
                                                     == (IData)(vlSelf->io_insn2_IsaRd))
                                                     ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27)
                                                     : 
                                                    ((0x1aU 
                                                      == (IData)(vlSelf->io_insn2_IsaRd))
                                                      ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26)
                                                      : 
                                                     ((0x19U 
                                                       == (IData)(vlSelf->io_insn2_IsaRd))
                                                       ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25)
                                                       : 
                                                      ((0x18U 
                                                        == (IData)(vlSelf->io_insn2_IsaRd))
                                                        ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24)
                                                        : 
                                                       ((0x17U 
                                                         == (IData)(vlSelf->io_insn2_IsaRd))
                                                         ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23)
                                                         : 
                                                        ((0x16U 
                                                          == (IData)(vlSelf->io_insn2_IsaRd))
                                                          ? (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22)
                                                          : (IData)(RcuAllocate__DOT___GEN_428))))))))))));
    } else {
        vlSelf->io_insn2_PhyRs1 = 0U;
        vlSelf->io_insn2_PhyRs2 = 0U;
        vlSelf->io_insn2_LPhyRd = 0U;
    }
}

void VRcuAllocate___024root___eval_initial(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vclklast__TOP__clock = vlSelf->clock;
}

void VRcuAllocate___024root___eval_settle(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___eval_settle\n"); );
    // Body
    VRcuAllocate___024root___settle__TOP__2(vlSelf);
}

void VRcuAllocate___024root___final(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___final\n"); );
}

void VRcuAllocate___024root___ctor_var_reset(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clock = VL_RAND_RESET_I(1);
    vlSelf->reset = VL_RAND_RESET_I(1);
    vlSelf->io_allocCount = VL_RAND_RESET_I(2);
    vlSelf->io_EN_Update = VL_RAND_RESET_I(1);
    vlSelf->io_IsaRd_Update = VL_RAND_RESET_I(5);
    vlSelf->io_Data_Update = VL_RAND_RESET_I(6);
    vlSelf->io_IntFreelist_phyrd1 = VL_RAND_RESET_I(6);
    vlSelf->io_IntFreelist_phyrd2 = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_IsaRs1 = VL_RAND_RESET_I(5);
    vlSelf->io_insn1_IsaRs2 = VL_RAND_RESET_I(5);
    vlSelf->io_insn1_IsaRd = VL_RAND_RESET_I(5);
    vlSelf->io_insn2_IsaRs1 = VL_RAND_RESET_I(5);
    vlSelf->io_insn2_IsaRs2 = VL_RAND_RESET_I(5);
    vlSelf->io_insn2_IsaRd = VL_RAND_RESET_I(5);
    vlSelf->io_WEN1_IntBusylist = VL_RAND_RESET_I(1);
    vlSelf->io_WEN2_IntBusylist = VL_RAND_RESET_I(1);
    vlSelf->io_insn1_PhyRs1 = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_PhyRs2 = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_PhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_insn1_LPhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRs1 = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRs2 = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_PhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_insn2_LPhyRd = VL_RAND_RESET_I(6);
    vlSelf->io_Freelist_pop_num = VL_RAND_RESET_I(2);
    vlSelf->RcuAllocate__DOT__IntRenameTable_0 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_1 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_2 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_3 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_4 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_5 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_6 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_7 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_8 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_9 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_10 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_11 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_12 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_13 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_14 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_15 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_16 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_17 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_18 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_19 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_20 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_21 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_22 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_23 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_24 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_25 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_26 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_27 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_28 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_29 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_30 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT__IntRenameTable_31 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_287 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_288 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_289 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_290 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_291 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_292 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_293 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_294 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_295 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_296 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_297 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_298 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_299 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_300 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_301 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_302 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_303 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_304 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_305 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_306 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_307 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_308 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_309 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_310 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_311 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_312 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_313 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_314 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_315 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_316 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_317 = VL_RAND_RESET_I(6);
    vlSelf->RcuAllocate__DOT___GEN_318 = VL_RAND_RESET_I(6);
}
