12 - OS: Context Switch
===

In this lecture, we will discuss about systick and how to implement the OS feature ─ context switch.

**<p style="color:crimson"><i class="fa fa-github"></i> Please fork the [LAB12](https://github.com/ybcsie/ESEmbedded_LAB12) repo, and then clone the repo you just forked.</p>**

**<p style="color:crimson">　 You will have to push the modified version to GitHub at the end of this lecture.</p>**

------------------------------------------------------------------------------------------------------------------------------


## The System Timer, SysTick

* The Cortex-M3 and Cortex-M4 processors have a built-in system tick timer called SysTick

* It is integrated as a part of the NVIC and can generate the **SysTick exception**

* In modern operating systems, a periodic interrupt is needed to ensure that the OS kernel can invoke regularly;

    for example, for task management and context switching.

* The reason for having the timer inside the processor is to help software **portability**

* Since all the Cortex-M processors have the same SysTick timer, an OS written for one Cortex-M3/M4 microcontroller can be reused on other Cortex-M3/M4 microcontrollers.

* If an embedded OS is not needed in your application, the SysTick timer can be used as a simple timer peripheral for periodic interrupt generation, delay generation, or timing measurement.

* The counter inside the SysTick is a 24-bit decrement counter.

  It can decrement using the processor’s clock, or using a reference clock signal.

  The exact implementation of the reference clock signal depends on the microcontroller design. In some cases it might not be available.

* Clocks of STM32F407 are described in [*RM0090 Reference manual STM32F407*] section 7.2 (p.215). Figure 21. (p.216) shows the clock tree of the STM32F407

  ![](http://125.227.181.237/uploads/upload_d5efad3910032e2492ad5a0600b400c6.png)

  The RCC feeds the external clock of the Cortex System Timer (SysTick) with the AHB clock (HCLK) divided by 8.
  
  The SysTick can work either with this clock or with the Cortex clock (HCLK).

------------------

## System Timer Register

1. The registers in the SysTick timer can **only be accessed when in privileged state**.

2. The timer consists of four registers. These registers are located in the System Control Space (SCS).

3. A summary of these registers is shown in the following table.

    |Address   |Name         |Type|Reset        |Function                           |
    |:--------:|:-----------:|:--:|:-----------:|:---------------------------------:|
    |0xE000E010|[SYST_CSR]   |RW  |0x0000000x^★^|SysTick Control and Status Register|
    |0xE000E014|[SYST_RVR]   |RW  |UNKNOWN      |SysTick Reload Value Register      |
    |0xE000E018|[SYST_CVR]   |RW  |UNKNOWN      |SysTick Current Value Register     |
    |0xE000E01C|[SYST_CALIB] |RO  |IMP DEF      |SysTick Calibration value Register |

    ★ See register description for information about the reset value of SYST_CSR bit[2]. All other bits reset to 0.

[SYST_CSR]: #SysTick-Control-and-Status-Register-SYST_CSR
[SYST_RVR]: #SysTick-Reload-Value-Register-SYST_RVR
[SYST_CVR]: #SysTick-Current-Value-Register-SYST_CVR
[SYST_CALIB]: #SysTick-Calibration-value-Register-SYST_CALIB


### SysTick Control and Status Register (SYST_CSR)

* This configures the SysTick clock, enables the counter, enables the SysTick interrupt, and indicates the counter status.

![](http://125.227.181.237/uploads/upload_48d0f86724cc6f0406f4f01baf2cfd96.png)


|Bits |Name     |Type  |Reset|Description                                                                                     |
|----:|:--------|:----:|:---:|:-----------------------------------------------------------------------------------------------|
|31:17|-        |-     |-    |Reserved.                                                                                       |
|16   |[COUNTFLAG]|RO    |0    |Indicates whether the counter has counted to 0 since the last read of this register.            |
|15:3 |-        |-     |-    |Reserved.                                                                                       |
|2    |[CLKSOURCE]|RW^★^ |0^★^|Indicates the SysTick clock source                                                              |
|1    |[TICKINT]  |RW    |0    |Indicates whether counting to 0 causes the status of the SysTick exception to change to pending.|
|0    |[ENABLE]   |RW    |0    |Indicates the enabled status of the SysTick counter                                             |


★ If no external clock is provided, the CLKSOURCE bit reads as 1 and ignores writes.

[COUNTFLAG]: #COUNTFLAG
[CLKSOURCE]: #CLKSOURCE
[TICKINT]: #TICKINT
[ENABLE]: #ENABLE


#### COUNTFLAG

|Value|Description|
|:---:|:----------|
|0    |Timer has not counted to 0.|
|1    |Timer has counted to 0.|

* COUNTFLAG is set to 1 by a count transition from 1 to 0.

* COUNTFLAG is cleared to 0 by a software read of this register, and by any write to the Current Value register.

#### CLKSOURCE

|Value|Description|
|:---:|:----------|
|0    |SysTick uses the IMPLEMENTATION DEFINED external reference clock.|
|1    |SysTick uses the processor clock.|

* If no external clock is provided, this bit reads as 1 and ignores writes.

#### TICKINT

|Value|Description|
|:---:|:----------|
|0    |Count to 0 does not affect the SysTick exception status.|
|1    |Count to 0 changes the SysTick exception status to pending.|

* Changing the value of the counter to 0 by writing zero to the SysTick Current Value register to 0 never changes the status of the SysTick exception.

#### ENABLE

|Value|Description|
|:---:|:----------|
|0    |Counter is disabled.|
|1    |Counter is operating.|



### SysTick Reload Value Register (SYST_RVR)

* Holds the reload value of the SYST_CVR.

![](http://125.227.181.237/uploads/upload_ccc08ca6bb4d96073521d83e5cc57500.png)


|Bits |Name  |Type  |Reset  |Description                                                    |
|----:|:-----|:----:|:-----:|:--------------------------------------------------------------|
|31:24|-     |-     |-      |Reserved.                                                      |
|23:0 |RELOAD|RW    |UNKNOWN|The value to load into the SYST_CVR when the counter reaches 0.|

* When the counter reaches zero, it reloads the value in SYST_RVR on the next clock edge

* If you want to set the SysTick interval to 1000, you should set the reload value to 999.

### SysTick Current Value Register (SYST_CVR)

* Reads or clears the current counter value.

![](http://125.227.181.237/uploads/upload_0148fde62b1cec7f7d3d257a6ada36e8.png)


|Bits |Name     |Type  |Reset  |Description                                                   |
|----:|:--------|:----:|:-----:|:-------------------------------------------------------------|
|31:0 |CURRENT  |RW~c~ |UNKNOWN|This is the value of the counter at the time it is sampled.|

* W~c~ : Any write to the register clears the register to zero.

* Unsupported bits are read as zero ( bits[31:24] )

### SysTick Calibration value Register (SYST_CALIB)

* Provide a calibration value.

* If this information is available, the lowest 24 bits of this register provide the reload value required to get 10 msec SysTick intervals.

![](http://125.227.181.237/uploads/upload_788cdb5b0fbd597899ef1f545c4c83ce.png)


|Bits |Name     |Type  |Reset  |Description                                                                                     |
|----:|:--------|:----:|:-----:|:-----------------------------------------------------------------------------------------------|
|31   |[NOREF]    |RO    |IMP DEF|Indicates whether the IMPLEMENTATION DEFINED reference clock is implemented.                     |
|30   |[SKEW]     |RO    |IMP DEF|Indicates whether the 10ms calibration value is exact.                                          |
|29:24|-        |-     |-      |Reserved.                                                                                       |
|23:0 |[TENMS]    |RO    |IMP DEF|Calibration value for 10 ms.; chip designer should provide this value. |

[NOREF]: #NOREF
[SKEW]: #SKEW
[TENMS]: #TENMS



#### NOREF

|Value|Description                            |
|:---:|:--------------------------------------|
|0    |The reference clock is implemented.    |
|1    |The reference clock is not implemented.|

#### SKEW

|Value|Description                                                       |
|:---:|:-----------------------------------------------------------------|
|0    |10ms calibration value is exact.                                  |
|1    |10ms calibration value is inexact, because of the clock frequency.|


#### TENMS

* Optionally, holds a reload value to be used for 10ms (100Hz) timing, subject to system clock skew errors.

* Many microcontrollers do not have this information and the calibration value would read as zero.


--------------------------------------------

## LAB12-1 Systick

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB12/1_systick`</p>**


### Source Code


#### <i class="fa fa-file-code-o" style="color:gray"></i> reg.h

The definitions of SYSTICK are added

```clike=
//SYSTICK
#define SYST_BASE 0xE000E010

#define SYST_CSR_OFFSET 0x00
#define SYST_CLKSOURCE_BIT 2
#define SYST_TICKINT_BIT 1
#define SYST_ENABLE_BIT 0


#define SYST_RVR_OFFSET 0x04
#define SYST_RELOAD_23_BIT 23
#define SYST_RELOAD_0_BIT 0
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> main.c

We have the `main` function like this:

```clike=
int main(void)
{
	setup_systick(??????); //200 ms
	blink(LED_BLUE); //should not return
}

```

Try to finish the `setup_systick` function:

```clike=
void setup_systick(uint32_t ticks)
{
	// set reload value
	??????

	// uses external reference clock
	??????

	// enable systick exception
	??????

	// enable systick
	??????
}

```

Lastly, we have the `systick_handler` to blink the orange LED for one time:

```clike=
void systick_handler(void)
{
	blink_count(LED_ORANGE, 1);
}

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
3. Connect an oscilloscope to GND and the pin that is connected to the orange LED (you can find it out in blink.h or in STM32F4DISCOVERY datasheet).

4. Make sure the frequency is correct.


----------------------------------------------------------------


## A Simple OS

* In a typical system with an embedded OS, the processing time is divided into a number of time slots.

* For a system with only two tasks, the two tasks are executed alternatively, as shown in the figure below

    ![](http://125.227.181.237/uploads/upload_7957af2e9189d8f3b02d2746b84ea574.png)

* The execution of an OS kernel can be triggered by:

  * Execution of SVC instruction from application tasks. For example, when an application task is stalled because it is waiting for some data or event, it can call a system service to swap in another task.

  * Periodic SysTick exception.




------

## LAB12-2 Context Switch

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB12/2_contexswitch`</p>**

### Context Switch Sequence

![](http://125.227.181.237/uploads/upload_d1034192f78a34ad65d4df9c25898add.png)

### User Stack Initialization

![](http://125.227.181.237/uploads/upload_aeb6dcd9d5d00ea652706ffb10cc5f15.png)


### User Stacks in Memory

![](http://125.227.181.237/uploads/upload_ef5714aeb4b1a4e8eae43f3005e72245.png)


#### <i class="fa fa-file-code-o" style="color:gray"></i> asm_func.s

```=
.syntax unified

.global	read_ctrl
read_ctrl:
	mrs	r0,	control
	bx	lr

.global	start_user
start_user:
	movs	lr,	r0
	msr	psp,	r1

	movs	r3,	#0b11
	msr	control,	r3
	isb

	bx	lr

.type systick_handler, %function
.global systick_handler
systick_handler:
	//save lr (EXC_RETURN) to main stack
	??????

	//save r4-r11 to user stack
	mrs	r0,	psp
	stmdb	r0!,	{r4-r11}

	//pass psp of curr task by r0 and get psp of the next task
	bl	sw_task
	//psp of the next task is now in r0

	//restore r4~r11 from stack of the next task
	??????

	//modify psp
	??????

	//restore lr (EXC_RETURN)
	??????

	bx	lr

```

#### <i class="fa fa-file-code-o" style="color:gray"></i> main.c

```clike=
#include <stdint.h>
#include <stdio.h>
#include "reg.h"
#include "blink.h"
#include "usart.h"
#include "asm_func.h"

#define TASK_NUM 3
#define PSTACK_SIZE_WORDS 1024 //user stack size = 4 kB

static uint32_t *psp_array[TASK_NUM];

void setup_systick(uint32_t ticks);

void init_task(unsigned int task_id, uint32_t *task_addr, uint32_t *psp_init)
{
	??????	  //xPSR (bit 24, T bit, has to be 1 in Thumb state)
	?????? //Return Address is being initialized to the task entry
	psp_array[task_id] = ??????	//initialize psp_array (stack frame: 8 + r4 ~ r11: 8)
}

void task0(void)
{
	printf("[Task0] Start in unprivileged thread mode.\r\n\n");
	printf("[Task0] Control: 0x%x \r\n", (unsigned int)read_ctrl());

	blink(LED_BLUE); //should not return
}

void task1(void)
{
	printf("[Task1] Start in unprivileged thread mode.\r\n\n");
	printf("[Task1] Control: 0x%x \r\n", (unsigned int)read_ctrl());

	blink(LED_GREEN); //should not return
}

void task2(void)
{
	printf("[Task2] Start in unprivileged thread mode.\r\n\n");
	printf("[Task2] Control: 0x%x \r\n", (unsigned int)read_ctrl());

	blink(LED_ORANGE); //should not return
}

int main(void)
{
	init_usart1();

	uint32_t user_stacks[TASK_NUM][PSTACK_SIZE_WORDS];

	//init user tasks
	init_task(0, ??????, ??????);
	init_task(1, ??????, ??????);
	init_task(2, ??????, ??????);

	printf("[Kernel] Start in privileged thread mode.\r\n\n");

	printf("[Kernel] Setting systick...\r\n\n");
	setup_systick(168e6 / 8 / 100); //10 ms

	//start user task
	printf("[Kernel] Switch to unprivileged thread mode & start user task0 with psp.\r\n\n");
	start_user((uint32_t *)task0, user_stacks[0]);

	while (1) //should not go here
		;
}

void setup_systick(uint32_t ticks)
{
	// set reload value
	WRITE_BITS(SYST_BASE + SYST_RVR_OFFSET, SYST_RELOAD_23_BIT, SYST_RELOAD_0_BIT, ticks - 1);

	// uses external reference clock
	CLEAR_BIT(SYST_BASE + SYST_CSR_OFFSET, SYST_CLKSOURCE_BIT);

	// enable systick exception
	SET_BIT(SYST_BASE + SYST_CSR_OFFSET, SYST_TICKINT_BIT);

	// enable systick
	SET_BIT(SYST_BASE + SYST_CSR_OFFSET, SYST_ENABLE_BIT);
}

uint32_t *sw_task(uint32_t *psp)
{
	static unsigned int curr_task_id = 0;

	psp_array[curr_task_id] = psp; //save current psp

	if (++curr_task_id > TASK_NUM - 1) //get next task id
		curr_task_id = 0;

	?????? //return next psp
}

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

### Serial Console

1. The USB to TTL module should be connected (see the guides in lecture 08 - [USART Basic]).

2. Use screen to see the result: (**replace ttyUSBx with yours**)

    ```shell
    sudo screen /dev/ttyUSBx 115200
    ```




---------------------------------------------------------------

## Further Related Reading:

* Joseph Yiu, 《The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors, Third Edition》

    Chapter 10 ─ OS Support Features



[*RM0090 Reference manual STM32F407*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/STM32F407_Reference_manual.pdf

[USART Basic]: http://www.nc.es.ncku.edu.tw/course/embedded/08/#Circuit-Schematic


-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`