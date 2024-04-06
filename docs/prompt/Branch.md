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
Branch
# Function：
1. This is a Branch Prediction Unit for handling instructions of type BRU_JAR, BRU_JALR, BRU_BEQ, BRU_BNE, BRU_BLT, BRU_BGE, BRU_BLTU, BRU_BGEU, etc
2. First, the next instruction address (PC + 4) of the instruction is calculated and the result is stored in the RdResult of the instruction.
3. Use the SubOp field of the instruction to branch:
 
It sets taken to true for the BRU_JAR or BRU_JALR operation.
In the case of BRU_BEQ, the equality of Operand1 and Operand2 is taken as a result.
In the case of BRU_BNE, it determines whether Operand1 and Operand2 are not equal as a result of taken.
If it is BRU_BLT, it determines whether Operand1 is less than Operand2 as a result of taken.
In the case of BRU_BGE, it determines whether Operand1 is greater than or equal to Operand2 as a result of taken.
In the case of BRU_BLTU, both operands are converted to unsigned numbers, and then Operand1 is checked to see whether Operand2 is less than operand2 as the result of taken.
In the case of BRU_BGEU, both operands are converted to unsigned numbers and then Operand1 is greater than or equal to Operand2 as the result of taken.
For any other action, taken is set to false.
4. If the SubOp field of the instruction is BRU_JALR, calculate the BruTarget address of the jump, which is Operand1 plus the immediate number imm. If the SubOp field of the instruction is another instruction, the jump target address is determined by the value of taken: if taken is true, the jump goes to the current PC plus the immediate number imm; If taken is false, jump to the address of the next instruction (PC + 4).
5. Finally, according to the condition judgment and prediction results, the BruMisPred field of the instruction is updated: if the value of taken is not consistent with the predicted branch direction or jump target, it is set to true. Otherwise, it is set to false.
# Parameter：

# Input:
[63:0]Insn_operand1//Operand 1
[63:0]Insn_operand2//Operand 2
[63:0]imm//Operands of the current instruction
[63:0]pc//pc for the current instruction
Pred_taken//Whether the current instruction has jumped
[63:0]pred_target//Branch prediction address for the current instruction
[3:0]Sub_OP_out//The specific action that the instruction needs to perform in the execution unit

# Output:
[63:0]Insn_rdresult//Result of operation
[63:0]Insn_BruTarget//Correct jump target address
BruMisPred//Whether a branch prediction error occurred

# reference model
In the reference model, the c++ code for the corresponding module is as follows. Please refer to the code and implement the exact same function in chisel without omitting anything:
```cpp
possible define:
#define BRU_JAR         0
#define BRU_JALR        1
#define BRU_BEQ         2
#define BRU_BNE         3
#define BRU_BLT         4
#define BRU_BGE         5
#define BRU_BLTU        6
#define BRU_BGEU        7    
   void Compute(InsnPtr_t& insn){
        bool taken;
        insn->RdResult = (uint64_t)insn->Pc +  4;
        switch (insn->SubOp)
        {
        case BRU_JAR  :taken = true;break;
        case BRU_JALR :taken = true;break;
        case BRU_BEQ  :taken = (int64_t)insn->Operand1 == (int64_t)insn->Operand2;break;
        case BRU_BNE  :taken = (int64_t)insn->Operand1 != (int64_t)insn->Operand2;break;
        case BRU_BLT  :taken = (int64_t)insn->Operand1 < (int64_t)insn->Operand2;break;
        case BRU_BGE  :taken = (int64_t)insn->Operand1 >= (int64_t)insn->Operand2;break;
        case BRU_BLTU :taken = (uint64_t)insn->Operand1 < (uint64_t)insn->Operand2;break;
        case BRU_BGEU :taken = (uint64_t)insn->Operand1 >= (uint64_t)insn->Operand2;break;         
        default:taken = false;break;
        }
        if(insn->SubOp == BRU_JALR)insn->BruTarget = (int64_t)insn->Operand1 + (int64_t)insn->imm;
        else insn->BruTarget = (uint64_t)insn->Pc + (taken ? (int64_t)insn->imm :  4);
        insn->BruMisPred = (taken != insn->Pred.Taken) || (insn->BruTarget != insn->Pred.Target);
    };
---

--------------------

**assistant**:
```xml
<Module type="Branch">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="Insn_operand1" type="[63:0]" description="Operand 1"/>
	        <Input name="Insn_operand2" type="[63:0]" description="Operand 2"/>
	        <Input name="imm" type="[63:0]" description="Operands of the current instruction"/>
	        <Input name="pc" type="[63:0]" description="pc for the current instruction"/>
	        <Input name="Pred_taken" type="" description="Whether the current instruction has jumped"/>
	        <Input name="pred_target" type="[63:0]" description="Branch prediction address for the current instruction"/>
	        <Input name="Sub_OP_out" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="Insn_rdresult" type="[63:0]" description="Result of operation"/>
	        <Output name="Insn_BruTarget" type="[63:0]" description="Correct jump target address"/>
	        <Output name="BruMisPred" type="" description="Whether a branch prediction error occurred"/>
	</Interface>
	<Function description="This is a Branch Prediction Unit for handling instructions of type BRU_JAR, BRU_JALR, BRU_BEQ, BRU_BNE, BRU_BLT, BRU_BGE, BRU_BLTU, BRU_BGEU, etc. It calculates the next instruction address, uses the SubOp field to determine branching, and updates the BruMisPred field based on condition judgment and prediction results."/>
<ReferenceModel code="possible define:
#define BRU_JAR         0
#define BRU_JALR        1
#define BRU_BEQ         2
#define BRU_BNE         3
#define BRU_BLT         4
#define BRU_BGE         5
#define BRU_BLTU        6
#define BRU_BGEU        7    
   void Compute(InsnPtr_t&amp; insn){
        bool taken;
        insn-&gt;RdResult = (uint64_t)insn-&gt;Pc +  4;
        switch (insn-&gt;SubOp)
        {
        case BRU_JAR  :taken = true;break;
        case BRU_JALR :taken = true;break;
        case BRU_BEQ  :taken = (int64_t)insn-&gt;Operand1 == (int64_t)insn-&gt;Operand2;break;
        case BRU_BNE  :taken = (int64_t)insn-&gt;Operand1 != (int64_t)insn-&gt;Operand2;break;
        case BRU_BLT  :taken = (int64_t)insn-&gt;Operand1 &lt; (int64_t)insn-&gt;Operand2;break;
        case BRU_BGE  :taken = (int64_t)insn-&gt;Operand1 &gt;= (int64_t)insn-&gt;Operand2;break;
        case BRU_BLTU :taken = (uint64_t)insn-&gt;Operand1 &lt; (uint64_t)insn-&gt;Operand2;break;
        case BRU_BGEU :taken = (uint64_t)insn-&gt;Operand1 &gt;= (uint64_t)insn-&gt;Operand2;break;         
        default:taken = false;break;
        }
        if(insn-&gt;SubOp == BRU_JALR)insn-&gt;BruTarget = (int64_t)insn-&gt;Operand1 + (int64_t)insn-&gt;imm;
        else insn-&gt;BruTarget = (uint64_t)insn-&gt;Pc + (taken ? (int64_t)insn-&gt;imm :  4);
        insn-&gt;BruMisPred = (taken != insn-&gt;Pred.Taken) || (insn-&gt;BruTarget != insn-&gt;Pred.Target);
    };" />    
</Module>
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
<Module type="Branch">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="Insn_operand1" type="[63:0]" description="Operand 1"/>
	        <Input name="Insn_operand2" type="[63:0]" description="Operand 2"/>
	        <Input name="imm" type="[63:0]" description="Operands of the current instruction"/>
	        <Input name="pc" type="[63:0]" description="pc for the current instruction"/>
	        <Input name="Pred_taken" type="" description="Whether the current instruction has jumped"/>
	        <Input name="pred_target" type="[63:0]" description="Branch prediction address for the current instruction"/>
	        <Input name="Sub_OP_out" type="[3:0]" description="The specific action that the instruction needs to perform in the execution unit"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="Insn_rdresult" type="[63:0]" description="Result of operation"/>
	        <Output name="Insn_BruTarget" type="[63:0]" description="Correct jump target address"/>
	        <Output name="BruMisPred" type="" description="Whether a branch prediction error occurred"/>
	</Interface>
	<Function description="This is a Branch Prediction Unit for handling instructions of type BRU_JAR, BRU_JALR, BRU_BEQ, BRU_BNE, BRU_BLT, BRU_BGE, BRU_BLTU, BRU_BGEU, etc. It calculates the next instruction address, uses the SubOp field to determine branching, and updates the BruMisPred field based on condition judgment and prediction results."/>
<ReferenceModel code="possible define:
#define BRU_JAR         0
#define BRU_JALR        1
#define BRU_BEQ         2
#define BRU_BNE         3
#define BRU_BLT         4
#define BRU_BGE         5
#define BRU_BLTU        6
#define BRU_BGEU        7    
   void Compute(InsnPtr_t&amp; insn){
        bool taken;
        insn-&gt;RdResult = (uint64_t)insn-&gt;Pc +  4;
        switch (insn-&gt;SubOp)
        {
        case BRU_JAR  :taken = true;break;
        case BRU_JALR :taken = true;break;
        case BRU_BEQ  :taken = (int64_t)insn-&gt;Operand1 == (int64_t)insn-&gt;Operand2;break;
        case BRU_BNE  :taken = (int64_t)insn-&gt;Operand1 != (int64_t)insn-&gt;Operand2;break;
        case BRU_BLT  :taken = (int64_t)insn-&gt;Operand1 &lt; (int64_t)insn-&gt;Operand2;break;
        case BRU_BGE  :taken = (int64_t)insn-&gt;Operand1 &gt;= (int64_t)insn-&gt;Operand2;break;
        case BRU_BLTU :taken = (uint64_t)insn-&gt;Operand1 &lt; (uint64_t)insn-&gt;Operand2;break;
        case BRU_BGEU :taken = (uint64_t)insn-&gt;Operand1 &gt;= (uint64_t)insn-&gt;Operand2;break;         
        default:taken = false;break;
        }
        if(insn-&gt;SubOp == BRU_JALR)insn-&gt;BruTarget = (int64_t)insn-&gt;Operand1 + (int64_t)insn-&gt;imm;
        else insn-&gt;BruTarget = (uint64_t)insn-&gt;Pc + (taken ? (int64_t)insn-&gt;imm :  4);
        insn-&gt;BruMisPred = (taken != insn-&gt;Pred.Taken) || (insn-&gt;BruTarget != insn-&gt;Pred.Target);
    };" />    
</Module>


--------------------

**assistant**:
To translate the functionality of the Branch Prediction Unit described in the XML input into Chisel code, let's proceed step by step, considering the functionality and how to implement it in Chisel.

### Reasoning Steps:
1. **Module Definition**: Start by defining a Chisel module `Branch` with the necessary I/O according to the XML interface.
2. **I/O Definition**: Define inputs and outputs using `UInt` for unsigned integers and a generic `Bool` for the boolean inputs and outputs. The widths of the inputs and outputs will match those specified (e.g., 64 bits for the instruction operands).
3. **Branch Logic Implementation**: Implement the logic for handling the different types of branch instructions (`BRU_JAR`, `BRU_JALR`, etc.) using a `switch` statement or equivalent in Chisel (`switch` and `is`).
4. **Calculation of `RdResult`, `BruTarget`, and `BruMisPred`**: Implement the logic for calculating the next instruction address, determining the branch target address, and updating the branch misprediction flag.
5. **Constants**: Define constants for the branch instruction types to improve code readability.

### Chisel Code Generation:

```scala
import chisel3._
import chisel3.util._

// Define the Branch Module
class Branch extends Module {
  val io = IO(new Bundle {
    // Inputs
    val insn_operand1 = Input(UInt(64.W))
    val insn_operand2 = Input(UInt(64.W))
    val imm = Input(UInt(64.W))
    val pc = Input(UInt(64.W))
    val pred_taken = Input(Bool())
    val pred_target = Input(UInt(64.W))
    val sub_OP_out = Input(UInt(4.W))
    
    // Outputs
    val insn_rdresult = Output(UInt(64.W))
    val insn_bruTarget = Output(UInt(64.W))
    val bruMisPred = Output(Bool())
  })

  // Default values
  io.insn_rdresult := 0.U
  io.insn_bruTarget := 0.U
  io.bruMisPred := false.B

  // Constants for instruction types
  val BRU_JAR = 0.U
  val BRU_JALR = 1.U
  val BRU_BEQ = 2.U
  val BRU_BNE = 3.U
  val BRU_BLT = 4.U
  val BRU_BGE = 5.U
  val BRU_BLTU = 6.U
  val BRU_BGEU = 7.U

  // Calculate RdResult
  io.insn_rdresult := io.pc + 4.U

  // Branch logic
  val taken = Wire(Bool())
  taken := false.B

  switch(io.sub_OP_out) {
    is(BRU_JAR) { taken := true.B }
    is(BRU_JALR) { taken := true.B }
    is(BRU_BEQ) { taken := io.insn_operand1 === io.insn_operand2 }
    is(BRU_BNE) { taken := io.insn_operand1 =/= io.insn_operand2 }
    is(BRU_BLT) { taken := io.insn_operand1.asSInt < io.insn_operand2.asSInt }
    is(BRU_BGE) { taken := io.insn_operand1.asSInt >= io.insn_operand2.asSInt }
    is(BRU_BLTU) { taken := io.insn_operand1 < io.insn_operand2 }
    is(BRU_BGEU) { taken := io.insn_operand1 >= io.insn_operand2 }
  }

  // Calculate BruTarget
  when(io.sub_OP_out === BRU_JALR) {
    io.insn_bruTarget := io.insn_operand1 + io.imm
  } .otherwise {
    io.insn_bruTarget := io.pc + Mux(taken, io.imm, 4.U)
  }

  // Update BruMisPred
  io.bruMisPred := (taken =/= io.pred_taken) || (io.insn_bruTarget =/= io.pred_target)
}

// Generate the Verilog code
println((new chisel3.stage.ChiselStage).emitVerilog(new Branch()))
```

This Chisel code snippet closely follows the functionality described in the XML input. It defines a module with the specified inputs and outputs, implements the branching logic, and calculates the necessary outputs based on the input conditions and branch type.