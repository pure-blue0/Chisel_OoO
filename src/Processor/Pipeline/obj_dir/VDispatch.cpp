// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VDispatch.h"
#include "VDispatch__Syms.h"

//============================================================
// Constructors

VDispatch::VDispatch(VerilatedContext* _vcontextp__, const char* _vcname__)
    : vlSymsp{new VDispatch__Syms(_vcontextp__, _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_DispatchCount{vlSymsp->TOP.io_DispatchCount}
    , io_insn1{vlSymsp->TOP.io_insn1}
    , io_insn2{vlSymsp->TOP.io_insn2}
    , io_Insn1_LSQTag_in{vlSymsp->TOP.io_Insn1_LSQTag_in}
    , io_Insn2_LSQTag_in{vlSymsp->TOP.io_Insn2_LSQTag_in}
    , io_Insn1_RobTag_in{vlSymsp->TOP.io_Insn1_RobTag_in}
    , io_Insn2_RobTag_in{vlSymsp->TOP.io_Insn2_RobTag_in}
    , io_insn1_PhyRs1_in{vlSymsp->TOP.io_insn1_PhyRs1_in}
    , io_insn1_PhyRs2_in{vlSymsp->TOP.io_insn1_PhyRs2_in}
    , io_insn1_PhyRd_in{vlSymsp->TOP.io_insn1_PhyRd_in}
    , io_insn1_LPhyRd_in{vlSymsp->TOP.io_insn1_LPhyRd_in}
    , io_insn2_PhyRs1_in{vlSymsp->TOP.io_insn2_PhyRs1_in}
    , io_insn2_PhyRs2_in{vlSymsp->TOP.io_insn2_PhyRs2_in}
    , io_insn2_PhyRd_in{vlSymsp->TOP.io_insn2_PhyRd_in}
    , io_insn2_LPhyRd_in{vlSymsp->TOP.io_insn2_LPhyRd_in}
    , io_Issue_num1{vlSymsp->TOP.io_Issue_num1}
    , io_insn1_Out{vlSymsp->TOP.io_insn1_Out}
    , io_Insn1_LSQTag{vlSymsp->TOP.io_Insn1_LSQTag}
    , io_insn1_PhyRs1{vlSymsp->TOP.io_insn1_PhyRs1}
    , io_insn1_PhyRs2{vlSymsp->TOP.io_insn1_PhyRs2}
    , io_insn1_PhyRd{vlSymsp->TOP.io_insn1_PhyRd}
    , io_insn1_LPhyRd{vlSymsp->TOP.io_insn1_LPhyRd}
    , io_Insn1_RobTag{vlSymsp->TOP.io_Insn1_RobTag}
    , io_Issue_num2{vlSymsp->TOP.io_Issue_num2}
    , io_insn2_Out{vlSymsp->TOP.io_insn2_Out}
    , io_Insn2_LSQTag{vlSymsp->TOP.io_Insn2_LSQTag}
    , io_insn2_PhyRs1{vlSymsp->TOP.io_insn2_PhyRs1}
    , io_insn2_PhyRs2{vlSymsp->TOP.io_insn2_PhyRs2}
    , io_insn2_PhyRd{vlSymsp->TOP.io_insn2_PhyRd}
    , io_insn2_LPhyRd{vlSymsp->TOP.io_insn2_LPhyRd}
    , io_Insn2_RobTag{vlSymsp->TOP.io_Insn2_RobTag}
    , rootp{&(vlSymsp->TOP)}
{
}

VDispatch::VDispatch(const char* _vcname__)
    : VDispatch(nullptr, _vcname__)
{
}

//============================================================
// Destructor

VDispatch::~VDispatch() {
    delete vlSymsp;
}

//============================================================
// Evaluation loop

void VDispatch___024root___eval_initial(VDispatch___024root* vlSelf);
void VDispatch___024root___eval_settle(VDispatch___024root* vlSelf);
void VDispatch___024root___eval(VDispatch___024root* vlSelf);
QData VDispatch___024root___change_request(VDispatch___024root* vlSelf);
#ifdef VL_DEBUG
void VDispatch___024root___eval_debug_assertions(VDispatch___024root* vlSelf);
#endif  // VL_DEBUG
void VDispatch___024root___final(VDispatch___024root* vlSelf);

static void _eval_initial_loop(VDispatch__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    VDispatch___024root___eval_initial(&(vlSymsp->TOP));
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial loop\n"););
        VDispatch___024root___eval_settle(&(vlSymsp->TOP));
        VDispatch___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VDispatch___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("Dispatch.v", 1, "",
                "Verilated model didn't DC converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VDispatch___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VDispatch::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VDispatch::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VDispatch___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        VDispatch___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VDispatch___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("Dispatch.v", 1, "",
                "Verilated model didn't converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VDispatch___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

//============================================================
// Invoke final blocks

void VDispatch::final() {
    VDispatch___024root___final(&(vlSymsp->TOP));
}

//============================================================
// Utilities

VerilatedContext* VDispatch::contextp() const {
    return vlSymsp->_vm_contextp__;
}

const char* VDispatch::name() const {
    return vlSymsp->name();
}
