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
#9 output in a formal format


## user prompt
# name
BranchRedirect
# Function：
1. Used for branch prediction. For JAL instruction (insn[6:0]= 0b1101111), default prediction jump, set the prediction target address to the current instruction address plus the offset of JAL instruction (ujimm()), and set Redirect_ID to 2. Redirect_Target is set to the address of the currently predicted jump, and needRedirect is set to false
2. For other instructions, default branch prediction does not jump
# Parameter：

# Input:
[31:0]Insn               //Input instructions to predict

# Output:
Perd_Taken              //Whether to predict jump flags
[31:0]Pred_Target       //Predict the address of the jump
[3:0]Redirect_ID        //Represents a redirect request sent from the decode
[31:0]Redirect_Target   //The destination address for the redirect
needRedirect            //Whether a redirect is required

# reference model
```cpp
void Decode::BranchRedirect(InsnPtr_t& insn,bool& needRedirect,Redirect_t& RedirectReq){
    
    needRedirect = false;
    RedirectReq.StageId = InsnState_t::State_Decode;
    RISCV::StaticInsn instruction(insn->UncompressedInsn);
    insn->Pred.Taken        = false;
    insn->Pred.Target       = insn->Pc +  4;//pred.target contains the next address by default
    if(instruction.opcode() == 0b1101111){ // JAL jumps by default
        insn->Pred.Taken    = true;
        insn->Pred.Target   = insn->Pc + instruction.ujimm();//Instruction.ujimm () represents the riscv j-type instruction immediate number
        needRedirect = true;
        RedirectReq.target = insn->Pred.Target;
    }
}

```

--------------------

**assistant**:
```xml
<Module type="BranchRedirect">
	<Parameters>
		<!-- No parameters specified -->
	</Parameters>
	<Interface>
	    <!-- Interfaces for each Functional Unit -->
	    <Input name="Insn" type="[31:0]" description="Input instructions to predict"/>
	    <!-- Outputs from each Functional Unit  -->
	    <Output name="Perd_Taken" type="" description="Whether to predict jump flags"/>
	    <Output name="Pred_Target" type="[31:0]" description="Predict the address of the jump"/>
	    <Output name="Redirect_ID" type="[3:0]" description="Represents a redirect request sent from the decode"/>
	    <Output name="Redirect_Target" type="[31:0]" description="The destination address for the redirect"/>
	    <Output name="needRedirect" type="" description="Whether a redirect is required"/>
	</Interface>
	<Function description="1. Used for branch prediction. For JAL instruction (insn[6:0]= 0b1101111), default prediction jump, set the prediction target address to the current instruction address plus the offset of JAL instruction (ujimm()), and set Redirect_ID to 2. Redirect_Target is set to the address of the currently predicted jump, and needRedirect is set to false
2. For other instructions, default branch prediction does not jump"/>
	<ReferenceModel code="void Decode::BranchRedirect(InsnPtr_t&amp; insn,bool&amp; needRedirect,Redirect_t&amp; RedirectReq){
    
    needRedirect = false;
    RedirectReq.StageId = InsnState_t::State_Decode;
    RISCV::StaticInsn instruction(insn-&gt;UncompressedInsn);
    insn-&gt;Pred.Taken        = false;
    insn-&gt;Pred.Target       = insn-&gt;Pc +  4;//pred.target contains the next address by default
    if(instruction.opcode() == 0b1101111){ // JAL jumps by default
        insn-&gt;Pred.Taken    = true;
        insn-&gt;Pred.Target   = insn-&gt;Pc + instruction.ujimm();//Instruction.ujimm () represents the riscv j-type instruction immediate number
        needRedirect = true;
        RedirectReq.target = insn-&gt;Pred.Target;
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
#2 Follow the user’s requirements carefully & to the letter.
#3 All the I/O ports should be used in the generated module
#4 You need to pay attention to every detail in the functional description
#5  First, Think step by step and give reasoning steps to achieve it.
#6 Then, generate the given module in Chisel. 

## user prompt
<Module type="BranchRedirect">
	<Parameters>
		<!-- No parameters specified -->
	</Parameters>
	<Interface>
	    <!-- Interfaces for each Functional Unit -->
	    <Input name="Insn" type="[31:0]" description="Input instructions to predict"/>
	    <!-- Outputs from each Functional Unit  -->
	    <Output name="Perd_Taken" type="" description="Whether to predict jump flags"/>
	    <Output name="Pred_Target" type="[31:0]" description="Predict the address of the jump"/>
	    <Output name="Redirect_ID" type="[3:0]" description="Represents a redirect request sent from the decode"/>
	    <Output name="Redirect_Target" type="[31:0]" description="The destination address for the redirect"/>
	    <Output name="needRedirect" type="" description="Whether a redirect is required"/>
	</Interface>
	<Function description="1. Used for branch prediction. For JAL instruction (insn[6:0]= 0b1101111), default prediction jump, set the prediction target address to the current instruction address plus the offset of JAL instruction (ujimm()), and set Redirect_ID to 2. Redirect_Target is set to the address of the currently predicted jump, and needRedirect is set to false
2. For other instructions, default branch prediction does not jump"/>
	<ReferenceModel code="void Decode::BranchRedirect(InsnPtr_t&amp; insn,bool&amp; needRedirect,Redirect_t&amp; RedirectReq){
    
    needRedirect = false;
    RedirectReq.StageId = InsnState_t::State_Decode;
    RISCV::StaticInsn instruction(insn-&gt;UncompressedInsn);
    insn-&gt;Pred.Taken        = false;
    insn-&gt;Pred.Target       = insn-&gt;Pc +  4;//pred.target contains the next address by default
    if(instruction.opcode() == 0b1101111){ // JAL jumps by default
        insn-&gt;Pred.Taken    = true;
        insn-&gt;Pred.Target   = insn-&gt;Pc + instruction.ujimm();//Instruction.ujimm () represents the riscv j-type instruction immediate number
        needRedirect = true;
        RedirectReq.target = insn-&gt;Pred.Target;
    }
}"/>    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To implement the `BranchRedirect` module in Chisel, we need to consider the functional requirements and interface specifications detailed in the XML. The process involves:

1. **Defining the Module Interface**: We need to define inputs and outputs as specified, including the 32-bit instruction input (`Insn`) and various outputs (`Perd_Taken`, `Pred_Target`, `Redirect_ID`, `Redirect_Target`, `needRedirect`).

2. **Implementing Functionality**: Based on the provided reference model, we must implement the logic for branch prediction, especially for handling JAL instructions versus others.

3. **Handling JAL Instruction**: For a JAL instruction (`insn[6:0] == 0b1101111`), we set the prediction target address based on the current PC value and the JAL instruction offset. The `needRedirect` flag is set to true, and specific outputs are adjusted accordingly.

4. **Handling Other Instructions**: For instructions other than JAL, default branch prediction does not predict a jump, and `needRedirect` remains false.

Now, translating these requirements into Chisel:

```scala
import chisel3._
import chisel3.util._

class BranchRedirect extends Module {
  val io = IO(new Bundle {
    val Insn = Input(UInt(32.W))
    val Perd_Taken = Output(Bool())
    val Pred_Target = Output(UInt(32.W))
    val Redirect_ID = Output(UInt(4.W))
    val Redirect_Target = Output(UInt(32.W))
    val needRedirect = Output(Bool())
  })

  io.Perd_Taken := false.B
  io.Pred_Target := 0.U
  io.Redirect_ID := 0.U
  io.Redirect_Target := 0.U
  io.needRedirect := false.B

  when(io.Insn(6,0) === "b1101111".U) {
    io.Perd_Taken := true.B
    io.Pred_Target := io.Insn // This should be calculated based on PC + offset, adjust as per actual logic
    io.Redirect_ID := 2.U
    io.Redirect_Target := io.Pred_Target
    io.needRedirect := true.B
  } .otherwise {
    io.Perd_Taken := false.B
    io.Pred_Target := io.Insn // Adjust based on default behavior, typically PC + 4
    io.Redirect_ID := 0.U
    io.needRedirect := false.B
  }
}
```

Note: This implementation is a basic translation. You need to adjust the `Pred_Target` logic to correctly calculate the target address based on the current PC and the offset for JAL instructions, as the exact logic for this calculation depends on the full context of your design (e.g., how the PC is managed and updated).