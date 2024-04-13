// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VALU_H_
#define VERILATED_VALU_H_  // guard

#include "verilated_heavy.h"

class Valu__Syms;
class Valu___024root;

// This class is the main interface to the Verilated model
class Valu VL_NOT_FINAL {
  private:
    // Symbol table holding complete model state (owned by this class)
    Valu__Syms* const vlSymsp;

  public:

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&clock,0,0);
    VL_IN8(&reset,0,0);
    VL_IN64(&io_Insn_operand1,63,0);
    VL_IN64(&io_Insn_operand2,63,0);
    VL_IN8(&io_Sub_OP_out,3,0);
    VL_OUT64(&io_Insn_rdresult,63,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Valu___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Valu(VerilatedContext* contextp, const char* name = "TOP");
    explicit Valu(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Valu();
  private:
    VL_UNCOPYABLE(Valu);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval(uint64_t& Rdresult) { 
      eval_step();
      Rdresult=io_Insn_rdresult; 
    }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Return current simulation context for this model.
    /// Used to get to e.g. simulation time via contextp()->time()
    VerilatedContext* contextp() const;
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

#endif  // guard
