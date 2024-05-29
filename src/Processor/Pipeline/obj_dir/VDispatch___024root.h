// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VDispatch.h for the primary calling header

#ifndef VERILATED_VDISPATCH___024ROOT_H_
#define VERILATED_VDISPATCH___024ROOT_H_  // guard

#include "verilated_heavy.h"

//==========

class VDispatch__Syms;

//----------

VL_MODULE(VDispatch___024root) {
  public:

    // PORTS
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_DispatchCount,1,0);
    VL_IN8(io_Insn1_LSQTag_in,3,0);
    VL_IN8(io_Insn2_LSQTag_in,3,0);
    VL_IN8(io_Insn1_RobTag_in,3,0);
    VL_IN8(io_Insn2_RobTag_in,3,0);
    VL_IN8(io_insn1_PhyRs1_in,5,0);
    VL_IN8(io_insn1_PhyRs2_in,5,0);
    VL_IN8(io_insn1_PhyRd_in,5,0);
    VL_IN8(io_insn1_LPhyRd_in,5,0);
    VL_IN8(io_insn2_PhyRs1_in,5,0);
    VL_IN8(io_insn2_PhyRs2_in,5,0);
    VL_IN8(io_insn2_PhyRd_in,5,0);
    VL_IN8(io_insn2_LPhyRd_in,5,0);
    VL_OUT8(io_Issue_num1,1,0);
    VL_OUT8(io_Insn1_LSQTag,3,0);
    VL_OUT8(io_insn1_PhyRs1,5,0);
    VL_OUT8(io_insn1_PhyRs2,5,0);
    VL_OUT8(io_insn1_PhyRd,5,0);
    VL_OUT8(io_insn1_LPhyRd,5,0);
    VL_OUT8(io_Insn1_RobTag,3,0);
    VL_OUT8(io_Issue_num2,1,0);
    VL_OUT8(io_Insn2_LSQTag,3,0);
    VL_OUT8(io_insn2_PhyRs1,5,0);
    VL_OUT8(io_insn2_PhyRs2,5,0);
    VL_OUT8(io_insn2_PhyRd,5,0);
    VL_OUT8(io_insn2_LPhyRd,5,0);
    VL_OUT8(io_Insn2_RobTag,3,0);
    VL_INW(io_insn1,127,0,4);
    VL_INW(io_insn2,127,0,4);
    VL_OUTW(io_insn1_Out,127,0,4);
    VL_OUTW(io_insn2_Out,127,0,4);

    // INTERNAL VARIABLES
    VDispatch__Syms* vlSymsp;  // Symbol table

    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VDispatch___024root);  ///< Copying not allowed
  public:
    VDispatch___024root(const char* name);
    ~VDispatch___024root();

    // INTERNAL METHODS
    void __Vconfigure(VDispatch__Syms* symsp, bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
