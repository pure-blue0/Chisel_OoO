// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VRfReadPort.h"
#include "VRfReadPort__Syms.h"

//============================================================
// Constructors

VRfReadPort::VRfReadPort(VerilatedContext* _vcontextp__, const char* _vcname__)
    : vlSymsp{new VRfReadPort__Syms(_vcontextp__, _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_Valid{vlSymsp->TOP.io_Valid}
    , io_Insn_Operand1Ready{vlSymsp->TOP.io_Insn_Operand1Ready}
    , io_Insn_PhyRs1_data{vlSymsp->TOP.io_Insn_PhyRs1_data}
    , io_Insn_Operand2Ready{vlSymsp->TOP.io_Insn_Operand2Ready}
    , io_Insn_PhyRs2_data{vlSymsp->TOP.io_Insn_PhyRs2_data}
    , io_desIndex_in{vlSymsp->TOP.io_desIndex_in}
    , io_Sub_OP_in{vlSymsp->TOP.io_Sub_OP_in}
    , io_imm_in{vlSymsp->TOP.io_imm_in}
    , io_pc_in{vlSymsp->TOP.io_pc_in}
    , io_Pred_taken_in{vlSymsp->TOP.io_Pred_taken_in}
    , io_pred_target_in{vlSymsp->TOP.io_pred_target_in}
    , io_Function_type_in{vlSymsp->TOP.io_Function_type_in}
    , io_Insn_operand1_in{vlSymsp->TOP.io_Insn_operand1_in}
    , io_Insn_operand2_in{vlSymsp->TOP.io_Insn_operand2_in}
    , io_RobTag_in{vlSymsp->TOP.io_RobTag_in}
    , io_LSQTag_in{vlSymsp->TOP.io_LSQTag_in}
    , io_Out_valid{vlSymsp->TOP.io_Out_valid}
    , io_desIndex{vlSymsp->TOP.io_desIndex}
    , io_Insn_operand1{vlSymsp->TOP.io_Insn_operand1}
    , io_Insn_operand2{vlSymsp->TOP.io_Insn_operand2}
    , io_Sub_OP_out{vlSymsp->TOP.io_Sub_OP_out}
    , io_desIndex_out{vlSymsp->TOP.io_desIndex_out}
    , io_imm_out{vlSymsp->TOP.io_imm_out}
    , io_pc_out{vlSymsp->TOP.io_pc_out}
    , io_Pred_taken_out{vlSymsp->TOP.io_Pred_taken_out}
    , io_pred_target_out{vlSymsp->TOP.io_pred_target_out}
    , io_Function_type_out{vlSymsp->TOP.io_Function_type_out}
    , io_RobTag_out{vlSymsp->TOP.io_RobTag_out}
    , io_LSQTag_out{vlSymsp->TOP.io_LSQTag_out}
    , rootp{&(vlSymsp->TOP)}
{
}

VRfReadPort::VRfReadPort(const char* _vcname__)
    : VRfReadPort(nullptr, _vcname__)
{
}

//============================================================
// Destructor

VRfReadPort::~VRfReadPort() {
    delete vlSymsp;
}

//============================================================
// Evaluation loop

void VRfReadPort___024root___eval_initial(VRfReadPort___024root* vlSelf);
void VRfReadPort___024root___eval_settle(VRfReadPort___024root* vlSelf);
void VRfReadPort___024root___eval(VRfReadPort___024root* vlSelf);
QData VRfReadPort___024root___change_request(VRfReadPort___024root* vlSelf);
#ifdef VL_DEBUG
void VRfReadPort___024root___eval_debug_assertions(VRfReadPort___024root* vlSelf);
#endif  // VL_DEBUG
void VRfReadPort___024root___final(VRfReadPort___024root* vlSelf);

static void _eval_initial_loop(VRfReadPort__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    VRfReadPort___024root___eval_initial(&(vlSymsp->TOP));
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial loop\n"););
        VRfReadPort___024root___eval_settle(&(vlSymsp->TOP));
        VRfReadPort___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VRfReadPort___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("RfReadPort.v", 1, "",
                "Verilated model didn't DC converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VRfReadPort___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VRfReadPort::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VRfReadPort::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VRfReadPort___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        VRfReadPort___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VRfReadPort___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("RfReadPort.v", 1, "",
                "Verilated model didn't converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VRfReadPort___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

//============================================================
// Invoke final blocks

void VRfReadPort::final() {
    VRfReadPort___024root___final(&(vlSymsp->TOP));
}

//============================================================
// Utilities

VerilatedContext* VRfReadPort::contextp() const {
    return vlSymsp->_vm_contextp__;
}

const char* VRfReadPort::name() const {
    return vlSymsp->name();
}
