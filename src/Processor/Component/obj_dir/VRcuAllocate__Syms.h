// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VRCUALLOCATE__SYMS_H_
#define VERILATED_VRCUALLOCATE__SYMS_H_  // guard

#include "verilated_heavy.h"

// INCLUDE MODEL CLASS

#include "VRcuAllocate.h"

// INCLUDE MODULE CLASSES
#include "VRcuAllocate___024root.h"

// SYMS CLASS (contains all model state)
class VRcuAllocate__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    VRcuAllocate* const __Vm_modelp;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    VRcuAllocate___024root         TOP;

    // CONSTRUCTORS
    VRcuAllocate__Syms(VerilatedContext* contextp, const char* namep, VRcuAllocate* modelp);
    ~VRcuAllocate__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
