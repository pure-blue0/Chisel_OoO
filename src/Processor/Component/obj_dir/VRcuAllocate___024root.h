// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VRcuAllocate.h for the primary calling header

#ifndef VERILATED_VRCUALLOCATE___024ROOT_H_
#define VERILATED_VRCUALLOCATE___024ROOT_H_  // guard

#include "verilated_heavy.h"

//==========

class VRcuAllocate__Syms;

//----------

VL_MODULE(VRcuAllocate___024root) {
  public:

    // PORTS
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_allocCount,1,0);
    VL_IN8(io_EN_Update,0,0);
    VL_IN8(io_IsaRd_Update,4,0);
    VL_IN8(io_Data_Update,5,0);
    VL_IN8(io_IntFreelist_phyrd1,5,0);
    VL_IN8(io_IntFreelist_phyrd2,5,0);
    VL_IN8(io_insn1_IsaRs1,4,0);
    VL_IN8(io_insn1_IsaRs2,4,0);
    VL_IN8(io_insn1_IsaRd,4,0);
    VL_IN8(io_insn2_IsaRs1,4,0);
    VL_IN8(io_insn2_IsaRs2,4,0);
    VL_IN8(io_insn2_IsaRd,4,0);
    VL_OUT8(io_WEN1_IntBusylist,0,0);
    VL_OUT8(io_WEN2_IntBusylist,0,0);
    VL_OUT8(io_insn1_PhyRs1,5,0);
    VL_OUT8(io_insn1_PhyRs2,5,0);
    VL_OUT8(io_insn1_PhyRd,5,0);
    VL_OUT8(io_insn1_LPhyRd,5,0);
    VL_OUT8(io_insn2_PhyRs1,5,0);
    VL_OUT8(io_insn2_PhyRs2,5,0);
    VL_OUT8(io_insn2_PhyRd,5,0);
    VL_OUT8(io_insn2_LPhyRd,5,0);
    VL_OUT8(io_Freelist_pop_num,1,0);

    // LOCAL SIGNALS
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_0;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_1;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_2;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_3;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_4;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_5;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_6;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_7;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_8;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_9;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_10;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_11;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_12;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_13;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_14;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_15;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_16;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_17;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_18;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_19;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_20;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_21;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_22;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_23;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_24;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_25;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_26;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_27;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_28;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_29;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_30;
    CData/*5:0*/ RcuAllocate__DOT__IntRenameTable_31;
    CData/*5:0*/ RcuAllocate__DOT___GEN_287;
    CData/*5:0*/ RcuAllocate__DOT___GEN_288;
    CData/*5:0*/ RcuAllocate__DOT___GEN_289;
    CData/*5:0*/ RcuAllocate__DOT___GEN_290;
    CData/*5:0*/ RcuAllocate__DOT___GEN_291;
    CData/*5:0*/ RcuAllocate__DOT___GEN_292;
    CData/*5:0*/ RcuAllocate__DOT___GEN_293;
    CData/*5:0*/ RcuAllocate__DOT___GEN_294;
    CData/*5:0*/ RcuAllocate__DOT___GEN_295;
    CData/*5:0*/ RcuAllocate__DOT___GEN_296;
    CData/*5:0*/ RcuAllocate__DOT___GEN_297;
    CData/*5:0*/ RcuAllocate__DOT___GEN_298;
    CData/*5:0*/ RcuAllocate__DOT___GEN_299;
    CData/*5:0*/ RcuAllocate__DOT___GEN_300;
    CData/*5:0*/ RcuAllocate__DOT___GEN_301;
    CData/*5:0*/ RcuAllocate__DOT___GEN_302;
    CData/*5:0*/ RcuAllocate__DOT___GEN_303;
    CData/*5:0*/ RcuAllocate__DOT___GEN_304;
    CData/*5:0*/ RcuAllocate__DOT___GEN_305;
    CData/*5:0*/ RcuAllocate__DOT___GEN_306;
    CData/*5:0*/ RcuAllocate__DOT___GEN_307;
    CData/*5:0*/ RcuAllocate__DOT___GEN_308;
    CData/*5:0*/ RcuAllocate__DOT___GEN_309;
    CData/*5:0*/ RcuAllocate__DOT___GEN_310;
    CData/*5:0*/ RcuAllocate__DOT___GEN_311;
    CData/*5:0*/ RcuAllocate__DOT___GEN_312;
    CData/*5:0*/ RcuAllocate__DOT___GEN_313;
    CData/*5:0*/ RcuAllocate__DOT___GEN_314;
    CData/*5:0*/ RcuAllocate__DOT___GEN_315;
    CData/*5:0*/ RcuAllocate__DOT___GEN_316;
    CData/*5:0*/ RcuAllocate__DOT___GEN_317;
    CData/*5:0*/ RcuAllocate__DOT___GEN_318;

    // LOCAL VARIABLES
    CData/*0:0*/ __Vclklast__TOP__clock;

    // INTERNAL VARIABLES
    VRcuAllocate__Syms* vlSymsp;  // Symbol table

    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VRcuAllocate___024root);  ///< Copying not allowed
  public:
    VRcuAllocate___024root(const char* name);
    ~VRcuAllocate___024root();

    // INTERNAL METHODS
    void __Vconfigure(VRcuAllocate__Syms* symsp, bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
