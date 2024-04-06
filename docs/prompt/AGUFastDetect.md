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
AGUFastDetect
# Function：
1. Set RobTag=RobTag_in to output the index of the current instruction in rob.
2. According to the index of rob, set the corresponding isStable variable to 1
3. If the current instruction is a storage instruction, and the data and address are ready, set the Done signal to 1, otherwise 0
4. If there is an exception in the current instruction, the done signal is set to 1, and the isExcp signal is also 1, indicating that the instruction has been completed but an exception occurred. If the current state of the reorder buffer is not idle (Rob_Idle), then the state of the reorder buffer is set to need to be undone (Rob_Undo).The instruction tag that triggered the undo operation (RobTag) is recorded in the RollBackTag, the cause of the exception (excp_cause_in) in the excp_cause_out, and the outlier (excpt_tval_in) in the excpt_tval_out. This means that the reorder buffer needs to undo some action or state because of this exception.
# Parameter：

# Input:
[3:0]RobTag_in//The position of the current instruction in the ROB
[2:0]Function_type//The execution unit that the instruction needs to invoke
Agu_addr_ready//Whether the address in memory is ready for the instruction to access data
Agu_data_ready//Whether the data stored in memory by the instruction is ready
[2:0]RobState//rob's current state
excp_vaild_in//Stores exceptions for the current instruction
[5:0]excp_cause_in//Stores the type of the current exception instruction
[31:0]excpt_tval_in//Store the address of the current exception instruction

# Output:
isStable//Is the current instruction stable
Done//Whether the address and data of the current STU instruction are ready
isExcp//Whether an exception occurred for the current store instruction
[3:0]RobTag//The position of the current instruction in the ROB
[3:0]RollBackTag//The updated rollback label of rob
excp_vaild_out//Stores exceptions for the current instruction
[5:0]excp_cause_out//Stores the type of the current exception instruction
[31:0]excpt_tval_out//Store the address of the current exception instruction

# reference model
In the reference model, the c++ code for the corresponding module is as follows. Please refer to the code and implement the exact same function in chisel without omitting anything:
Possible enumeration types:
enum rob_state_t{
    Rob_Idle, Rob_Undo, Rob_Redo, Rob_WaitForResume, Rob_FlushBackend
};
enum funcType_t {
    ALU , BRU, CSR, LDU, STU
};
```cpp
void  Rcu::AGUFastDetect(InsnPtr_t& insn){
    this->m_Rob[insn->RobTag].isStable = true;
    if(insn->Fu == funcType_t::STU && insn->Agu_addr_ready && insn->Agu_data_ready)
    {
        this->m_Rob[insn->RobTag].done = true;
    }
    else this->m_Rob[insn->RobTag].done = false;
    if(insn->Excp.valid){
        this->m_Rob[insn->RobTag].done   = true;
        this->m_Rob[insn->RobTag].isExcp = true;
        if(this->m_RobState != rob_state_t::Rob_Idle )
        {
            this->m_RobState = rob_state_t::Rob_Undo;
            this->m_RollBackTag = insn->RobTag;
            this->m_ExcpCause   = insn->Excp.Cause;
            this->m_ExcpTval    = insn->Excp.Tval;
        }
    }
}

--------------------

**assistant**:
```xml
<Module type="AGUFastDetect">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="RobTag_in" type="[3:0]" description="The position of the current instruction in the ROB"/>
	        <Input name="Function_type" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	        <Input name="Agu_addr_ready" type="Boolean" description="Whether the address in memory is ready for the instruction to access data"/>
	        <Input name="Agu_data_ready" type="Boolean" description="Whether the data stored in memory by the instruction is ready"/>
	        <Input name="RobState" type="[2:0]" description="ROB's current state"/>
	        <Input name="excp_vaild_in" type="Boolean" description="Stores exceptions for the current instruction"/>
	        <Input name="excp_cause_in" type="[5:0]" description="Stores the type of the current exception instruction"/>
	        <Input name="excpt_tval_in" type="[31:0]" description="Store the address of the current exception instruction"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="isStable" type="Boolean" description="Is the current instruction stable"/>
	        <Output name="Done" type="Boolean" description="Whether the address and data of the current STU instruction are ready"/>
	        <Output name="isExcp" type="Boolean" description="Whether an exception occurred for the current store instruction"/>
	        <Output name="RobTag" type="[3:0]" description="The position of the current instruction in the ROB"/>
	        <Output name="RollBackTag" type="[3:0]" description="The updated rollback label of rob"/>
	        <Output name="excp_vaild_out" type="Boolean" description="Stores exceptions for the current instruction"/>
	        <Output name="excp_cause_out" type="[5:0]" description="Stores the type of the current exception instruction"/>
	        <Output name="excpt_tval_out" type="[31:0]" description="Store the address of the current exception instruction"/>
	</Interface>
	<Function description="1. Set RobTag=RobTag_in to output the index of the current instruction in rob. 2. According to the index of rob, set the corresponding isStable variable to 1. 3. If the current instruction is a storage instruction, and the data and address are ready, set the Done signal to 1, otherwise 0. 4. If there is an exception in the current instruction, the done signal is set to 1, and the isExcp signal is also 1, indicating that the instruction has been completed but an exception occurred. If the current state of the reorder buffer is not idle (Rob_Idle), then the state of the reorder buffer is set to need to be undone (Rob_Undo).The instruction tag that triggered the undo operation (RobTag) is recorded in the RollBackTag, the cause of the exception (excp_cause_in) in the excp_cause_out, and the outlier (excpt_tval_in) in the excpt_tval_out. This means that the reorder buffer needs to undo some action or state because of this exception."/>
<ReferenceModel code="void  Rcu::AGUFastDetect(InsnPtr_t&amp; insn){    this-&gt;m_Rob[insn-&gt;RobTag].isStable = true;    if(insn-&gt;Fu == funcType_t::STU &amp;&amp; insn-&gt;Agu_addr_ready &amp;&amp; insn-&gt;Agu_data_ready)    {        this-&gt;m_Rob[insn-&gt;RobTag].done = true;    }    else this-&gt;m_Rob[insn-&gt;RobTag].done = false;    if(insn-&gt;Excp.valid){        this-&gt;m_Rob[insn-&gt;RobTag].done   = true;        this-&gt;m_Rob[insn-&gt;RobTag].isExcp = true;        if(this-&gt;m_RobState != rob_state_t::Rob_Idle )        {            this-&gt;m_RobState = rob_state_t::Rob_Undo;            this-&gt;m_RollBackTag = insn-&gt;RobTag;            this-&gt;m_ExcpCause   = insn-&gt;Excp.Cause;            this-&gt;m_ExcpTval    = insn-&gt;Excp.Tval;        }    }}" />    
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
<Module type="AGUFastDetect">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="RobTag_in" type="[3:0]" description="The position of the current instruction in the ROB"/>
	        <Input name="Function_type" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	        <Input name="Agu_addr_ready" type="Boolean" description="Whether the address in memory is ready for the instruction to access data"/>
	        <Input name="Agu_data_ready" type="Boolean" description="Whether the data stored in memory by the instruction is ready"/>
	        <Input name="RobState" type="[2:0]" description="ROB's current state"/>
	        <Input name="excp_vaild_in" type="Boolean" description="Stores exceptions for the current instruction"/>
	        <Input name="excp_cause_in" type="[5:0]" description="Stores the type of the current exception instruction"/>
	        <Input name="excpt_tval_in" type="[31:0]" description="Store the address of the current exception instruction"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="isStable" type="Boolean" description="Is the current instruction stable"/>
	        <Output name="Done" type="Boolean" description="Whether the address and data of the current STU instruction are ready"/>
	        <Output name="isExcp" type="Boolean" description="Whether an exception occurred for the current store instruction"/>
	        <Output name="RobTag" type="[3:0]" description="The position of the current instruction in the ROB"/>
	        <Output name="RollBackTag" type="[3:0]" description="The updated rollback label of rob"/>
	        <Output name="excp_vaild_out" type="Boolean" description="Stores exceptions for the current instruction"/>
	        <Output name="excp_cause_out" type="[5:0]" description="Stores the type of the current exception instruction"/>
	        <Output name="excpt_tval_out" type="[31:0]" description="Store the address of the current exception instruction"/>
	</Interface>
	<Function description="1. Set RobTag=RobTag_in to output the index of the current instruction in rob. 2. According to the index of rob, set the corresponding isStable variable to 1. 3. If the current instruction is a storage instruction, and the data and address are ready, set the Done signal to 1, otherwise 0. 4. If there is an exception in the current instruction, the done signal is set to 1, and the isExcp signal is also 1, indicating that the instruction has been completed but an exception occurred. If the current state of the reorder buffer is not idle (Rob_Idle), then the state of the reorder buffer is set to need to be undone (Rob_Undo).The instruction tag that triggered the undo operation (RobTag) is recorded in the RollBackTag, the cause of the exception (excp_cause_in) in the excp_cause_out, and the outlier (excpt_tval_in) in the excpt_tval_out. This means that the reorder buffer needs to undo some action or state because of this exception."/>
<ReferenceModel code="void  Rcu::AGUFastDetect(InsnPtr_t&amp; insn){    this-&gt;m_Rob[insn-&gt;RobTag].isStable = true;    if(insn-&gt;Fu == funcType_t::STU &amp;&amp; insn-&gt;Agu_addr_ready &amp;&amp; insn-&gt;Agu_data_ready)    {        this-&gt;m_Rob[insn-&gt;RobTag].done = true;    }    else this-&gt;m_Rob[insn-&gt;RobTag].done = false;    if(insn-&gt;Excp.valid){        this-&gt;m_Rob[insn-&gt;RobTag].done   = true;        this-&gt;m_Rob[insn-&gt;RobTag].isExcp = true;        if(this-&gt;m_RobState != rob_state_t::Rob_Idle )        {            this-&gt;m_RobState = rob_state_t::Rob_Undo;            this-&gt;m_RollBackTag = insn-&gt;RobTag;            this-&gt;m_ExcpCause   = insn-&gt;Excp.Cause;            this-&gt;m_ExcpTval    = insn-&gt;Excp.Tval;        }    }}" />    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To implement the `AGUFastDetect` module as described, in Chisel, we will follow these steps logically:

1. **Define the Module and its I/O**: We will define a Chisel module named `AGUFastDetect`. This module will have inputs and outputs as specified in the XML description.

2. **Implement the Logic for Stability, Done Signal, and Exception Handling**: Based on the function description, we will implement the logic to set the `isStable` flag, determine the `Done` signal based on whether it is a storage instruction (`STU`) and if address and data are ready, and handle exceptions to set `isExcp`, `RobTag`, `RollBackTag`, `excp_vaild_out`, `excp_cause_out`, and `excpt_tval_out`.

3. **Use Chisel Data Types Appropriately**: We will use `Bool` for Boolean types, `UInt` for unsigned integers with specified widths.

4. **Initialization of Outputs**: All outputs will be initialized appropriately, considering that Chisel does not allow unassigned outputs.

Here is how the Chisel code for `AGUFastDetect` might look based on the given specifications:

```scala
import chisel3._

class AGUFastDetect extends Module {
  val io = IO(new Bundle {
    // Inputs
    val RobTag_in = Input(UInt(4.W))
    val Function_type = Input(UInt(3.W))
    val Agu_addr_ready = Input(Bool())
    val Agu_data_ready = Input(Bool())
    val RobState = Input(UInt(3.W))
    val excp_vaild_in = Input(Bool())
    val excp_cause_in = Input(UInt(6.W))
    val excpt_tval_in = Input(UInt(32.W))
    
    // Outputs
    val isStable = Output(Bool())
    val Done = Output(Bool())
    val isExcp = Output(Bool())
    val RobTag = Output(UInt(4.W))
    val RollBackTag = Output(UInt(4.W))
    val excp_vaild_out = Output(Bool())
    val excp_cause_out = Output(UInt(6.W))
    val excpt_tval_out = Output(UInt(32.W))
  })

  // Logic
  io.isStable := true.B // As per the description, isStable is always set to true when a new instruction is detected
  io.RobTag := io.RobTag_in // Directly output the input RobTag
  
  // Determine if the Done signal should be set
  io.Done := io.Function_type === 4.U && io.Agu_addr_ready && io.Agu_data_ready // Assuming STU is represented by '4'
  
  // Exception handling
  when(io.excp_vaild_in) {
    io.isExcp := true.B
    io.Done := true.B
    when(io.RobState =/= 0.U) { // Assuming Rob_Idle is represented by '0'
      io.RobState := 1.U // Assuming Rob_Undo is represented by '1'
      io.RollBackTag := io.RobTag_in
      io.excp_cause_out := io.excp_cause_in
      io.excpt_tval_out := io.excpt_tval_in
    }
  } .otherwise {
    io.isExcp := false.B
    io.excp_vaild_out := false.B
    io.excp_cause_out := 0.U
    io.excpt_tval_out := 0.U
  }
}

println((new chisel3.stage.ChiselStage).emitVerilog(new AGUFastDetect))
```

This Chisel code snippet is a direct translation of the provided specifications and functionality into Chisel, a hardware construction language used for designing digital circuits.