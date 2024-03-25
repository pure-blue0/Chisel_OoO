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
# Output
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
	        <Input name="" type="" width="" description=""/>
            <Input name="" type="" description=""/>
	        <!-- Outputs from each Functional Unit  -->
            <Output name="" type="" width="" description=""/>
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
sendsuccess //成功向内存发送请求的标志
PcRegisterout_vaild //pc输出寄存器有效标志
[31:0]PcRegisterout//pc寄存器的输出地址
//Redirect message
[31:0]Decode_redirectTag//decode stage 传来的重定向地址
Decode_redirect_vaild//decode stage 传来的重定向有效标志位
 
[31:0]IEW_redirectTag//IEW stage 传来的重定向地址
IEW_redirect_vaild//IEW stage 传来的重定向有效标志位
 
[31:0]commit_redirectTag//IEW stage 传来的重定向地址
commit_redirect_vaild//IEW stage 传来的重定向有效标志位
 
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
To design a module named `GenAddress` in Chisel, fulfilling the specified requirements and functionality, we'll approach this step by step:

### Understanding the Requirements:
1. **Inputs:**
    - `sendsuccess`: Indicates if the memory request was successful.
    - `PcRegisterout_valid`: Indicates if the PC output register is valid.
    - `Decode_redirectTag`, `IEW_redirectTag`, `commit_redirectTag`: Redirect addresses from different stages.
    - `Decode_redirect_valid`, `IEW_redirect_valid`, `commit_redirect_valid`: Validity flags for the respective redirect addresses.

2. **Output:**
    - `PcRegister_in`: The next fetch address.

3. **Parameters:**
    - `FetchByteWidth`: Width of fetch in bytes.
    - `iCacheAlignByte`: Alignment bytes for cache block.

4. **Functionality:**
    - Generate the next fetch address with a priority order: backend redirect > predicted PC > PC + 4.

### Reasoning Steps:
1. **Default Increment:**
    - If `PcRegisterout_valid` is true and `sendsuccess` is false, increment the PC by 4.

2. **SendSuccess Handling:**
    - On `sendsuccess`, align the current PC to cache alignment and add `FetchByteWidth`.

3. **Redirect Handling:**
    - Check the validity of redirect signals. Priority is given to `commit_redirect` > `IEW_redirect` > `Decode_redirect`.

### Chisel Implementation:
Here's a Chisel module that implements the described functionality:

```scala
import chisel3._
import chisel3.util._

class GenAddress extends Module {
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

  val FetchByteWidth = 8.U
  val iCacheAlignByte = 8.U

  when(io.commit_redirect_valid) {
    io.PcRegister_in := io.commit_redirectTag
  } .elsewhen(io.IEW_redirect_valid) {
    io.PcRegister_in := io.IEW_redirectTag
  } .elsewhen(io.Decode_redirect_valid) {
    io.PcRegister_in := io.Decode_redirectTag
  } .elsewhen(io.PcRegisterout_valid && !io.sendsuccess) {
    io.PcRegister_in := io.PcRegister_in + 4.U
  } .elsewhen(io.sendsuccess) {
    io.PcRegister_in := (io.PcRegister_in & ~(iCacheAlignByte - 1.U)) + FetchByteWidth
  } .otherwise {
    io.PcRegister_in := io.PcRegister_in // Hold current value if no conditions are met
  }
}

```

This module takes the inputs and parameters, checks the conditions specified in the functionality, and sets the `PcRegister_in` output accordingly, adhering to the prioritization of redirect signals and handling the `sendsuccess` condition as specified.