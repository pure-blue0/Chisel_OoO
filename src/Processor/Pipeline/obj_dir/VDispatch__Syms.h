// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VDISPATCH__SYMS_H_
#define VERILATED_VDISPATCH__SYMS_H_  // guard

#include "verilated_heavy.h"

// INCLUDE MODEL CLASS

#include "VDispatch.h"

// INCLUDE MODULE CLASSES
#include "VDispatch___024root.h"

// SYMS CLASS (contains all model state)
class VDispatch__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    VDispatch* const __Vm_modelp;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    VDispatch___024root            TOP;

    // CONSTRUCTORS
    VDispatch__Syms(VerilatedContext* contextp, const char* namep, VDispatch* modelp);
    ~VDispatch__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
