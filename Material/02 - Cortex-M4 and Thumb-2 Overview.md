02 - Cortex-M4 and Thumb-2 Overview
===

In this lecture, we are going to read the

* [*ARM® Cortex®-M4 Processor Technical Reference Manual*]

and the

* [*ARM Architecture Reference Manual Thumb-2 Supplement*]

Definitely, not everything. The text book, *The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors, Third Edition*, has simpler descriptions so don't forget to use it.

For more details, see the

* [*ARM®v7-M Architecture Reference Manual*]

**<p style="color:crimson"><i class="fa fa-github"></i> You can clone the LAB02 repo with:</p>**

```shell
git clone https://github.com/ybcsie/ESEmbedded_LAB02.git
```


---

## About the ARM® Cortex®-M4 Processor

1. The Cortex-M4 processor is a **low-power processor** that features:

    * low gate count

    * low interrupt latency

    * low-cost debug

1. The Cortex-M4 includes optional **floating point arithmetic functionality**.

1. The processor is intended for deeply **embedded applications** that require **fast interrupt response** features.

--------------------------------------------


## Processor Type

1. All the ARM Cortex-M processors are 32-bit RISC (Reduced Instruction Set Computing) processors. They have:

    * 32-bit registers

    * 32-bit internal data path

    * 32-bit bus interface

1. The Cortex-M3 and Cortex-M4 processors both have a **three-stage pipeline** design (instruction **fetch**, **decode**, and **execution**).

    ![](http://125.227.181.237/uploads/upload_f0db1ab8de1d0721d7086e1d12d4a178.png)

1. The Cortex-M3 and Cortex-M4 processors both have a **Harvard architecture**, which allows **simultaneous instruction fetches and data accesses**.

    ![architectures compare](http://125.227.181.237/uploads/upload_d30b2111bb9259f46add1d70c5ffbfd3.png)

1. The memory system of the ARM Cortex-M processors uses 32-bit addressing, which allows a **maximum 4GB address space**.

1. The memory space is used by the program code, data, peripherals, and some of the debug support components inside the processors.

1. Just like any other ARM processors, the Cortex-M processors are based on a **load-store architecture**. This means data needs to be loaded from the memory, processed, and then written back to memory using a number of separate instructions.

    For example, to increment a data value stored in SRAM, the processor needs to use:

    1. One instruction to read the data from SRAM and put it in a register inside the processor
    
    1. A second instruction to increment the value of the register
    
    1. A third instruction to write the value back to memory.


--------------------------------------------



## Features

The Cortex-M4 processor incorporates the following features:

* A processor core.

* A Nested Vectored Interrupt Controller (NVIC) closely integrated with the processor core to achieve low-latency interrupt processing.

* Multiple high-performance bus interfaces.

* A low-cost debug solution

* An optional Memory Protection Unit (MPU).

* An optional Floating Point Unit (FPU).

![Cortex-M4 block diagram](http://125.227.181.237/uploads/upload_475f505433fbf82c0eb5ecea08757293.png)



--------------------------------------------


## Modes of Operation and Execution

The Cortex-M4 processor supports **Thread and Handler operating modes**, and may be run in **Thumb or Debug operating states**. In addition, the processor can limit or exclude access to some resources by executing code in **privileged or unprivileged mode**.

![Operation states and modes](http://125.227.181.237/uploads/upload_d9fdf23ce8551fe4e6c27ac6894229c6.png)

1. The processor enters Thread mode on Reset, or as a result of an exception return.

1. The processor enters Handler mode as a result of an exception.

1. Handler mode is always privileged.

1. Thread mode can be privileged or unprivileged.



--------------------------------------------


## Memory System

1. The Cortex-M3 and M4 processors themselves do not include memories (i.e., they do not have program memory, SRAM, or cache). Instead, they come with a generic on-chip bus interface, so microcontroller vendors can add their own memory system to their design.

1. Typically, the microcontroller vendor will need to add:
    
    * Program memory, typically flash
    
    * Data memory, typically SRAM
    
    * Peripherals

### System Address Map

![System address map](http://125.227.181.237/uploads/upload_26aa26b4623ac3b802c2221551b1f4c1.png)

### Memory Regions Table

![Memory regions](http://125.227.181.237/uploads/upload_ca85dda2bf80d68c5f5ce09c3d49be74.png)


--------------------------------------------


## Bus Interfaces

1. The bus interfaces on the Cortex-M processors are 32-bit, and based on the Advanced Microcontroller Bus Architecture (AMBA) standard.

1. The Cortex-M4 processor contains:
    
    * Three external Advanced High-performance Bus (**AHB**)-Lite bus interfaces
        
        * ICode Memory Interface
        
        * DCode Memory Interface
        
        * System Interface
    
    * One Advanced Peripheral Bus (**APB**) interface
        
        * Private Peripheral Bus (PPB)

1. Multiple bus interface for different memory regions

    ![Multiple bus interface for different memory regions](http://125.227.181.237/uploads/upload_e5122cb33b853c83b354ba9784c12355.png)

### ICode Memory Interface

Instruction fetches from Code memory space, 0x00000000 to 0x1FFFFFFC, are performed over the 32-bit AHB-Lite bus.

### DCode Memory Interface

Data and debug accesses to Code memory space, 0x00000000 to 0x1FFFFFFF, are performed over the 32-bit AHB-Lite bus.

### System Interface

Instruction fetches and data and debug accesses to address ranges 0x20000000 to 0xDFFFFFFF and 0xE0100000 to 0xFFFFFFFF are performed over the 32-bit AHB-Lite bus.

### Private Peripheral Bus (PPB)

Data and debug accesses to external PPB space, 0xE0040000 to 0xE00FFFFF, are performed over the 32-bit Advanced Peripheral Bus (APB) bus. Vendor specific peripherals are on this bus.


--------------------------------------------


## Processor Core Register Summary

1. The processor has the following 32-bit registers:
    
    * 13 general-purpose registers, **R0-R12**.
    
    * Stack Pointer (**SP**), R13 alias of banked registers, SP_process and SP_main.
    
    * Link Register (**LR**), R14.
    
    * Program Counter (**PC**), R15.
    
    * Special-purpose Program Status Registers, (**xPSR**).

1. The general-purpose registers R0-R12 have no special architecturally-defined uses.

1. Registers R13, R14, and R15 have special functions.

![Processor register set](http://125.227.181.237/uploads/upload_7735c7a6565bef896d22350655368991.png)


### Low Registers

* Registers R0-R7 are accessible **by all instructions** that specify a general-purpose register.


### High Registers

* Registers R8-R12 are accessible **by all 32-bit instructions** that specify a general-purpose register.

* Registers R8-R12 are not accessible by most 16-bit instructions.



### Stack Pointer

* Register R13 is used as the Stack Pointer (SP).

* Because the SP ignores writes to bits [1:0], it is autoaligned to a word, four-byte boundary.

* Handler mode always uses SP_main, but you can configure Thread mode to use either SP_main or SP_process.


### Link Register

* Register R14 is the subroutine Link Register (LR).

* The LR receives the return address from PC when a Branch and Link (BL) or Branch and Link with Exchange (BLX) instruction is executed.

* The LR is also used for exception return.

* At all other times, you can treat R14 as a general-purpose register.


### Program Counter

* Register R15 is the Program Counter (PC).

* A read returns the current instruction address plus 4 (this is due to the pipeline nature of the design. Writing to PC causes a branch operation.

* Since the instructions must be aligned to half-word or word addresses, the Least Significant Bit (LSB) of the PC is zero.

* When using some of the branch instructions to update the PC, you need to set the LSB of the new PC value to 1 to indicate the Thumb state. Otherwise, a fault exception can be triggered. In high-level programming languages (including C, C++), the setting of LSB in branch targets is handled by the compiler automatically.

### Program Status Register

* The Program Status Register is composed of three status registers:

    * Application PSR (APSR)
    
    * Execution PSR (EPSR)
    
    * Interrupt PSR (IPSR)

    ![Program Status Registers](http://125.227.181.237/uploads/upload_eb3ac4f043b205843c5879132746f00a.png)

    ![APSR, IPSR, and EPSR](http://125.227.181.237/uploads/upload_9754d4907217a247644d0fd655e23c7e.png)

* These three registers can be accessed as one combined register, referred to as xPSR in some documentation.

    ![Combined xPSR](http://125.227.181.237/uploads/upload_4c5b5237df98a84f3a7a6f335a43bc3a.png)


* It is quite different from classic ARM processors such as the ARM7TDMI. If you compare this with the Current Program Status Register (CPSR) in ARM7, you might find that some of the bit fields used in ARM7 are gone.

    ![Bit Fields in Program Status Registers](http://125.227.181.237/uploads/upload_71909c35ee146f6e6f23311ddef9902d.png)


------------------------------------------------------------





## Reset and Reset Sequence

1. After reset and before the processor starts executing the program, the Cortex-M processors **read the first two words from the memory**:
    
    * The **initial value for the Main Stack Pointer (MSP)**
    
    * The reset vector (the starting **address** of the reset handler)

1. After these two words are read by the processor, the processor then sets up the MSP and the Program Counter (PC) with these values.

1. The stack operations in the Cortex-M3 or Cortex-M4 processors are based on **full descending stack (SP decrement before store)**.

1. Initial Stack Pointer value and Initial Program Counter value example:

    ![Initial Stack Pointer value and Initial Program Counter value example](http://125.227.181.237/uploads/upload_f5ac9d5832453fbdf9269feae1d2cdc4.png)

1. The Stack Pointer initialization behavior is different from classic ARM processors such as the ARM7TDMI, where upon reset the processor executes instructions from address 0x00000000, and the stack pointers must be initialized by software.

1. In classic ARM processors, the vector table holds instruction code rather than address values.


---------------------------------------------------------------



## LAB02-1 First Assembly Program

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB02/1_first_asm`</p>**

### Setup the Cross Compiling Environment in Ubuntu

1. Install cross compiler:

    ```shell
    sudo apt-get install gcc-arm-none-eabi
    ```

2. Check if it's work:

    ```shell
    arm-none-eabi-gcc -v
    ```

3. Install GNU Debuger for ARM:

    ```shell
    sudo apt-get install gdb-arm-none-eabi
    ```


### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> first_asm.s

```=
.syntax unified

.word 0x20000100
.word _start

.global _start
.type _start, %function
_start:
	b _start

```

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: first_asm.elf

first_asm.elf: first_asm.s
	$(CROSS-COMPILER)gcc -mcpu=cortex-m4 -nostartfiles -Ttext=0x0 first_asm.s -o first_asm.elf
	$(CROSS-COMPILER)objdump -D first_asm.elf

clean:
	rm -rf *.o *.elf
```

### Build

1. Under the path of **`ESEmbedded_LAB02/1_first_asm`**, build with:

    ```shell
    make
    ````

2. You will see the results from objdump.

#### The results from objdump:

```shell
arm-none-eabi-gcc -mcpu=cortex-m4 -nostartfiles -Ttext=0x0 first_asm.s -o first_asm.elf
arm-none-eabi-objdump -D first_asm.elf

first_asm.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <_start-0x8>:
   0:   20000100        andcs   r0, r0, r0, lsl #2
   4:   00000009        andeq   r0, r0, r9

00000008 <_start>:
   8:   f7ff bffe       b.w     8 <_start>

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:   00002041        andeq   r2, r0, r1, asr #32
   4:   61656100        cmnvs   r5, r0, lsl #2
   8:   01006962        tsteq   r0, r2, ror #18
   c:   00000016        andeq   r0, r0, r6, lsl r0
  10:   726f4305        rsbvc   r4, pc, #335544320      ; 0x14000000
  14:   2d786574        cfldr64cs       mvdx6, [r8, #-464]!     ; 0xfffffe30
  18:   0600344d        streq   r3, [r0], -sp, asr #8
  1c:   094d070d        stmdbeq sp, {r0, r2, r3, r8, r9, sl}^
  20:   Address 0x0000000000000020 is out of bounds.
```



----------------------------------------------------------------------------------------



## Lab02-2 Thumb-2 Instructions Practice

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB02/2_thumb2_practice`</p>**

* See the [*ARM® Cortex®-M4 Processor Technical Reference Manual*] - Section 3.3 Instruction set summary

* See the [*ARM Architecture Reference Manual Thumb-2 Supplement*] - Section 4.6 Alphabetical list of Thumb instructions

### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> thumb2_practice.s

```=
.syntax unified

.word 0x20000100
.word _start

.global _start
.type _start, %function
_start:
	//
	//mov # to reg
	//
	movs	r0,	#100
	movs	r1,	#0x11000000
	mov	r2,	#102
	movw	r3,	#103

	//
	//mov reg to reg
	//
	movs	r4,	r0
	mov	r5,	r1

	//
	//add
	//
	adds	r4,	r1,	#1
	add	r5,	r4,	#5

	//
	//push
	//
	push	{r0}
	push	{r1, r2, r3, r4}


	//
	//pop
	//
	pop	{r4, r5, r6, r7}
	pop	{r3}


	//
	//ldr
	//
	movs	r0,	#0x0
	ldr	r1,	[r0]
	ldr	r2,	[r0, #4]

	//
	//str
	//
	movs	r0,	#0x20000000
	str	r1,	[r0]
	str	r2,	[r0, #4]

	//
	//b bl
	//
	bl	label01

sleep:
	b	sleep

label01:
	nop
	nop
	bx	lr

```

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-
QEMU = ./gnu-mcu-eclipse-qemu/bin/qemu-system-gnuarmeclipse

all: thumb2_practice.bin

thumb2_practice.bin: thumb2_practice.s
	$(CROSS-COMPILER)gcc -Wall -mcpu=cortex-m4 -nostartfiles -Ttext=0x0 thumb2_practice.s -o thumb2_practice.elf
	$(CROSS-COMPILER)objcopy -O binary thumb2_practice.elf thumb2_practice.bin
	$(CROSS-COMPILER)objdump -D thumb2_practice.elf

qemu:
	@echo
	@echo "Press Ctrl+A and then press X to exit QEMU"
	@echo
	$(QEMU) -M STM32F4-Discovery -nographic -gdb tcp::1234 -S -kernel thumb2_practice.bin

clean:
	rm -rf *.o *.elf *.bin
```

### Build

1. Under the path of **`ESEmbedded_LAB02/2_thumb2_practice`**, build with:

    ```shell
    make
    ```

2. You will get the results from objdump.



#### The results from objdump:

```shell
arm-none-eabi-gcc -Wall -mcpu=cortex-m4 -nostartfiles -Ttext=0x0 thumb2_practice.s -o thumb2_practice.elf
arm-none-eabi-objcopy -O binary thumb2_practice.elf thumb2_practice.bin
arm-none-eabi-objdump -D thumb2_practice.elf

thumb2_practice.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <_start-0x8>:
   0:   20000100        andcs   r0, r0, r0, lsl #2
   4:   00000009        andeq   r0, r0, r9

00000008 <_start>:
   8:   2064            movs    r0, #100        ; 0x64
   a:   f05f 5188       movs.w  r1, #285212672  ; 0x11000000
   e:   f04f 0266       mov.w   r2, #102        ; 0x66
  12:   f240 0367       movw    r3, #103        ; 0x67
  16:   0004            movs    r4, r0
  18:   460d            mov     r5, r1
  1a:   1c4c            adds    r4, r1, #1
  1c:   f104 0505       add.w   r5, r4, #5
  20:   b401            push    {r0}
  22:   b41e            push    {r1, r2, r3, r4}
  24:   bcf0            pop     {r4, r5, r6, r7}
  26:   bc08            pop     {r3}
  28:   2000            movs    r0, #0
  2a:   6801            ldr     r1, [r0, #0]
  2c:   6842            ldr     r2, [r0, #4]
  2e:   f05f 5000       movs.w  r0, #536870912  ; 0x20000000
  32:   6001            str     r1, [r0, #0]
  34:   6042            str     r2, [r0, #4]
  36:   f000 f801       bl      3c <label01>

0000003a <sleep>:
  3a:   e7fe            b.n     3a <sleep>

0000003c <label01>:
  3c:   bf00            nop
  3e:   bf00            nop
  40:   4770            bx      lr

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:   00002041        andeq   r2, r0, r1, asr #32
   4:   61656100        cmnvs   r5, r0, lsl #2
   8:   01006962        tsteq   r0, r2, ror #18
   c:   00000016        andeq   r0, r0, r6, lsl r0
  10:   726f4305        rsbvc   r4, pc, #335544320      ; 0x14000000
  14:   2d786574        cfldr64cs       mvdx6, [r8, #-464]!     ; 0xfffffe30
  18:   0600344d        streq   r3, [r0], -sp, asr #8
  1c:   094d070d        stmdbeq sp, {r0, r2, r3, r8, r9, sl}^
  20:   Address 0x0000000000000020 is out of bounds.
```

### Emulation with QEMU

1. Download the qemu pack [**here**](http://www.nc.es.ncku.edu.tw/course/embedded/utils/gnu-mcu-eclipse-qemu.zip)

2. Extract `gnu-mcu-eclipse-qemu.zip` into **`ESEmbedded_LAB02/2_thumb2_practice`**

3. Under the path of **`ESEmbedded_LAB02/2_thumb2_practice`**, start emulation with:

    ```shell
    make qemu
    ```

4. Open another terminal window to start GNU Debugger:

    ```shell
    arm-none-eabi-gdb
    ```

5. Connect to qemu:

    ```shell
    target remote 127.0.0.1:1234
    ```

6. To display more information, type `info registers` or `layout regs` or **(press ctrl + x then press 2) x 2 times**

7. Using the following commands to observe the registers, memory values, and instructions.

    `x addr` to print memory addr

    `x` to print next

    `x/20 addr` to print memory addr ~ addr + 20

    `x/20 addr - 20` to print memory addr - 20 ~ addr

    `si` or `stepi` to exe one instruction
    

[*ARM® Cortex®-M4 Processor Technical Reference Manual*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/Cortex-M4.pdf
[*ARM Architecture Reference Manual Thumb-2 Supplement*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/Thumb2.pdf
[*ARM®v7-M Architecture Reference Manual*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/armv7m.pdf



-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`