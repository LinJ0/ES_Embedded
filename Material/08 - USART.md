08 - USART
===

In this lecture, we will use the universal synchronous asynchronous receiver transmitter (USART) to communicate with the host computer.

You will first finish the basic version (without using the usart interrupt), and then try the interrupt version.


**<p style="color:crimson"><i class="fa fa-github"></i> Please fork the [LAB08](https://github.com/ybcsie/ESEmbedded_LAB08) repo, and then clone the repo you just forked.</p>**

**<p style="color:crimson">　 You will have to push the modified version to GitHub at the end of this lecture.</p>**

---

## USART Functional Description

* See [*RM0090 Reference manual STM32F407*] ─ *30.3 USART functional description* for more details.

### USART Block Diagram

![Figure 296. USART block diagram](http://125.227.181.237/uploads/upload_98490ee6fd3ef7f6f54cb11f7da5766a.png)

* Character transmission:
    1. Data shifts out least significant bit first on the TX pin.
    2. In this mode, the **USART_DR** register consists of Transmit data register (TDR).

* Character reception:
    1. Data shifts in least significant bit first through the RX pin.
    2. In this mode, the **USART_DR** register consists of Receive data register (RDR).


### USART Configuration

1. Enable the USART by writing the **UE** bit in **USART_CR1** register to 1.

2. Select the desired baud rate using the **USART_BRR** register.

3. Set the **TE** bit in **USART_CR1** to enable the transmitter.

4. Set the **RE** bit in **USART_CR1** to enable the receiver.


### Character Transmission

1. Wait until the **TXE** bit is set.

2. Write the data to send in the **USART_DR** register (this clears the TXE bit).

#### The TXE Bit

1. The TXE bit is set by hardware and it indicates:
    * The data has been moved from TDR to the shift register and the data transmission has started.
    * The TDR register is empty.
    * The next data can be written in the USART_DR register without overwriting the previous data.

2. The TXE flag generates an interrupt if the **TXEIE** bit is set.

3. When a transmission is taking place, a write instruction to the USART_DR register stores the data in the TDR register and which is copied in the shift register at the end of the current transmission.

4. When no transmission is taking place, a write instruction to the USART_DR register places the data directly in the shift register, the data transmission starts, and the TXE bit is immediately set.


### Character Reception

1. When a character is received, the **RXNE** bit is set.

2. The error flags can be set if a frame error, noise or an overrun error has been detected during reception.

3. Read to the **USART_DR** register (this clears the RXNE bit).

#### The RXNE Bit

1. The RXNE bit is set by hardware and it indicates:
    * The content of the shift register is transferred to the RDR.
    * Data has been received and can be read.

2. An interrupt is generated if the **RXNEIE** bit is set.

3. The RXNE flag can also be cleared by writing a zero to it.

4. The RXNE bit must be cleared before the end of the reception of the next character to avoid an overrun error.


### Baud Rate

* See [*RM0090 Reference manual STM32F407*] ─ *30.3.4 Fractional baud rate generation* for more details.

* The baud rate for the receiver and transmitter (Rx and Tx) are both set to the same value as programmed in the **Mantissa** and **Fraction** values of **USARTDIV**.

* Formula:

    ![](http://125.227.181.237/uploads/upload_c30aa2321d3ff0eae2345c61cbf7e3fe.png)

    ![](http://125.227.181.237/uploads/upload_88f9014070e3d244e78cab0c279e8e1f.png)

    * **Mantissa** = xxx

    * **Fraction** = The nearest integer of 0.yyy * 16

* For example:

    * f~CK~ = 168 MHz

    * baud = 115200 bps

    ![](http://125.227.181.237/uploads/upload_3e39431725747205e7d226012e968461.png)



-----------------------------------------------------------------------



## USART Interrupts
* In [*RM0090 Reference manual STM32F407*] ─ *30.4 USART interrupts*.

![](http://125.227.181.237/uploads/upload_aa356dbdd0f1a02939348cf74f6a85ca.png)



-----------------------------------------------------------------------




## LAB08-1 USART Basic

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB08/1_usart_basic`</p>**

* In this lab, we are going to use **USART1** on the STM32F407 mcu to communicate with the host computer.

* We will send `Hello world` after the mcu is reset, and then get a key from the host computer.

### USART Pins Mapping

* According to [*UM1472 User manual Discovery kit with STM32F407VG MCU*], **PB6** and **PB7** can be used for **USART1_TX** and **USART1_RX**.

    ![STM32 pin description](http://125.227.181.237/uploads/upload_8f62a696863e59f54fed35a1698dbdfa.png)

### GPIO Alternate Function

* Using the **GPIOx_AFRL** and **GPIOx_AFRH** alternate function registers, user can select one out of the sixteen alternate function inputs/outputs available for each I/O.

    ![](http://125.227.181.237/uploads/upload_c41688158bf2f8fd15efcfa2ae0ce33e.png)


### Hints

1. Remember to enable USART1 in RCC.
2. GPIO:
    * Mode: Alternate function mode
    * Output type: Output push-pull
    * Output speed: Medium speed works fine.
    * No pull-up, pull-down

### Register Details

1. Memory map: [*RM0090 Reference manual STM32F407*] ─ *2.3 Memory map*
2. RCC registers: [*RM0090 Reference manual STM32F407*] ─ *7.3 RCC registers*
3. GPIO registers: [*RM0090 Reference manual STM32F407*] ─ *8.4 GPIO registers*
4. USART Registers: [*RM0090 Reference manual STM32F407*] ─ *30.6 USART registers*


### Source Code

:::warning

Try to modify `main.c`

:::

#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/07/#-vector_tables1)

#### <i class="fa fa-file-code-o" style="color:gray"></i> startup.c

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/07/#-startupc1)

#### <i class="fa fa-file-code-o" style="color:gray"></i> main.c

```clike=
#include <stdint.h>
#include "reg.h"

void init_usart1(void)
{
	//PB6 PB7

	//RCC EN GPIOB
	??????

	//GPIO Configurations
	??????

	//RCC EN USART1
	??????

	//Baud
	const unsigned int BAUD = 115200;
	const unsigned int SYSCLK_MHZ = 168;
	const double USARTDIV = SYSCLK_MHZ * 1.0e6 / 16 / BAUD;

	const uint32_t DIV_MANTISSA = ??????
	const uint32_t DIV_FRACTION = ??????

	//USART  Configurations
	??????

}

void usart1_send_char(const char ch)
{
	??????
}

char usart1_receive_char(void)
{
	??????
}

int main(void)
{
	init_usart1();

	char *hello = "Hello world!\r\n";

	//send Hello world
	while (*hello != '\0')
		usart1_send_char(*hello++);


	//receive char and resend it
	char ch;
	while (1)
	{
		ch = usart1_receive_char();

		if (ch == '\r')
			usart1_send_char('\n');

		usart1_send_char(ch);
	}
}
```

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

#define READ_BIT(addr, bit) ((REG(addr) >> (bit)) & UINT32_1)

//FLASH
#define FLASH_BASE 0x40023C00

#define FLASH_ACR_OFFSET 0x00
#define PRFTEN_BIT 8
#define LATENCY_2_BIT 2
#define LATENCY_0_BIT 0

//RCC
#define RCC_BASE 0x40023800

#define RCC_CR_OFFSET 0x00
#define PLLRDY_BIT 25
#define PLLON_BIT 24
#define HSERDY_BIT 17
#define HSEON_BIT 16

#define RCC_PLLCFGR_OFFSET 0x04

#define PLLSRC_BIT 22

#define PLLP_1_BIT 17
#define PLLP_0_BIT 16

#define PLLN_8_BIT 14
#define PLLN_0_BIT 6

#define PLLM_5_BIT 5
#define PLLM_0_BIT 0

#define RCC_CFGR_OFFSET 0x08
#define MCO2_1_BIT 31
#define MCO2_0_BIT 30

#define MCO2PRE_2_BIT 29
#define MCO2PRE_0_BIT 27

#define SWS_1_BIT 3
#define SWS_0_BIT 2

#define SW_1_BIT 1
#define SW_0_BIT 0

#define RCC_AHB1ENR_OFFSET 0x30
#define GPIO_EN_BIT(port) (port)

#define RCC_APB2ENR_OFFSET 0x44
#define SYSCFGEN_BIT 14
#define USART1EN 4

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

#define GPIOx_AFRL_OFFSET 0x20
#define AFRLy_3_BIT(y) ((y)*4 + 3)
#define AFRLy_0_BIT(y) ((y)*4)

//EXTI
#define EXTI_BASE 0x40013C00

#define EXTI_IMR_OFFSET 0x00

#define EXTI_RTSR_OFFSET 0x08

#define EXTI_FTSR_OFFSET 0x0C

#define EXTI_PR_OFFSET 0x14

//SYSCFG
#define SYSCFG_BASE 0x40013800

#define SYSCFG_EXTICR1_OFFSET 0x08

#define EXTI0_3_BIT 3
#define EXTI0_0_BIT 0

//USART1
#define USART1_BASE 0x40011000

#define USART_SR_OFFSET 0x00
#define TXE_BIT 7
#define RXNE_BIT 5

#define USART_DR_OFFSET 0x04

#define USART_BRR_OFFSET 0x08
#define DIV_MANTISSA_11_BIT 15
#define DIV_MANTISSA_0_BIT 4
#define DIV_FRACTION_3_BIT 3
#define DIV_FRACTION_0_BIT 0

#define USART_CR1_OFFSET 0x0C
#define UE_BIT 13
#define TE_BIT 3
#define RE_BIT 2

//NVIC
#define NVIC_ISER_BASE 0xE000E100

#define NVIC_ISERn_OFFSET(n) (0x00 + 4 * (n))

#endif
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.h

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/06/#-blinkh)

#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.c

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/06/#-blinkc1)

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> stm32f4.ld

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/06/#-stm32f4ld)

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

```makefile=
CROSS-COMPILER = arm-none-eabi-

all: usart.bin

usart.bin: main.c blink.c startup.c vector_table.s
	$(CROSS-COMPILER)gcc -std=c11 -Wall -mcpu=cortex-m4 -mthumb -nostartfiles -T stm32f4.ld main.c blink.c startup.c vector_table.s -o usart.elf
	$(CROSS-COMPILER)objcopy -O binary usart.elf usart.bin

flash:
	st-flash --reset write usart.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
```


### Build
1. Generate the .bin file with

    ```shell
    make
    ```

2. Connect your STM32F4DISCOVERY board, and flash by

    ```shell
    make flash
    ```

### Circuit Schematic

![](http://125.227.181.237/uploads/upload_de2ee1045228677bc2fe746d3588a303.png)


### Serial Console

1. Install screen:

    ```shell
    sudo apt-get install screen
    ```

2. Connect the USB to TTL module to the host computer and find the device name:

    ```shell
    ls /dev/ttyUSB*
    ```

3. Connect to the serial console: (replace ttyUSBx with yours)

    ```shell
    sudo screen /dev/ttyUSBx 115200
    ```

4. Press the reset button on your STM32F4DISCOVERY board. You should see the result. And then try type something in the console.

5. To leave screen, press `CTRL+A` and then press `k`.


-----------------------------------------------------------------------




## LAB08-2 USART Interrupt

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB08/2_usart_interrupt`</p>**

* Now, let's try to use interrupt for receiving data.

* We will send `Hello world` after the mcu is reset, and then **start to blink a led forever**.

* When the mcu receive data, the USART1 handler will read it and resend it out.

### Hints

1. Set **RXNEIE** bit in **USART_CR1** register to enable RXNE interrupt.

2. When RXNE is set by hardware, the USART1 interrupt will be triggered.

3. Remember to configure **NVIC**: [*ARM®v7-M Architecture Reference Manual*] ─ *B3.4 Nested Vectored Interrupt Controller, NVIC*

4. The IRQ number of the USART1 interrupt can be found at [*RM0090 Reference manual STM32F407*] ─ *Table 61. Vector table for STM32F405xx/07xx and STM32F415xx/17xx*

5. Setting RXNEIE will also enable the overrun error interrupt, so remember to handle the overrun error in your handler (the **ORE** bit in **USART_SR** register).


### Source Code

:::warning

Please copy your completed `main.c` in the previous lab to this lab directory, and then try to modify it to the interrupt version.

:::

#### <i class="fa fa-file-code-o" style="color:gray"></i> vector_table.s

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/07/#-vector_tables1)

#### <i class="fa fa-file-code-o" style="color:gray"></i> startup.c

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/07/#-startupc1)

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

#define READ_BIT(addr, bit) ((REG(addr) >> (bit)) & UINT32_1)

//FLASH
#define FLASH_BASE 0x40023C00

#define FLASH_ACR_OFFSET 0x00
#define PRFTEN_BIT 8
#define LATENCY_2_BIT 2
#define LATENCY_0_BIT 0

//RCC
#define RCC_BASE 0x40023800

#define RCC_CR_OFFSET 0x00
#define PLLRDY_BIT 25
#define PLLON_BIT 24
#define HSERDY_BIT 17
#define HSEON_BIT 16

#define RCC_PLLCFGR_OFFSET 0x04

#define PLLSRC_BIT 22

#define PLLP_1_BIT 17
#define PLLP_0_BIT 16

#define PLLN_8_BIT 14
#define PLLN_0_BIT 6

#define PLLM_5_BIT 5
#define PLLM_0_BIT 0

#define RCC_CFGR_OFFSET 0x08
#define MCO2_1_BIT 31
#define MCO2_0_BIT 30

#define MCO2PRE_2_BIT 29
#define MCO2PRE_0_BIT 27

#define SWS_1_BIT 3
#define SWS_0_BIT 2

#define SW_1_BIT 1
#define SW_0_BIT 0

#define RCC_AHB1ENR_OFFSET 0x30
#define GPIO_EN_BIT(port) (port)

#define RCC_APB2ENR_OFFSET 0x44
#define SYSCFGEN_BIT 14
#define USART1EN 4

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

#define GPIOx_AFRL_OFFSET 0x20
#define AFRLy_3_BIT(y) ((y)*4 + 3)
#define AFRLy_0_BIT(y) ((y)*4)

//EXTI
#define EXTI_BASE 0x40013C00

#define EXTI_IMR_OFFSET 0x00

#define EXTI_RTSR_OFFSET 0x08

#define EXTI_FTSR_OFFSET 0x0C

#define EXTI_PR_OFFSET 0x14

//SYSCFG
#define SYSCFG_BASE 0x40013800

#define SYSCFG_EXTICR1_OFFSET 0x08

#define EXTI0_3_BIT 3
#define EXTI0_0_BIT 0

//USART1
#define USART1_BASE 0x40011000

#define USART_SR_OFFSET 0x00
#define TXE_BIT 7
#define RXNE_BIT 5
#define ORE_BIT 3

#define USART_DR_OFFSET 0x04

#define USART_BRR_OFFSET 0x08
#define DIV_MANTISSA_11_BIT 15
#define DIV_MANTISSA_0_BIT 4
#define DIV_FRACTION_3_BIT 3
#define DIV_FRACTION_0_BIT 0

#define USART_CR1_OFFSET 0x0C
#define UE_BIT 13
#define RXNEIE_BIT 5
#define TE_BIT 3
#define RE_BIT 2

//NVIC
#define NVIC_ISER_BASE 0xE000E100

#define NVIC_ISERn_OFFSET(n) (0x00 + 4 * (n))

#endif
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.h

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/06/#-blinkh)

#### <i class="fa fa-file-code-o" style="color:gray"></i> blink.c

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/06/#-blinkc1)

### Linker Script

#### <i class="fa fa-file-text-o" style="color:gray"></i> stm32f4.ld

[**～～～**](http://www.nc.es.ncku.edu.tw/course/embedded/06/#-stm32f4ld)

### Makefile

#### <i class="fa fa-file-text-o" style="color:gray"></i> makefile

[**～～～**](#-makefile)


[*RM0090 Reference manual STM32F407*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/STM32F407_Reference_manual.pdf
[*UM1472 User manual Discovery kit with STM32F407VG MCU*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/STM32F4DISCOVERY.pdf
[*ARM®v7-M Architecture Reference Manual*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/armv7m.pdf




-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`