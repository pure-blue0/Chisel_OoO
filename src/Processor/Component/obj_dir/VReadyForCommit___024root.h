// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VReadyForCommit.h for the primary calling header

#ifndef VERILATED_VREADYFORCOMMIT___024ROOT_H_
#define VERILATED_VREADYFORCOMMIT___024ROOT_H_  // guard

#include "verilated_heavy.h"

//==========

class VReadyForCommit__Syms;

//----------

VL_MODULE(VReadyForCommit___024root) {
  public:

    // PORTS
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_Rob_Tag,3,0);
    VL_IN8(io_Rob_Header,3,0);
    VL_IN8(io_Rob_NextHeader,3,0);
    VL_IN8(io_m_RobState,2,0);
    VL_IN8(io_m_RollBackTag,3,0);
    VL_IN8(io_Rob_Usage,3,0);
    VL_IN8(io_Header_isStable,0,0);
    VL_IN8(io_Header_Function_type,2,0);
    VL_OUT8(io_Ready,0,0);

    // INTERNAL VARIABLES
    VReadyForCommit__Syms* vlSymsp;  // Symbol table

    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VReadyForCommit___024root);  ///< Copying not allowed
  public:
    VReadyForCommit___024root(const char* name);
    ~VReadyForCommit___024root();

    // INTERNAL METHODS
    void __Vconfigure(VReadyForCommit__Syms* symsp, bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
