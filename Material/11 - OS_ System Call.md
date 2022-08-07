11 - OS: System Call
===

In this lecture, we will discuss how the hardware supports the OS feature ─ system call.

**<p style="color:crimson"><i class="fa fa-github"></i> Please fork the [LAB11](https://github.com/ybcsie/ESEmbedded_LAB11) repo, and then clone the repo you just forked.</p>**

**<p style="color:crimson">　 You will have to push the modified version to GitHub at the end of this lecture.</p>**


------------------------------------------------------------------------------------------------------------------------------


## SVC (Supervisor Call) Exception

* In many systems, the SVC mechanism can be used as an API to allow application tasks to access system resources.

![](http://125.227.181.237/uploads/upload_0cc433bbf2e3803ce4a7f4a74359e9a9.png)

* The SVC exception is generated using the **SVC instruction**.

* **An immediate value is required** for this instruction, which works as a parameter-passing method. For example:

    ```=
    SVC #0x3 ; Call SVC function 3
    ```

* The SVC exception handler can then extract the parameter and determine what action it needs to perform.

    ![](http://125.227.181.237/uploads/upload_02e4b86ef7249f58299b0f16bfcb5a47.png)

### Exception Handling Sequence Review

* Exception handling sequence example:

    ![](http://125.227.181.237/uploads/upload_bb0b31280982dc9138d7cfc1109f7bac.png)

* Stack frame review:

    * For the Cortex-M3 processor or Cortex-M4 processor without the floating point unit, the stack frames are always eight words.

    ![](http://125.227.181.237/uploads/upload_e4a58163d0da08bb070a00f90094b257.png)

* EXC_RETURN review:

    ![](http://125.227.181.237/uploads/upload_72a12469dbf378ea773c9135667449f4.png)

    * The valid values of the EXC_RETURN value is shown below.

    ![](http://125.227.181.237/uploads/upload_b8fafa305b08ce44cd1673cd780175a5.png)


### Extract the SVC Parameter

* The SVC handler can determine the immediate data value **in the SVC instruction** by reading the **stacked return address**.

    ![](http://125.227.181.237/uploads/upload_d8ac68d4af884d5fa452be0ef880cfbf.png)

* The program that executed the SVC could have either been using the **main stack** or the **process stack**. So we need to **find out which stack was used for the stacking process**.

* This can be determined from the link register value when the handler is entered (EXC_RETURN).

* The SVC parameter extraction sequences:

    :::warning

    **<br>Get EXC_RETURN value to check which SP was used (MSP or PSP)<br><br>**

    ::::

    <font size=4 color=orange>　　　　　　　　▼ </font>

    :::warning

    **<br>Get SP value (The lowest address of the stack frame)<br><br>**

    :::

    <font size=4 color=orange>　　　　　　　　▼ </font>

    :::warning

    **<br>Get stacked return address in the stack frame<br><br>**

    :::

    <font size=4 color=orange>　　　　　　　　▼ </font>

    :::warning

    **<br>Get the address of SVC instruction<br><br>**

    :::

    <font size=4 color=orange>　　　　　　　　▼ </font>

    :::warning

    **<br>Read the SVC instruction and extract the parameter<br><br>**

    :::



------------------------------------------------------------------------------------------------------------------------------



## LAB11-1 SVC Handler: Read LR Value


**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB11/1_svc_lr`</p>**

### SVC Handler

* To implement a SVC handler, we will first try to get the EXC_RETURN value stored in LR.

* In previous lecture, we used several assembly functions to read register values.

    Like `read sp`:

    ```=
    read_sp:
    	mov	r0,	sp
    	bx	lr
    ```

* If you come up with the same tricks to get the LR value, please think about the reason why the following method won't work correctly.

    ```=
    read_lr:
    	movs	r0,	lr
    	bx	lr
    ```

    ```clike=
    uint32_t read_lr(void);

    void svc_handler(void)
    {
    	uint32_t lr_value = read_lr();

    	// ...
    	// ...
    	// ...
    }
    ```

* We need to separate the SVC handler into two parts:

    1. The first part **passes the value of the LR (EXC_RETURN) to the second part as an input parameter**. This needs to be done in assembly because we need to **read the value of the LR immediately after the SVC handler is entered**, which cannot be done in C.

    2. The second part is the main portion of the SVC handler, which is written in C.

* The second part of the SVC handler in this lab will just print out some information of registers. We will complete it later.

    ![](http://125.227.181.237/uploads/upload_206a8fe94f725366241da0047dc3ed61.png)

* Assembly portion (the first part)

    ```=
    .type svc_handler, %function
    .global svc_handler
    svc_handler:
    	??????
    ```

* C portion (the second part)

    ```clike=
    void svc_handler_c(??????)
    {
    	printf("[SVC Handler] LR: 0x%X\r\n", (unsigned int)(??????);
    	printf("[SVC Handler] Control: 0x%X\r\n", (unsigned int)read_ctrl());
    	printf("[SVC Handler] SP: 0x%X \r\n", (unsigned int)read_sp());
    	printf("[SVC Handler] MSP: 0x%X \r\n", (unsigned int)read_msp());
    	printf("[SVC Handler] PSP: 0x%X \r\n\n", (unsigned int)read_psp());
    }
    ```

* Try to complete the `??????`.

### SVC in C

* In `asm_func.h`, declare the svc function:

    ```clike=
    int32_t sys_call(void);
    ```

* In `asm_func.s`, implement the function with:

    ```=
    .global	sys_call
    sys_call:
    	??????
    ```

* Try to complete the `??????`.

### System Call

* In the `main`  function, we have:

```clike=
int main(void)
{
	extern uint32_t _msp_init;

	uint32_t *msp_init = &_msp_init;
	uint32_t *psp_init = msp_init - 8 * 1024;

	init_usart1();

	printf("[Kernel] Start in privileged thread mode.\r\n\n");

	printf("[Kernel] Control: 0x%X \r\n", (unsigned int)read_ctrl());
	printf("[Kernel] SP: 0x%X \r\n", (unsigned int)read_sp());
	printf("[Kernel] MSP: 0x%X \r\n", (unsigned int)read_msp());
	printf("[Kernel] PSP: 0x%X \r\n\n", (unsigned int)read_psp());

	printf("[Kernel] SVC.\r\n\n");
	sys_call();
	printf("[Kernel] SVC return.\r\n\n");

	printf("[Kernel] Switch to unprivileged thread mode & start user task with psp.\r\n\n");

	//start user task
	start_user((uint32_t *)user_task, psp_init);

	while (1) //should not go here
		;
}
```

* In `user_task`:

```clike=
void user_task(void)
{
	printf("[User] Start in unprivileged thread mode.\r\n\n");

	printf("[User] Control: 0x%X \r\n", (unsigned int)read_ctrl());
	printf("[User] SP: 0x%X \r\n", (unsigned int)read_sp());
	printf("[User] MSP: 0x%X \r\n", (unsigned int)read_msp());
	printf("[User] PSP: 0x%X \r\n\n", (unsigned int)read_psp());

	printf("[User] SVC.\r\n\n");
	sys_call();
	printf("[User] SVC return.\r\n\n");

	blink(LED_BLUE); //should not return
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

3. Observe the output messages. You should notice the EXC_RETURN values are different (sys_call from thread mode using msp and from thread mode using psp).

------------------------------------------------------------------------------------------------------------------------------




## LAB11-2 SVC Handler: Extract the SVC Parameter

**<p style="color:crimson"><i class="fa fa-folder" style="color:orange"></i> The lab files are in `ESEmbedded_LAB11/2_svc_num`</p>**

:::warning

Please copy your completed `asm_func.s` in the previous lab (LAB11-1) to this lab directory, and then follow the steps below to modify it.

:::

### SVC Handling Flow

![](http://125.227.181.237/uploads/upload_5642f5cb53837c3555670a4346d84557.png)

* The SVC handler in assembly has to pass LR and MSP value to the C portion of the handler.

    * **LR**: Contains EXC_RETURN value, necessary for checking which SP was used.

    * **MSP**: The code in the C portion of the handler might use the main stack and change the value of MSP, so the original value of MSP has to be saved.

    ![](http://125.227.181.237/uploads/upload_2302a691367e16ad9625f12eaeda1b64.png)


### Stack Frame Extraction

* The **stacked return address** in the stack frame is necessary for the SVC parameter extraction.

* It is optional to extract other information like stacked register values.

* If `SP value` is known, it can be used as the base pointer for accessing the stack frame:

    ![](http://125.227.181.237/uploads/upload_876b1890fedfd67a8e763201b88a4192.png)


### SVC Parameter Extraction

* The SVC instruction can be read by referencing to the **stacked return address** like this:

    ```clike=
    uint16_t svc_instruction = *((uint16_t *)stacked_return_addr - 1);
    ```
    
    ![](http://125.227.181.237/uploads/upload_97434647267189bee03e9270773e9e18.png)

* The SVC exception handler needs to extract the SVC parameter and determine what action it needs to perform.

* The SVC parameter is **8-bit**, and the value itself does not affect the behavior of the SVC exception.

* The 8-bit parameter is encoded with the SVC instruction, so the SVC handler can get it like this:

    ```clike=
    uint8_t svc_num = (uint8_t)svc_instruction;
    ```

    ![](http://125.227.181.237/uploads/upload_74f65409a7ca58481d469d063ba063b7.png)




### Source Code

* In `asm_func.h`, we have an additional function declaration:

    ```clike=
    int32_t sys_call(void);
    int32_t sys_call_add(int32_t a, int32_t b);
    ```

* In `asm_func.s`, try to

    1. Implement the `sys_call_add` function with SVC number `0xA`

    2. Modify the `svc_handler` function to pass the `LR` value and the `MSP` value to the `svc_handler_c` function.

* In `main.c`, try to complete the `??????` in the `svc_handler_c` function:

```clike=
int main(void)
{
	extern uint32_t _msp_init;

	uint32_t *msp_init = &_msp_init;
	uint32_t *psp_init = msp_init - 8 * 1024;

	init_usart1();

	printf("[Kernel] Start in privileged thread mode.\r\n\n");

	printf("[Kernel] Control: 0x%X \r\n", (unsigned int)read_ctrl());
	printf("[Kernel] SP: 0x%X \r\n", (unsigned int)read_sp());
	printf("[Kernel] MSP: 0x%X \r\n", (unsigned int)read_msp());
	printf("[Kernel] PSP: 0x%X \r\n\n", (unsigned int)read_psp());

	printf("[Kernel] SVC 0.\r\n\n");
	printf("[Kernel] SVC return %d.\r\n\n", (int)sys_call());

	printf("[Kernel] Switch to unprivileged thread mode & start user task with psp.\r\n\n");

	//start user task
	start_user((uint32_t *)user_task, psp_init);

	while (1) //should not go here
		;
}

void user_task(void)
{
	printf("[User] Start in unprivileged thread mode.\r\n\n");

	printf("[User] Control: 0x%X \r\n", (unsigned int)read_ctrl());
	printf("[User] SP: 0x%X \r\n", (unsigned int)read_sp());
	printf("[User] MSP: 0x%X \r\n", (unsigned int)read_msp());
	printf("[User] PSP: 0x%X \r\n\n", (unsigned int)read_psp());

	printf("[User] SVC add.\r\n\n");
	printf("[User] SVC return %d.\r\n\n", (int)sys_call_add(8, 9));

	blink(LED_BLUE); //should not return
}

void svc_handler_c(??????, ??????)
{
	printf("[SVC Handler] LR: 0x%X\r\n", (unsigned int)??????);
	printf("[SVC Handler] MSP Backup: 0x%X \r\n", (unsigned int)??????);
	printf("[SVC Handler] Control: 0x%X\r\n", (unsigned int)read_ctrl());
	printf("[SVC Handler] SP: 0x%X \r\n", (unsigned int)read_sp());
	printf("[SVC Handler] MSP: 0x%X \r\n", (unsigned int)read_msp());
	printf("[SVC Handler] PSP: 0x%X \r\n\n", (unsigned int)read_psp());

	uint32_t *stack_frame_ptr;
	if (??????) //Test bit 2 of EXC_RETURN
	{
		stack_frame_ptr = ?????? //if 1, stacking used PSP
		printf("[SVC Handler] Stacking used PSP: 0x%X \r\n\n", (unsigned int)stack_frame_ptr);
	}
	else
	{
		stack_frame_ptr = ?????? //if 0, stacking used MSP
		printf("[SVC Handler] Stacking used MSP: 0x%X \r\n\n", (unsigned int)stack_frame_ptr);
	}

	uint32_t stacked_r0 = ??????
	uint32_t stacked_r1 = ??????
	uint32_t stacked_return_addr = ??????

	uint16_t svc_instruction = ??????
	uint8_t svc_num = ??????

	printf("[SVC Handler] Stacked R0: 0x%X \r\n", (unsigned int)stacked_r0);
	printf("[SVC Handler] Stacked R1: 0x%X \r\n", (unsigned int)stacked_r1);
	printf("[SVC Handler] SVC number: 0x%X \r\n\n", (unsigned int)svc_num);

	if (svc_num == 0xA)
		//return r0 + r1
		??????
	else
		//return 0
		??????
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


[USART Basic]: http://www.nc.es.ncku.edu.tw/course/embedded/08/#Circuit-Schematic




-------------------------------

###### tags: `ES EMBEDDED COURSE Handout` `STM32` `ARM`