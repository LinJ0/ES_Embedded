06 - Startup Code ─ Clock
===

In this lecture, we are going to initialize clock settings of the STM32F407VG MCU in our startup code.

We will have to use an oscilloscope to make certain the configurations have been successfully set.


**<p style="color:crimson"><i class="fa fa-github"></i> You can clone the LAB06 repo with:</p>**

```shell
git clone https://github.com/ybcsie/ESEmbedded_LAB06.git
```

----------------------------

## LAB06-1 Output the Clock

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB06/1_clock_output`</p>**


:::warning
What is the default clock frequency?
:::

Of course you can find the answer in [*RM0090 Reference manual STM32F407*], but before this, we can firstly measure it: output the clock, and then use an oscilloscope to display the wave form.

The discriptions are in Section 7.2.10 (p.222). We are going to output the **system clock (SYSCLK)**, the clock source of the entire system.

![](http://125.227.181.237/uploads/upload_15dec19b10391525b6c74511a5ebead8.png)

### RCC - MCO2 Configurations

Section 7.3.3 (p.228) shows the configurations of **MCO2**.

* **MCO2[1:0]** is used for clock source selection.

* **MCO2PRE[2:0]** is the prescaler configuration of the MCO2

![](http://125.227.181.237/uploads/upload_77d26f6bcf205bd061f3ed391b416da5.png)

### RCC - GPIO Enable

To enable GPIOC, [*RM0090 Reference manual STM32F407*] tells us to set the bit2 at `0x40023800 + 0x30`. The address `0x40023800` is defined in the memory map.

![](http://125.227.181.237/uploads/upload_d7586fec58aeafce73002d3b698f3222.png)

![](http://125.227.181.237/uploads/upload_0d6bc7aca25fbb82ba581d3add7408f2.png)


### GPIO Configurations

![](http://125.227.181.237/uploads/upload_0bbf043a8ddb6bdd47ca569151c310a6.png)

We want **GPIOC9** to be **AF(alternate function mode) PP(push pull)**, so we have to set GPIOC:

* **`MODER(9)` = `10`**

* **`OTYPER(9)` = `0`**

* **`OSPEEDER(9)` = maximum I/O speed**

* **`PUPDR(9)` = `00`**

![](http://125.227.181.237/uploads/upload_42c9b21216b6f38e1023ec781031fded.png)

![](http://125.227.181.237/uploads/upload_4e30bb5aaf1829f5497fc0689d487150.png)

![](http://125.227.181.237/uploads/upload_8ddf278d3ef888b939f2c3fbb72fd540.png)

![](http://125.227.181.237/uploads/upload_9924c2e8e9ce2f0bee517af0de68d4ec.png)

### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> reg.h

```clike=
#ifndef REG_H
#define REG_H

//REG OPERATIONS
#define UINT32_1 ((uint32_t)1)

#define REG(addr) (*((volatile uint32_t *)(addr)))

#define SET_BIT(addr, bit) (REG(addr) |= UINT32_1 << (bit))
#define CLEAR_BIT(addr, bit) (REG(addr) &= ~(UINT32_1 << (bit)))


//RCC
#define RCC_BASE 0x40023800

#define RCC_CFGR_OFFSET 0x08
#define MCO2_1_BIT 31
#define MCO2_0_BIT 30

#define MCO2PRE_2_BIT 29
#define MCO2PRE_0_BIT 27

#define RCC_AHB1ENR_OFFSET 0x30
#define GPIO_EN_BIT(port) (port)

//GPIO
#define GPIO_PORTA 0
#define GPIO_PORTB 1
#define GPIO_PORTC 2
#define GPIO_PORTD 3

#define GPIO_BASE(port) (0x40020000 + 0x400 * (port))

#define GPIOx_MODER_OFFSET 0x00
#define MODERy_1_BIT(y) ((y)*2 + 1)
#define MODERy_0_BIT(y) ((y)*2)

#define GPIOx_OTYPER_OFFSET 0x04
#define OTy_BIT(y) (y)

#define GPIOx_OSPEEDR_OFFSET 0x08
#define OSPEEDRy_1_BIT(y) ((y)*2 + 1)
#define OSPEEDRy_0_BIT(y) ((y)*2)

#define GPIOx_PUPDR_OFFSET 0x0C
#define PUPDRy_1_BIT(y) ((y)*2 + 1)
#define PUPDRy_0_BIT(y) ((y)*2)

#define GPIOx_BSRR_OFFSET 0x18
#define BRy_BIT(y) ((y) + 16)
#define BSy_BIT(y) (y)

#endif
```

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
void blink(unsigned int led)
{
	led_init(led);

	unsigned int i;

	while (1)
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
#include <stdint.h>
#include "reg.h"
#include "blink.h"

void op_sysclk(unsigned int div);

int main(void)
{
	op_sysclk(1);
	blink(LED_BLUE);
}

/**
 * 
 * output sysclk (PC9)
 * 
 */
void op_sysclk(unsigned int div)
{
	//RCC
	CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_1_BIT);
	CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_0_BIT);

	if (div == 1)
		CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2PRE_2_BIT);

	else if (div >= 2 && div <= 5)
	{
		SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2PRE_2_BIT);
		REG(RCC_BASE + RCC_CFGR_OFFSET) = (REG(RCC_BASE + RCC_CFGR_OFFSET) & ~(((uint32_t)0b11) << (MCO2PRE_0_BIT))) | ((uint32_t)(div - 2) << (MCO2PRE_0_BIT));
	}
	else
		while(1)
			;

	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTC));

	//GPIO MODER
	????????

	//Output push-pull
	????????

	//OSPEEDR9 = 11 => Very high speed
	????????

	//PUPDR9 = 00 => No pull-up, pull-down
	????????
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

all: clock.bin

clock.bin: main.c blink.c startup.c vector_table.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld main.c blink.c startup.c vector_table.s -o clock.elf
	$(CROSS-COMPILER)objcopy -O binary clock.elf clock.bin

flash:
	st-flash --reset write clock.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
```


### Build

1. Under the path of **`ESEmbedded_LAB06/1_clock_output`**, generate the .bin file with:

    ```shell
    make
    ```

1. **Connect your STM32F4DISCOVERY board**, and flash by:

    ```shell
    make flash
    ```

1. Connect an oscilloscope to pin PC9 and GND and then observe the result.

    ![](http://125.227.181.237/uploads/upload_a6c03bd808f856e920cd4eefae8975f7.jpg)

    ![](http://125.227.181.237/uploads/upload_6a4cead4ba45a84cc1b81c17f18f24aa.png)

1. Try to prescale the clock output with "division by 4"

    ![](http://125.227.181.237/uploads/upload_7e16e50610ddc096592f0262acff4e1f.png)

---------------------------------------------------------

## Clocks

1. Clocks of STM32F407 are described in [*RM0090 Reference manual STM32F407*] section 7.2 (p.215).

2. Three different clock sources can be used to drive the system clock (SYSCLK):

    * HSI(high speed internal clock signal) oscillator clock

    * HSE(high speed external clock signal) oscillator clock

    * PLL clock

3. Figure 21. (p.216) shows the clock tree of the STM32F407

    ![](http://125.227.181.237/uploads/upload_d5efad3910032e2492ad5a0600b400c6.png)

    FCLK acts as Cortex®-M4 with FPU free-running clock.


### HSE Clock

1. The high speed external clock signal (HSE) can be generated from two possible clock sources:

    * HSE external crystal/ceramic resonator

    * HSE external user clock

    ![](http://125.227.181.237/uploads/upload_7b026feb8d2633473e7892cf3591c375.png)

2. In [*UM1472 User manual Discovery kit with STM32F407VG MCU*], we can find the electrical schematic of external crystal resonator

    ![](http://125.227.181.237/uploads/upload_0adcab33cc1c8d41d54b8fc56083fe31.png)

    ![](http://125.227.181.237/uploads/upload_fb5cc77ae6757d146e2dc8c4dcf0d736.png)


### HSI Clock

1. The HSI clock signal is generated from an internal 16 MHz RC oscillator

2. Can be used directly as a system clock, or used as PLL input.

3. The HSI RC oscillator has the advantage of providing a clock source at low cost (no external components).

4. It also has a faster startup time than the HSE crystal oscillator

5. However, even with calibration the frequency is **less accurate** than an external crystal oscillator or ceramic resonator.

### PLL

1. A main PLL (PLL) clocked by the HSE or HSI oscillator and featuring two different output clocks:

    * The first output is used to generate the high speed system clock (up to 168 MHz)

    * The second output is used to generate the clock for the USB OTG FS (48 MHz), the random analog generator (≤48 MHz) and the SDIO (≤ 48 MHz).

2. Since the main-PLL configuration parameters cannot be changed once PLL is enabled, it is recommended to configure PLL before enabling it.

3. **RCC PLL configuration register (RCC_PLLCFGR)** can be used to configure PLL.


### System Clock (SYSCLK) Selection

1. After a system reset, the **HSI** oscillator is selected as the system clock.

2. When a clock source is used directly or through PLL as the system clock, it is not possible to stop it.

3. A switch from one clock source to another occurs **only if the target clock source is ready** (clock stable after startup delay or PLL locked).

4. Status bits in the **RCC clock control register (RCC_CR)** indicate which clock(s) is (are) ready and which clock is currently used as the system clock.

-------------------------

## LAB06-2 Clock Initialization ─ Using the PLL

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB06/2_clock_pll`</p>**

1. To generate 168 MHz clock source from the HSE clock (8 MHz), we can use the main-PLL inside the STM32F407VG SoC.

1. Additionally, since we are increasing the system clock frequency, **the flash latency should be adjusted**.

    Having the latency too high for the system clock is mostly harmless ─ it will slow the system down slightly (but that's it).

    But **having the latency set too low may cause malfunctions**.

1. The following steps must be done in sequence:

    :::warning
    
    <b>

    1. Turn on the HSE clock.

    2. Wait until the HSE clock is stable.

    3. Configure the main PLL parameters:

        * Selection of the HSI or HSE oscillator as PLL clock source

        * Configuration of division factors M, N, P

    4. Turn on the main PLL

    5. Wait until the main PLL is ready

    6. Enable flash memory prefetch and set the latency

    7. Select the main PLL as system clock source

    8. Wait until the main PLL is used as system clock source

    </b>

    :::


### RCC_CR

![RCC_CR](http://125.227.181.237/uploads/upload_e834ce456cc964e5a3786db9bd9c4014.png)

![PLLON](http://125.227.181.237/uploads/upload_f170f25a8726f92f9bec81583cc5c6e9.png)

![HSEON](http://125.227.181.237/uploads/upload_ab4e96e541ebd71c20d62eb313f220b3.png)


### RCC_PLLCFGR

![RCC_PLLCFGR](http://125.227.181.237/uploads/upload_3b296a96f91c8858ba7e72ce825ec69e.png)

![PLL calc](http://125.227.181.237/uploads/upload_e1059344ed7e5987c7ff509edf2cb9bf.png)

* Configuration example:

    :::warning
    
    <b>

    f~HSE~ = 8 MHz

    N = 168
    M = 4

    f~VCO~ = 8 * 168 / 4 = 168 * 2

    P = 2

    f~PLL_out~ = 168 MHz

    </b>

    :::

### Relation Between CPU Clock Frequency and Flash Memory Read Time

![Table 10.](http://125.227.181.237/uploads/upload_b9fac4224f790e0749b8efc9fe56c81f.png)

![sequences](http://125.227.181.237/uploads/upload_f625fc0334970ef59bb5f9a05bc7415d.png)

### Flash_ACR

![](http://125.227.181.237/uploads/upload_0c744c2542886b2fde33a055c694c02a.png)

### RCC_CFGR

![](http://125.227.181.237/uploads/upload_8f79cc4e8ffdadc7231abd8071dca3cc.png)

![](http://125.227.181.237/uploads/upload_d577640402214faca905d760663ba149.png)


### Source Code

Try to complete the ?????? inside the `reg.h` and `startup.c`.

#### <i class="fa fa-file-code-o" style="color:gray"></i> reg.h

```clike=
#ifndef REG_H
#define REG_H

//REG OPERATIONS
#define UINT32_1 ((uint32_t)1)

#define REG(addr) (*((volatile uint32_t *)(addr)))

#define CLEAR_MASK(highest_bit, lowest_bit) (((highest_bit) - (lowest_bit)) >= 31 ? (uint32_t)0xFFFFFFFF : ~(((UINT32_1 << ((highest_bit) - (lowest_bit) + 1)) - 1) << (lowest_bit)))
#define WRITE_BITS(addr, highest_bit, lowest_bit, data) (REG(addr) = (REG(addr) & CLEAR_MASK(highest_bit, lowest_bit)) | ((uint32_t)(data) << (lowest_bit)))

#define SET_BIT(addr, bit) (REG(addr) |= UINT32_1 << (bit))
#define CLEAR_BIT(addr, bit) (REG(addr) &= ~(UINT32_1 << (bit)))

#define READ_BIT(addr, bit) (????????)

//FLASH
#define FLASH_BASE ????????

#define FLASH_ACR_OFFSET ????????
#define PRFTEN_BIT ????????
#define LATENCY_2_BIT ????????
#define LATENCY_0_BIT ????????

//RCC
#define RCC_BASE 0x40023800

#define RCC_CR_OFFSET 0x00
#define PLLRDY_BIT ????????
#define PLLON_BIT ????????
#define HSERDY_BIT ????????
#define HSEON_BIT ????????

#define RCC_PLLCFGR_OFFSET ????????

#define PLLSRC_BIT ????????

#define PLLP_1_BIT ????????
#define PLLP_0_BIT ????????

#define PLLN_8_BIT 14
#define PLLN_0_BIT 6

#define PLLM_5_BIT ????????
#define PLLM_0_BIT ????????

#define RCC_CFGR_OFFSET 0x08
#define MCO2_1_BIT 31
#define MCO2_0_BIT 30

#define MCO2PRE_2_BIT 29
#define MCO2PRE_0_BIT 27

#define SWS_1_BIT 3
#define SWS_0_BIT 2

#define SW_1_BIT ????????
#define SW_0_BIT ????????

#define RCC_AHB1ENR_OFFSET 0x30
#define GPIO_EN_BIT(port) (port)

//GPIO
#define GPIO_PORTA 0
#define GPIO_PORTB 1
#define GPIO_PORTC 2
#define GPIO_PORTD 3

#define GPIO_BASE(port) (0x40020000 + 0x400 * (port))

#define GPIOx_MODER_OFFSET 0x00
#define MODERy_1_BIT(y) ((y)*2 + 1)
#define MODERy_0_BIT(y) ((y)*2)

#define GPIOx_OTYPER_OFFSET 0x04
#define OTy_BIT(y) (y)

#define GPIOx_OSPEEDR_OFFSET 0x08
#define OSPEEDRy_1_BIT(y) ((y)*2 + 1)
#define OSPEEDRy_0_BIT(y) ((y)*2)

#define GPIOx_PUPDR_OFFSET 0x0C
#define PUPDRy_1_BIT(y) ((y)*2 + 1)
#define PUPDRy_0_BIT(y) ((y)*2)

#define GPIOx_BSRR_OFFSET 0x18
#define BRy_BIT(y) ((y) + 16)
#define BSy_BIT(y) (y)

#endif
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.h

[**～～～**](#-blinkh)

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
void blink(unsigned int led)
{
	led_init(led);

	unsigned int i;

	while (1)
	{
		//set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(led));

		for (i = 0; i < 500000; i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));

		for (i = 0; i < 500000; i++)
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

		for (i = 0; i < 500000; i++)
			;

		//reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(led));

		for (i = 0; i < 500000; i++)
			;
	}
}

```

#### <i class="fa fa-file-code-o" style="color:gray"></i> main.c

:::warning

Please copy your completed [`main.c` in the previous lab (LAB06-01)](#-mainc) to this lab directory, and then try to prescale the clock output with **"division by 4"**

:::


#### <i class="fa fa-file-code-o" style="color:gray"></i> startup.c

```clike=
#include <stdint.h>
#include "reg.h"

extern int main(void);

void set_sysclk_pll(void);

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

	set_sysclk_pll();

	main();
}

/**
 * 
 * set sysclk pll (168 MHz)
 * 
 */
void set_sysclk_pll(void)
{
	//enable HSE
	????????

	//wait
	while (READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSERDY_BIT) != 1)
		;

	//set pll
	???????? //use HSE for PLL source

	//f_HSE = 8 MHz
	//
	//N = 168
	//M = 4
	//
	//f_VCO = 8 * 168 / 4 = 168 * 2
	//
	//P = 2
	//
	//f_PLL_out = 168
	//
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b00);
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLN_8_BIT, PLLN_0_BIT, 168);
	????????

	//enable pll
	????????

	//wait
	????????

	//enable flash prefetch buffer
	????????

	//set flash wait state = 5
	????????

	//use pll
	????????

	//wait
	????????
}
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

[**～～～**](#-vector_tables)

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> stm32f4.ld

[**～～～**](#-stm32f4ld)


### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

[**～～～**](#-makefile)

### Build

1. Under the path of **`ESEmbedded_LAB06/2_clock_pll`**, generate the .bin file with:

    ```shell
    make
    ```

1. **Connect your STM32F4DISCOVERY board**, and flash by:

    ```shell
    make flash
    ```

1. Connect an oscilloscope to pin PC9 and GND and then observe the result.

    ![](http://125.227.181.237/uploads/upload_262c4bc9dd9a16f65080d92719161279.png)


[*RM0090 Reference manual STM32F407*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/STM32F407_Reference_manual.pdf
[*UM1472 User manual Discovery kit with STM32F407VG MCU*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/STM32F4DISCOVERY.pdf





-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`