# Embedded - HW04 repo
###### tags: `2019_ES Embedded System Development and OS Design`
## Requirements
1. Please practice to reference the user manuals mentioned in Lecture 04, and try to use the user button (the blue one on the STM32F4DISCOVERY board).
2. After reset, the device starts to wait until the user button has been pressed, and then starts to blink the blue led forever.
3. Try to define a macro function READ_BIT(addr, bit) in reg.h for reading the value of the user button.
4. Push your repo to your github. (Use .gitignore to exclude the output files like object files, executable files, or binary files)


## Push buttons on the STM32F4DISCOVERY
### Step1. Push buttons on the STM32F4DISCOVERY
> UM1472 User manual
> Discovery kit with STM32F407VG MCU

In order to use the push buttons (B1 USER), we have to control the GPIO pin PA0 (0th pin of GPIOA).
![](https://i.imgur.com/Rtpb2VP.png)

In addition, we have to figure out PA0 default voltage without pushing button is VDD or GND. 
UM1472 User manual shows that the PA0 default to pull down, so set up GPIO port pull-up/pull-down register (in Step4-4) to PD.
![](https://i.imgur.com/6H56lUK.png)

> RM0090 Reference manual
> STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439 advanced Arm®-based 32-bit MCUs

### Step2. Memory map of peripherals - GPIO
The base address of **GPIOA** is 0x40020000 and the details are described in section 8(p.287).
In addition, we will have to setup **RCC(Reset and clock control) register** to enable the clock for GPIOA.

*2.3 Memory map*

![](https://i.imgur.com/UIncs5A.png)

### Step3. RCC - GPIO Enable
*7.3.24 RCC register map*

![](https://i.imgur.com/CHjcolc.png)


![](https://i.imgur.com/CAWfErp.png)

### Step4. GPIO Configurations
Section 8.3 described that each of the GPIOs has four 32-bit memory-mapped control registers:
- MODER
- OTYPER
- OSPEEDR
- PUPDR

*8.3 GPIO functional description*

![](https://i.imgur.com/jebkSTn.png)
![](https://i.imgur.com/sdAqRMF.png)

We want the pin GPIOA0 to be **input pull down**, so we have to configure the GPIOA:

- MODER(0) = 00 (input)
- OTYPER(0) = x
- OSPEEDER(0) = xx
- PUPDR(0) = 10 (pull down)


![](https://i.imgur.com/0cAcQjh.png)
![](https://i.imgur.com/i1Zrfjg.png)

#### Step4-1. `MODER(0)` = `00`
![](https://i.imgur.com/dK4hrB6.png)

#### Step4-2. `OTYPER(0)` = `x`
![](https://i.imgur.com/08DcA6q.png)

#### Step4-3. `OSPEEDER(0)` = `xx`
![](https://i.imgur.com/AF3K3GC.png)

#### Step4-4. `PUPDR(0)` = `10`
![](https://i.imgur.com/wsyA3MR.png)
![](https://i.imgur.com/X57We7b.png)

#### Step4-5. GPIO input control
GPIOx_IDR is read-only.

![](https://i.imgur.com/5zfWhid.png)

## Methods
### reg.h
```c=
#ifndef REG_H
#define REG_H

//REG OPERATIONS

#define UINT32_1 ((uint32_t)1)

#define REG(addr) (*((volatile uint32_t *)(addr)))

#define SET_BIT(addr, bit) (REG(addr) |= UINT32_1 << (bit))
#define CLEAR_BIT(addr, bit) (REG(addr) &= ~(UINT32_1 << (bit)))
#define READ_BIT(addr, bit) (REG(addr) & (UINT32_1 << (bit)))

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

#define GPIOx_IDR_OFFSET 0x10
#define IDRy_BIT(y) (y)

#define GPIOx_ODR_OFFSET 0x14
#define ODRy_BIT(y) (y)

#define GPIOx_BSRR_OFFSET 0x18
#define BRy_BIT(y) ((y) + 16)
#define BSy_BIT(y) (y)

#endif
```
- REG OPERATIONS
 I implement read bit by `(bit) & 1` operation.
```
#define READ_BIT(addr, bit) (REG(addr) & (UINT32_1 << (bit)))
```
- GPIO 
Add GPIOx_IDR register and GPIOx_ODR register.
```
#define GPIOx_IDR_OFFSET 0x10 
#define IDRy_BIT(y) (y)

#define GPIOx_ODR_OFFSET 0x14
#define ODRy_BIT(y) (y)
```

### button.h
```c=
#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_USER 0

unsigned int button_read(unsigned int button);

#endif
```

### button.c
```c=
#include <stdint.h>
#include "reg.h"

//button init
void button_init(unsigned int button)
{
    SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));

    //MODER button pin = 00 => General purpose input mode
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(button));
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(button));

    //OT button pin = x => xx

    //OSPEEDR button pin = 00 => Low speed
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(button));
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(button));

    //PUPDR button pin = 10 => Pull-down
    SET_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(button));
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(button));
}

unsigned int button_read(unsigned int button)
{
    button_init(button);

    if(READ_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_IDR_OFFSET, IDRy_BIT(button)))
        return 1;
    else    
        return 0;
}
```

### main.c
```c=
#include "blink.h"
#include "button.h"

int main(void)
{   
    unsigned int led_flag = 0;
    unsigned int pre_state = 0;

    for(;;pre_state = button_read(BUTTON_USER)){
        (led_flag)? blink_count(LED_BLUE, 1) : blink_count(LED_ORANGE, 1);
        led_flag ^= (pre_state == button_read(BUTTON_USER))? 0 : button_read(BUTTON_USER);
    }
}
```
## Reference
- UM1472 User manual - Discovery kit with STM32F407VG MCU
- RM0090 Reference manual - STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439 advanced Arm®-based 32-bit MCUs
