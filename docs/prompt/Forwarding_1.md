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
#9 Do not omit or summarize the function description, describe it completely
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
Forwarding
# Function(Do not omit or summarize the function description, describe it completely)：
1. Set all outputs to 0 by default.
2. First, check data_valid to determine whether the input data is valid. If the valid flag is true, it means that there is an instruction to be processed.
3. If the target register index (IsaRd) of the instruction is not 0 (that is, a valid target register exists), the following is done to send an update to IntBusylist and IntRegfile:
(1) Set update_valid to 1, indicating that data needs to be written to IntBusylist and IntRegfile.
(2) Set the forwarding flag of the instruction target register to true, indicating that the value of the register will be forwarded.
(3) Set the done flag of the instruction target register to true, indicating that the calculation result of the register is ready and can be used by subsequent instructions.
(4) Write the calculation result (RdResult_in) to RdResult_out, and write PhyRd_in to PhyRd_out.
4. If the above conditions are not met, set all outputs to 0.
# Parameter：

# Input:
data_valid//Describe whether the data input is valid
[4:0]isa_rd//Describes the isa rd register id of the current instruction
[5:0]PhyRd_in//Describes the Rd physical register id of the current instruction
[63:0]RdResult_in//Data calculated by exetcute

# Output:
update_valid//  Indicates that the output data is valid, and also serves as a valid signal to update IntBusylist and IntRegfile
forwarding//Indicates whether the calculation result of the instruction is forwarded
done//Indicates whether the data in the PhyRd register is ready
[63:0]RdResult_out//Data calculated by exetcute
[5:0]PhyRd_out//Describes the Rd physical register id of the current instruction

# reference model
In the reference model, the c++ code of the corresponding module is as follows, refer to the code, and implement the exact same functionality in chisel (if there is a conflict with the Function description, refer to the Function description) without omitting anything:
```cpp
void  WriteBackPort::Forwarding(){
    if(this->m_CalcuPipe.InPort->valid){
        InsnPtr_t& insn = this->m_CalcuPipe.InPort->data;
        if(insn->IsaRd != 0){
            this->m_Processor->getRcuPtr()->m_IntBusylist[insn->PhyRd].forwarding = true;
            this->m_Processor->getRcuPtr()->m_IntBusylist[insn->PhyRd].done = true;//Indicates that the rd of the current instruction was passed forward
            this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRd] = insn->RdResult;//Write the rd data directly to the rd physical register from which future read operands can be read
        }
    }
}
```

--------------------

**assistant**:
```xml
<Module type="Forwarding">
	<Parameters>
		<!-- No parameters specified -->
	</Parameters>
	<Interface>
	        <!-- Interface for Input -->
	        <Input name="data_valid" type="bool" description="Describe whether the data input is valid"/>
	        <Input name="isa_rd" type="[4:0]" description="Describes the isa rd register id of the current instruction"/>
	        <Input name="PhyRd_in" type="[5:0]" description="Describes the Rd physical register id of the current instruction"/>
	        <Input name="RdResult_in" type="[63:0]" description="Data calculated by execute"/>

	        <!-- Interface for Output -->
	        <Output name="update_valid" type="bool" description="Indicates that the output data is valid, and also serves as a valid signal to update IntBusylist and IntRegfile"/>
	        <Output name="forwarding" type="bool" description="Indicates whether the calculation result of the instruction is forwarded"/>
	        <Output name="done" type="bool" description="Indicates whether the data in the PhyRd register is ready"/>
	        <Output name="RdResult_out" type="[63:0]" description="Data calculated by execute"/>
	        <Output name="PhyRd_out" type="[5:0]" description="Describes the Rd physical register id of the current instruction"/>
	</Interface>
	<!-- No Instances specified -->
    <Function description="1. Set all outputs to 0 by default.
2. First, check data_valid to determine whether the input data is valid. If the valid flag is true, it means that there is an instruction to be processed.
3. If the target register index (IsaRd) of the instruction is not 0 (that is, a valid target register exists), the following is done to send an update to IntBusylist and IntRegfile:
(1) Set update_valid to 1, indicating that data needs to be written to IntBusylist and IntRegfile.
(2) Set the forwarding flag of the instruction target register to true, indicating that the value of the register will be forwarded.
(3) Set the done flag of the instruction target register to true, indicating that the calculation result of the register is ready and can be used by subsequent instructions.
(4) Write the calculation result (RdResult_in) to RdResult_out, and write PhyRd_in to PhyRd_out.
4. If the above conditions are not met, set all outputs to 0."/>
<ReferenceModel code="void  WriteBackPort::Forwarding(){
    if(this->m_CalcuPipe.InPort-&gt;valid){
        InsnPtr_t&amp; insn = this->m_CalcuPipe.InPort-&gt;data;
        if(insn-&gt;IsaRd != 0){
            this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntBusylist[insn-&gt;PhyRd].forwarding = true;
            this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntBusylist[insn-&gt;PhyRd].done = true;//Indicates that the rd of the current instruction was passed forward
            this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntRegfile[insn-&gt;PhyRd] = insn-&gt;RdResult;//Write the rd data directly to the rd physical register from which future read operands can be read
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
<Module type="Forwarding">
	<Parameters>
		<!-- No parameters specified -->
	</Parameters>
	<Interface>
	        <!-- Interface for Input -->
	        <Input name="data_valid" type="bool" description="Describe whether the data input is valid"/>
	        <Input name="isa_rd" type="[4:0]" description="Describes the isa rd register id of the current instruction"/>
	        <Input name="PhyRd_in" type="[5:0]" description="Describes the Rd physical register id of the current instruction"/>
	        <Input name="RdResult_in" type="[63:0]" description="Data calculated by execute"/>

	        <!-- Interface for Output -->
	        <Output name="update_valid" type="bool" description="Indicates that the output data is valid, and also serves as a valid signal to update IntBusylist and IntRegfile"/>
	        <Output name="forwarding" type="bool" description="Indicates whether the calculation result of the instruction is forwarded"/>
	        <Output name="done" type="bool" description="Indicates whether the data in the PhyRd register is ready"/>
	        <Output name="RdResult_out" type="[63:0]" description="Data calculated by execute"/>
	        <Output name="PhyRd_out" type="[5:0]" description="Describes the Rd physical register id of the current instruction"/>
	</Interface>
	<!-- No Instances specified -->
    <Function description="1. Set all outputs to 0 by default.
2. First, check data_valid to determine whether the input data is valid. If the valid flag is true, it means that there is an instruction to be processed.
3. If the target register index (IsaRd) of the instruction is not 0 (that is, a valid target register exists), the following is done to send an update to IntBusylist and IntRegfile:
(1) Set update_valid to 1, indicating that data needs to be written to IntBusylist and IntRegfile.
(2) Set the forwarding flag of the instruction target register to true, indicating that the value of the register will be forwarded.
(3) Set the done flag of the instruction target register to true, indicating that the calculation result of the register is ready and can be used by subsequent instructions.
(4) Write the calculation result (RdResult_in) to RdResult_out, and write PhyRd_in to PhyRd_out.
4. If the above conditions are not met, set all outputs to 0."/>
<ReferenceModel code="void  WriteBackPort::Forwarding(){
    if(this->m_CalcuPipe.InPort-&gt;valid){
        InsnPtr_t&amp; insn = this->m_CalcuPipe.InPort-&gt;data;
        if(insn-&gt;IsaRd != 0){
            this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntBusylist[insn-&gt;PhyRd].forwarding = true;
            this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntBusylist[insn-&gt;PhyRd].done = true;//Indicates that the rd of the current instruction was passed forward
            this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntRegfile[insn-&gt;PhyRd] = insn-&gt;RdResult;//Write the rd data directly to the rd physical register from which future read operands can be read
        }
    }
}"/>    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To translate the given module into Chisel, we will follow these reasoning steps:

1. Define a module `Forwarding` with the specified input and output ports.
2. Initialize all outputs to 0 at the start, since the function description specifies this should be the default state.
3. Check if `data_valid` is true. If not, outputs remain at their default state.
4. If `data_valid` is true, check if `isa_rd` is not 0, which implies a valid target register.
5. If `isa_rd` is not 0, perform the following actions:
   a. Set `update_valid` to true.
   b. Set `forwarding` to true.
   c. Set `done` to true.
   d. Assign `RdResult_in` to `RdResult_out`.
   e. Assign `PhyRd_in` to `PhyRd_out`.

Now, let's implement this in Chisel:

```scala
import chisel3._

class Forwarding extends Module {
  val io = IO(new Bundle {
    // Define Inputs
    val data_valid = Input(Bool())
    val isa_rd = Input(UInt(5.W))
    val PhyRd_in = Input(UInt(6.W))
    val RdResult_in = Input(UInt(64.W))

    // Define Outputs
    val update_valid = Output(Bool())
    val forwarding = Output(Bool())
    val done = Output(Bool())
    val RdResult_out = Output(UInt(64.W))
    val PhyRd_out = Output(UInt(6.W))
  })

  // Initialize outputs to 0
  io.update_valid := false.B
  io.forwarding := false.B
  io.done := false.B
  io.RdResult_out := 0.U
  io.PhyRd_out := 0.U

  // Check if input data is valid
  when(io.data_valid) {
    // Check if isa_rd is not equal to 0
    when(io.isa_rd =/= 0.U) {
      // Set outputs according to the function description
      io.update_valid := true.B
      io.forwarding := true.B
      io.done := true.B
      io.RdResult_out := io.RdResult_in
      io.PhyRd_out := io.PhyRd_in
    }
  }
}

println((new chisel3.stage.ChiselStage).emitVerilog(new Forwarding()))
```

This Chisel code describes the module `Forwarding` with the functionality as specified in the user prompt. It defines the inputs and outputs, initializes the outputs, and implements the logic to update the outputs based on the described conditions.