04 - Getting Started with STM32F407
===

Datasheet reading:

* [*STM32F407xx Datasheet*]

* [*RM0090 Reference manual STM32F407*]

* [*UM1472 User manual Discovery kit with STM32F407VG MCU*]


**<p style="color:crimson"><i class="fa fa-github"></i> Please fork the [LAB04](https://github.com/ybcsie/ESEmbedded_LAB04) repo, and then clone the repo you just forked.</p>**

**<p style="color:crimson">　 You will have to push the modified version to GitHub at the end of this lecture.</p>**


---

## STM32F407VG Overview

Excerpted from [*STM32F407xx Datasheet*]

1. The STM32F407xx family is based on the high-performance **ARM® Cortex®-M4 32-bit RISC core** operating at a frequency of **up to 168 MHz**.

1. High-speed embedded memories (**Flash memory up to 1 Mbyte**, **up to 192 Kbytes of SRAM**)

1. Up to 4 Kbytes of backup SRAM

1. An extensive range of enhanced I/Os and peripherals connected to two APB buses, three AHB buses and a 32-bit multi-AHB bus matrix.

1. All devices offer three 12-bit ADCs, two DACs, a low-power RTC, twelve general-purpose 16-bit timers including two PWM timers for motor control, two general-purpose 32-bit timers.

1. They also feature standard and advanced communication interfaces:

    * Up to three I^2^Cs

    * Three SPIs, two I^2^Ss full duplex. To achieve audio class accuracy, the I^2^S peripherals can be clocked via a dedicated internal audio PLL or via an external clock to allow synchronization.

    * Four USARTs plus two UARTs

    * An USB OTG full-speed and a USB OTG high-speed with full-speed capability (with the ULPI),

    * Two CANs

    * An SDIO/MMC interface

    * Ethernet and the camera interface available on STM32F407xx devices only.

1. New advanced peripherals include an SDIO, an enhanced flexible static memory control (FSMC) interface (for devices offered in packages of 100 pins and more), a camera interface for CMOS sensors.

### Features and Peripheral Counts

![](http://125.227.181.237/uploads/upload_3c5a75359b15621df73a0c8e92fb98ac.png)

We can find the STM32F407VG microcontroller features:

* **512-Kbyte or 1-Mbyte Flash memory**

* **192-Kbyte RAM**

* LQFP100 package

### Block Diagram

![](http://125.227.181.237/uploads/upload_57fae2f284acfe98f29e4b17954f42fc.png)

### Pinouts and Pin Description

The STM32F405xx and STM32F407xx family offers devices in various packages ranging from 64 pins to 176 pins. The set of included peripherals changes with the device chosen.

![](http://125.227.181.237/uploads/upload_6b7403aa2772eaf76f5f53b865d2f0d5.png)

#### Pin Definitions

See Table 7. (p.47) in [*STM32F407xx Datasheet*]

#### Alternate Function Mapping

See Table 9. (p.62) in [*STM32F407xx Datasheet*]

### Memory Mapping

![](http://125.227.181.237/uploads/upload_f3350b788ed2e1c73f3cda50a473df02.png)

#### Register Boundary Addresses

![](http://125.227.181.237/uploads/upload_5f4e7231165865477416215268585a07.png)




------------------------------------------------------------------------------------------------


## STM32F407VG Boot Configuration

Excerpted from [*RM0090 Reference manual STM32F407*]

1. Due to its fixed memory map, the code area starts from address `0x00000000` (accessed through the ICode/DCode buses) while the data area (SRAM) starts from address `0x20000000` (accessed through the system bus).

2. The Cortex®-M4 with FPU CPU always fetches the reset vector on the ICode bus, which implies to have the boot space available only in the code area (typically, Flash memory).

3. STM32F4xx microcontrollers implement a special mechanism to be able to boot from other memories (like the internal SRAM).

4. In the STM32F4xx, three different boot modes can be selected through the BOOT[1:0] pins

![](http://125.227.181.237/uploads/upload_4bc834a03413c5d158dbc17b3d63a54e.png)

### Embedded Bootloader

1. The embedded bootloader mode is used to reprogram the Flash memory using one of the following serial interfaces:

    * USART1 (PA9/PA10)

    * USART3 (PB10/11 and PC10/11)

    * CAN2 (PB5/13)

    * USB OTG FS (PA11/12) in Device mode (DFU: device firmware upgrade).

1. The embedded bootloader code is located in **system memory**.

1. It is programmed by ST during production. For additional information, refer to application note AN2606.

### Physical Remap

1. Once the boot pins are selected, the application software can modify the memory accessible in the code area (in this way the code can be executed through the ICode bus in place of the System bus).

1. This modification is performed by programming the Section 9.2.1 in [*RM0090 Reference manual STM32F407*]: SYSCFG memory remap register (SYSCFG_MEMRMP) in the SYSCFG controller.

1. The following memories can thus be remapped:

    * Main Flash memory

    * System memory

    * Embedded SRAM1 (112 KB)

    * FSMC bank 1 (NOR/PSRAM 1 and 2)

![](http://125.227.181.237/uploads/upload_04aad3a585393d7fc61158f98c405990.png)


------------------------------------------------------------------------------------------------

## STM32F4DISCOVERY ─ Discovery Kit with STM32F407VG MCU

Excerpted from [*UM1472 User manual Discovery kit with STM32F407VG MCU*]

![](http://125.227.181.237/uploads/upload_00ea1ec0bfe55e5961c72f7fdb156ef6.png)

1. STM32F407VGT6 microcontroller featuring 32-bit ARM Cortex® -M4 with FPU core, **1-Mbyte Flash memory**, **192-Kbyte RAM** in an LQFP100 package

1. **On-board ST-LINK/V2 on STM32F4DISCOVERY**

1. USB ST-LINK with re-enumeration capability and three different interfaces:

    * Virtual COM port (with ST-LINK/V2-A only)

    * Mass storage (with ST-LINK/V2-A only)

    * Debug port

1. **Board power supply**:

    * **Through USB bus**

    * **External power sources: 3 V and 5 V**

1. LIS302DL or LIS3DSH ST MEMS 3-axis accelerometer

1. MP45DT02 ST MEMS audio sensor omni-directional digital microphone

1. CS43L22 audio DAC with integrated class D speaker driver

1. **Eight LEDs**:

    * LD1 (red/green) for USB communication

    * LD2 (red) for 3.3 V power on

    * Four user LEDs, LD3 (orange), LD4 (green), LD5 (red) and LD6 (blue)

    * 2 USB OTG LEDs LD7 (green) VBUS and LD8 (red) over-current

* **Two push buttons (user and reset)**

* USB OTG FS with micro-AB connector

* **Extension header for all LQFP100 I/Os for quick connection to prototyping board and easy probing**


------------------------------------------------------------------------------------------------------------------


## LAB04-1 Simple Program ─ Blink


**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB04/1_blink`</p>**


### Setup the STM32F4DISCOVERY Linux Programmer

1. **Stlink** is a tool for flashing binary files to STM32 devices.

2. We are using the open source version. **You can download the [installer for Ubuntu] and follow the step below**.

    ( If you want to install from source, follow the steps [here](https://github.com/texane/stlink). )

    1. Extract `stlink.zip` and `cd` into the `stlink` folder.

    2. Install by command:

        ```shell
        sudo ./install
        ```

    3. You can check the installation by:

        ```shell
        st-flash --version
        ```

    4. After success, you can remove the `stlink` folder if you want.


### LEDs on the STM32F4DISCOVERY

1. To light up the LED on the STM32F4DISCOVERY, we have to know which GPIO pin the LED was connected to.

1. We can find this in [*UM1472 User manual Discovery kit with STM32F407VG MCU*]: section 6.3 (p.16), which defines the connections of LEDS.

1. Here we will use the **blue LED(LD6)** in the example program, so we have to control the GPIO pin **PD15(15th pin of GPIOD)**.

![](http://125.227.181.237/uploads/upload_30607d48fc30e40f8c17b867c2a53712.png)


### General-purpose I/Os (GPIO)

1. We can find hardware configuration details **in [*RM0090 Reference manual STM32F407*]**

1. In section 2.3(p.64), there is a **memory map of peripherals**.

1. The **base address of GPIOD is `0x40020C00`** and the details are described in section 8(p.267).

1. In addition, we will have to setup **RCC(Reset and clock control) register** to enable the clock for GPIOD.

1. The **base address of RCC is `0x40023800`** and the details are described in section 7(p.213).

![](http://125.227.181.237/uploads/upload_573f53d8bb0674e057f474a69454c48b.png)


### RCC - GPIO Enable

To enable GPIOD, [*RM0090 Reference manual STM32F407*] tells us to **set the bit3 at `0x40023800 + 0x30`**. The address `0x40023800` is defined in the memory map.

![](http://125.227.181.237/uploads/upload_f9bd29a1f59d6ce7a706b9126ec4e37f.png)

![](http://125.227.181.237/uploads/upload_42db8dfeac3a1c2d30e92f31695c1202.png)


### GPIO Configurations

1. Section 8.3 described that each of the GPIOs has four 32-bit memory-mapped control registers:

    * **MODER**

    * **OTYPER**

    * **OSPEEDR**

    * **PUPDR**

1. GPIOA0 ~ GPIOA15, GPIOB0 ~ GPIOB15, ... , GPIOK0 ~ GPIOK15, **evrey pin** has its own settings.

    ![](http://125.227.181.237/uploads/upload_e7c7bf2799d67013fc2c17d7cb20fa25.png)

    ![](http://125.227.181.237/uploads/upload_0bbf043a8ddb6bdd47ca569151c310a6.png)

1. We want the pin GPIOD15 to be output PP(push pull) without pull-up or pull-down resistors, so we have to configure the GPIOD:

    * **`MODER(15)` = `01`**

    * **`OTYPER(15)` = `0`**

    * **`OSPEEDER(15)`** can be ignored (update frequency is not that important in this case.)

    * **`PUPDR(15)` = `00`**

1. The offsets of these control registers is described in section 8.4(p.281)：

    ![](http://125.227.181.237/uploads/upload_42c9b21216b6f38e1023ec781031fded.png)

    ![](http://125.227.181.237/uploads/upload_4e30bb5aaf1829f5497fc0689d487150.png)

    ![](http://125.227.181.237/uploads/upload_8ddf278d3ef888b939f2c3fbb72fd540.png)

    ![](http://125.227.181.237/uploads/upload_9924c2e8e9ce2f0bee517af0de68d4ec.png)

1. To control GPIO output, we can use **GPIO port output data register (GPIOx_ODR)** or **GPIO port bit set/reset register (GPIOx_BSRR)**.

    * GPIOx_ODR is readable and writable

    * GPIOx_BSRR is only writable

    ![](http://125.227.181.237/uploads/upload_a19f940dd69066c26657a0cb9fddb7f4.png)

    ![](http://125.227.181.237/uploads/upload_3453ea478755420b4910cb22b50868b3.png)




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



#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.c


```clike=
#include <stdint.h>
#include "reg.h"

void blink(void)
{
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTD));

	//MODER led pin = 01 => General purpose output mode
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_1_BIT(15));
	SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_0_BIT(15));

	//OT led pin = 0 => Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OTYPER_OFFSET, OTy_BIT(15));

	//OSPEEDR led pin = 00 => Low speed
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(15));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(15));

	//PUPDR led pin = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(15));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(15));

	unsigned int i;
	while (1)
	{
		//set GPIOD15
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BSy_BIT(15));

		for (i = 0; i < 100000; i++)
			;

		//reset GPIOD15
		SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_BSRR_OFFSET, BRy_BIT(15));

		for (i = 0; i < 100000; i++)
			;
	}
}
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

```=
.section .vector_table

.word 0x20000100
.word blink
```

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> stm32f4.ld

```=
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
}

SECTIONS
{
	.mytext :
	{
		KEEP(*(.vector_table))
		*(.text)
	} > FLASH
}
```

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: blink.bin

blink.bin: blink.c vector_table.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld blink.c vector_table.s -o blink.elf
	$(CROSS-COMPILER)objcopy -O binary blink.elf blink.bin

flash:
	st-flash --reset write blink.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
```

### Build

1. Under the path of **`ESEmbedded_LAB04/1_blink`**, build with:

    ```shell
    make
    ```

2. You should get the .bin file.

3. **Connect your STM32F4DISCOVERY board**, and flash by:

    ```shell
    make flash
    ```

4. You should see the blue led blinking.


### Exercise

Follow the steps below to modify the files:

1. Create **`blink.h`** and **`main.c`** in **`ESEmbedded_LAB04/1_blink`**.

2. Modify the following files and try to complete the `??????`.

3. Follow the steps above to build your program again.


#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.h


```clike=
#ifndef BLINK_H
#define BLINK_H

#define LED_GREEN ??????
#define LED_ORANGE ??????
#define LED_RED ??????
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
    ??????
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

	??????
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


#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

```=
.section .vector_table

.word 0x20000100
.word main
```

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: blink.bin

blink.bin: main.c blink.c vector_table.s
	$(CROSS-COMPILER)gcc -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld main.c blink.c vector_table.s -o blink.elf
	$(CROSS-COMPILER)objcopy -O binary blink.elf blink.bin

flash:
	st-flash --reset write blink.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
```



-----------------------------------------------

## LAB04-2 About `Volatile`

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB04/2_volatile`</p>**


### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> test.c

```clike=
#include <stdint.h>

int test_volatile(void)
{
	volatile uint32_t *reg_a = (uint32_t *)0x40023800;

	uint32_t val1;
	uint32_t val2;

	val1 = *reg_a;

	val2 = *reg_a;

	if (val1 == val2)
		return 0;

	return 1;
}
```

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> empty.ld

( empty file )


### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: test.elf

test.elf: test.c
	$(CROSS-COMPILER)gcc -O1 -Wall -std=c11 -mcpu=cortex-m4 -mthumb -nostartfiles -T empty.ld test.c -o test.elf
	$(CROSS-COMPILER)objdump -D test.elf

clean:
	rm -f *.o *.elf
```


### Build

1. Under the path of **`ESEmbedded_LAB04/2_volatile`**, build with:

    ```shell
    make
    ```

2. You will get the results from objdump.

3. Try to remove `volatile`, and then `make` again.

4. Compare the two results.

#### The results from objdump ( with `volatile` ):

```shell
arm-none-eabi-objdump -D test.elf

test.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <test_volatile>:
   0:   4b03            ldr     r3, [pc, #12]   ; (10 <test_volatile+0x10>)
   2:   6818            ldr     r0, [r3, #0]
   4:   681b            ldr     r3, [r3, #0]
   6:   1ac0            subs    r0, r0, r3
   8:   bf18            it      ne
   a:   2001            movne   r0, #1
   c:   4770            bx      lr
   e:   bf00            nop
  10:   40023800        andmi   r3, r2, r0, lsl #16


......

```

#### The results from objdump ( without `volatile` ):

```shell
arm-none-eabi-objdump -D test.elf

test.elf:     file format elf32-littlearm


Disassembly of section .text:

00000000 <test_volatile>:
   0:   2000            movs    r0, #0
   2:   4770            bx      lr


......

```





[*STM32F407xx Datasheet*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/stm32f407vg.pdf
[*RM0090 Reference manual STM32F407*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/STM32F407_Reference_manual.pdf
[*UM1472 User manual Discovery kit with STM32F407VG MCU*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/STM32F4DISCOVERY.pdf
[installer for Ubuntu]: http://www.nc.es.ncku.edu.tw/course/embedded/utils/stlink.zip








-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`