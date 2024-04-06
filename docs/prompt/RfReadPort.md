**system**:
You are a helpful assistant. You can help me by answering my questions. You can also ask me questions.

--------------------

**user**:
# IR Generate
## System prompt

#1 Act as a professional IC designer who are proficient in writing xml file.
#2 Follow the user’s requirements carefully & to the letter.
#3 Your responses should be informative and logical.
#4 You should answer in English
#5 Then output the code in a single code block.
#6 First, analyse the following information and generate a xml file 
#7 You mustn't  generate Instance tag if there's no Instance in user's input
#8 the input and output is shown below
#9 output in a formal format
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
RfReadPort
Compatible with HDL changes:
1. The way the data is sent has been changed to be compatible with hdl, which outputs a desIndex_out, which is then matched by each execute unit, and if a match is found, the passed data is processed.
2. Input data is no longer based on the value of PhyRs read from IntRegfile as in the module. Instead, data is read from IntRegfile based on the value of PhyRs in the issue, and whether it is assigned to Operand depends on whether operand is ready.
# Function：
When the input data vaild is valid, execute the following operation:
1. If Insn_Operand1Ready=0, the operand is not ready and nothing is done; if Insn_Operand1Ready=1, Insn_PhyRs1_data is assigned to Operand1
2. If Insn_Operand2Ready=0, the operand is not ready and nothing is done; if Insn_Operand2Ready=1, Insn_PhyRs2_data is assigned to Operand2
3. Assign the signal with the suffix in such as desIndex_in to the corresponding port signal with the suffix out
# Parameter：

# Input:
Vaild//Indicates that the input data of each port of the current module is valid
Insn_Operand1Ready//Whether Operand 1 is ready
[63:0]Insn_PhyRs1_data//The data stored in physical register 1
Insn_Operand2Ready//Whether Operand 2 is ready
[63:0]Insn_PhyRs2_data//The data stored in physical register 2
[2:0]desIndex_in//The ID of the functional unit to which insn is assigned
[3:0]Sub_OP_in//The specific action that the instruction needs to perform in the execution unit
[63:0]imm_in//Operands of the current instruction
[63:0]pc_in//pc of the current instruction
Pred_taken_in//Whether the current instruction has jumped
[63:0]pred_target_in//Branch prediction address for the current instruction
[2:0]Function_type_in//The execution unit that the instruction needs to invoke

# Output:
[2:0]desIndex//The ID of the functional unit to which insn is assigned
[63:0]Insn_operand1//Operand 1
[63:0]Insn_operand2//Operand 2
[3:0]Sub_OP_out//The specific action that the instruction needs to perform in the execution unit
[2:0]desIndex_out//The ID of the functional unit to which insn is assigned
[63:0]imm_out//Operands of the current instruction
[63:0]pc_out//pc for the current instruction
Pred_taken_out//Whether the current instruction has jumped
[63:0]pred_target_out//Branch prediction address for the current instruction
[2:0]Function_type_in//The execution unit that the instruction needs to invoke


# reference model
In the reference model, the c++ code for the corresponding module is as follows. Please refer to the code and implement the exact same function in chisel without omitting anything:
```cpp
void RfReadPort::Evaluate(){
    if(this->m_CalcuPipe.OutPort->valid){
        IssueInfo& t    = this->m_CalcuPipe.OutPort->data;
        InsnPtr_t& insn = t.insn;
        if(!insn->Operand1Ready){ 
                insn->Operand1 =  this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRs1];            
        }
        if(!insn->Operand2Ready)
        {
                insn->Operand2 =  this->m_Processor->getRcuPtr()->m_IntRegfile[insn->PhyRs2];
        }
        this->m_Processor->m_FuncUnitVec[t.desIndex]->Issue(insn);//根据匹配的function模块的id，将数据发送给对应的function unit
    }
}

--------------------

**assistant**:
```xml
<Module type="RfReadPort">
	<Parameters>
	</Parameters>
	<Interface>
	    <!-- Inputs to the RfReadPort -->
	    <Input name="Vaild" type="boolean" description="Indicates that the input data of each port of the current module is valid"/>
	    <Input name="Insn_Operand1Ready" type="boolean" description="Whether Operand 1 is ready"/>
	    <Input name="Insn_PhyRs1_data" type="[63:0]" description="The data stored in physical register 1"/>
	    <Input name="Insn_Operand2Ready" type="boolean" description="Whether Operand 2 is ready"/>
	    <Input name="Insn_PhyRs2_data" type="[63:0]" description="The data stored in physical register 2"/>
	    <Input name="desIndex_in" type="[2:0]" description="The ID of the functional unit to which insn is assigned"/>
	    <Input name="Sub_OP_in" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	    <Input name="imm_in" type="[63:0]" description="Operands of the current instruction"/>
	    <Input name="pc_in" type="[63:0]" description="pc of the current instruction"/>
	    <Input name="Pred_taken_in" type="boolean" description="Whether the current instruction has jumped"/>
	    <Input name="pred_target_in" type="[63:0]" description="Branch prediction address for the current instruction"/>
	    <Input name="Function_type_in" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	    
	    <!-- Outputs from the RfReadPort -->
	    <Output name="desIndex" type="[2:0]" description="The ID of the functional unit to which insn is assigned"/>
	    <Output name="Insn_operand1" type="[63:0]" description="Operand 1"/>
	    <Output name="Insn_operand2" type="[63:0]" description="Operand 2"/>
	    <Output name="Sub_OP_out" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	    <Output name="desIndex_out" type="[2:0]" description="The ID of the functional unit to which insn is assigned"/>
	    <Output name="imm_out" type="[63:0]" description="Operands of the current instruction"/>
	    <Output name="pc_out" type="[63:0]" description="pc for the current instruction"/>
	    <Output name="Pred_taken_out" type="boolean" description="Whether the current instruction has jumped"/>
	    <Output name="pred_target_out" type="[63:0]" description="Branch prediction address for the current instruction"/>
	    <Output name="Function_type_in" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	</Interface>
	<Function description="When the input data valid is valid, execute: 1. Assign Insn_PhyRs1_data to Operand1 if Insn_Operand1Ready=1. 2. Assign Insn_PhyRs2_data to Operand2 if Insn_Operand2Ready=1. 3. Assign the signal with the suffix in to the corresponding port signal with the suffix out."/>
	<ReferenceModel code="void RfReadPort::Evaluate(){ if(this-&gt;m_CalcuPipe.OutPort-&gt;valid){ IssueInfo&amp; t = this-&gt;m_CalcuPipe.OutPort-&gt;data; InsnPtr_t&amp; insn = t.insn; if(!insn-&gt;Operand1Ready){ insn-&gt;Operand1 = this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntRegfile[insn-&gt;PhyRs1]; } if(!insn-&gt;Operand2Ready) { insn-&gt;Operand2 = this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntRegfile[insn-&gt;PhyRs2]; } this-&gt;m_Processor-&gt;m_FuncUnitVec[t.desIndex]-&gt;Issue(insn); }}"/>
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->
```

--------------------

**user**:
# Chisel Generate
## System prompt
#1 Please act as a professional IC designer. 
#2 Follow the user’s requirements carefully & to the letter.
#3 All the I/O ports should be used in the generated module
#4 You need to pay attention to every detail in the functional description
#5  First, Think step by step and give reasoning steps to achieve it.
#6 Then, generate the given module in Chisel. 

## user prompt
<Module type="RfReadPort">
	<Parameters>
	</Parameters>
	<Interface>
	    <!-- Inputs to the RfReadPort -->
	    <Input name="Vaild" type="boolean" description="Indicates that the input data of each port of the current module is valid"/>
	    <Input name="Insn_Operand1Ready" type="boolean" description="Whether Operand 1 is ready"/>
	    <Input name="Insn_PhyRs1_data" type="[63:0]" description="The data stored in physical register 1"/>
	    <Input name="Insn_Operand2Ready" type="boolean" description="Whether Operand 2 is ready"/>
	    <Input name="Insn_PhyRs2_data" type="[63:0]" description="The data stored in physical register 2"/>
	    <Input name="desIndex_in" type="[2:0]" description="The ID of the functional unit to which insn is assigned"/>
	    <Input name="Sub_OP_in" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	    <Input name="imm_in" type="[63:0]" description="Operands of the current instruction"/>
	    <Input name="pc_in" type="[63:0]" description="pc of the current instruction"/>
	    <Input name="Pred_taken_in" type="boolean" description="Whether the current instruction has jumped"/>
	    <Input name="pred_target_in" type="[63:0]" description="Branch prediction address for the current instruction"/>
	    <Input name="Function_type_in" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	    
	    <!-- Outputs from the RfReadPort -->
	    <Output name="desIndex" type="[2:0]" description="The ID of the functional unit to which insn is assigned"/>
	    <Output name="Insn_operand1" type="[63:0]" description="Operand 1"/>
	    <Output name="Insn_operand2" type="[63:0]" description="Operand 2"/>
	    <Output name="Sub_OP_out" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	    <Output name="desIndex_out" type="[2:0]" description="The ID of the functional unit to which insn is assigned"/>
	    <Output name="imm_out" type="[63:0]" description="Operands of the current instruction"/>
	    <Output name="pc_out" type="[63:0]" description="pc for the current instruction"/>
	    <Output name="Pred_taken_out" type="boolean" description="Whether the current instruction has jumped"/>
	    <Output name="pred_target_out" type="[63:0]" description="Branch prediction address for the current instruction"/>
	    <Output name="Function_type_in" type="[2:0]" description="The execution unit that the instruction needs to invoke"/>
	</Interface>
	<Function description="When the input data valid is valid, execute: 1. Assign Insn_PhyRs1_data to Operand1 if Insn_Operand1Ready=1. 2. Assign Insn_PhyRs2_data to Operand2 if Insn_Operand2Ready=1. 3. Assign the signal with the suffix in to the corresponding port signal with the suffix out."/>
	<ReferenceModel code="void RfReadPort::Evaluate(){ if(this-&gt;m_CalcuPipe.OutPort-&gt;valid){ IssueInfo&amp; t = this-&gt;m_CalcuPipe.OutPort-&gt;data; InsnPtr_t&amp; insn = t.insn; if(!insn-&gt;Operand1Ready){ insn-&gt;Operand1 = this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntRegfile[insn-&gt;PhyRs1]; } if(!insn-&gt;Operand2Ready) { insn-&gt;Operand2 = this-&gt;m_Processor-&gt;getRcuPtr()-&gt;m_IntRegfile[insn-&gt;PhyRs2]; } this-&gt;m_Processor-&gt;m_FuncUnitVec[t.desIndex]-&gt;Issue(insn); }}"/>
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To achieve the functionality described for the `RfReadPort` module in Chisel, we need to follow these steps:

### Step 1: Define Module Interface
First, we define the inputs and outputs as per the XML specification. We need to ensure that all inputs and outputs are correctly typed according to the Chisel syntax. For example, `boolean` types in XML correspond to `Bool` in Chisel, and `[63:0]` arrays correspond to `UInt(64.W)`.

### Step 2: Implement the Functionality
Based on the function description, we need to write Chisel code that:
1. Checks if the input data is valid.
2. If `Insn_Operand1Ready` is true, `Insn_PhyRs1_data` is assigned to `Operand1`, and similarly for `Operand2`.
3. Passes through signals like `desIndex_in` to `desIndex_out`, ensuring to match the input description to the output.

### Step 3: Wire Inputs to Outputs as Specified
We must ensure that inputs that directly map to outputs (e.g., `desIndex_in` to `desIndex_out`) are correctly wired in the Chisel code.

### Chisel Implementation
```scala
import chisel3._

class RfReadPort extends Module {
  val io = IO(new Bundle {
    // Inputs
    val valid = Input(Bool())
    val insn_operand1Ready = Input(Bool())
    val insn_phyRs1_data = Input(UInt(64.W))
    val insn_operand2Ready = Input(Bool())
    val insn_phyRs2_data = Input(UInt(64.W))
    val desIndex_in = Input(UInt(3.W))
    val sub_OP_in = Input(UInt(4.W))
    val imm_in = Input(UInt(64.W))
    val pc_in = Input(UInt(64.W))
    val pred_taken_in = Input(Bool())
    val pred_target_in = Input(UInt(64.W))
    val function_type_in = Input(UInt(3.W))

    // Outputs
    val desIndex = Output(UInt(3.W))
    val insn_operand1 = Output(UInt(64.W))
    val insn_operand2 = Output(UInt(64.W))
    val sub_OP_out = Output(UInt(4.W))
    val desIndex_out = Output(UInt(3.W))
    val imm_out = Output(UInt(64.W))
    val pc_out = Output(UInt(64.W))
    val pred_taken_out = Output(Bool())
    val pred_target_out = Output(UInt(64.W))
    val function_type_out = Output(UInt(3.W))
  })

  // Implementing the functionality
  when(io.valid) {
    io.insn_operand1 := Mux(io.insn_operand1Ready, io.insn_phyRs1_data, 0.U)
    io.insn_operand2 := Mux(io.insn_operand2Ready, io.insn_phyRs2_data, 0.U)
  }

  // Direct assignments for signals that pass through
  io.desIndex := io.desIndex_in
  io.desIndex_out := io.desIndex_in
  io.sub_OP_out := io.sub_OP_in
  io.imm_out := io.imm_in
  io.pc_out := io.pc_in
  io.pred_taken_out := io.pred_taken_in
  io.pred_target_out := io.pred_target_in
  io.function_type_out := io.function_type_in
}

// Generate the Verilog code
println((new chisel3.stage.ChiselStage).emitVerilog(new RfReadPort))
```

This Chisel code defines a module `RfReadPort` according to the provided XML specification. It includes all the inputs and outputs, correctly types them according to Chisel syntax, and implements the specified functionality using conditional logic and direct signal assignments.