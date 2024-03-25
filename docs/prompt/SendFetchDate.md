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
SendFetchData
# Function：
1.If the Inflightqueue is not empty and frontentry has received data from the cache, set pop_falg to 1, indicating that frontentry will be ejected from the queue.
2.If killed=0, frontentry is still valid. At this time, output Address, InsnByte, Excp_valid, Excp_Cause, and Excp_Tval to the corresponding port.
3.If none of the above is true, set all output ports to 0.
# Parameter：

# Input:
Inflightqueue_empty     //Signal whether the queue is empty
busy                    //entry whether the data returned by mem was received
Killed                  //A signal indicating whether entry is valid
[31:0]Address           //The first address of the extracted data in mem
Excp_vaild              //Did an exception occur?
[5:0]Excp_Cause         //The cause of the exception
[31:0]Excp_Tval         //Holds the address at which the exception was triggered
[63:0]InsnByte          //Fetched instruction

# Output:
pop_flag                //Whether to eject entry from the queue
Out_Address             //The first address of the extracted data in mem
Out_Excp_vaild          //Did an exception occur?
[5:0]Out_Excp_Cause     //The cause of the exception
[31:0]Out_Excp_Tval     //Holds the address at which the exception was triggered
[63:0]Out_InsnByte      //Fetched instruction

# reference model
```cpp
void Fetch1::SendFetchData(bool& pop_flag){
    InflighQueueEntry_t& frontEntry = this->m_InflightQueue.front();
    if(!this->m_InflightQueue.empty() && !frontEntry.Busy){//The queue is not empty and the frontentry has already fetched the data returned from the cache
        if(!frontEntry.Killed){//If frontentry is not killed
            InsnPkg_t insnPkg;
            auto insn = this->m_Processor->CreateInsn();
            insn->Pc   = frontEntry.Address;
            insn->Excp = frontEntry.Excp;
            insn->InsnByte=frontEntry.InsnByte;
            insnPkg.emplace_back(insn);
            this->m_StageOutPort->set(insnPkg);//Send data to the output port
        }
        pop_flag=true;
    }
}
```

--------------------

**assistant**:
```xml
<Module type="SendFetchData">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="Inflightqueue_empty" type="boolean" description="Signal whether the queue is empty"/>
	        <Input name="busy" type="boolean" description="Entry whether the data returned by mem was received"/>
	        <Input name="Killed" type="boolean" description="A signal indicating whether entry is valid"/>
	        <Input name="Address" type="[31:0]" description="The first address of the extracted data in mem"/>
	        <Input name="Excp_vaild" type="boolean" description="Did an exception occur?"/>
	        <Input name="Excp_Cause" type="[5:0]" description="The cause of the exception"/>
	        <Input name="Excp_Tval" type="[31:0]" description="Holds the address at which the exception was triggered"/>
	        <Input name="InsnByte" type="[63:0]" description="Fetched instruction"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="pop_flag" type="boolean" description="Whether to eject entry from the queue"/>
	        <Output name="Out_Address" type="[31:0]" description="The first address of the extracted data in mem"/>
	        <Output name="Out_Excp_vaild" type="boolean" description="Did an exception occur?"/>
	        <Output name="Out_Excp_Cause" type="[5:0]" description="The cause of the exception"/>
	        <Output name="Out_Excp_Tval" type="[31:0]" description="Holds the address at which the exception was triggered"/>
	        <Output name="Out_InsnByte" type="[63:0]" description="Fetched instruction"/>
	</Interface>
    <Function description="1.If the Inflightqueue is not empty and frontentry has received data from the cache, set pop_flag to 1, indicating that frontentry will be ejected from the queue.
2.If killed=0, frontentry is still valid. At this time, output Address, InsnByte, Excp_valid, Excp_Cause, and Excp_Tval to the corresponding port.
3.If none of the above is true, set all output ports to 0."/>
<ReferenceModel code="void Fetch1::SendFetchData(bool&amp; pop_flag){
    InflighQueueEntry_t&amp; frontEntry = this-&gt;m_InflightQueue.front();
    if(!this-&gt;m_InflightQueue.empty() &amp;&amp; !frontEntry.Busy){//The queue is not empty and the frontentry has already fetched the data returned from the cache
        if(!frontEntry.Killed){//If frontentry is not killed
            InsnPkg_t insnPkg;
            auto insn = this-&gt;m_Processor-&gt;CreateInsn();
            insn-&gt;Pc   = frontEntry.Address;
            insn-&gt;Excp = frontEntry.Excp;
            insn-&gt;InsnByte=frontEntry.InsnByte;
            insnPkg.emplace_back(insn);
            this-&gt;m_StageOutPort-&gt;set(insnPkg);//Send data to the output port
        }
        pop_flag=true;
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
<Module type="SendFetchData">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="Inflightqueue_empty" type="boolean" description="Signal whether the queue is empty"/>
	        <Input name="busy" type="boolean" description="Entry whether the data returned by mem was received"/>
	        <Input name="Killed" type="boolean" description="A signal indicating whether entry is valid"/>
	        <Input name="Address" type="[31:0]" description="The first address of the extracted data in mem"/>
	        <Input name="Excp_vaild" type="boolean" description="Did an exception occur?"/>
	        <Input name="Excp_Cause" type="[5:0]" description="The cause of the exception"/>
	        <Input name="Excp_Tval" type="[31:0]" description="Holds the address at which the exception was triggered"/>
	        <Input name="InsnByte" type="[63:0]" description="Fetched instruction"/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="pop_flag" type="boolean" description="Whether to eject entry from the queue"/>
	        <Output name="Out_Address" type="[31:0]" description="The first address of the extracted data in mem"/>
	        <Output name="Out_Excp_vaild" type="boolean" description="Did an exception occur?"/>
	        <Output name="Out_Excp_Cause" type="[5:0]" description="The cause of the exception"/>
	        <Output name="Out_Excp_Tval" type="[31:0]" description="Holds the address at which the exception was triggered"/>
	        <Output name="Out_InsnByte" type="[63:0]" description="Fetched instruction"/>
	</Interface>
    <Function description="1.If the Inflightqueue is not empty and frontentry has received data from the cache, set pop_flag to 1, indicating that frontentry will be ejected from the queue.
2.If killed=0, frontentry is still valid. At this time, output Address, InsnByte, Excp_valid, Excp_Cause, and Excp_Tval to the corresponding port.
3.If none of the above is true, set all output ports to 0."/>
<ReferenceModel code="void Fetch1::SendFetchData(bool&amp; pop_flag){
    InflighQueueEntry_t&amp; frontEntry = this-&gt;m_InflightQueue.front();
    if(!this-&gt;m_InflightQueue.empty() &amp;&amp; !frontEntry.Busy){//The queue is not empty and the frontentry has already fetched the data returned from the cache
        if(!frontEntry.Killed){//If frontentry is not killed
            InsnPkg_t insnPkg;
            auto insn = this-&gt;m_Processor-&gt;CreateInsn();
            insn-&gt;Pc   = frontEntry.Address;
            insn-&gt;Excp = frontEntry.Excp;
            insn-&gt;InsnByte=frontEntry.InsnByte;
            insnPkg.emplace_back(insn);
            this-&gt;m_StageOutPort-&gt;set(insnPkg);//Send data to the output port
        }
        pop_flag=true;
    }
}"/>    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To create the `SendFetchData` module in Chisel, we need to consider its functionalities and how to translate the XML interface definitions and behaviors into Chisel code. Chisel is a hardware construction language embedded in Scala that supports advanced hardware design using highly parameterized generators and layered domain-specific hardware languages.

### Step by Step Reasoning:

1. **Define the Module Inputs and Outputs**: The XML specifies a series of inputs and outputs, which must be declared in our Chisel module. These will include boolean signals as well as wider bit-width signals for addresses, exception codes, and instruction bytes.

2. **Implement the Logic**: Based on the function description, the module needs to:
   - Check if the `Inflightqueue` is not empty and if `frontentry` has received data (i.e., `busy` is false).
   - Check if `frontentry` is valid (i.e., `Killed` is false).
   - Set output ports based on these conditions.

3. **Output Assignment**: Depending on the conditions evaluated, we need to assign values to the output ports, ensuring we match the specified logic.

### Chisel Module Generation:

```scala
import chisel3._

class SendFetchData extends Module {
  val io = IO(new Bundle {
    // Inputs
    val Inflightqueue_empty = Input(Bool())
    val busy = Input(Bool())
    val Killed = Input(Bool())
    val Address = Input(UInt(32.W))
    val Excp_vaild = Input(Bool())
    val Excp_Cause = Input(UInt(6.W))
    val Excp_Tval = Input(UInt(32.W))
    val InsnByte = Input(UInt(64.W))

    // Outputs
    val pop_flag = Output(Bool())
    val Out_Address = Output(UInt(32.W))
    val Out_Excp_vaild = Output(Bool())
    val Out_Excp_Cause = Output(UInt(6.W))
    val Out_Excp_Tval = Output(UInt(32.W))
    val Out_InsnByte = Output(UInt(64.W))
  })

  // Initial assignment to outputs
  io.pop_flag := false.B
  io.Out_Address := 0.U
  io.Out_Excp_vaild := false.B
  io.Out_Excp_Cause := 0.U
  io.Out_Excp_Tval := 0.U
  io.Out_InsnByte := 0.U

  // Logic for pop_flag and outputs based on the input conditions
  when(!io.Inflightqueue_empty && !io.busy) {
    when(!io.Killed) {
      io.Out_Address := io.Address
      io.Out_InsnByte := io.InsnByte
      io.Out_Excp_vaild := io.Excp_vaild
      io.Out_Excp_Cause := io.Excp_Cause
      io.Out_Excp_Tval := io.Excp_Tval
    }
    io.pop_flag := true.B
  } .otherwise {
    // If none of the conditions are met, all output ports are set to 0/default.
    io.pop_flag := false.B
    io.Out_Address := 0.U
    io.Out_Excp_vaild := false.B
    io.Out_Excp_Cause := 0.U
    io.Out_Excp_Tval := 0.U
    io.Out_InsnByte := 0.U
  }
}

```

This Chisel code defines a module named `SendFetchData` with inputs and outputs that mirror those described in the XML. The logic within the module reflects the specified behaviors, using Chisel's `when` construct to implement conditional logic.