// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VReadyForCommit.h"
#include "VReadyForCommit__Syms.h"

//============================================================
// Constructors

VReadyForCommit::VReadyForCommit(VerilatedContext* _vcontextp__, const char* _vcname__)
    : vlSymsp{new VReadyForCommit__Syms(_vcontextp__, _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_Rob_Tag{vlSymsp->TOP.io_Rob_Tag}
    , io_Rob_Header{vlSymsp->TOP.io_Rob_Header}
    , io_Rob_NextHeader{vlSymsp->TOP.io_Rob_NextHeader}
    , io_m_RobState{vlSymsp->TOP.io_m_RobState}
    , io_m_RollBackTag{vlSymsp->TOP.io_m_RollBackTag}
    , io_Rob_Usage{vlSymsp->TOP.io_Rob_Usage}
    , io_Header_isStable{vlSymsp->TOP.io_Header_isStable}
    , io_Header_Function_type{vlSymsp->TOP.io_Header_Function_type}
    , io_Ready{vlSymsp->TOP.io_Ready}
    , rootp{&(vlSymsp->TOP)}
{
}

VReadyForCommit::VReadyForCommit(const char* _vcname__)
    : VReadyForCommit(nullptr, _vcname__)
{
}

//============================================================
// Destructor

VReadyForCommit::~VReadyForCommit() {
    delete vlSymsp;
}

//============================================================
// Evaluation loop

void VReadyForCommit___024root___eval_initial(VReadyForCommit___024root* vlSelf);
void VReadyForCommit___024root___eval_settle(VReadyForCommit___024root* vlSelf);
void VReadyForCommit___024root___eval(VReadyForCommit___024root* vlSelf);
QData VReadyForCommit___024root___change_request(VReadyForCommit___024root* vlSelf);
#ifdef VL_DEBUG
void VReadyForCommit___024root___eval_debug_assertions(VReadyForCommit___024root* vlSelf);
#endif  // VL_DEBUG
void VReadyForCommit___024root___final(VReadyForCommit___024root* vlSelf);

static void _eval_initial_loop(VReadyForCommit__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    VReadyForCommit___024root___eval_initial(&(vlSymsp->TOP));
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial loop\n"););
        VReadyForCommit___024root___eval_settle(&(vlSymsp->TOP));
        VReadyForCommit___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VReadyForCommit___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("ReadyForCommit.v", 12, "",
                "Verilated model didn't DC converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VReadyForCommit___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VReadyForCommit::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VReadyForCommit::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VReadyForCommit___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        VReadyForCommit___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VReadyForCommit___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("ReadyForCommit.v", 12, "",
                "Verilated model didn't converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VReadyForCommit___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

//============================================================
// Invoke final blocks

void VReadyForCommit::final() {
    VReadyForCommit___024root___final(&(vlSymsp->TOP));
}

//============================================================
// Utilities

VerilatedContext* VReadyForCommit::contextp() const {
    return vlSymsp->_vm_contextp__;
}

const char* VReadyForCommit::name() const {
    return vlSymsp->name();
}
