// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Valu.h for the primary calling header

#ifndef VERILATED_VALU___024ROOT_H_
#define VERILATED_VALU___024ROOT_H_  // guard

#include "verilated_heavy.h"

//==========

class Valu__Syms;

//----------

VL_MODULE(Valu___024root) {
  public:

    // PORTS
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_Sub_OP_out,3,0);
    VL_IN64(io_Insn_operand1,63,0);
    VL_IN64(io_Insn_operand2,63,0);
    VL_OUT64(io_Insn_rdresult,63,0);

    // INTERNAL VARIABLES
    Valu__Syms* vlSymsp;  // Symbol table

    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Valu___024root);  ///< Copying not allowed
  public:
    Valu___024root(const char* name);
    ~Valu___024root();

    // INTERNAL METHODS
    void __Vconfigure(Valu__Syms* symsp, bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
