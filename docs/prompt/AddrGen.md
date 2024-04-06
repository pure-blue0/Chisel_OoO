**system**:
You are a helpful assistant. You can help me by answering my questions. You can also ask me questions.

--------------------

**user**:
# IR Generate
## System prompt

#1 Act as a professional IC designer who are proficient in writing xml file.
#2 Please follow the user's requirements carefully and to the letter.
#3 Your responses should be informative and logical.
#4 Output the code in a single code block.
#5 Analyse the following information and generate a xml file. 
#6 Do not generate an Instance tag if there is no Instance in the user's input.
#7 The input and output are shown below.
#8 Output in a formal format.
## Input 
```
# Name
Module type
# Function
Function description
# Parameter
Parameters of module
# Input
# Output
I/O port of module
# Instance
Instances of the module 
# Reference model
The reference model writen in C++, Contains the functions of the module
```
## output
```xml
<Module type="">
	<Parameters>
		<Parameter name="" type="" value="" description=""/>	
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="" type="" description=""/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="" type="" description=""/>
	</Interface>
	<!-- Instances of Functional Units -->        
    <Instance name="" type="">
        <!-- Interface for the Instance -->
    </Instance>
    <Function description=""/>
<ReferenceModel code=""/>    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->
```


## user prompt
# name
AddrGen
# Function：
1. Compute the address of the load or store instruction to access data in memory 
2. Check if the addresses are aligned: If a load (LDU) instruction is accessed, check if the load address is aligned by load type (such as byte, half-word, word). If a storage (STU) instruction is accessed, the storage address is checked if it is aligned by the storage type (such as half-word, word, double-word). If the address is not aligned, the exception information is set for subsequent processing.
# Parameter：

# Input:
[63:0]Insn_operand1//Operand 1
[63:0]imm//Operands of the current instruction
[2:0]Function_type//The execution unit that the instruction needs to invoke
[3:0]Sub_OP//The specific action that the instruction needs to perform in the execution unit

# Output:
[31:0]Agu_addr//The address in memory at which the instruction accesses data
Agu_addr_ready//Whether the address of the instruction to access data in memory is ready
excp_vaild//Stores exceptions for the current instruction
[5:0]excp_cause//Stores the type of the current exception instruction
[31:0]excpt_tval//The address where the current exception instruction is stored

# reference model
In the reference model, the c++ code for the corresponding module is as follows. Please refer to the code and implement the exact same function in chisel without omitting anything:
Possible enumeration types:
enum funcType_t {
    ALU , BRU, CSR, LDU, STU
};
enum ExcpCause_t{
    INSTR_ADDR_MISALIGNED = 0,
    INSTR_ACCESS_FAULT    = 1,  // Illegal access as governed by PMPs and PMAs
    ILLEGAL_INSTR         = 2,
    BREAKPOINT            = 3,
    LD_ADDR_MISALIGNED    = 4,
    LD_ACCESS_FAULT       = 5,  // Illegal access as governed by PMPs and PMAs
    ST_ADDR_MISALIGNED    = 6,
    ST_ACCESS_FAULT       = 7,  // Illegal access as governed by PMPs and PMAs
    ENV_CALL_UMODE        = 8,  // environment call from user mode
    ENV_CALL_SMODE        = 9,  // environment call from supervisor mode
    ENV_CALL_MMODE        = 11, // environment call from machine mode
    INSTR_PAGE_FAULT      = 12, // Instruction page fault
    LOAD_PAGE_FAULT       = 13, // Load page fault
    STORE_PAGE_FAULT      = 15, // Store page fault
    DEBUG_REQUEST         = 24  // Debug request
};

Definitions that may be used:
#define LDU_LB          0
#define LDU_LH          1
#define LDU_LW          2
#define LDU_LD          3
#define LDU_LBU         4
#define LDU_LHU         5
#define LDU_LWU         6

#define STU_SB          0
#define STU_SH          1
#define STU_SW          2
#define STU_SD          3

```cpp
void AddrGen(InsnPtr_t& insn){
        insn->Agu_addr  =  insn->Operand1 + insn->imm;
        insn->Agu_addr_ready = true;
        if(insn->Fu == funcType_t::LDU){

            switch (insn->SubOp)
            {
            case LDU_LH  :
                if((insn->Agu_addr & 0b1) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case LDU_LW  :
                if((insn->Agu_addr & 0b11) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case LDU_LD  :
                if((insn->Agu_addr & 0b111) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case LDU_LHU :
                if((insn->Agu_addr & 0b1) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
            case LDU_LWU :
                if((insn->Agu_addr & 0b11) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            default:
                break;
            }
        }else if (insn->Fu == funcType_t::STU){
            switch (insn->SubOp)
            {
            case STU_SH:
                if((insn->Agu_addr & 0b1) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case STU_SW:
                if((insn->Agu_addr & 0b11) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            case STU_SD:
                if((insn->Agu_addr & 0b111) != 0){
                    insn->Excp.valid = false;
                    insn->Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn->Excp.Tval  = insn->Agu_addr;
                }
                break;
            default:
                break;
            }
        }
    };

--------------------

**assistant**:
```xml
<Module type="AddrGen">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="Insn_operand1" type="[63:0]" description="Operand 1"/>
	        <Input name="imm" type="[63:0]" description="Operands of the current instruction"/>
	        <Input name="Function_type" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	        <Input name="Sub_OP" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="Agu_addr" type="[31:0]" description="The address in memory at which the instruction accesses data"/>
	        <Output name="Agu_addr_ready" type="" description="Whether the address of the instruction to access data in memory is ready"/>
	        <Output name="excp_vaild" type="" description="Stores exceptions for the current instruction"/>
	        <Output name="excp_cause" type="[5:0]" description="Stores the type of the current exception instruction"/>
	        <Output name="excpt_tval" type="[31:0]" description="The address where the current exception instruction is stored"/>
	</Interface>
	<Function description="1. Compute the address of the load or store instruction to access data in memory 2. Check if the addresses are aligned: If a load (LDU) instruction is accessed, check if the load address is aligned by load type (such as byte, half-word, word). If a storage (STU) instruction is accessed, the storage address is checked if it is aligned by the storage type (such as half-word, word, double-word). If the address is not aligned, the exception information is set for subsequent processing."/>
	<ReferenceModel code="void AddrGen(InsnPtr_t&amp;amp; insn){
        insn-&amp;gt;Agu_addr  =  insn-&amp;gt;Operand1 + insn-&amp;gt;imm;
        insn-&amp;gt;Agu_addr_ready = true;
        if(insn-&amp;gt;Fu == funcType_t::LDU){

            switch (insn-&amp;gt;SubOp)
            {
            case LDU_LH  :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b1) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case LDU_LW  :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b11) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case LDU_LD  :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b111) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case LDU_LHU :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b1) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
            case LDU_LWU :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b11) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            default:
                break;
            }
        }else if (insn-&amp;gt;Fu == funcType_t::STU){
            switch (insn-&amp;gt;SubOp)
            {
            case STU_SH:
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b1) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case STU_SW:
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b11) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case STU_SD:
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b111) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            default:
                break;
            }
        }
    }"/>
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->
```

--------------------

**user**:
# Chisel Generate
## System prompt
#1 Please act as a professional IC designer. 
#2 Follow the user’s requirements carefully and to the letter.
#3 All the I/O ports should be used in the generated module
#4 You need to pay attention to every detail in the functional description
#5 First, Think step by step and give reasoning steps to achieve it.
#6 Then, generate the given module in Chisel. 
#7 Do not omit any signal definition or initialization.
#8 Do not omit any logic and make sure that complete functionality is achieved.

## user prompt
<Module type="AddrGen">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="Insn_operand1" type="[63:0]" description="Operand 1"/>
	        <Input name="imm" type="[63:0]" description="Operands of the current instruction"/>
	        <Input name="Function_type" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	        <Input name="Sub_OP" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="Agu_addr" type="[31:0]" description="The address in memory at which the instruction accesses data"/>
	        <Output name="Agu_addr_ready" type="" description="Whether the address of the instruction to access data in memory is ready"/>
	        <Output name="excp_vaild" type="" description="Stores exceptions for the current instruction"/>
	        <Output name="excp_cause" type="[5:0]" description="Stores the type of the current exception instruction"/>
	        <Output name="excpt_tval" type="[31:0]" description="The address where the current exception instruction is stored"/>
	</Interface>
	<Function description="1. Compute the address of the load or store instruction to access data in memory 2. Check if the addresses are aligned: If a load (LDU) instruction is accessed, check if the load address is aligned by load type (such as byte, half-word, word). If a storage (STU) instruction is accessed, the storage address is checked if it is aligned by the storage type (such as half-word, word, double-word). If the address is not aligned, the exception information is set for subsequent processing."/>
	<ReferenceModel code="void AddrGen(InsnPtr_t&amp;amp; insn){
        insn-&amp;gt;Agu_addr  =  insn-&amp;gt;Operand1 + insn-&amp;gt;imm;
        insn-&amp;gt;Agu_addr_ready = true;
        if(insn-&amp;gt;Fu == funcType_t::LDU){

            switch (insn-&amp;gt;SubOp)
            {
            case LDU_LH  :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b1) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case LDU_LW  :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b11) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case LDU_LD  :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b111) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case LDU_LHU :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b1) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
            case LDU_LWU :
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b11) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::LD_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            default:
                break;
            }
        }else if (insn-&amp;gt;Fu == funcType_t::STU){
            switch (insn-&amp;gt;SubOp)
            {
            case STU_SH:
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b1) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case STU_SW:
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b11) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            case STU_SD:
                if((insn-&amp;gt;Agu_addr &amp;amp; 0b111) != 0){
                    insn-&amp;gt;Excp.valid = false;
                    insn-&amp;gt;Excp.Cause = RISCV::ExcpCause_t::ST_ADDR_MISALIGNED;
                    insn-&amp;gt;Excp.Tval  = insn-&amp;gt;Agu_addr;
                }
                break;
            default:
                break;
            }
        }
    }"/>
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To implement the `AddrGen` module in Chisel, following the detailed functionality and interface described, we will proceed with the reasoning steps and then translate those steps into Chisel code.

### Reasoning Steps:
1. **Module Interface**: Define the inputs and outputs according to the given specifications. All inputs and outputs are explicitly defined, including operand inputs, function type, and sub-operation type, as well as the computed address, readiness flag, and exception information.

2. **Address Generation**: Compute the address by adding `Insn_operand1` and `imm`. This operation is straightforward and performed regardless of the instruction type.

3. **Alignment Check**: Depending on the function type (LDU or STU) and the specific sub-operation, check if the generated address is aligned. This involves bitwise operations to check the lower bits of the address according to the required alignment for each operation type.

4. **Exception Handling**: If an alignment error is detected, set the exception valid flag to true, specify the exception cause, and store the faulting address in `excpt_tval`. 

5. **Output Readiness**: Once the address is computed and any necessary alignment checks are performed, set the `Agu_addr_ready` signal to true.

### Chisel Code:

```scala
import chisel3._
import chisel3.util._

class AddrGen extends Module {
  val io = IO(new Bundle {
    // Inputs
    val Insn_operand1 = Input(UInt(64.W))
    val imm = Input(UInt(64.W))
    val Function_type = Input(UInt(3.W))
    val Sub_OP = Input(UInt(4.W))
    // Outputs
    val Agu_addr = Output(UInt(32.W))
    val Agu_addr_ready = Output(Bool())
    val excp_valid = Output(Bool())
    val excp_cause = Output(UInt(6.W))
    val excpt_tval = Output(UInt(32.W))
  })

  // Default assignments
  io.Agu_addr := (io.Insn_operand1 + io.imm)(31, 0)
  io.Agu_addr_ready := true.B
  io.excp_valid := false.B
  io.excp_cause := 0.U
  io.excpt_tval := 0.U

  // Alignment check based on function type and specific sub-operation
  when(io.Function_type === 3.U) { // LDU
    switch(io.Sub_OP) {
      is(1.U) { // LDU_LH
        when((io.Agu_addr & 1.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 4.U // LD_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(2.U) { // LDU_LW
        when((io.Agu_addr & 3.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 4.U // LD_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(3.U) { // LDU_LD
        when((io.Agu_addr & 7.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 4.U // LD_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
    }
  }.elsewhen(io.Function_type === 4.U) { // STU
    switch(io.Sub_OP) {
      is(1.U) { // STU_SH
        when((io.Agu_addr & 1.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 6.U // ST_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(2.U) { // STU_SW
        when((io.Agu_addr & 3.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 6.U // ST_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
      is(3.U) { // STU_SD
        when((io.Agu_addr & 7.U) =/= 0.U) {
          io.excp_valid := true.B
          io.excp_cause := 6.U // ST_ADDR_MISALIGNED
          io.excpt_tval := io.Agu_addr
        }
      }
    }
  }
}

// Generate the Verilog code
object AddrGen extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new AddrGen)
}
```

This Chisel module follows the provided specifications, calculating the address from the given inputs, checking for alignment based on the operation type, and setting the exception information accordingly.