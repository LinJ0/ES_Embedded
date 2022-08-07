05 - Startup Code ─ Data Initialization
===

In this lecture, we are going to use global variables in our program.

You may think that sounds easy, but there is an initialization procedure before global variables are able to work.


**<p style="color:crimson"><i class="fa fa-github"></i> You can clone the LAB05 repo with:</p>**

```shell
git clone https://github.com/ybcsie/ESEmbedded_LAB05.git
```


----------------------------


## Blink with Global ─ NG

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The files are in `ESEmbedded_LAB05/global_var_ng`</p>**

First of all, let's directly add a global variable to [Exercise] in lecture 04.

:::warning

Will it work?

:::

You can flash it to your device and think about why.


#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.c


```clike=
#include <stdint.h>
#include "reg.h"

/**
 * 
 * LED init
 * 
 */
void led_init(unsigned int led)
{
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTD));

	//MODER led pin = 01 => General purpose output mode
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_1_BIT(led));
	SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_0_BIT(led));

	//OT led pin = 0 => Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OTYPER_OFFSET, OTy_BIT(led));

	//OSPEEDR led pin = 00 => Low speed
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(led));

	//PUPDR led pin = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(led));
}

/**
 * 
 * blink LED forever
 * 
 */
unsigned int global_i = 1;

void blink(unsigned int led)
{
	led_init(led);

	while (1)
	{
		//set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(led));

		for (global_i = 0; global_i < 100000; global_i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));

		for (global_i = 0; global_i < 100000; global_i++)
			;
	}
}

/**
 * 
 * blink LED x count
 * 
 */
void blink_count(unsigned int led, unsigned int count)
{
	led_init(led);

	unsigned int i;

	while (count--)
	{
		//set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(led));

		for (i = 0; i < 100000; i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));

		for (i = 0; i < 100000; i++)
			;
	}
}

```

### Build

1. Under the path of **`ESEmbedded_LAB05/global_var_ng`**, generate the .bin file with:

    ```shell
    make
    ```

1. **Connect your STM32F4DISCOVERY board**, and flash by:

    ```shell
    make flash
    ```




------------------------------------




## Why Is the Data Initialization Necessary?

By using objdump:

```shell
arm-none-eabi-objdump -D blink.elf
```

you can see:

```shell
blink.elf:     file format elf32-littlearm


Disassembly of section .mytext:

00000000 <main-0x8>:
   0:   20000100        andcs   r0, r0, r0, lsl #2
   4:   00000009        andeq   r0, r0, r9

00000008 <main>:


．．．．．．


Disassembly of section .data:

000001f4 <global_i>:
 1f4:   00000001        andeq   r0, r0, r1


．．．．．．

```

:::warning

Notice the address of your global variable.

The whole data section is located in the flash region which is unwritable.

:::

So now you will have to figure out how to put the data section in the SRAM region.



--------------------------



## Data Initialization Sequence

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB05/global_var_ng2`</p>**

To put the data section in the SRAM region, you may come up with modifying the linker script like this:


```=
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
	} > FLASH

	.mydata :
	{
		*(.data)
	} > SRAM

}
```

:::danger

Unfortunately, not that easy.

:::

This will finally generate a .bin file like below.

**Before**

![](http://125.227.181.237/uploads/upload_91a386c0a1fa51a11f20ba3b71a02202.png)


**After**

![](http://125.227.181.237/uploads/upload_005a91550ee1ca6b35f943e658d3289f.png)


**Before**

![](http://125.227.181.237/uploads/upload_b0f67d34e3dfddab3efcfbae468d62fe.png)

**After**

![](http://125.227.181.237/uploads/upload_bd37089a30bf5186650f3a68336b339a.png)



The solution will be described with the following steps.

1. The data section have to be loaded into the flash region.

2. We have to manually copy it from flash to the SRAM at the beginning of the startup code.

3. The program will read and write the data by referencing with the SRAM address.

![](http://125.227.181.237/uploads/upload_a3df85be117ab93d5d473f7a271e3102.png)




---------------------------




## VMA vs. LMA
:::success

If we are telling the linker that a section will be **referenced** by the address in a specific memory region, that means we are asking the linker to set the **VMA** (virtual memory address) of the section.

:::

:::success

If we are telling the linker that a section will be **loaded** into a specific memory region, that means we are asking the linker to set the **LMA** (load memory address) of the section.

:::

:::info

If the LMA is not manually defined, the linker will set the LMA default to the VMA.

:::

For example, the following linker script can tell the linker:

1. The VMA of .mytext section will be in the flash region.

2. The VMA of .mydata section will be in the SRAM region.

3. The LMA is not mentioned, so LMA = VMA.

```=
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
	} > FLASH

	.mydata :
	{
		*(.data)
	} > SRAM

}
```

If you generate the `blink.elf` with the linker script above and use objdump with

```shell
arm-none-eabi-objdump -h -D blink.elf
```

you will get:

```shell
blink.elf:     file format elf32-littlearm

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .mytext       000001f4  00000000  00000000  00010000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .mydata       00000004  20000000  20000000  00020000  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .comment      00000038  00000000  00000000  00020004  2**0
                  CONTENTS, READONLY
  3 .ARM.attributes 00000031  00000000  00000000  0002003c  2**0
                  CONTENTS, READONLY

Disassembly of section .mytext:

00000000 <main-0x8>:
   0:   20000100        andcs   r0, r0, r0, lsl #2
   4:   00000009        andeq   r0, r0, r9

00000008 <main>:


．．．．．．


Disassembly of section .mydata:

20000000 <global_i>:
20000000:       00000001        andeq   r0, r0, r1


．．．．．．

```

:::warning

Notice the VMA and LMA of .mydata section.

:::

To make .mydata section be loaded into the flash region, we can modify the linker script as below.

```=
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
        _mydata_lma_start = .;
	} > FLASH

	.mydata : AT(_mydata_lma_start)
	{
		*(.data)
	} > SRAM

}
```

Now generate the `blink.elf` with the linker script above and use objdump with

```shell
arm-none-eabi-objdump -h -D blink.elf
```

you will get:

```shell
blink.elf:     file format elf32-littlearm

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .mytext       000001f4  00000000  00000000  00010000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .mydata       00000004  20000000  000001f4  00020000  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .comment      00000038  00000000  00000000  00020004  2**0
                  CONTENTS, READONLY
  3 .ARM.attributes 00000031  00000000  00000000  0002003c  2**0
                  CONTENTS, READONLY

Disassembly of section .mytext:

00000000 <main-0x8>:
   0:   20000100        andcs   r0, r0, r0, lsl #2
   4:   00000009        andeq   r0, r0, r9

00000008 <main>:


．．．．．．


Disassembly of section .mydata:

20000000 <global_i>:
20000000:       00000001        andeq   r0, r0, r1


．．．．．．


```

:::warning

Notice the VMA and LMA of .mydata section.

:::

The LMA of .mydata is now in the flash region with the VMA in the SRAM region.

Lastly, we have to copy data from LMA to VMA, but how does the C code get the information of the final LMA and VMA?

By using nm, you will find that the `_mydata_lma_start` in the linker script above is a **symbol**.

![](http://125.227.181.237/uploads/upload_5316badad31395650cc3785b3827dacd.png)

That means in the linker script, you can define several symbols to indicate the addresses the copy procedure requires.



-------------------


## LAB05-1 Variables and Symbols

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB05/1_symbols`</p>**

For understanding the relationship between variables and symbols, run the sample code below.


### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> symbols.c

```clike=
#include <stdint.h>

uint32_t global_var = 0x1234;

extern uint32_t _mydata_lma_start;
extern uint32_t _mydata_vma_start;

void symbols(void)
{
	uint32_t *sram_debug = (uint32_t *)0x20000200;

	sram_debug[0] = 0xAAAA;

	sram_debug[1] = global_var;
	sram_debug[2] = (uint32_t)&global_var;

	sram_debug[3] = _mydata_lma_start;
	sram_debug[4] = (uint32_t)&_mydata_lma_start;

	sram_debug[5] = _mydata_vma_start;
	sram_debug[6] = (uint32_t)&_mydata_vma_start;

	while (1)
		;
}
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

```=
.section .vector_table

.word 0x20000100
.word symbols
```



### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> stm32f4.ld

```=
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
		_mydata_lma_start = .;
	} > FLASH

	.mydata : AT(_mydata_lma_start)
	{
		_mydata_vma_start = .;
		*(.data)
	} > SRAM

}
```


### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile


```makefile=
CROSS-COMPILER = arm-none-eabi-

all: symbols.bin

symbols.bin: symbols.c vector_table.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld symbols.c vector_table.s -o symbols.elf
	$(CROSS-COMPILER)objcopy -O binary symbols.elf symbols.bin

flash:
	st-flash --reset write symbols.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
```

### Build

1. Generate the .bin file with

    ```shell
    make
    ```

1. Connect your STM32F4DISCOVERY board, and flash by

    ```shell
    make flash
    ```

1. Use nm to display the symbols table:

    ```shell
    arm-none-eabi-nm symbols.elf
    ```

#### Symbols table of `symbols.elf`:

![](http://125.227.181.237/uploads/upload_f72ce870ab1b729b4e2d9bf838f747a8.png)


### Varify with GDB

1.  Start the ST-Link GDB server:

    ```shell
    st-util -p 1234
    ```

1. Open a new terminal window, and start GDB:

    ````shell
    arm-none-eabi-gdb
    ````

1. Connet to stlink gdb server:

    ````shell
    target remote localhost:1234
    ````

1. Dump memory:

    ````shell
    x 0x20000200
    ````

1. Keep pressing ENTER to dump more.


#### GDB results:

![](http://125.227.181.237/uploads/upload_ff74e698cb82ef8790a12e8cd2025f5a.png)





--------------------------------------



## LAB05-2 Blink with a Global Variable

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB05/2_global_var`</p>**

Data initialization tasks for a startup code:

1. Copy the data section from LMA to VMA.

2. Set the initial values of the bss section to zeros at VMA.

3. The rodata section is read only, so we can just put it in flash.

![](http://125.227.181.237/uploads/upload_de47c6312eed75151b8f1ffdd93627fc.png)


### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> reg.h

~

#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.h

```clike=
#ifndef BLINK_H
#define BLINK_H

#define LED_GREEN 12
#define LED_ORANGE 13
#define LED_RED 14
#define LED_BLUE 15

void blink(unsigned int led);
void blink_count(unsigned int led, unsigned int count);

#endif
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.c

```clike=
#include <stdint.h>
#include "reg.h"

/**
 * 
 * LED init
 * 
 */
void led_init(unsigned int led)
{
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTD));

	//MODER led pin = 01 => General purpose output mode
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_1_BIT(led));
	SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_0_BIT(led));

	//OT led pin = 0 => Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OTYPER_OFFSET, OTy_BIT(led));

	//OSPEEDR led pin = 00 => Low speed
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(led));

	//PUPDR led pin = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(led));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(led));
}

/**
 * 
 * blink LED forever
 * 
 */
unsigned int global_i = 1;
unsigned int global_j = 0x1234;
unsigned int global_k = 0x2234;

unsigned int global_arr[10];

void blink(unsigned int led)
{
	led_init(led);

	while (1)
	{
		//set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(led));

		for (global_i = 0; global_i < 100000; global_i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));

		for (global_i = 0; global_i < 100000; global_i++)
			;
	}
}

/**
 * 
 * blink LED x count
 * 
 */
void blink_count(unsigned int led, unsigned int count)
{
	led_init(led);

	unsigned int i;

	while (count--)
	{
		//set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(led));

		for (i = 0; i < 100000; i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));

		for (i = 0; i < 100000; i++)
			;
	}
}
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> main.c

```clike=
#include "blink.h"

int main(void)
{
	blink_count(LED_GREEN, 10);
	blink_count(LED_ORANGE, 10);
	blink_count(LED_RED, 10);
	blink(LED_BLUE);
}
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> startup.c

```clike=
#include <stdint.h>

extern int main(void);

void reset_handler(void)
{
	//symbols defined in linker script
	extern uint32_t _mydata_lma_start;
	extern uint32_t _mydata_vma_start;
	extern uint32_t _mydata_vma_end;
	extern uint32_t _mybss_vma_start;
	extern uint32_t _mybss_vma_end;
	//note that for the variables above, "symbol value" is equivalent to the address we want
	//use "&" operator to get symbol values

	uint32_t *mydata_lstart_ptr = &_mydata_lma_start;
	uint32_t *mydata_vstart_ptr = &_mydata_vma_start;
	uint32_t *mydata_vend_ptr = &_mydata_vma_end;

	uint32_t *mybss_vstart_ptr = &_mybss_vma_start;
	uint32_t *mybss_vend_ptr = &_mybss_vma_end;

	uint32_t *src_ptr, *dst_ptr;

	src_ptr = mydata_lstart_ptr;
	dst_ptr = mydata_vstart_ptr;

	while (dst_ptr < mydata_vend_ptr)
		*dst_ptr++ = *src_ptr++;

	dst_ptr = mybss_vstart_ptr;

	while (dst_ptr < mybss_vend_ptr)
		*dst_ptr++ = 0;

	main();
}
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

```=
.section .vector_table

.word _msp_init
.word reset_handler
```

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> stm32f4.ld


```=
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
		*(.rodata)
		_mydata_lma_start = .; /* LMA is equivalent to VMA in this section */
	} > FLASH

	.mydata : AT(_mydata_lma_start) /* tell linker whrere (LMA) this section should be put at */
	{
		_mydata_vma_start = .;
		*(.data)
		_mydata_vma_end = .;
	} > SRAM

	.mybss :
	{
		_mybss_vma_start = .;
		*(.bss)
		*(COMMON)
		_mybss_vma_end = .;
	} > SRAM
	
	_msp_init = ORIGIN(SRAM) + LENGTH(SRAM);
}
```

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile


```makefile=
CROSS-COMPILER = arm-none-eabi-

all: blink.bin

blink.bin: main.c blink.c startup.c vector_table.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld main.c blink.c startup.c vector_table.s -o blink.elf
	$(CROSS-COMPILER)objcopy -O binary blink.elf blink.bin

flash:
	st-flash --reset write blink.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
```

### Build


1. Under the path of **`ESEmbedded_LAB05/2_global_var`**, generate the .bin file with:

    ```shell
    make
    ```

1. **Connect your STM32F4DISCOVERY board**, and flash by:

    ```shell
    make flash
    ```

1. Use objdump with

    ```shell
    arm-none-eabi-objdump -h -D blink.elf
    ```


#### Disassembly of `blink.elf`:

```shell
blink.elf:     file format elf32-littlearm

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .mytext       00000268  00000000  00000000  00010000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .mydata       0000000c  20000000  00000268  00020000  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .mybss        00000028  2000000c  00000274  0002000c  2**2
                  ALLOC
  3 .comment      00000038  00000000  00000000  0002000c  2**0
                  CONTENTS, READONLY
  4 .ARM.attributes 00000031  00000000  00000000  00020044  2**0
                  CONTENTS, READONLY

Disassembly of section .mytext:

00000000 <main-0x8>:
   0:   20020000        andcs   r0, r2, r0
   4:   000001f5        strdeq  r0, [r0], -r5

00000008 <main>:


．．．．．．


Disassembly of section .mydata:

20000000 <_mydata_vma_start>:
20000000:       00000001        andeq   r0, r0, r1

20000004 <global_j>:
20000004:       00001234        andeq   r1, r0, r4, lsr r2

20000008 <global_k>:
20000008:       00002234        andeq   r2, r0, r4, lsr r2

Disassembly of section .mybss:

2000000c <_mybss_vma_start>:
        ...


．．．．．．

````
    
### Varify with GDB

1.  Start the ST-Link GDB server:

    ```shell
    st-util -p 1234
    ```

1. Open a new terminal window, and start GDB:

    ````shell
    arm-none-eabi-gdb
    ````

1. Connet to stlink gdb server:

    ````shell
    target remote localhost:1234
    ````

1. Dump memory:

    ````shell
    x 0x20000000
    ````

1. Keep pressing ENTER to dump more.

    ![](http://125.227.181.237/uploads/upload_a0e5ae954bac477ec35522cabcf2f73f.png)




[Exercise]:http://www.nc.es.ncku.edu.tw/course/embedded/04/#Exercise



-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`