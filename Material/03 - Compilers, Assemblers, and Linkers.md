03 - Compilers, Assemblers, and Linkers
===

We have learned how to write a simple program with assembly for ARM Cortex-M4 devices in lecture 02.

However, we don't really want to write everything in assembly.

In this lecture, we are going to program on ARM Cortex-M4 devices in C, and learn how the C code is built into the binary file.

**<p style="color:crimson"><i class="fa fa-github"></i> You can clone the LAB03 repo with:</p>**

```shell
git clone https://github.com/ybcsie/ESEmbedded_LAB03.git
```


------------------------------------------------------------------------------

## ARM Cortex-M4 Programming ─ in C

* In lecture 02 - [LAB02-1 First Assembly Program], we introduced a minimal program for Cortex-M4 devices:

    ```=
    .syntax unified

    .word 0x20000100
    .word _start

    .global _start
    .type _start, %function
    _start:
    	b _start

    ```

* To program in C, we can simply use a branch instruction inside our minimal program:

    ```=
    .syntax unified

    .word 0x20000100
    .word _start

    .global _start
    .type _start, %function
    _start:
    	b C_FUNCTION

    ```

* The `C_FUNCTION` is a C function that is implemented in another C file.

* The gcc toolchain contains a C compiler, assembler, linker, and additional utilities. It will combine the input files all together.


--------------------------------------------------------------------------------


## LAB03-1 First C Program

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB03/1_first_c`</p>**

### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> simple.c

```clike=
void mymain(void)
{
    while (1);
}
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> startup.s

```=
.syntax unified

.word 0x20000100
.word _start

.global _start
.type _start, %function
_start:
	b	mymain

```

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile_v1

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: simple.elf

simple.elf: simple.c startup.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -Ttext=0x0 simple.c startup.s -o simple.elf
	$(CROSS-COMPILER)objdump -D simple.elf

clean:
	rm -rf *.o *.elf
```

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile_v2

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: simple.elf

simple.elf: simple.c startup.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -Ttext=0x0 startup.s simple.c -o simple.elf
	$(CROSS-COMPILER)objdump -D simple.elf

clean:
	rm -rf *.o *.elf
```

:::warning

<br>

**Compare the two makefiles above carefully.**

<br>

**Will the difference affect the result?**

<br>

:::


### Build

1. Under the path of **`ESEmbedded_LAB03/1_first_c`**, copy the target makefile:

    ```shell
    cp makefile_v1 makefile
    ```
    
    or
    
    ```shell
    cp makefile_v2 makefile
    ```

2. And then build with:

    ```shell
    make
    ```

3. Before rebuild, remember to clean the target files:

    ```shell
    make clean
    ```

4. You will get the results from objdump after make.


#### Using the `makefile_v1`, you will get:

```shell
arm-none-eabi-objdump -D simple.elf

simple.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <mymain>:
   0:   b480            push    {r7}
   2:   af00            add     r7, sp, #0
   4:   e7fe            b.n     4 <mymain+0x4>
   6:   bf00            nop
   8:   20000100        andcs   r0, r0, r0, lsl #2
   c:   00000011        andeq   r0, r0, r1, lsl r0

00000010 <_start>:
  10:   f7ff bff6       b.w     0 <mymain>


......

```

* Notice the content at address 0x00000000.

    ( The content of `startup.s` should at the beginning. )





#### Using the `makefile_v2`, you will get:

```shell
arm-none-eabi-objdump -D simple.elf

simple.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <_start-0x8>:
   0:   20000100        andcs   r0, r0, r0, lsl #2
   4:   00000009        andeq   r0, r0, r9

00000008 <_start>:
   8:   f000 b800       b.w     c <mymain>

0000000c <mymain>:
   c:   b480            push    {r7}
   e:   af00            add     r7, sp, #0
  10:   e7fe            b.n     10 <mymain+0x4>
  12:   bf00            nop


......

```

* It seems to work.

* We are going to discuss the way to force the linker to put something at the location we want (independent on the sequence of the input files).

* Before this, let's talk about the behavior behind GCC.


---------------------------------------------------------------------------------------------------------




## C to Binary ─ Compilation Process

* Normally, the C program building process involves four stages:

    **1. Preprocessing**

    **2. Compilation**

    **3. Assembly**

    **4. Linking**

* At the end there should be a single executable file.

* The example for the simple C program in LAB03-1:

    ![](http://125.227.181.237/uploads/upload_c93c67445056c45257c5945ba1a08f4d.png)


---------------------------------------------------------------------------------------------------------



## Preprocessing

* Preprocessing is the first pass of any C compilation.

* It processes the lines that start with `#` (**include-files**, **conditional compilation instructions** and **macros**).

* To display the output of the preprocessor:

    ```shell
    arm-none-eabi-gcc -E simple.c
    ```

    or

    ```shell
    arm-none-eabi-cpp simple.c
    ```

#### You will get:

```shell
# 1 "simple.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "simple.c"
void mymain(void)
{
 while (1);
}
```




---------------------------------------------------------------------------------------------------------


## Compilation

* Compilation is the second pass.

* It takes the output of the preprocessor, and generates assembly source code.

* Generate the assembly code from C source files:

    (preprocessing + compilation)

    ```shell
    arm-none-eabi-gcc -S -std=c11 -mcpu=cortex-m4 -mthumb simple.c
    ```

* You will get `simple.s`, just take a look.

#### <i class="fa fa-file-code-o" style="color:gray"></i> simple.s

```=
        .syntax unified
        .cpu cortex-m4
        .fpu softvfp
        .eabi_attribute 20, 1
        .eabi_attribute 21, 1
        .eabi_attribute 23, 3
        .eabi_attribute 24, 1
        .eabi_attribute 25, 1
        .eabi_attribute 26, 1
        .eabi_attribute 30, 6
        .eabi_attribute 34, 1
        .eabi_attribute 18, 4
        .thumb
        .file   "simple.c"
        .text
        .align  2
        .global mymain
        .thumb
        .thumb_func
        .type   mymain, %function
mymain:
        @ args = 0, pretend = 0, frame = 0
        @ frame_needed = 1, uses_anonymous_args = 0
        @ link register save eliminated.
        push    {r7}
        add     r7, sp, #0
.L2:
        b       .L2
        .size   mymain, .-mymain
        .ident  "GCC: (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)"

```


---------------------------------------------------------------------------------------------------------




## Assembly

* Assembly is the third stage of compilation.

* It takes the assembly source code and produces an assembly listing with offsets.

* The assembler output is stored in an object file.

* Generate object files from assembly files:

    ```shell
    arm-none-eabi-gcc -c -mcpu=cortex-m4 simple.s startup.s
    ```

    or

    ```shell
    arm-none-eabi-as -mcpu=cortex-m4 simple.s -o simple.o
    arm-none-eabi-as -mcpu=cortex-m4 startup.s -o startup.o
    ```

* Or you can generate object files from assembly source files and C source files:

    (preprocessing + compilation + assembly)

    ```shell
    arm-none-eabi-gcc -c -mcpu=cortex-m4 simple.c startup.s
    ```

* Now you will get `simple.o` and `startup.o`


### Using Objdump

1. Use objdump to display the information of the object files:

    ```shell
    arm-none-eabi-objdump -D simple.o
    ```
    
    and
    
    ```shell
    arm-none-eabi-objdump -D startup.o
    ```

2. You will get the disassembly results of each file.

#### Disassembly of `simple.o`:

```shell
simple.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <mymain>:
   0:   b480            push    {r7}
   2:   af00            add     r7, sp, #0
   4:   e7fe            b.n     4 <mymain+0x4>
   6:   bf00            nop


......

```

#### Disassembly of `startup.o`:

```shell
startup.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <_start-0x8>:
   0:   20000100        andcs   r0, r0, r0, lsl #2
   4:   00000000        andeq   r0, r0, r0

00000008 <_start>:
   8:   f7ff bffe       b.w     0 <mymain>


......

```


### Using Nm

1. Use **nm** to display the **symbol table** of the object file:

    ```shell
    arm-none-eabi-nm startup.o
    ```

    and

    ```shell
    arm-none-eabi-nm simple.o
    ```

2. You will get the symbol tables of each object file.

#### The symbol tables of `startup.o`:

![](http://125.227.181.237/uploads/upload_d4735592ecd13134bcca24c7ead6ad7d.png)

#### The symbol tables of `simple.o`:

![](http://125.227.181.237/uploads/upload_172acfdeffb82d7c4562908d5dd52200.png)




---------------------------------------------------------------------------------------------------------



## Linking

* Linking is the final stage of compilation.

* A linker takes one or more object files or libraries as input and combines them to produce a single (usually executable) file (here we will only discuss static linking).

* While combining the object files together, the linker performs the following operations:

    **1. Symbol Resolution**

    **2. Relocation**

* Link object files into an executable file:

    ```shell
    arm-none-eabi-gcc -nostartfiles -Ttext=0x0 startup.o simple.o -o simple.elf
    ```

    or

    ```shell
    arm-none-eabi-ld -nostartfiles -Ttext=0x0 startup.o simple.o -o simple.elf
    ```

* Before linking:

    ![](http://125.227.181.237/uploads/upload_cda60da5851301a508ea17254be8a601.png)

* After linking:

    ```shell
    arm-none-eabi-objdump -D simple.elf
    ```

    ![](http://125.227.181.237/uploads/upload_bb79cd4bd6b9ca72bb4029a23c3ddde4.png)

    ```shell
    arm-none-eabi-nm simple.elf
    ```

    ![](http://125.227.181.237/uploads/upload_515b27703af8589874eeaa60f0567402.png)


### About the Symbol "_start"

:::warning

<br>

**Why do we have to name the entry function as `_start` in `startup.s`?**

<br>

:::

* You can try to modify it:

    ```=
    .syntax unified

    .word 0x20000100
    .word reset_handler

    .global reset_handler
    .type reset_handler, %function
    reset_handler:
        b	mymain

    ```

* And then `make`. You will get a warning message:

    ```shell
    arm-none-eabi-gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -Ttext=0x0 startup.s simple.c -o simple.elf
    /usr/lib/gcc/arm-none-eabi/4.9.3/../../../arm-none-eabi/bin/ld: warning: cannot find entry symbol _start; defaulting to 0000000000000000
    ```

:::warning

<br>

**Who defined the entry symbol as `_start`?**

<br>

:::

* Now in your terminal, type:

    ```shell
    arm-none-eabi-ld -verbose
    ```

* You will see the default **linker script**, which defines external symbols and controls the linker.

    ![](http://125.227.181.237/uploads/upload_622b41449e1d6e0524b10353deee6998.png)

* You will have to write your own linker script if you need to control the linker by yourself.



---------------------------------------------------------------------------------------------------



## LAB03-2 Simple Linker Script


**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB03/2_linker_script`</p>**

### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

```=
.section .vector_table

.word 0x20000100
.word reset_handler

```

#### <i class="fa fa-file-code-o" style="color:gray"></i> simple.c

```clike=
void reset_handler(void)
{
	while (1)
		;
}
```

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> m4.ld

```=
SECTIONS
{
	. = 0x0;
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
	}
}
```

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: simple.elf

simple.elf: simple.c vector_table.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T m4.ld simple.c vector_table.s -o simple.elf
	$(CROSS-COMPILER)objdump -D simple.elf

clean:
	rm -rf *.o *.elf
```


### Build

1. Under the path of **`ESEmbedded_LAB03/2_linker_script`**, build with:

    ```shell
    make
    ```

2. You will get the results from objdump.

#### The results from objdump:

![](http://125.227.181.237/uploads/upload_b2333e6b78c268f8198edd4775adf519.png)




---------------------------------------------------------------------




## LAB03-3 More About Sections

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB03/3_sections`</p>**


* The Executable and Linking Format used by GNU/Linux and other operating systems, defines a number of **sections** in an executable program.

* These sections are used to provide instruction to the binary file and allowing inspection.

* The partial list of the sections that we are interested in:

    1. **.text** section: For code (instructions)

    2. **.rodata** section: For read only data

    3. **.data** section: For initialized data

    4. **.bss** section: For uninitialized data


### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> sections.c

```clike=
int init_global = 0x1111;
int uninit_global;

const int global_const = 0x2222;

char init_global_rw_str[] = "AAA"; // ASCII 'A' : 0x41
char uninit_global_rw_str[3];
char *global_ro_str = "BBB"; // ASCII 'B' : 0x42

static int init_static_global = 0x3333;
static int uninit_static_global;

void mymain(void)
{
	int init_local = 0x4444;
	int uninit_local;

	const int local_const = 0x5555;

	char init_local_rw_str[] = "CCC"; // ASCII 'C' : 0x43
	char uninit_local_rw_str[3];
	char *local_ro_str = "DDD"; // ASCII 'D' : 0x44

	static int init_static_local = 0x6666;
	static int uninit_static_local;

	uninit_local_rw_str[0] = uninit_local_rw_str[1] = uninit_local_rw_str[2] = 'E'; // ASCII 'D' : 0x45

	while (1);
}

void func(void)
{
	static int init_static_local = 0x7777;
	static int uninit_static_local;
}
```

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> empty.ld

( empty file )


### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: sections.elf

sections.elf: sections.c
	$(CROSS-COMPILER)gcc -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T empty.ld sections.c -o sections.elf
	$(CROSS-COMPILER)objdump -D sections.elf

clean:
	rm -rf *.o *.elf
```


### Build

1. Under the path of **`ESEmbedded_LAB03/3_sections`**, build with:

    ```shell
    make
    ```

2. You will get the results from objdump.

#### The results from objdump:

```shell
arm-none-eabi-objdump -D sections.elf

sections.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <mymain>:
   0:   b480            push    {r7}
   2:   b087            sub     sp, #28
   4:   af00            add     r7, sp, #0
   6:   f244 4344       movw    r3, #17476      ; 0x4444
   a:   617b            str     r3, [r7, #20]
   c:   f245 5355       movw    r3, #21845      ; 0x5555
  10:   613b            str     r3, [r7, #16]
  12:   4b05            ldr     r3, [pc, #20]   ; (28 <mymain+0x28>)
  14:   60bb            str     r3, [r7, #8]
  16:   4b05            ldr     r3, [pc, #20]   ; (2c <mymain+0x2c>)
  18:   60fb            str     r3, [r7, #12]
  1a:   2345            movs    r3, #69 ; 0x45
  1c:   71bb            strb    r3, [r7, #6]
  1e:   79bb            ldrb    r3, [r7, #6]
  20:   717b            strb    r3, [r7, #5]
  22:   797b            ldrb    r3, [r7, #5]
  24:   713b            strb    r3, [r7, #4]
  26:   e7fe            b.n     26 <mymain+0x26>
  28:   00434343        subeq   r4, r3, r3, asr #6
  2c:   00000044        andeq   r0, r0, r4, asr #32

00000030 <func>:
  30:   b480            push    {r7}
  32:   af00            add     r7, sp, #0
  34:   46bd            mov     sp, r7
  36:   f85d 7b04       ldr.w   r7, [sp], #4
  3a:   4770            bx      lr

Disassembly of section .rodata:

0000003c <global_const>:
  3c:   00002222        andeq   r2, r0, r2, lsr #4
  40:   00424242        subeq   r4, r2, r2, asr #4
  44:   00444444        subeq   r4, r4, r4, asr #8

Disassembly of section .data:

00000048 <init_global>:
  48:   00001111        andeq   r1, r0, r1, lsl r1

0000004c <init_global_rw_str>:
  4c:   00414141        subeq   r4, r1, r1, asr #2

00000050 <global_ro_str>:
  50:   00000040        andeq   r0, r0, r0, asr #32

00000054 <init_static_global>:
  54:   00003333        andeq   r3, r0, r3, lsr r3

00000058 <init_static_local.3961>:
  58:   00006666        andeq   r6, r0, r6, ror #12

0000005c <init_static_local.3967>:
  5c:   00007777        andeq   r7, r0, r7, ror r7

Disassembly of section .bss:

00000060 <uninit_static_global>:
  60:   00000000        andeq   r0, r0, r0

00000064 <uninit_static_local.3962>:
  64:   00000000        andeq   r0, r0, r0

00000068 <uninit_static_local.3968>:
  68:   00000000        andeq   r0, r0, r0

0000006c <uninit_global_rw_str>:
  6c:   00000000        andeq   r0, r0, r0

00000070 <uninit_global>:
  70:   00000000        andeq   r0, r0, r0


......

```

---------------------------------------------------------------

## Further Related Reading:

1. 俞甲子、石凡、潘愛民, 《程式設計師的自我修養：連結、載入、程式庫》

2. [COMPILER, ASSEMBLER, LINKER AND LOADER: A BRIEF STORY](https://www.tenouk.com/ModuleW.html)


[LAB02-1 First Assembly Program]: http://www.nc.es.ncku.edu.tw/course/embedded/02/#LAB02-1-First-Assembly-Program

-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`