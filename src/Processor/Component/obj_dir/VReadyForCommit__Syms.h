// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VREADYFORCOMMIT__SYMS_H_
#define VERILATED_VREADYFORCOMMIT__SYMS_H_  // guard

#include "verilated_heavy.h"

// INCLUDE MODEL CLASS

#include "VReadyForCommit.h"

// INCLUDE MODULE CLASSES
#include "VReadyForCommit___024root.h"

// SYMS CLASS (contains all model state)
class VReadyForCommit__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    VReadyForCommit* const __Vm_modelp;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    VReadyForCommit___024root      TOP;

    // CONSTRUCTORS
    VReadyForCommit__Syms(VerilatedContext* contextp, const char* namep, VReadyForCommit* modelp);
    ~VReadyForCommit__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
