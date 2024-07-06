// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VRFREADPORT_H_
#define VERILATED_VRFREADPORT_H_  // guard

#include "verilated_heavy.h"

class VRfReadPort__Syms;
class VRfReadPort___024root;

// This class is the main interface to the Verilated model
class VRfReadPort VL_NOT_FINAL {
  private:
    // Symbol table holding complete model state (owned by this class)
    VRfReadPort__Syms* const vlSymsp;

  public:

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&clock,0,0);
    VL_IN8(&reset,0,0);
    VL_IN8(&io_Valid,0,0);
    VL_IN8(&io_Insn_Operand1Ready,0,0);
    VL_IN64(&io_Insn_PhyRs1_data,63,0);
    VL_IN8(&io_Insn_Operand2Ready,0,0);
    VL_IN64(&io_Insn_PhyRs2_data,63,0);
    VL_IN8(&io_desIndex_in,2,0);
    VL_IN8(&io_Sub_OP_in,3,0);
    VL_IN64(&io_imm_in,63,0);
    VL_IN64(&io_pc_in,63,0);
    VL_IN8(&io_Pred_taken_in,0,0);
    VL_IN64(&io_pred_target_in,63,0);
    VL_IN8(&io_Function_type_in,2,0);
    VL_IN64(&io_Insn_operand1_in,63,0);
    VL_IN64(&io_Insn_operand2_in,63,0);
    VL_IN8(&io_RobTag_in,3,0);
    VL_IN8(&io_LSQTag_in,3,0);
    VL_OUT8(&io_Out_valid,0,0);
    VL_OUT8(&io_desIndex,2,0);
    VL_OUT64(&io_Insn_operand1,63,0);
    VL_OUT64(&io_Insn_operand2,63,0);
    VL_OUT8(&io_Sub_OP_out,3,0);
    VL_OUT8(&io_desIndex_out,2,0);
    VL_OUT64(&io_imm_out,63,0);
    VL_OUT64(&io_pc_out,63,0);
    VL_OUT8(&io_Pred_taken_out,0,0);
    VL_OUT64(&io_pred_target_out,63,0);
    VL_OUT8(&io_Function_type_out,2,0);
    VL_OUT8(&io_RobTag_out,3,0);
    VL_OUT8(&io_LSQTag_out,3,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    VRfReadPort___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit VRfReadPort(VerilatedContext* contextp, const char* name = "TOP");
    explicit VRfReadPort(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~VRfReadPort();
  private:
    VL_UNCOPYABLE(VRfReadPort);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
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
