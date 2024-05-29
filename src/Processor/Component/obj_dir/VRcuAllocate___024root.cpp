// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VRcuAllocate.h for the primary calling header

#include "VRcuAllocate___024root.h"
#include "VRcuAllocate__Syms.h"

//==========

VL_INLINE_OPT void VRcuAllocate___024root___combo__TOP__1(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___combo__TOP__1\n"); );
    // Body
    vlSelf->io_WEN2_IntBusylist = ((2U == (IData)(vlSelf->io_allocCount)) 
                                   & (0U != (IData)(vlSelf->io_insn2_IsaRd)));
    vlSelf->io_WEN1_IntBusylist = ((0U < (IData)(vlSelf->io_allocCount)) 
                                   & (0U != (IData)(vlSelf->io_insn1_IsaRd)));
    vlSelf->io_insn1_PhyRd = ((0U < (IData)(vlSelf->io_allocCount))
                               ? ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                   ? (IData)(vlSelf->io_IntFreelist_phyrd1)
                                   : 0U) : 0U);
    vlSelf->io_Freelist_pop_num = (((IData)(vlSelf->io_WEN1_IntBusylist) 
                                    & (IData)(vlSelf->io_WEN2_IntBusylist))
                                    ? 2U : ((IData)(vlSelf->io_WEN1_IntBusylist) 
                                            | (IData)(vlSelf->io_WEN2_IntBusylist)));
}

VL_INLINE_OPT void VRcuAllocate___024root___sequent__TOP__3(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___sequent__TOP__3\n"); );
    // Body
    if (vlSelf->reset) {
        vlSelf->RcuAllocate__DOT__IntRenameTable_31 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_30 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_29 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_28 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_27 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_26 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_25 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_24 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_23 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_22 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_9 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_8 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_7 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_6 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_5 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_1 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_10 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_0 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_4 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_2 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_3 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_11 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_12 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_13 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_14 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_15 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_16 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_17 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_18 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_19 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_20 = 0U;
        vlSelf->RcuAllocate__DOT__IntRenameTable_21 = 0U;
    } else {
        vlSelf->RcuAllocate__DOT__IntRenameTable_31 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x1fU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_318))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_318))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_318));
        vlSelf->RcuAllocate__DOT__IntRenameTable_30 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x1eU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_317))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_317))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_317));
        vlSelf->RcuAllocate__DOT__IntRenameTable_29 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x1dU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_316))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_316))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_316));
        vlSelf->RcuAllocate__DOT__IntRenameTable_28 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x1cU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_315))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_315))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_315));
        vlSelf->RcuAllocate__DOT__IntRenameTable_27 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x1bU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_314))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_314))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_314));
        vlSelf->RcuAllocate__DOT__IntRenameTable_26 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x1aU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_313))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_313))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_313));
        vlSelf->RcuAllocate__DOT__IntRenameTable_25 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x19U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_312))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_312))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_312));
        vlSelf->RcuAllocate__DOT__IntRenameTable_24 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x18U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_311))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_311))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_311));
        vlSelf->RcuAllocate__DOT__IntRenameTable_23 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x17U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_310))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_310))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_310));
        vlSelf->RcuAllocate__DOT__IntRenameTable_22 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x16U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_309))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_309))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_309));
        vlSelf->RcuAllocate__DOT__IntRenameTable_9 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((9U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_296))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_296))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_296));
        vlSelf->RcuAllocate__DOT__IntRenameTable_8 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((8U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_295))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_295))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_295));
        vlSelf->RcuAllocate__DOT__IntRenameTable_7 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((7U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_294))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_294))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_294));
        vlSelf->RcuAllocate__DOT__IntRenameTable_6 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((6U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_293))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_293))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_293));
        vlSelf->RcuAllocate__DOT__IntRenameTable_5 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((5U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_292))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_292))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_292));
        vlSelf->RcuAllocate__DOT__IntRenameTable_1 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((1U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_288))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_288))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_288));
        vlSelf->RcuAllocate__DOT__IntRenameTable_10 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0xaU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_297))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_297))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_297));
        vlSelf->RcuAllocate__DOT__IntRenameTable_0 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_287))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_287))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_287));
        vlSelf->RcuAllocate__DOT__IntRenameTable_4 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((4U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_291))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_291))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_291));
        vlSelf->RcuAllocate__DOT__IntRenameTable_2 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((2U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_289))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_289))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_289));
        vlSelf->RcuAllocate__DOT__IntRenameTable_3 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((3U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_290))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_290))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_290));
        vlSelf->RcuAllocate__DOT__IntRenameTable_11 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0xbU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_298))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_298))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_298));
        vlSelf->RcuAllocate__DOT__IntRenameTable_12 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0xcU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_299))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_299))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_299));
        vlSelf->RcuAllocate__DOT__IntRenameTable_13 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0xdU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_300))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_300))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_300));
        vlSelf->RcuAllocate__DOT__IntRenameTable_14 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0xeU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_301))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_301))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_301));
        vlSelf->RcuAllocate__DOT__IntRenameTable_15 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0xfU == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_302))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_302))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_302));
        vlSelf->RcuAllocate__DOT__IntRenameTable_16 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x10U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_303))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_303))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_303));
        vlSelf->RcuAllocate__DOT__IntRenameTable_17 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x11U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_304))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_304))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_304));
        vlSelf->RcuAllocate__DOT__IntRenameTable_18 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x12U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_305))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_305))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_305));
        vlSelf->RcuAllocate__DOT__IntRenameTable_19 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x13U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_306))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_306))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_306));
        vlSelf->RcuAllocate__DOT__IntRenameTable_20 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x14U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_307))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_307))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_307));
        vlSelf->RcuAllocate__DOT__IntRenameTable_21 
            = ((2U == (IData)(vlSelf->io_allocCount))
                ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                    ? ((0x15U == (IData)(vlSelf->io_insn2_IsaRd))
                        ? (IData)(vlSelf->io_insn2_PhyRd)
                        : (IData)(vlSelf->RcuAllocate__DOT___GEN_308))
                    : (IData)(vlSelf->RcuAllocate__DOT___GEN_308))
                : (IData)(vlSelf->RcuAllocate__DOT___GEN_308));
    }
}

VL_INLINE_OPT void VRcuAllocate___024root___combo__TOP__4(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___combo__TOP__4\n"); );
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
    vlSelf->io_insn2_PhyRd = ((2U == (IData)(vlSelf->io_allocCount))
                               ? ((0U != (IData)(vlSelf->io_insn2_IsaRd))
                                   ? ((IData)(vlSelf->io_WEN1_IntBusylist)
                                       ? (IData)(vlSelf->io_IntFreelist_phyrd2)
                                       : (IData)(vlSelf->io_IntFreelist_phyrd1))
                                   : 0U) : 0U);
    if (vlSelf->io_EN_Update) {
        RcuAllocate__DOT___GEN_71 = ((0x1fU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_31));
        RcuAllocate__DOT___GEN_70 = ((0x1eU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_30));
        RcuAllocate__DOT___GEN_69 = ((0x1dU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_29));
        RcuAllocate__DOT___GEN_68 = ((0x1cU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_28));
        RcuAllocate__DOT___GEN_67 = ((0x1bU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_27));
        RcuAllocate__DOT___GEN_66 = ((0x1aU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_26));
        RcuAllocate__DOT___GEN_65 = ((0x19U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_25));
        RcuAllocate__DOT___GEN_64 = ((0x18U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_24));
        RcuAllocate__DOT___GEN_63 = ((0x17U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_23));
        RcuAllocate__DOT___GEN_62 = ((0x16U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_22));
        RcuAllocate__DOT___GEN_49 = ((9U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_9));
        RcuAllocate__DOT___GEN_48 = ((8U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_8));
        RcuAllocate__DOT___GEN_47 = ((7U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_7));
        RcuAllocate__DOT___GEN_46 = ((6U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_6));
        RcuAllocate__DOT___GEN_45 = ((5U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_5));
        RcuAllocate__DOT___GEN_41 = ((1U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_1));
        RcuAllocate__DOT___GEN_50 = ((0xaU == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_10));
        RcuAllocate__DOT___GEN_40 = ((0U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_0));
        RcuAllocate__DOT___GEN_44 = ((4U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_4));
        RcuAllocate__DOT___GEN_42 = ((2U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_2));
        RcuAllocate__DOT___GEN_43 = ((3U == (IData)(vlSelf->io_IsaRd_Update))
                                      ? (IData)(vlSelf->io_Data_Update)
                                      : (IData)(vlSelf->RcuAllocate__DOT__IntRenameTable_3));
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
    } else {
        RcuAllocate__DOT___GEN_71 = vlSelf->RcuAllocate__DOT__IntRenameTable_31;
        RcuAllocate__DOT___GEN_70 = vlSelf->RcuAllocate__DOT__IntRenameTable_30;
        RcuAllocate__DOT___GEN_69 = vlSelf->RcuAllocate__DOT__IntRenameTable_29;
        RcuAllocate__DOT___GEN_68 = vlSelf->RcuAllocate__DOT__IntRenameTable_28;
        RcuAllocate__DOT___GEN_67 = vlSelf->RcuAllocate__DOT__IntRenameTable_27;
        RcuAllocate__DOT___GEN_66 = vlSelf->RcuAllocate__DOT__IntRenameTable_26;
        RcuAllocate__DOT___GEN_65 = vlSelf->RcuAllocate__DOT__IntRenameTable_25;
        RcuAllocate__DOT___GEN_64 = vlSelf->RcuAllocate__DOT__IntRenameTable_24;
        RcuAllocate__DOT___GEN_63 = vlSelf->RcuAllocate__DOT__IntRenameTable_23;
        RcuAllocate__DOT___GEN_62 = vlSelf->RcuAllocate__DOT__IntRenameTable_22;
        RcuAllocate__DOT___GEN_49 = vlSelf->RcuAllocate__DOT__IntRenameTable_9;
        RcuAllocate__DOT___GEN_48 = vlSelf->RcuAllocate__DOT__IntRenameTable_8;
        RcuAllocate__DOT___GEN_47 = vlSelf->RcuAllocate__DOT__IntRenameTable_7;
        RcuAllocate__DOT___GEN_46 = vlSelf->RcuAllocate__DOT__IntRenameTable_6;
        RcuAllocate__DOT___GEN_45 = vlSelf->RcuAllocate__DOT__IntRenameTable_5;
        RcuAllocate__DOT___GEN_41 = vlSelf->RcuAllocate__DOT__IntRenameTable_1;
        RcuAllocate__DOT___GEN_50 = vlSelf->RcuAllocate__DOT__IntRenameTable_10;
        RcuAllocate__DOT___GEN_40 = vlSelf->RcuAllocate__DOT__IntRenameTable_0;
        RcuAllocate__DOT___GEN_44 = vlSelf->RcuAllocate__DOT__IntRenameTable_4;
        RcuAllocate__DOT___GEN_42 = vlSelf->RcuAllocate__DOT__IntRenameTable_2;
        RcuAllocate__DOT___GEN_43 = vlSelf->RcuAllocate__DOT__IntRenameTable_3;
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
    if ((0U < (IData)(vlSelf->io_allocCount))) {
        vlSelf->RcuAllocate__DOT___GEN_318 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1fU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_71))
                                               : (IData)(RcuAllocate__DOT___GEN_71));
        vlSelf->RcuAllocate__DOT___GEN_317 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1eU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_70))
                                               : (IData)(RcuAllocate__DOT___GEN_70));
        vlSelf->RcuAllocate__DOT___GEN_316 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1dU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_69))
                                               : (IData)(RcuAllocate__DOT___GEN_69));
        vlSelf->RcuAllocate__DOT___GEN_315 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1cU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_68))
                                               : (IData)(RcuAllocate__DOT___GEN_68));
        vlSelf->RcuAllocate__DOT___GEN_314 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1bU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_67))
                                               : (IData)(RcuAllocate__DOT___GEN_67));
        vlSelf->RcuAllocate__DOT___GEN_313 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x1aU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_66))
                                               : (IData)(RcuAllocate__DOT___GEN_66));
        vlSelf->RcuAllocate__DOT___GEN_312 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x19U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_65))
                                               : (IData)(RcuAllocate__DOT___GEN_65));
        vlSelf->RcuAllocate__DOT___GEN_311 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x18U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_64))
                                               : (IData)(RcuAllocate__DOT___GEN_64));
        vlSelf->RcuAllocate__DOT___GEN_310 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x17U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_63))
                                               : (IData)(RcuAllocate__DOT___GEN_63));
        vlSelf->RcuAllocate__DOT___GEN_309 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0x16U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_62))
                                               : (IData)(RcuAllocate__DOT___GEN_62));
        vlSelf->RcuAllocate__DOT___GEN_296 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((9U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_49))
                                               : (IData)(RcuAllocate__DOT___GEN_49));
        vlSelf->RcuAllocate__DOT___GEN_295 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((8U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_48))
                                               : (IData)(RcuAllocate__DOT___GEN_48));
        vlSelf->RcuAllocate__DOT___GEN_294 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((7U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_47))
                                               : (IData)(RcuAllocate__DOT___GEN_47));
        vlSelf->RcuAllocate__DOT___GEN_293 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((6U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_46))
                                               : (IData)(RcuAllocate__DOT___GEN_46));
        vlSelf->RcuAllocate__DOT___GEN_292 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((5U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_45))
                                               : (IData)(RcuAllocate__DOT___GEN_45));
        vlSelf->RcuAllocate__DOT___GEN_288 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((1U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_41))
                                               : (IData)(RcuAllocate__DOT___GEN_41));
        vlSelf->RcuAllocate__DOT___GEN_297 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0xaU 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_50))
                                               : (IData)(RcuAllocate__DOT___GEN_50));
        vlSelf->RcuAllocate__DOT___GEN_287 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((0U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_40))
                                               : (IData)(RcuAllocate__DOT___GEN_40));
        vlSelf->RcuAllocate__DOT___GEN_291 = ((0U != (IData)(vlSelf->io_insn1_IsaRd))
                                               ? ((4U 
                                                   == (IData)(vlSelf->io_insn1_IsaRd))
                                                   ? (IData)(vlSelf->io_insn1_PhyRd)
                                                   : (IData)(RcuAllocate__DOT___GEN_44))
                                               : (IData)(RcuAllocate__DOT___GEN_44));
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
        vlSelf->RcuAllocate__DOT___GEN_318 = RcuAllocate__DOT___GEN_71;
        vlSelf->RcuAllocate__DOT___GEN_317 = RcuAllocate__DOT___GEN_70;
        vlSelf->RcuAllocate__DOT___GEN_316 = RcuAllocate__DOT___GEN_69;
        vlSelf->RcuAllocate__DOT___GEN_315 = RcuAllocate__DOT___GEN_68;
        vlSelf->RcuAllocate__DOT___GEN_314 = RcuAllocate__DOT___GEN_67;
        vlSelf->RcuAllocate__DOT___GEN_313 = RcuAllocate__DOT___GEN_66;
        vlSelf->RcuAllocate__DOT___GEN_312 = RcuAllocate__DOT___GEN_65;
        vlSelf->RcuAllocate__DOT___GEN_311 = RcuAllocate__DOT___GEN_64;
        vlSelf->RcuAllocate__DOT___GEN_310 = RcuAllocate__DOT___GEN_63;
        vlSelf->RcuAllocate__DOT___GEN_309 = RcuAllocate__DOT___GEN_62;
        vlSelf->RcuAllocate__DOT___GEN_296 = RcuAllocate__DOT___GEN_49;
        vlSelf->RcuAllocate__DOT___GEN_295 = RcuAllocate__DOT___GEN_48;
        vlSelf->RcuAllocate__DOT___GEN_294 = RcuAllocate__DOT___GEN_47;
        vlSelf->RcuAllocate__DOT___GEN_293 = RcuAllocate__DOT___GEN_46;
        vlSelf->RcuAllocate__DOT___GEN_292 = RcuAllocate__DOT___GEN_45;
        vlSelf->RcuAllocate__DOT___GEN_288 = RcuAllocate__DOT___GEN_41;
        vlSelf->RcuAllocate__DOT___GEN_297 = RcuAllocate__DOT___GEN_50;
        vlSelf->RcuAllocate__DOT___GEN_287 = RcuAllocate__DOT___GEN_40;
        vlSelf->RcuAllocate__DOT___GEN_291 = RcuAllocate__DOT___GEN_44;
        vlSelf->RcuAllocate__DOT___GEN_289 = RcuAllocate__DOT___GEN_42;
        vlSelf->RcuAllocate__DOT___GEN_290 = RcuAllocate__DOT___GEN_43;
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

void VRcuAllocate___024root___eval(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___eval\n"); );
    // Body
    VRcuAllocate___024root___combo__TOP__1(vlSelf);
    if (((IData)(vlSelf->clock) & (~ (IData)(vlSelf->__Vclklast__TOP__clock)))) {
        VRcuAllocate___024root___sequent__TOP__3(vlSelf);
    }
    VRcuAllocate___024root___combo__TOP__4(vlSelf);
    // Final
    vlSelf->__Vclklast__TOP__clock = vlSelf->clock;
}

QData VRcuAllocate___024root___change_request_1(VRcuAllocate___024root* vlSelf);

VL_INLINE_OPT QData VRcuAllocate___024root___change_request(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___change_request\n"); );
    // Body
    return (VRcuAllocate___024root___change_request_1(vlSelf));
}

VL_INLINE_OPT QData VRcuAllocate___024root___change_request_1(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___change_request_1\n"); );
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VRcuAllocate___024root___eval_debug_assertions(VRcuAllocate___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VRcuAllocate__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VRcuAllocate___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_allocCount & 0xfcU))) {
        Verilated::overWidthError("io_allocCount");}
    if (VL_UNLIKELY((vlSelf->io_EN_Update & 0xfeU))) {
        Verilated::overWidthError("io_EN_Update");}
    if (VL_UNLIKELY((vlSelf->io_IsaRd_Update & 0xe0U))) {
        Verilated::overWidthError("io_IsaRd_Update");}
    if (VL_UNLIKELY((vlSelf->io_Data_Update & 0xc0U))) {
        Verilated::overWidthError("io_Data_Update");}
    if (VL_UNLIKELY((vlSelf->io_IntFreelist_phyrd1 
                     & 0xc0U))) {
        Verilated::overWidthError("io_IntFreelist_phyrd1");}
    if (VL_UNLIKELY((vlSelf->io_IntFreelist_phyrd2 
                     & 0xc0U))) {
        Verilated::overWidthError("io_IntFreelist_phyrd2");}
    if (VL_UNLIKELY((vlSelf->io_insn1_IsaRs1 & 0xe0U))) {
        Verilated::overWidthError("io_insn1_IsaRs1");}
    if (VL_UNLIKELY((vlSelf->io_insn1_IsaRs2 & 0xe0U))) {
        Verilated::overWidthError("io_insn1_IsaRs2");}
    if (VL_UNLIKELY((vlSelf->io_insn1_IsaRd & 0xe0U))) {
        Verilated::overWidthError("io_insn1_IsaRd");}
    if (VL_UNLIKELY((vlSelf->io_insn2_IsaRs1 & 0xe0U))) {
        Verilated::overWidthError("io_insn2_IsaRs1");}
    if (VL_UNLIKELY((vlSelf->io_insn2_IsaRs2 & 0xe0U))) {
        Verilated::overWidthError("io_insn2_IsaRs2");}
    if (VL_UNLIKELY((vlSelf->io_insn2_IsaRd & 0xe0U))) {
        Verilated::overWidthError("io_insn2_IsaRd");}
}
#endif  // VL_DEBUG
