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
 GenAddress
# Function：
生成下一个字节的取指地址，优先级为 backend redirect > predicted PC > PC + 4，其中越后端的stage发出的重定向请求，优先级越高。
# Parameter：
FetchByteWidth=8//取指的宽度
iCacheAlignByte=8//缓存块对齐字节数

# Input:
sendsuccess //成功向内存发送请求的标志
PcRegisterout_vaild //pc输出寄存器有效标志

//Redirect message
[31:0]Decode_redirectTag//decode stage 传来的重定向地址
Decode_redirect_valid//decode stage 传来的重定向有效标志位
[31:0]IEW_redirectTag//IEW stage 传来的重定向地址
IEW_redirect_valid//IEW stage 传来的重定向有效标志位
[31:0]commit_redirectTag//IEW stage 传来的重定向地址
commit_redirect_valid//IEW stage 传来的重定向有效标志位

# Output:
[31:0]PcRegister_in//下一个要取指的地址
# reference model
```cpp
void 
Fetch1::GenNextFetchAddress(bool& SendSuccess){
    InsnState_t redirectTag = InsnState_t::State_Fetch1;
    if(this->m_PcRegister.OutPort->valid){
        this->m_PcRegister.InPort->set(this->m_PcRegister.OutPort->data);
    }
    if(SendSuccess){
        this->m_PcRegister.InPort->set((this->m_PcRegister.OutPort->data & ~(this->m_iCacheAlignByte - 1)) + this->m_FetchByteWidth);//保证每次的取指请求都是对齐的。Ps（帮助理解）：这行代码可以理解为一个寄存器，相当于向PcRegister的输入放入了一个数据，下一个周期这个数据会到PcRegister的输出上
    }
    for(auto RedirectPort : this->m_RedirectPortVec){
        //如果某个重定向端口的数据有效且所在阶段大于或等于 Fetch1，则表示有更高优先级的重定向请求,越往后优先级越高
        if(RedirectPort->valid && RedirectPort->data.StageId >= redirectTag){
            this->m_PcRegister.InPort->set(RedirectPort->data.target);
            redirectTag = RedirectPort->data.StageId;
        }
    }
}
```


---
## GPT output
```
<Module type="GenAddress">
<Parameters>
    <Parameter name="FetchByteWidth" type="integer" value="8" description="Width of fetch in bytes"/>
    <Parameter name="iCacheAlignByte" type="integer" value="8" description="Alignment bytes for cache block"/>
</Parameters>
<Interface>
    <Input name="sendsuccess" type="bool" description="Flag indicating successful memory request"/>
    <Input name="PcRegisterout_valid" type="bool" description="Flag indicating validity of pc output register"/>
    <Input name="Decode_redirectTag" type="integer" size="32" description="Redirect address from decode stage"/>
    <Input name="Decode_redirect_valid" type="bool"
           description="Flag indicating validity of redirect address from decode stage"/>
    <Input name="IEW_redirectTag" type="integer" size="32" description="Redirect address from IEW stage"/>
    <Input name="IEW_redirect_valid" type="bool"
           description="Flag indicating validity of redirect address from IEW stage"/>
    <Input name="commit_redirectTag" type="integer" size="32" description="Redirect address from commit stage"/>
    <Input name="commit_redirect_valid" type="bool"
           description="Flag indicating validity of redirect address from commit stage"/>
    <Output name="PcRegister_in" type="integer" size="32" description="Next fetch address"/>
</Interface>
<Function description="Generate next fetch address. Priority order: backend redirect > predicted PC > PC + 4"/>
<ReferenceModel code="void GenNextFetchAddress(bool& SendSuccess){ InsnState_t redirectTag = InsnState_t::State_Fetch1;
    if(this->m_PcRegister.OutPort->valid){ this->m_PcRegister.InPort->set(this->m_PcRegister.OutPort->data); }
    if(SendSuccess){ this->m_PcRegister.InPort->set((this->m_PcRegister.OutPort->data & ~(this->m_iCacheAlignByte - 1))
    + this->m_FetchByteWidth); } for(auto RedirectPort : this->m_RedirectPortVec){ if(RedirectPort->valid &&
    RedirectPort->data.StageId >= redirectTag){ this->m_PcRegister.InPort->set(RedirectPort->data.target); redirectTag =
    RedirectPort->data.StageId; } } }"/>
</Module>
```



---


# Chisel Generate
## System prompt
#1 Please act as a professional IC designer. 
#2 Follow the user’s requirements carefully & to the letter.
#3 All the I/O ports should be used in the generated module
#4 You need to pay attention to every detail in the functional description
#5  First, Think step by step and give reasoning steps to achieve it.
#6 Then, generate the given module in Chisel. 

## user prompt
xml文件，即上文中GPT output

## GPT output
To design a Chisel module based on the given requirements, let’s follow the steps outlined:

### Step 1: Understand the Requirements
1. **FetchByteWidth**: This parameter defines the width of a fetch operation in bytes.
2. **iCacheAlignByte**: This parameter specifies the alignment bytes for a cache block.
3. Inputs and outputs are specified for handling various stages of instruction processing, including redirect signals from different pipeline stages and a PC (Program Counter) register update.
4. The module should prioritize redirect addresses over the predicted PC, and then the incremented PC (PC + 4).

### Step 2: Define the Module in Chisel
1. Define a Chisel module with the specified parameters and I/O ports.
2. Implement the logic to generate the next fetch address, respecting the priority of the backend redirect, the predicted PC, and then PC + 4, as per the function description.

### Step 3: Implement the Logic
1. **Redirect Logic**: Check the valid flags for redirect addresses from the commit, IEW, and decode stages. Use the one with the highest priority that is valid.
2. **Predicted PC and PC+4 Logic**: If no valid redirect address is found, use the current PC value; if `sendsuccess` is true, increment the PC by `FetchByteWidth` aligned to `iCacheAlignByte`.

### Chisel Code Implementation

```scala
import chisel3._
import chisel3.util._

class GenAddress(val FetchByteWidth: Int, val iCacheAlignByte: Int) extends Module {
  val io = IO(new Bundle {
    val sendsuccess = Input(Bool())
    val PcRegisterout_valid = Input(Bool())
    val Decode_redirectTag = Input(UInt(32.W))
    val Decode_redirect_valid = Input(Bool())
    val IEW_redirectTag = Input(UInt(32.W))
    val IEW_redirect_valid = Input(Bool())
    val commit_redirectTag = Input(UInt(32.W))
    val commit_redirect_valid = Input(Bool())
    val PcRegister_in = Output(UInt(32.W))
  })

  // Default next PC is current PC + 4, aligned to iCacheAlignByte
  val defaultNextPC = (io.PcRegister_in + FetchByteWidth.U) & ~(iCacheAlignByte.U - 1.U)

  // Check for redirects and prioritize them
  val redirectAddress = MuxCase(defaultNextPC, Seq(
    io.commit_redirect_valid -> io.commit_redirectTag,
    io.IEW_redirect_valid -> io.IEW_redirectTag,
    io.Decode_redirect_valid -> io.Decode_redirectTag
  ))

  // Set next fetch address
  io.PcRegister_in := Mux(io.sendsuccess, redirectAddress, defaultNextPC)
}

```

### Explanation
- The `GenAddress` module is parameterized with `FetchByteWidth` and `iCacheAlignByte`.
- Input and output ports are defined according to the specification.
- Logic for prioritizing redirect tags and calculating the next PC address is implemented.
- The module uses Chisel's `MuxCase` and `Mux` functions to select between redirect addresses and the incremented PC value.
