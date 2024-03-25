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
InflighQueue
A FIFO, containing 8 entries, each entry structure is as follows:
 busy               //indicates whether the entry has received the cached data, 0 indicates received, 1 indicates not received
 Killed             //indicates whether the entry is valid, 0 indicates valid, 1 indicates invalid
 [31:0]address      //the starting address of each packet in memory
 [63:0]insnByte     //the data taken out of memory, double-emitting so the data width is 64 bits
 excp_valid         //stores whether an exception occurs during the fetch process, 1 indicates an exception
 [5:0]excp_cause    //stores the type of the exception
 [31:0]excpt_tval   //stores the address where the exception occurs
# Function：
1. Reset according to the reset signal
2. Able to read and write all data in the entry
3. Able to pop the entry according to the pop signal
4. Able to record the current header and tail pointers and update them
5. Output whether the current queue is full or empty
# Parameter：

# Input:
clk (input): The clock signal to synchronize the FIFO's operations.
reset (input): A signal to reset the FIFO, initializing the head and tail pointers and clearing entries.
pop (input): A signal to indicate that an entry should be popped out from the FIFO.
push (input): A signal to indicate that a new entry is being added to the FIFO.
write_data (input [94:0]): The data to be written into an entry, encapsulating all the entry structure fields as a single vector (1 bit for busy, 1 bit for killed, 32 bits for address, 64 bits for insnByte, 1 bit for excp_vaild, 6 bits for excp_cause, and 32 bits for excpt_tval).


# Output:
full (output): Indicates the FIFO is full and cannot accept new entries.
empty (output): Indicates the FIFO is empty and there's nothing to pop.
read_data (output [94:0]): The data from the entry at the head of the FIFO, following the same structure as write_data.
head_ptr (output [2:0]): The current position of the head pointer. This might be used for debugging or specific control logic outside the FIFO.
tail_ptr (output [2:0]): The current position of the tail pointer, also potentially useful for debugging or control.

--------------------

**assistant**:
```xml
<Module type="InflighQueue">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="clk" type="signal" description="The clock signal to synchronize the FIFO's operations."/>
	        <Input name="reset" type="signal" description="A signal to reset the FIFO, initializing the head and tail pointers and clearing entries."/>
	        <Input name="pop" type="signal" description="A signal to indicate that an entry should be popped out from the FIFO."/>
	        <Input name="push" type="signal" description="A signal to indicate that a new entry is being added to the FIFO."/>
	        <Input name="write_data" type="vector[94:0]" description="The data to be written into an entry, encapsulating all the entry structure fields as a single vector."/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="full" type="signal" description="Indicates the FIFO is full and cannot accept new entries."/>
	        <Output name="empty" type="signal" description="Indicates the FIFO is empty and there's nothing to pop."/>
	        <Output name="read_data" type="vector[94:0]" description="The data from the entry at the head of the FIFO, following the same structure as write_data."/>
	        <Output name="head_ptr" type="vector[2:0]" description="The current position of the head pointer. This might be used for debugging or specific control logic outside the FIFO."/>
	        <Output name="tail_ptr" type="vector[2:0]" description="The current position of the tail pointer, also potentially useful for debugging or control."/>
	</Interface>
	<Function description="1. Reset according to the reset signal. 2. Able to read and write all data in the entry. 3. Able to pop the entry according to the pop signal. 4. Able to record the current header and tail pointers and update them. 5. Output whether the current queue is full or empty"/>
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
<Module type="InflighQueue">
	<Parameters>
	</Parameters>
	<Interface>
	        <!-- Interfaces for each Functional Unit -->
	        <Input name="clk" type="signal" description="The clock signal to synchronize the FIFO's operations."/>
	        <Input name="reset" type="signal" description="A signal to reset the FIFO, initializing the head and tail pointers and clearing entries."/>
	        <Input name="pop" type="signal" description="A signal to indicate that an entry should be popped out from the FIFO."/>
	        <Input name="push" type="signal" description="A signal to indicate that a new entry is being added to the FIFO."/>
	        <Input name="write_data" type="vector[94:0]" description="The data to be written into an entry, encapsulating all the entry structure fields as a single vector."/>
	        <!-- Outputs from each Functional Unit  -->
	        <Output name="full" type="signal" description="Indicates the FIFO is full and cannot accept new entries."/>
	        <Output name="empty" type="signal" description="Indicates the FIFO is empty and there's nothing to pop."/>
	        <Output name="read_data" type="vector[94:0]" description="The data from the entry at the head of the FIFO, following the same structure as write_data."/>
	        <Output name="head_ptr" type="vector[2:0]" description="The current position of the head pointer. This might be used for debugging or specific control logic outside the FIFO."/>
	        <Output name="tail_ptr" type="vector[2:0]" description="The current position of the tail pointer, also potentially useful for debugging or control."/>
	</Interface>
	<Function description="1. Reset according to the reset signal. 2. Able to read and write all data in the entry. 3. Able to pop the entry according to the pop signal. 4. Able to record the current header and tail pointers and update them. 5. Output whether the current queue is full or empty"/>
<ReferenceModel code=""/>    
</Module>
<!-- Definitions for Request and Result types for each Functional Unit -->


--------------------

**assistant**:
To create the `InflighQueue` module in Chisel, we need to translate the specified inputs, outputs, and behaviors into Chisel code, considering the details of the FIFO's functionality. Here’s how we can approach this:

1. **Define the Module Interface:** 
   - The module will have inputs `clk`, `reset`, `pop`, `push`, and `write_data[94:0]`.
   - The outputs will be `full`, `empty`, `read_data[94:0]`, `head_ptr[2:0]`, and `tail_ptr[2:0]`.

2. **Implementing the FIFO Logic:**
   - The FIFO needs to manage a buffer with 8 entries, each 95 bits wide.
   - We must handle the read and write operations, track the head and tail pointers, and manage the full and empty states.

3. **Reset Logic:**
   - On reset, clear the FIFO contents and initialize pointers.

4. **Read and Write Operations:**
   - Implement logic to add entries to the tail and remove entries from the head.
   - Update pointers and status signals accordingly.

5. **Head and Tail Pointer Management:**
   - These pointers track the current read and write positions in the FIFO.

6. **Full and Empty Indicators:**
   - The FIFO is full when the head is one position behind the tail.
   - It is empty when the head and tail pointers are equal.

Here’s the Chisel code implementing `InflighQueue`:

```scala
import chisel3._
import chisel3.util._

class InflighQueue extends Module {
  val io = IO(new Bundle {
    val clk = Input(Clock())
    val reset = Input(Bool())
    val pop = Input(Bool())
    val push = Input(Bool())
    val write_data = Input(UInt(95.W))
    val full = Output(Bool())
    val empty = Output(Bool())
    val read_data = Output(UInt(95.W))
    val head_ptr = Output(UInt(3.W))
    val tail_ptr = Output(UInt(3.W))
  })

  val queue = Reg(Vec(8, UInt(95.W)))
  val head = RegInit(0.U(3.W))
  val tail = RegInit(0.U(3.W))

  // Reset logic
  when(io.reset) {
    head := 0.U
    tail := 0.U
  }

  // FIFO full and empty logic
  io.full := ((tail + 1.U) % 8.U) === head
  io.empty := head === tail

  // FIFO read and write logic
  when(io.push && !io.full) {
    queue(tail) := io.write_data
    tail := (tail + 1.U) % 8.U
  }

  when(io.pop && !io.empty) {
    head := (head + 1.U) % 8.U
  }

  // Output current data at head
  io.read_data := queue(head)
  io.head_ptr := head
  io.tail_ptr := tail
}

```
This Chisel code implements a FIFO queue with the specified functionalities and interfaces.