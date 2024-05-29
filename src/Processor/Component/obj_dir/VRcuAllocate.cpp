// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VRcuAllocate.h"
#include "VRcuAllocate__Syms.h"

//============================================================
// Constructors

VRcuAllocate::VRcuAllocate(VerilatedContext* _vcontextp__, const char* _vcname__)
    : vlSymsp{new VRcuAllocate__Syms(_vcontextp__, _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_allocCount{vlSymsp->TOP.io_allocCount}
    , io_EN_Update{vlSymsp->TOP.io_EN_Update}
    , io_IsaRd_Update{vlSymsp->TOP.io_IsaRd_Update}
    , io_Data_Update{vlSymsp->TOP.io_Data_Update}
    , io_IntFreelist_phyrd1{vlSymsp->TOP.io_IntFreelist_phyrd1}
    , io_IntFreelist_phyrd2{vlSymsp->TOP.io_IntFreelist_phyrd2}
    , io_insn1_IsaRs1{vlSymsp->TOP.io_insn1_IsaRs1}
    , io_insn1_IsaRs2{vlSymsp->TOP.io_insn1_IsaRs2}
    , io_insn1_IsaRd{vlSymsp->TOP.io_insn1_IsaRd}
    , io_insn2_IsaRs1{vlSymsp->TOP.io_insn2_IsaRs1}
    , io_insn2_IsaRs2{vlSymsp->TOP.io_insn2_IsaRs2}
    , io_insn2_IsaRd{vlSymsp->TOP.io_insn2_IsaRd}
    , io_WEN1_IntBusylist{vlSymsp->TOP.io_WEN1_IntBusylist}
    , io_WEN2_IntBusylist{vlSymsp->TOP.io_WEN2_IntBusylist}
    , io_insn1_PhyRs1{vlSymsp->TOP.io_insn1_PhyRs1}
    , io_insn1_PhyRs2{vlSymsp->TOP.io_insn1_PhyRs2}
    , io_insn1_PhyRd{vlSymsp->TOP.io_insn1_PhyRd}
    , io_insn1_LPhyRd{vlSymsp->TOP.io_insn1_LPhyRd}
    , io_insn2_PhyRs1{vlSymsp->TOP.io_insn2_PhyRs1}
    , io_insn2_PhyRs2{vlSymsp->TOP.io_insn2_PhyRs2}
    , io_insn2_PhyRd{vlSymsp->TOP.io_insn2_PhyRd}
    , io_insn2_LPhyRd{vlSymsp->TOP.io_insn2_LPhyRd}
    , io_Freelist_pop_num{vlSymsp->TOP.io_Freelist_pop_num}
    , rootp{&(vlSymsp->TOP)}
{
}

VRcuAllocate::VRcuAllocate(const char* _vcname__)
    : VRcuAllocate(nullptr, _vcname__)
{
}

//============================================================
// Destructor

VRcuAllocate::~VRcuAllocate() {
    delete vlSymsp;
}

//============================================================
// Evaluation loop

void VRcuAllocate___024root___eval_initial(VRcuAllocate___024root* vlSelf);
void VRcuAllocate___024root___eval_settle(VRcuAllocate___024root* vlSelf);
void VRcuAllocate___024root___eval(VRcuAllocate___024root* vlSelf);
QData VRcuAllocate___024root___change_request(VRcuAllocate___024root* vlSelf);
#ifdef VL_DEBUG
void VRcuAllocate___024root___eval_debug_assertions(VRcuAllocate___024root* vlSelf);
#endif  // VL_DEBUG
void VRcuAllocate___024root___final(VRcuAllocate___024root* vlSelf);

static void _eval_initial_loop(VRcuAllocate__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    VRcuAllocate___024root___eval_initial(&(vlSymsp->TOP));
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial loop\n"););
        VRcuAllocate___024root___eval_settle(&(vlSymsp->TOP));
        VRcuAllocate___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VRcuAllocate___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("RcuAllocate.v", 1, "",
                "Verilated model didn't DC converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VRcuAllocate___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VRcuAllocate::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VRcuAllocate::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VRcuAllocate___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        VRcuAllocate___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VRcuAllocate___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("RcuAllocate.v", 1, "",
                "Verilated model didn't converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VRcuAllocate___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

//============================================================
// Invoke final blocks

void VRcuAllocate::final() {
    VRcuAllocate___024root___final(&(vlSymsp->TOP));
}

//============================================================
// Utilities

VerilatedContext* VRcuAllocate::contextp() const {
    return vlSymsp->_vm_contextp__;
}

const char* VRcuAllocate::name() const {
    return vlSymsp->name();
}
