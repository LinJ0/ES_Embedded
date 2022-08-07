10 - OS: User Mode and Kernel Mode
===

In this lecture, we will discuss how the hardware supports the OS feature ─ user mode and kernel mode.

Besides, to prevent non-privileged tasks from accessing certain memory regions, we are going to introduce the MPU.

**<p style="color:crimson"><i class="fa fa-github"></i> Please fork the [LAB10](https://github.com/ybcsie/ESEmbedded_LAB10) repo, and then clone the repo you just forked.</p>**

**<p style="color:crimson">　 You will have to push the modified version to GitHub at the end of this lecture.</p>**


------------------------------------------------------------------------------------------------------------------------------


## Privileged and Non-privileged Operation Modes

* The Cortex-M3 and Cortex-M4 processors are designed to support embedded OSs.

  They support the separation of privileged and non-privileged operation modes.

  (start in privileged mode by default)

* When an OS is used and **user tasks** are executed, the execution of user tasks can be carried out in non-privileged operation mode.

* Certain restrictions can be enforced under non-privileged operation mode, such as blocking access to some NVIC registers.

* The separation of privileged and non-privileged operation modes can also be used with the **MPU** to prevent non-privileged tasks from accessing certain memory regions.

  In this way a user task cannot corrupt data used by the OS kernel or other tasks, thus enhancing the system’s stability.

* Most simple applications do not require the use of non-privileged mode at all. But when building an embedded system that **requires high reliability**, the separation of privileged and non-privileged tasks may allow the system to continue operation even if a non-privileged task has failed.

* Operation states and modes:

    ![Operation states and modes](http://125.227.181.237/uploads/upload_d9fdf23ce8551fe4e6c27ac6894229c6.png)

    1. The processor enters Thread mode on Reset, or as a result of an exception return.
    1. The processor enters Handler mode as a result of an exception.
    1. Handler mode is always privileged.
    1. The processor can be either in privileged access level or unprivileged access level under thread mode.
    1. Thread mode can switch to using a separate shadowed Stack Pointer (SP). Again, this allows the stack memory for application tasks to be separated from the stack used by the OS kernel, thus allowing better system reliability.

    3. Non-privileged operation mode restrictions:

        * Limited access to the `MSR` and `MRS` instructions
        * Cannot use the `CPS` instruction
        * Cannot access the system timer, NVIC, or system control block
        * Might have restricted access to memory or peripherals


------------------------------------------------------------------------------------------------------------------------------


## Switch Between Privileged and Non-privileged Operation Modes

* Software can switch the processor in privileged Thread mode to unprivileged Thread mode.

* It cannot switch itself back from unprivileged to privileged. (has to use the exception mechanism to handle the switch - SVC)

* The processor's access level under thread mode is controlled by a special register called **`CONTROL`**.


### CONTROL Register

* Besides the registers in the register bank, there are a number of special registers.

* These registers contain the processor status and define the operation states and interrupt/exception masking.

![](http://125.227.181.237/uploads/upload_87542e968fb91eab88448f71281fbe48.png)

* In the development of simple applications with high level programming languages such as C, there are not many scenarios that require access to these registers. 

* However, they are needed for development of an embedded OS, or when advanced interrupt masking features are needed.

* **Special registers are not memory mapped**, and can be accessed using special register access instructions.

* The CONTROL register defines:
    1. The selection of stack pointer (Main Stack Point/Process Stack Pointer)
    2. Access level in Thread mode (Privileged/Unprivileged)
    3. For Cortex-M4 processor with a floating point unit, one bit of the `CONTROL` register indicates if the current context uses the floating point unit or not.

* The `CONTROL` register can only be modified in the privileged access level and can be read in both privileged and unprivileged access levels.

* The `CONTROL` register is a 2-bit or 3-bit register defined as follows:

|Bits |Name |Type|Reset|Description                                                                                                  |
|----:|:----|:--:|:---:|:------------------------------------------------------------------------------------------------------------|
|2    |FPCA |RW  |0    |Defines whether the FP extension is active in the current context if the processor includes the FP extension.|
|1    |SPSEL|RW  |0    |Defines the stack to be used.                                                                                |
|0    |nPRIV|RW  |0    |Defines the execution privilege in Thread mode.                                                              |

#### SPSEL

|Value|Description|
|:---:|:----------|
|0    |Use SP_main as the current stack.|
|1    |In Thread mode, use SP_process as the current stack. <br>In Handler mode, this value is reserved.|

![](http://125.227.181.237/uploads/upload_66cef4a0054c2a9580267dcaca48dc83.png)

#### nPRIV

|Value|Description                         |
|:---:|:-----------------------------------|
|0    |Thread mode has privileged access.  |
|1    |Thread mode has unprivileged access.|

* In Handler mode, execution is always privileged.

* A program in unprivileged access level cannot switch itself back to privileged access level. This is essential in order to provide a basic security usage model.

### Access the CONTROL Register

* When an embedded OS is used, the `CONTROL` register could be reprogrammed at each context switch to allow some application tasks to run with privileged access level and the others to run with unprivileged access level.

* In most simple applications without an embedded OS, there is no need to change the value of the `CONTROL` register. The whole application can run in privileged access level and use only the MSP.

* To access the `CONTROL` register in assembly, the **`MRS`** and **`MSR`** instructions are used.
    

### MRS and MSR Instructions

* Described in [*ARM®v7-M Architecture Reference Manual*] ─ B5.2 ARMv7-M system instruction descriptions.

* To access the `CONTROL` register for example:

    ```=
    MRS r0, CONTROL ; Read CONTROL register into R0
    MSR CONTROL, r0 ; Write R0 into CONTROL register
    ISB
    ```

* If unprivileged code attempts to read any **stack pointer**, the **priority masks**, or the **IPSR**, the read **returns zero**.

* The processor **ignores writes from unprivileged Thread mode** to any **stack pointer**, the **EPSR**, the **IPSR**, the **masks**, or **CONTROL**.

* After any Thread mode transition from privileged to unprivileged execution, software must issue an **`ISB`** instruction to ensure instruction fetch correctness.


------------------------------------------------------------------------------------------------------------------------------


## LAB10-1 Unprivileged

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB10/1_unprivileged`</p>**

### Source Code

#### <i class="fa fa-file-code-o" style="color:gray"></i> asm_func.h

```clike=
#ifndef ASM_FUNC_H
#define ASM_FUNC_H

#include <stdint.h>

uint32_t read_sp(void);   //return the value of sp
uint32_t read_psp(void);  //return the value of psp
uint32_t read_msp(void);  //return the value of msp
uint32_t read_ctrl(void); //return the value of control

void start_user(uint32_t *task_addr, uint32_t *psp_init); //start user task at task_addr in unprivileged mode and set sp to psp with value psp_init
void sw_priv(void);										  //try to switch to privileged mode from unprivileged by directly modifying the control register

#endif
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> asm_func.s

```=
.syntax unified

.global	read_sp
read_sp:
	??????

.global	read_msp
read_msp:
	??????

.global	read_psp
read_psp:
	??????

.global	read_ctrl
read_ctrl:
	??????

.global	start_user
start_user:
    ??????

.global	sw_priv
sw_priv:
    ??????

```

#### <i class="fa fa-file-code-o" style="color:gray"></i> main.c

```clike=
#include <stdint.h>
#include <stdio.h>
#include "reg.h"
#include "blink.h"
#include "usart.h"
#include "asm_func.h"

void user_task(void);

int main(void)
{
	extern uint32_t _msp_init;

	uint32_t *msp_init = &_msp_init;
	uint32_t *psp_init = msp_init - 8 * 1024;

	init_usart1();

	printf("[Kernel] Start in privileged thread mode (msp_init = 0x%x).\r\n\n", (unsigned int)msp_init);

	printf("[Kernel] Control: 0x%x \r\n", (unsigned int)read_ctrl());
	printf("[Kernel] SP: 0x%x \r\n", (unsigned int)read_sp());
	printf("[Kernel] MSP: 0x%x \r\n", (unsigned int)read_msp());
	printf("[Kernel] PSP: 0x%x \r\n\n", (unsigned int)read_psp());

	printf("[Kernel] Switch to unprivileged thread mode & start user task (psp_init = 0x%x).\r\n\n", (unsigned int)psp_init);

	//start user task
	??????

	while (1) //should not go here
		;
}

void user_task(void)
{
	printf("[User] Start in unprivileged thread mode.\r\n\n");

	printf("[User] Control: 0x%x \r\n", (unsigned int)read_ctrl());
	printf("[User] SP: 0x%x \r\n", (unsigned int)read_sp());
	printf("[User] MSP: 0x%x \r\n", (unsigned int)read_msp());
	printf("[User] PSP: 0x%x \r\n\n", (unsigned int)read_psp());

	printf("[User] Switch to privileged thread mode by writing the control register directly. (forbidden)\r\n\n");

	sw_priv();

	printf("[User] Control: 0x%x \r\n", (unsigned int)read_ctrl());
	printf("[User] SP: 0x%x \r\n", (unsigned int)read_sp());
	printf("[User] MSP: 0x%x \r\n", (unsigned int)read_msp());
	printf("[User] PSP: 0x%x \r\n\n", (unsigned int)read_psp());

	blink(LED_BLUE); //should not return
}

```

1. Try to implement the functions with assembly in `asm_func.s`.

2. Try to complete the `main.c`.

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
3. You should see the following results:

    ```shell
    [Kernel] Start in privileged thread mode (msp_init = 0x20020000).

    [Kernel] Control: 0x0
    [Kernel] SP: 0x2001ffc8
    [Kernel] MSP: 0x2001ffc8
    [Kernel] PSP: 0x0

    [Kernel] Switch to unprivileged thread mode & start user task (psp_init = 0x20018000).

    [User] Start in unprivileged thread mode.

    [User] Control: 0x3
    [User] SP: 0x20017ff8
    [User] MSP: 0x0
    [User] PSP: 0x0

    [User] Switch to privileged thread mode by writing the control register directly. (forbidden)

    [User] Control: 0x3
    [User] SP: 0x20017ff8
    [User] MSP: 0x0
    [User] PSP: 0x0
    ```


------------------------------------------------------------------------------------------------------------------------------


## Memory Protection Unit (MPU) Overview

* The MPU is a programmable device that can be used to define:
    * memory access permissions (e.g., privileged access only or full access)
    * memory attributes (e.g., bufferable, cacheable)

* The MPU on Cortex-M3 and Cortex-M4 processors can support up to eight programmable memory regions

* Each memory region has its own programmable starting addresses, sizes, and settings.

* The MPU can be used to make an embedded system more robust, and in some cases it can make the system more secure.

* In systems **without** an embedded OS, the MPU can be programmed to have a static configuration:
    * Setting a RAM/SRAM region to be **read-only** to protect important data from accidental corruption
    * Making a portion of RAM/SRAM space at the bottom of the stack inaccessible to **detect stack overflow**
    * Setting a RAM/SRAM region to be XN to **prevent code injection attacks**

* In systems **with** an embedded OS, the MPU can be programmed at each context switch so that each application task can have a different MPU configuration:
    * Define memory access permissions so that stack operations of an application task can only access their own allocated stack space, thus **preventing stack corruptions of other stacks in the case of a stack leak**
    * Define memory access permissions so that an application task can only have access to a **limited set of peripherals**
    * Define memory access permissions so that an application task can only access **its own data**, or access its own program data (these are much trickier to set up because in most cases the OS and the program code are compiled together, so the data could be mixed together in the memory map)




------------------------------------------------------------------------------------------------------------------------------




## MPU Registers

* The MPU contains a number of registers. These registers are located in the System Control Space (SCS).

* A summary of these registers is shown in the following table.

    |Address   |Name       |Type|Reset                 |Function                                        |
    |:--------:|:---------:|:--:|:--------------------:|:----------------------------------------------:|
    |0xE000ED90|MPU_TYPE   |RO  |IMPLEMENTATION DEFINED|Provides information about the MPU              |
    |0xE000ED94|MPU_CTRL   |RW  |0x00000000            |MPU enable/disable and background region control|
    |0xE000ED98|MPU_RNR    |RW  |UNKNOWN               |Select which MPU region to be configured        |
    |0xE000ED9C|MPU_RBAR   |RW  |UNKNOWN               |Defines base address of a MPU region            |
    |0xE000EDA0|MPU_RASR   |RW  |UNKNOWN               |Defines size and attributes of a MPU region     |
    |0xE000EDA4|MPU_RBAR_A1|RW  |-                     |Alias 1 of MPU_RBAR                             |
    |0xE000EDA8|MPU_RASR_A1|RW  |-                     |Alias 1 of MPU_RASR                             |
    |0xE000EDAC|MPU_RBAR_A2|RW  |-                     |Alias 2 of MPU_RBAR                             |
    |0xE000EDB0|MPU_RASR_A2|RW  |-                     |Alias 2 of MPU_RASR                             |
    |0xE000EDB4|MPU_RBAR_A3|RW  |-                     |Alias 3 of MPU_RBAR                             |
    |0xE000EDB8|MPU_RASR_A3|RW  |-                     |Alias 3 of MPU_RASR                             |

* See [*ARM®v7-M Architecture Reference Manual*] for more details.


### MPU Type Register (MPU_TYPE)

* The MPU Type Register indicates how many regions the MPU support.

* Software can use it to determine if the processor implements an MPU.


### MPU Control Register (MPU_CTRL)

* Enables the MPU

* When the MPU is enabled, controls whether the default memory map is enabled as a background region for privileged accesses

* Whether the MPU is enabled for HardFaults, NMIs.

![](http://125.227.181.237/uploads/upload_403ff5f1095687226003200dc31f31cf.png)

|Bits |Name      |Type|Reset|Description                                                  |
|----:|:---------|:--:|:---:|:------------------------------------------------------------|
|31:3 |-         |-   |-    |Reserved.                                                    |
|2    |PRIVDEFENA|RW  |0    |Privileged default memory map enable.                        |
|1    |HFNMIENA  |RW  |0    |Enables the MPU during the HardFault handler and NMI handler.|
|0    |ENABLE    |RW  |0    |Enables the MPU.                                             |


#### PRIVDEFENA

|Value|Description|
|:---:|:----------|
|0    |The background region is disabled and any access not covered by any enabled region will cause a fault.|
|1    |If the MPU is enabled, the default memory map will be used for privileged accesses as a background region.|

* The PRIVDEFENA bit in the MPU Control Register is used to **enable the background region (region -1)**.

* By using PRIVDEFENA and if no other regions are set up, privileged programs will be able to access all memory locations, and only unprivileged programs will be blocked.

* If other MPU regions are programmed and enabled, they can override the background region.

* Example:

    ![](http://125.227.181.237/uploads/upload_e497417d54ce8086315f928995bdba31.png)

#### HFNMIENA

|Value|Description|
|:---:|:----------|
|0    |The MPU is not enabled for the HardFault handler and NMI.|
|1    |Enables the MPU during the HardFault handler and NMI handler.|

* The HFNMIENA is used to define the behavior of the MPU during execution of NMI, HardFault handlers.

* By default, the MPU is disabled in these cases. This allows the HardFault handler and the NMI handler to execute even if the MPU was set up incorrectly.


#### ENABLE

|Value|Description|
|:---:|:----------|
|0    |The MPU is disabled.|
|1    |The MPU is enabled.|

* To enable the MPU, the software **should first set up the settings for each MPU region, and then set the ENABLE bit**.

* Setting the enable bit in the MPU Control register is **usually the last step in the MPU setup code**. Otherwise, the MPU might generate faults accidentally before the region configuration is done.

* In many cases, especially in embedded OSs with dynamic MPU configurations, **the MPU should be disabled at the start of the MPU configuration routine to make sure that the MemManage fault won’t be triggered by accident during configuration of MPU regions**.


### MPU Region Number Register (MPU_RNR)

* Before each region is set up, write to this register to select the region to be programmed.

![](http://125.227.181.237/uploads/upload_79a2458a60772a6b0718f215144211ee.png)

|Bits |Name      |Type|Reset|Description                                |
|----:|:---------|:--:|:---:|:------------------------------------------|
|31:8 |-         |-   |-    |Reserved.                                  |
|7:0  |REGION    |RW  |-    |Select the region that is being programmed.|


### MPU Region Base Address Register (MPU_RBAR)

* Define the **starting address** of each region.

* Using the VALID and REGION fields in this register, we can skip the step of programming the MPU Region Number register (MPU_RNR).

    This can reduce the complexity of the program code, especially if the whole MPU setup is defined in a lookup table.

![](http://125.227.181.237/uploads/upload_87b26cd000a6a040570bd3c50e06fe3b.png)

|Bits |Name  |Type|Reset|Description                                                                 |
|----:|:-----|:--:|:---:|:---------------------------------------------------------------------------|
|31:5 |ADDR  |RW  |-    |Base address of the region.                                                 |
|4    |VALID |RW  |-    |The REGION defined in bit[3:0] will be used in this programming step or not.|
|3:0  |REGION|RW  |-    |Overrides the MPU Region Number register (MPU_RNR) if VALID is 1.           |

* To help set up the `MPU_RBAR`, we define the following macros:

```clike=
#define UINT32_1 ((uint32_t)1)
#define REG(addr) (*((volatile uint32_t *)(addr)))

#define MPU_BASE 0xE000ED90
#define MPU_RBAR_OFFSET 0x0C

#define MPU_RBAR_VALUE(addr, region) (((uint32_t)(addr)) | (UINT32_1 << 4) | ((uint32_t)(region)))
```

* To set region 0 with `0x40000000` for example, we can skip the step of setting the `MPU_RNR` register and write:

```clike=
REG(MPU_BASE + MPU_RBAR_OFFSET) = MPU_RBAR_VALUE(0x40000000, 0);
```

### MPU Region Base Attribute and Size Register (MPU_RASR)

* Determine the size of the region

* Sub-region disable field is used to divide a region into eight equal sub-regions and then to define each as enabled or disabled.

* Defines the Access Permission (AP) of the region.

* An instruction fetch from this region is allowed or not.

* Memory attributes configurations.

![](http://125.227.181.237/uploads/upload_783790a89cd31c52726d353d9ca14638.png)


|Bits |Name                  |Type|Reset|Description        |
|----:|:---------------------|:--:|:---:|:------------------|
|31:29|-                     |-   |-    |Reserved.          |
|28   |XN                    |RW  |-    |Execute Never.     |
|27   |-                     |-   |-    |Reserved.          |
|26:24|AP                    |RW  |-    |Access Permission. |
|23:22|-                     |-   |-    |Reserved.          |
|21:16|MEMTYPE (TXE, S, C, B)|RW  |-    |Memory type.       |
|15:8 |SRD                   |RW  |-    |Sub-region disable.|
|7:6  |-                     |-   |-    |Reserved.          |
|5:1  |SIZE                  |RW  |-    |Region size.       |
|0    |ENABLE                |RW  |-    |Region enable.     |

* To help set up the `MPU_RASR`, we additionally define the following macros:

```clike=
#define MPU_RASR_OFFSET 0x10
#define MPU_RASR_VALUE(xn, ap, type, srd, size) (((uint32_t)(xn) << 28) | ((uint32_t)(ap) << 24) | ((uint32_t)(type) << 16) | ((uint32_t)(srd) << 8) | ((uint32_t)(size) << 1) | UINT32_1)
```


#### XN

|Value|Description                                                        |
|:---:|:------------------------------------------------------------------|
|0    |Execution of an instruction fetched from this region permitted.    |
|1    |Execution of an instruction fetched from this region not permitted.|

* When this field is set to 1, all instructions fetched from this region will generate a MemManage fault when they enter the execution stage.

* Define macros:

```clike=
#define MPU_XN_DISABLE 0
#define MPU_XN_ENABLE 1
```

#### AP

|Value|Privileged Access|User Access  |Description                                        |
|:---:|:----------------|:------------|:--------------------------------------------------|
|0b000|No access        |No access    |Any access generates a permission fault            |
|0b001|Read/Write       |No access    |Privileged access only                             |
|0b010|Read/Write       |Read-only    |Any unprivileged write generates a permission fault|
|0b011|Read/Write       |Read/Write   |Full access                                        |
|0b100|UNPREDICTABLE    |UNPREDICTABLE|Reserved                                           |
|0b101|Read-only        |No access    |Privileged read-only                               |
|0b110|Read-only        |Read-only    |Privileged and unprivileged read-only              |
|0b111|Read-only        |Read-only    |Privileged and unprivileged read-only              |

* Define macros:

```clike=
#define MPU_AP_NO_ACCESS 0b000
#define MPU_AP_PRIV_ACCESS 0b001
#define MPU_AP_NPRIV_RO 0b010
#define MPU_AP_FULL_ACCESS 0b011
#define MPU_AP_PRIV_RO 0b101
#define MPU_AP_RO 0b110
```

#### MEMTYPE

|Memory Type                  |Commonly Used Value  |
|:----------------------------|:-------------------:|
|ROM, flash (program memories)|0b000010             |
|Internal SRAM                |0b000110             |
|External RAM                 |0b000111             |
|Peripherals                  |0b000101             |

* Define macros:

```clike=
#define MPU_TYPE_FLASH 0b000010
#define MPU_TYPE_SRAM 0b000110
#define MPU_TYPE_EXRAM 0b000111
#define MPU_TYPE_PERIPHERALS 0b000101
```

#### SRD

* For any region of 256 bytes or larger, the MPU divides the region into eight equally-sized subregions.

* For region sizes of 32, 64, and 128 bytes, the effect of setting one or more bits of the SRD field to 1 is UNPREDICTABLE.

* Setting a bit in the SRD field to 1 disables the corresponding subregion:
    * The least significant bit of the field, `MPU_RASR[8]`, controls the subregion with the lowest address range.
    * The most significant bit of the field, `MPU_RASR[15]`, controls the subregion with the highest address range.

|Value|Description        |
|:---:|:------------------|
|0    |Subregion enabled. |
|1    |Subregion disabled.|


#### SIZE

* The region size, in bytes, is 2^(SIZE+1)^.

* SIZE field values less than 4 are reserved, because the smallest supported region size is 32 bytes.

![](http://125.227.181.237/uploads/upload_419a9d73ddbee56c34345d3a911e98b0.png)

* Define macros:

```clike=
#define MPU_REGION_SIZE_32B 0b00100
#define MPU_REGION_SIZE_64B 0b00101
#define MPU_REGION_SIZE_128B 0b00110
#define MPU_REGION_SIZE_256B 0b00111
#define MPU_REGION_SIZE_512B 0b01000
#define MPU_REGION_SIZE_1KB 0b01001
#define MPU_REGION_SIZE_2KB 0b01010
#define MPU_REGION_SIZE_4KB 0b01011
#define MPU_REGION_SIZE_8KB 0b01100
#define MPU_REGION_SIZE_16KB 0b01101
#define MPU_REGION_SIZE_32KB 0b01110
#define MPU_REGION_SIZE_64KB 0b01111
#define MPU_REGION_SIZE_128KB 0b10000
#define MPU_REGION_SIZE_256KB 0b10001
#define MPU_REGION_SIZE_512KB 0b10010
#define MPU_REGION_SIZE_1MB 0b10011
#define MPU_REGION_SIZE_2MB 0b10100
#define MPU_REGION_SIZE_4MB 0b10101
#define MPU_REGION_SIZE_8MB 0b10110
#define MPU_REGION_SIZE_16MB 0b10111
#define MPU_REGION_SIZE_32MB 0b11000
#define MPU_REGION_SIZE_64MB 0b11001
#define MPU_REGION_SIZE_128MB 0b11010
#define MPU_REGION_SIZE_256MB 0b11011
#define MPU_REGION_SIZE_512MB 0b11100
#define MPU_REGION_SIZE_1GB 0b11101
#define MPU_REGION_SIZE_2GB 0b11110
#define MPU_REGION_SIZE_4GB 0b11111
```


#### ENABLE

|Value|Description                                      |
|:---:|:------------------------------------------------|
|0    |When the MPU is enabled, this region is disabled.|
|1    |When the MPU is enabled, this region is enabled. |

* To disable current region, we can directly write `0` to `MPU_RASR`:

```clike=
REG(MPU_BASE + MPU_RASR_OFFSET) = 0;
```



------------------------------------------------------------------------------------------------------------------------------

## LAB10-2 MPU

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB10/2_mpu`</p>**

:::warning

Please copy your completed `asm_func.s` in the previous lab (LAB10-1) to this lab directory, and then follow the steps below to modify it.

:::

### Source Code

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
#define TC_BIT 6
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

//MPU
#define MPU_BASE 0xE000ED90

#define MPU_CTRL_OFFSET 0x04
#define MPU_PRIVDEFENA_BIT 2
#define MPU_ENABLE_BIT 0

#define MPU_RBAR_OFFSET 0x0C
#define MPU_RBAR_VALUE(addr, region) (((uint32_t)(addr)) | (UINT32_1 << 4) | ((uint32_t)(region)))

#define MPU_RASR_OFFSET 0x10
#define MPU_RASR_VALUE(xn, ap, type, srd, size) (((uint32_t)(xn) << 28) | ((uint32_t)(ap) << 24) | ((uint32_t)(type) << 16) | ((uint32_t)(srd) << 8) | ((uint32_t)(size) << 1) | UINT32_1)
#define MPU_XN_DISABLE 0
#define MPU_XN_ENABLE 1
#define MPU_AP_NO_ACCESS 0b000
#define MPU_AP_PRIV_ACCESS 0b001
#define MPU_AP_NPRIV_RO 0b010
#define MPU_AP_FULL_ACCESS 0b011
#define MPU_AP_PRIV_RO 0b101
#define MPU_AP_RO 0b110
#define MPU_TYPE_FLASH 0b000010
#define MPU_TYPE_SRAM 0b000110
#define MPU_TYPE_EXRAM 0b000111
#define MPU_TYPE_PERIPHERALS 0b000101
#define MPU_REGION_SIZE_32B 0b00100
#define MPU_REGION_SIZE_64B 0b00101
#define MPU_REGION_SIZE_128B 0b00110
#define MPU_REGION_SIZE_256B 0b00111
#define MPU_REGION_SIZE_512B 0b01000
#define MPU_REGION_SIZE_1KB 0b01001
#define MPU_REGION_SIZE_2KB 0b01010
#define MPU_REGION_SIZE_4KB 0b01011
#define MPU_REGION_SIZE_8KB 0b01100
#define MPU_REGION_SIZE_16KB 0b01101
#define MPU_REGION_SIZE_32KB 0b01110
#define MPU_REGION_SIZE_64KB 0b01111
#define MPU_REGION_SIZE_128KB 0b10000
#define MPU_REGION_SIZE_256KB 0b10001
#define MPU_REGION_SIZE_512KB 0b10010
#define MPU_REGION_SIZE_1MB 0b10011
#define MPU_REGION_SIZE_2MB 0b10100
#define MPU_REGION_SIZE_4MB 0b10101
#define MPU_REGION_SIZE_8MB 0b10110
#define MPU_REGION_SIZE_16MB 0b10111
#define MPU_REGION_SIZE_32MB 0b11000
#define MPU_REGION_SIZE_64MB 0b11001
#define MPU_REGION_SIZE_128MB 0b11010
#define MPU_REGION_SIZE_256MB 0b11011
#define MPU_REGION_SIZE_512MB 0b11100
#define MPU_REGION_SIZE_1GB 0b11101
#define MPU_REGION_SIZE_2GB 0b11110
#define MPU_REGION_SIZE_4GB 0b11111

//NVIC
#define NVIC_ISER_BASE 0xE000E100

#define NVIC_ISERn_OFFSET(n) (0x00 + 4 * (n))

#endif
```

#### <i class="fa fa-file-code-o" style="color:gray"></i> main.c

```clike=
#include <stdint.h>
#include <stdio.h>
#include "reg.h"
#include "blink.h"
#include "usart.h"
#include "asm_func.h"

void user_task(void);
void set_mpu(void);

int main(void)
{
	extern uint32_t _msp_init;

	uint32_t *msp_init = &_msp_init;
	uint32_t *psp_init = msp_init - 8 * 1024;

	init_usart1();

	printf("[Kernel] Start in privileged thread mode (msp_init = 0x%x).\r\n\n", (unsigned int)msp_init);

	printf("[Kernel] Control: 0x%x \r\n", (unsigned int)read_ctrl());
	printf("[Kernel] SP: 0x%x \r\n", (unsigned int)read_sp());
	printf("[Kernel] MSP: 0x%x \r\n", (unsigned int)read_msp());
	printf("[Kernel] PSP: 0x%x \r\n\n", (unsigned int)read_psp());

	set_mpu();

	printf("[Kernel] Switch to unprivileged thread mode & start user task (psp_init = 0x%x).\r\n\n", (unsigned int)psp_init);

	//start user task
	??????

	while (1) //should not go here
		;
}

void user_task(void)
{
	printf("[User] Try to print something.\r\n\n");
	blink(LED_BLUE); //should not return
}

void set_mpu(void)
{
	//set region 0: flash (0x00000000), 1MB, allow execution, full access, enable all subregion
	??????

	//set region 1: sram (0x20000000), 128KB, forbid execution, full access, enable all subregion
	??????

	//set region 2: RCC_AHB1ENR, 32B, forbid execution, full access, enable all subregion
	??????

	//set region 3: GPIOD, 32B, forbid execution, full access, enable all subregion
	??????

	//disable region 4 ~ 7
	for (??????)
	{
		??????
	}

	//enable the default memory map as a background region for privileged access (PRIVDEFENA)
	??????

	//enable mpu
	??????
}

```

1. We will allow the user task to:
    * access flash
    * access sram
    * enable GPIOD in RCC
    * blink LED (GPIOD)

    So using usart1 in the user task will cause hardfault (blink the red led).

2. Try to complete the `main.c`.


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
3. You should see the hardfault_handler being triggered without printing messages out from the user task.

4. Try remove the printf inside the user task. You should see the blue led blinking.

[*ARM®v7-M Architecture Reference Manual*]: http://www.nc.es.ncku.edu.tw/course/embedded/pdf/armv7m.pdf
[USART Basic]: http://www.nc.es.ncku.edu.tw/course/embedded/08/#Circuit-Schematic




-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`