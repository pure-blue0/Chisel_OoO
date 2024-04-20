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
#9 Do not omit or summarize the function description, describe it completely.
#10 Do not omit enumeration types.
## Input 
```
# Name
Module type
# function
Function description
# parameter
Parameters of module
# Input
# output
I/O port of module
# Instance
Instances of the module 
# reference model
the reference model writen in C++, Contains the functions of the module
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
1. Set m_RollBackTag_new to m_RollBackTag, m_RobState_new to m_RobState, m_ExcpCause_new to m_ExcpCause, and m_ExcpTval_new to m_ExcpTval, with all other outputs set to 0. This indicates that no changes to any state are made, and the ROB is not updated.
2. Assign RobTag to RobTag_in, outputting the current instruction's index in the ROB; set ROB_update to 1, indicating that ROB data needs to be updated, and set the isStable variable to 1, signifying that the instruction has stabilized.
3. If the current instruction is a store instruction and both the data and address are ready, set the Done signal to 1; otherwise, it remains 0.
4. If the current instruction has an exception, set both the done signal and the isExcp signal to 1, indicating that the instruction is completed but has encountered an exception. If the state of m_RobState is not idle (Rob_Idle), then set m_RobState_new to the undo state (Rob_Undo), record the tag of the instruction triggering the undo operation in m_RollBackTag_new (RobTag_in), record the exception cause in m_ExcpCause_new (excp_cause_in), and the exception value in m_ExcpTval_new (excpt_tval_in). This means that due to this exception, the reorder buffer needs to undo some operations or states.
# Parameter：

# Input:
[3:0] RobTag_in: The current instruction's position in the ROB.
[2:0] Function_type: The execution unit required by the instruction.
Agu_addr_ready: Whether the address for the instruction's memory access is ready.
Agu_data_ready: Whether the data for the instruction's memory storage is ready.
excp_valid_in: Stores the exception status of the current instruction.
[5:0] excp_cause_in: Stores the type of the current exception instruction.
[31:0] excpt_tval_in: Stores the address of the current exception instruction.
[3:0] m_RollBackTag: Rollback tag.
[2:0] m_RobState: ROB state.
[5:0] m_ExcpCause: Exception cause.
[31:0] m_ExcpTval: Exception address.

# Output:
ROB_update: ROB update signal.
isStable: Whether the current instruction is stable.
Done: Whether the address and data for the current STU instruction are ready.
isExcp: Whether the current store instruction has encountered an exception.
[3:0] RobTag: The current instruction's position in the ROB.
[3:0] m_RollBackTag_new: New rollback tag.
[2:0] m_RobState_new: New ROB state.
[5:0] m_ExcpCause_new: New exception cause.
[31:0] m_ExcpTval_new: New exception address.

# reference model
In the reference model, the c++ code for the corresponding module is as follows. Please refer to the code and implement the exact same function in chisel without omitting anything:
enumeration types:
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
	    <!-- Inputs for the AGUFastDetect Unit -->
	    <Input name="RobTag_in" type="[3:0]" description="The current instruction's position in the ROB."/>
	    <Input name="Function_type" type="[2:0]" description="The execution unit required by the instruction."/>
	    <Input name="Agu_addr_ready" type="Boolean" description="Whether the address for the instruction's memory access is ready."/>
	    <Input name="Agu_data_ready" type="Boolean" description="Whether the data for the instruction's memory storage is ready."/>
	    <Input name="excp_valid_in" type="Boolean" description="Stores the exception status of the current instruction."/>
	    <Input name="excp_cause_in" type="[5:0]" description="Stores the type of the current exception instruction."/>
	    <Input name="excpt_tval_in" type="[31:0]" description="Stores the address of the current exception instruction."/>
	    <Input name="m_RollBackTag" type="[3:0]" description="Rollback tag."/>
	    <Input name="m_RobState" type="[2:0]" description="ROB state."/>
	    <Input name="m_ExcpCause" type="[5:0]" description="Exception cause."/>
	    <Input name="m_ExcpTval" type="[31:0]" description="Exception address."/>
	    <!-- Outputs from the AGUFastDetect Unit -->
	    <Output name="ROB_update" type="Boolean" description="ROB update signal."/>
	    <Output name="isStable" type="Boolean" description="Whether the current instruction is stable."/>
	    <Output name="Done" type="Boolean" description="Whether the address and data for the current STU instruction are ready."/>
	    <Output name="isExcp" type="Boolean" description="Whether the current store instruction has encountered an exception."/>
	    <Output name="RobTag" type="[3:0]" description="The current instruction's position in the ROB."/>
	    <Output name="m_RollBackTag_new" type="[3:0]" description="New rollback tag."/>
	    <Output name="m_RobState_new" type="[2:0]" description="New ROB state."/>
	    <Output name="m_ExcpCause_new" type="[5:0]" description="New exception cause."/>
	    <Output name="m_ExcpTval_new" type="[31:0]" description="New exception address."/>
	</Interface>
	<Function description="1. Set m_RollBackTag_new to m_RollBackTag, m_RobState_new to m_RobState, m_ExcpCause_new to m_ExcpCause, and m_ExcpTval_new to m_ExcpTval, with all other outputs set to 0. This indicates that no changes to any state are made, and the ROB is not updated.&#10;2. Assign RobTag to RobTag_in, outputting the current instruction's index in the ROB; set ROB_update to 1, indicating that ROB data needs to be updated, and set the isStable variable to 1, signifying that the instruction has stabilized.&#10;3. If the current instruction is a store instruction and both the data and address are ready, set the Done signal to 1; otherwise, it remains 0.&#10;4. If the current instruction has an exception, set both the done signal and the isExcp signal to 1, indicating that the instruction is completed but has encountered an exception. If the state of m_RobState is not idle (Rob_Idle), then set m_RobState_new to the undo state (Rob_Undo), record the tag of the instruction triggering the undo operation in m_RollBackTag_new (RobTag_in), record the exception cause in m_ExcpCause_new (excp_cause_in), and the exception value in m_ExcpTval_new (excpt_tval_in). This means that due to this exception, the reorder buffer needs to undo some operations or states."/>
	<ReferenceModel code="void  Rcu::AGUFastDetect(InsnPtr_t&amp; insn){
    this->m_Rob[insn->RobTag].isStable = true;
    if(insn->Fu == funcType_t::STU &amp;&amp; insn->Agu_addr_ready &amp;&amp; insn->Agu_data_ready)
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
<Module type="AGUFastDetect">
	<Parameters>
	</Parameters>
	<Interface>
	    <!-- Inputs for the AGUFastDetect Unit -->
	    <Input name="RobTag_in" type="[3:0]" description="The current instruction's position in the ROB."/>
	    <Input name="Function_type" type="[2:0]" description="The execution unit required by the instruction."/>
	    <Input name="Agu_addr_ready" type="Boolean" description="Whether the address for the instruction's memory access is ready."/>
	    <Input name="Agu_data_ready" type="Boolean" description="Whether the data for the instruction's memory storage is ready."/>
	    <Input name="excp_valid_in" type="Boolean" description="Stores the exception status of the current instruction."/>
	    <Input name="excp_cause_in" type="[5:0]" description="Stores the type of the current exception instruction."/>
	    <Input name="excpt_tval_in" type="[31:0]" description="Stores the address of the current exception instruction."/>
	    <Input name="m_RollBackTag" type="[3:0]" description="Rollback tag."/>
	    <Input name="m_RobState" type="[2:0]" description="ROB state."/>
	    <Input name="m_ExcpCause" type="[5:0]" description="Exception cause."/>
	    <Input name="m_ExcpTval" type="[31:0]" description="Exception address."/>
	    <!-- Outputs from the AGUFastDetect Unit -->
	    <Output name="ROB_update" type="Boolean" description="ROB update signal."/>
	    <Output name="isStable" type="Boolean" description="Whether the current instruction is stable."/>
	    <Output name="Done" type="Boolean" description="Whether the address and data for the current STU instruction are ready."/>
	    <Output name="isExcp" type="Boolean" description="Whether the current store instruction has encountered an exception."/>
	    <Output name="RobTag" type="[3:0]" description="The current instruction's position in the ROB."/>
	    <Output name="m_RollBackTag_new" type="[3:0]" description="New rollback tag."/>
	    <Output name="m_RobState_new" type="[2:0]" description="New ROB state."/>
	    <Output name="m_ExcpCause_new" type="[5:0]" description="New exception cause."/>
	    <Output name="m_ExcpTval_new" type="[31:0]" description="New exception address."/>
	</Interface>
	<Function description="1. Set m_RollBackTag_new to m_RollBackTag, m_RobState_new to m_RobState, m_ExcpCause_new to m_ExcpCause, and m_ExcpTval_new to m_ExcpTval, with all other outputs set to 0. This indicates that no changes to any state are made, and the ROB is not updated.&#10;2. Assign RobTag to RobTag_in, outputting the current instruction's index in the ROB; set ROB_update to 1, indicating that ROB data needs to be updated, and set the isStable variable to 1, signifying that the instruction has stabilized.&#10;3. If the current instruction is a store instruction and both the data and address are ready, set the Done signal to 1; otherwise, it remains 0.&#10;4. If the current instruction has an exception, set both the done signal and the isExcp signal to 1, indicating that the instruction is completed but has encountered an exception. If the state of m_RobState is not idle (Rob_Idle), then set m_RobState_new to the undo state (Rob_Undo), record the tag of the instruction triggering the undo operation in m_RollBackTag_new (RobTag_in), record the exception cause in m_ExcpCause_new (excp_cause_in), and the exception value in m_ExcpTval_new (excpt_tval_in). This means that due to this exception, the reorder buffer needs to undo some operations or states."/>
	<ReferenceModel code="void  Rcu::AGUFastDetect(InsnPtr_t&amp; insn){
    this->m_Rob[insn->RobTag].isStable = true;
    if(insn->Fu == funcType_t::STU &amp;&amp; insn->Agu_addr_ready &amp;&amp; insn->Agu_data_ready)
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
}"/>    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To implement the `AGUFastDetect` module in Chisel, we need to follow the functionality described in the XML definition. Let's break down the steps according to the given functionality:

1. Initialize the necessary Chisel IO classes for the inputs and outputs.
2. Set the `m_RollBackTag_new` to `m_RollBackTag`, `m_RobState_new` to `m_RobState`, `m_ExcpCause_new` to `m_ExcpCause`, and `m_ExcpTval_new` to `m_ExcpTval`.
3. Assign `RobTag` to `RobTag_in` and set the `ROB_update` and `isStable` signals to `true` to indicate an update is needed and the instruction is stable.
4. Check if the current instruction is a store instruction, and if both address and data are ready, then set `Done` to `true`.
5. If there is an exception, set `done` and `isExcp` to `true`. If `m_RobState` is not idle, then set `m_RobState_new` to the undo state, update `m_RollBackTag_new`, `m_ExcpCause_new`, and `m_ExcpTval_new` with the current instruction's values.

Now, let's translate this description into Chisel code.

```scala
import chisel3._
import chisel3.util._

class AGUFastDetect extends Module {
  val io = IO(new Bundle {
    // Inputs
    val RobTag_in = Input(UInt(4.W))
    val Function_type = Input(UInt(3.W))
    val Agu_addr_ready = Input(Bool())
    val Agu_data_ready = Input(Bool())
    val excp_valid_in = Input(Bool())
    val excp_cause_in = Input(UInt(6.W))
    val excpt_tval_in = Input(UInt(32.W))
    val m_RollBackTag = Input(UInt(4.W))
    val m_RobState = Input(UInt(3.W))
    val m_ExcpCause = Input(UInt(6.W))
    val m_ExcpTval = Input(UInt(32.W))
    
    // Outputs
    val ROB_update = Output(Bool())
    val isStable = Output(Bool())
    val Done = Output(Bool())
    val isExcp = Output(Bool())
    val RobTag = Output(UInt(4.W))
    val m_RollBackTag_new = Output(UInt(4.W))
    val m_RobState_new = Output(UInt(3.W))
    val m_ExcpCause_new = Output(UInt(6.W))
    val m_ExcpTval_new = Output(UInt(32.W))
  })

  // Step 1: Initialization
  io.RobTag := io.RobTag_in
  io.ROB_update := false.B
  io.isStable := false.B
  io.Done := false.B
  io.isExcp := false.B
  io.m_RollBackTag_new := io.m_RollBackTag
  io.m_RobState_new := io.m_RobState
  io.m_ExcpCause_new := io.m_ExcpCause
  io.m_ExcpTval_new := io.m_ExcpTval

  // Step 2 and 3: Update ROB and set stable
  when(io.RobTag_in === io.RobTag) {
    io.ROB_update := true.B
    io.isStable := true.B
  }

  // Step 4: Check if store instruction and address/data are ready
  when(io.Function_type === funcType_t.STU && io.Agu_addr_ready && io.Agu_data_ready) {
    io.Done := true.B
  }

  // Step 5: Check for exceptions
  when(io.excp_valid_in) {
    io.Done := true.B
    io.isExcp := true.B
    when(io.m_RobState =/= RobIdle) {
      io.m_RobState_new := RobUndo
      io.m_RollBackTag_new := io.RobTag_in
      io.m_ExcpCause_new := io.excp_cause_in
      io.m_ExcpTval_new := io.excpt_tval_in
    }
  }
}

object funcType_t {
  val ALU = 0.U(3.W)
  val BRU = 1.U(3.W)
  val CSR = 2.U(3.W)
  val LDU = 3.U(3.W)
  val STU = 4.U(3.W)
}

object RobState {
  val RobIdle = 0.U(3.W)
  val RobUndo = 1.U(3.W)
  val RobRedo = 2.U(3.W)
  val RobWaitForResume = 3.U(3.W)
  val RobFlushBackend = 4.U(3.W)
}
```

This Chisel code describes the `AGUFastDetect` module and its behavior according to your XML specification. Note that the enumerated types `funcType_t` and `RobState` are not directly supported in Chisel, so they have been created as objects with `UInt` constants. This code is ready to be integrated into a hardware design and simulated or synthesized using a Chisel toolchain.