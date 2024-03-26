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
Decode_Queue
A controllable double-in double-out FIFO, containing 16 entries, each entry structure is as follows:
[4:0] IsaRs1 //address of rs1 register
Operand1Ready //whether the operand of rs1 is ready
[4:0] IsaRs2//address of rs2 register
Operand2Ready//whether the operand of rs2 is ready
[4:0] IsaRd//address of rd register
ControlFlowInsn//whether it is a control flow instruction, used to indicate whether the instruction will change the execution flow of the program
[2:0] Function_type//the execution unit that the instruction needs to be called, including ALU, BRU, CSR, LDU, STU
[3:0] Sub_OP//the specific operation that the instruction needs to perform in the execution unit
[31:0] imm//storage immediately number
excp_valid//storage the exception of the current instruction
[5:0] excp_cause//storage the type of the current exception instruction
[31:0] excpt_tval//storage the address of the current exception instruction
# Function：
1. Reset according to reset signal
2. It can read and write all the data in the entry
3. It has two input data ports decoded_insn1 and decoded_insn2 and two write enable signals wr_en1 and wr_en2, which can write one data or two data in sequence according to the value of write enable signal in one cycle.
4. Can read a specified number of entries out of the FIFO and pop them according to the pop_count signal
5. It can record the current head pointer and tail pointer and update them
6. Output whether the current queue is full or empty
7. Be able to keep track of how many valid entries were actually output
# Parameter：

# Input:
clk：A clock signal.
reset：Reset signal, used to initialize or reset the FIFO state.
[95:0]decoded_insn1：The first input data port, used to write data for the first instruction.
[95:0]decoded_insn2：The second input data port is used to write the data of the second instruction.
wr_en1：The first write enable signal controls whether decoded_insn1 is written to the FIFO.
wr_en2：The second write enable signal controls whether decoded_insn2 is written to the FIFO.
[1:0]pop_count：Specifies the number of entries to be read from and ejected from the FIFO.


# Output:
[95:0]entry_out1：Used to output data in FIFO.
[95:0]entry_out2：Used to output data in FIFO.
fifo_full：Indicates whether the FIFO is full.
fifo_empty：Indicates whether the FIFO is empty.
[3:0]head_ptr：The current header pointer.
[3:0]tail_ptr：The current tail pointer.
[1:0]out_count:How many valid entries were actually output



# reference model

--------------------

**assistant**:
```xml
<Module type="Decode_Queue">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="clk" type="signal" description="A clock signal."/>
	        <Input name="reset" type="signal" description="Reset signal, used to initialize or reset the FIFO state."/>
	        <Input name="decoded_insn1" type="[95:0]" description="The first input data port, used to write data for the first instruction."/>
	        <Input name="decoded_insn2" type="[95:0]" description="The second input data port, used to write the data of the second instruction."/>
	        <Input name="wr_en1" type="signal" description="The first write enable signal controls whether decoded_insn1 is written to the FIFO."/>
	        <Input name="wr_en2" type="signal" description="The second write enable signal controls whether decoded_insn2 is written to the FIFO."/>
	        <Input name="pop_count" type="[1:0]" description="Specifies the number of entries to be read from and ejected from the FIFO."/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="entry_out1" type="[95:0]" description="Used to output data in FIFO."/>
	        <Output name="entry_out2" type="[95:0]" description="Used to output data in FIFO."/>
	        <Output name="fifo_full" type="signal" description="Indicates whether the FIFO is full."/>
	        <Output name="fifo_empty" type="signal" description="Indicates whether the FIFO is empty."/>
	        <Output name="head_ptr" type="[3:0]" description="The current header pointer."/>
	        <Output name="tail_ptr" type="[3:0]" description="The current tail pointer."/>
	        <Output name="out_count" type="[1:0]" description="How many valid entries were actually output"/>
	</Interface>
	<Function description="1. Reset according to reset signal 2. It can read and write all the data in the entry 3. It has two input data ports decoded_insn1 and decoded_insn2 and two write enable signals wr_en1 and wr_en2, which can write one data or two data in sequence according to the value of write enable signal in one cycle. 4. Can read a specified number of entries out of the FIFO and pop them according to the pop_count signal 5. It can record the current head pointer and tail pointer and update them 6. Output whether the current queue is full or empty 7. Be able to keep track of how many valid entries were actually output"/>
<ReferenceModel code=""/>    
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
<Module type="Decode_Queue">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="clk" type="signal" description="A clock signal."/>
	        <Input name="reset" type="signal" description="Reset signal, used to initialize or reset the FIFO state."/>
	        <Input name="decoded_insn1" type="[95:0]" description="The first input data port, used to write data for the first instruction."/>
	        <Input name="decoded_insn2" type="[95:0]" description="The second input data port, used to write the data of the second instruction."/>
	        <Input name="wr_en1" type="signal" description="The first write enable signal controls whether decoded_insn1 is written to the FIFO."/>
	        <Input name="wr_en2" type="signal" description="The second write enable signal controls whether decoded_insn2 is written to the FIFO."/>
	        <Input name="pop_count" type="[1:0]" description="Specifies the number of entries to be read from and ejected from the FIFO."/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="entry_out1" type="[95:0]" description="Used to output data in FIFO."/>
	        <Output name="entry_out2" type="[95:0]" description="Used to output data in FIFO."/>
	        <Output name="fifo_full" type="signal" description="Indicates whether the FIFO is full."/>
	        <Output name="fifo_empty" type="signal" description="Indicates whether the FIFO is empty."/>
	        <Output name="head_ptr" type="[3:0]" description="The current header pointer."/>
	        <Output name="tail_ptr" type="[3:0]" description="The current tail pointer."/>
	        <Output name="out_count" type="[1:0]" description="How many valid entries were actually output"/>
	</Interface>
	<Function description="1. Reset according to reset signal 2. It can read and write all the data in the entry 3. It has two input data ports decoded_insn1 and decoded_insn2 and two write enable signals wr_en1 and wr_en2, which can write one data or two data in sequence according to the value of write enable signal in one cycle. 4. Can read a specified number of entries out of the FIFO and pop them according to the pop_count signal 5. It can record the current head pointer and tail pointer and update them 6. Output whether the current queue is full or empty 7. Be able to keep track of how many valid entries were actually output"/>
<ReferenceModel code=""/>    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To design the `Decode_Queue` module in Chisel, we need to follow these steps:

### Reasoning Steps:
1. **Define the Module Interface**: Start by defining the module and its IOs based on the provided XML interface description. This includes inputs for clock, reset, two decoded instructions, write enable signals, and pop count. Outputs will include two instruction entries, FIFO full and empty indicators, head and tail pointers, and output count.

2. **FIFO Implementation**: Implement the FIFO logic that handles the enqueue and dequeue operations. The FIFO has to support double-in double-out operations based on the write enable signals and pop_count.

3. **Pointers and Counters**: Implement logic for head and tail pointers and a counter to keep track of the number of valid entries. This will help manage the FIFO's state, indicating whether it is full, empty, and the number of valid outputs.

4. **Reset Logic**: Implement the logic to reset the FIFO state, including the head and tail pointers and the content of the FIFO, based on the reset signal.

5. **Data Handling**: Implement the logic to handle data writing into the FIFO based on the write enable signals and data reading from the FIFO based on the pop_count signal.

6. **Output Logic**: Implement logic to update the outputs, including the FIFO entries to be output, the FIFO full and empty status, and the output count.

### Chisel Code:

```scala
import chisel3._
import chisel3.util._

class Decode_Queue extends Module {
  val io = IO(new Bundle {
    val clk = Input(Clock())
    val reset = Input(Bool())
    val decoded_insn1 = Input(UInt(96.W))
    val decoded_insn2 = Input(UInt(96.W))
    val wr_en1 = Input(Bool())
    val wr_en2 = Input(Bool())
    val pop_count = Input(UInt(2.W))
    val entry_out1 = Output(UInt(96.W))
    val entry_out2 = Output(UInt(96.W))
    val fifo_full = Output(Bool())
    val fifo_empty = Output(Bool())
    val head_ptr = Output(UInt(4.W))
    val tail_ptr = Output(UInt(4.W))
    val out_count = Output(UInt(2.W))
  })

  // Assuming a FIFO depth of 16 entries
  val fifoDepth = 16
  val fifo = RegInit(VecInit(Seq.fill(fifoDepth)(0.U(96.W))))
  val head = RegInit(0.U(4.W))
  val tail = RegInit(0.U(4.W))
  val count = RegInit(0.U(5.W)) // 5 bits to represent 0-16

  // Reset logic
  when(io.reset) {
    head := 0.U
    tail := 0.U
    count := 0.U
  }

  // Write logic
  when(io.wr_en1 && !io.fifo_full) {
    fifo(tail) := io.decoded_insn1
    tail := tail + 1.U
    count := count + 1.U
  }
  when(io.wr_en2 && !io.fifo_full) {
    fifo(tail) := io.decoded_insn2
    tail := tail + 1.U
    count := count + 1.U
  }

  // Read logic
  io.entry_out1 := 0.U
  io.entry_out2 := 0.U
  when(io.pop_count >= 1.U && !io.fifo_empty) {
    io.entry_out1 := fifo(head)
    head := head + 1.U
    count := count - 1.U
  }
  when(io.pop_count === 2.U && !io.fifo_empty) {
    io.entry_out2 := fifo(head)
    head := head + 1.U
    count := count - 1.U
  }

  // Status signals
  io.fifo_full := count === fifoDepth.U
  io.fifo_empty := count === 0.U
  io.head_ptr := head
  io.tail_ptr := tail
  io.out_count := Mux(io.pop_count > count, count, io.pop_count)

  // Clock not directly used since Chisel handles it
}

// Generate the Verilog
println((new chisel3.stage.ChiselStage).emitVerilog(new Decode_Queue))
```

This Chisel code defines a `Decode_Queue` module with the specified inputs and outputs, implementing a basic FIFO logic that supports double-in double-out functionality, reset behavior, and dynamically updates the FIFO's status indicators. Note that the actual depth and behavior might need adjustments based on more detailed specifications or requirements not covered in the initial description.