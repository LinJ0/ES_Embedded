# Embedded - HW03 repo
###### tags: `2019_ES Embedded System Development and OS Design`
## Requirements
1. **How do C functions pass and return parameters?** Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.
2. Modify **main.c** to observe what you found.
3. You have to state how you designed the observation (code), and how you performed it.Just like how you did in HW02.
4. If there are any official data that define the rules, you can also use them as references.
5. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

**Keywords:**
Calling Conventions
AAPCS - Procedure Call Standard for Arm Architecture

> 參數(Parameter)：建立函式時，預設帶入的變數。
> 引數(Argument)：呼叫函式時，傳給該函式的參數的值。

## Methods
### main.c
```c=
int int_1w1 = 111;
int int_1w2 = 222;
int int_1w3 = 333;
int int_1w4 = 444;
int int_1w5 = 555;
float float_1w = 1111.2222;
double double_2w = 3333.4444;


int callee_int(int i1, int i2, int i3, int i4, int i5)
{
    return (i1 + i2 + i3 + i4 + i5);
}
float callee_float(float f){return f;}
double callee_double(double d){return d;}


void reset_handler(void)
{
    int temp1;
    float temp2;
    double temp3;

    temp1 = callee_int(int_1w1, int_1w2, int_1w3, int_1w4, int_1w5);
    temp2 = callee_float(float_1w);
    temp3 = callee_double(double_2w);
    while (1);
}
```
## Result
### The result from objdump
```
Disassembly of section .mytext:

00000000 <callee_int-0x8>:
   0:	20000100 	andcs	r0, r0, r0, lsl #2
   4:	00000061 	andeq	r0, r0, r1, rrx

00000008 <callee_int>:
   8:	b480      	push	{r7}
   a:	b085      	sub	sp, #20
   c:	af00      	add	r7, sp, #0
   e:	60f8      	str	r0, [r7, #12]
  10:	60b9      	str	r1, [r7, #8]
  12:	607a      	str	r2, [r7, #4]
  14:	603b      	str	r3, [r7, #0]
  16:	68fa      	ldr	r2, [r7, #12]
  18:	68bb      	ldr	r3, [r7, #8]
  1a:	441a      	add	r2, r3
  1c:	687b      	ldr	r3, [r7, #4]
  1e:	441a      	add	r2, r3
  20:	683b      	ldr	r3, [r7, #0]
  22:	441a      	add	r2, r3
  24:	69bb      	ldr	r3, [r7, #24]
  26:	4413      	add	r3, r2
  28:	4618      	mov	r0, r3
  2a:	3714      	adds	r7, #20
  2c:	46bd      	mov	sp, r7
  2e:	bc80      	pop	{r7}
  30:	4770      	bx	lr

00000032 <callee_float>:
  32:	b480      	push	{r7}
  34:	b083      	sub	sp, #12
  36:	af00      	add	r7, sp, #0
  38:	6078      	str	r0, [r7, #4]
  3a:	687b      	ldr	r3, [r7, #4]
  3c:	4618      	mov	r0, r3
  3e:	370c      	adds	r7, #12
  40:	46bd      	mov	sp, r7
  42:	bc80      	pop	{r7}
  44:	4770      	bx	lr

00000046 <callee_double>:
  46:	b480      	push	{r7}
  48:	b083      	sub	sp, #12
  4a:	af00      	add	r7, sp, #0
  4c:	e9c7 0100 	strd	r0, r1, [r7]
  50:	e9d7 2300 	ldrd	r2, r3, [r7]
  54:	4610      	mov	r0, r2
  56:	4619      	mov	r1, r3
  58:	370c      	adds	r7, #12
  5a:	46bd      	mov	sp, r7
  5c:	bc80      	pop	{r7}
  5e:	4770      	bx	lr

00000060 <reset_handler>:
  60:	b590      	push	{r4, r7, lr}
  62:	b087      	sub	sp, #28
  64:	af02      	add	r7, sp, #8
  66:	4b0f      	ldr	r3, [pc, #60]	; (a4 <reset_handler+0x44>)
  68:	6818      	ldr	r0, [r3, #0]
  6a:	4b0f      	ldr	r3, [pc, #60]	; (a8 <reset_handler+0x48>)
  6c:	6819      	ldr	r1, [r3, #0]
  6e:	4b0f      	ldr	r3, [pc, #60]	; (ac <reset_handler+0x4c>)
  70:	681a      	ldr	r2, [r3, #0]
  72:	4b0f      	ldr	r3, [pc, #60]	; (b0 <reset_handler+0x50>)
  74:	681c      	ldr	r4, [r3, #0]
  76:	4b0f      	ldr	r3, [pc, #60]	; (b4 <reset_handler+0x54>)
  78:	681b      	ldr	r3, [r3, #0]
  7a:	9300      	str	r3, [sp, #0]
  7c:	4623      	mov	r3, r4
  7e:	f7ff ffc3 	bl	8 <callee_int>
  82:	60f8      	str	r0, [r7, #12]
  84:	4b0c      	ldr	r3, [pc, #48]	; (b8 <reset_handler+0x58>)
  86:	681b      	ldr	r3, [r3, #0]
  88:	4618      	mov	r0, r3
  8a:	f7ff ffd2 	bl	32 <callee_float>
  8e:	60b8      	str	r0, [r7, #8]
  90:	4b0a      	ldr	r3, [pc, #40]	; (bc <reset_handler+0x5c>)
  92:	e9d3 2300 	ldrd	r2, r3, [r3]
  96:	4610      	mov	r0, r2
  98:	4619      	mov	r1, r3
  9a:	f7ff ffd4 	bl	46 <callee_double>
  9e:	e9c7 0100 	strd	r0, r1, [r7]
  a2:	e7fe      	b.n	a2 <reset_handler+0x42>
  a4:	000000c0 	andeq	r0, r0, r0, asr #1
  a8:	000000c4 	andeq	r0, r0, r4, asr #1
  ac:	000000c8 	andeq	r0, r0, r8, asr #1
  b0:	000000cc 	andeq	r0, r0, ip, asr #1
  b4:	000000d0 	ldrdeq	r0, [r0], -r0	; <UNPREDICTABLE>
  b8:	000000d4 	ldrdeq	r0, [r0], -r4
  bc:	000000d8 	ldrdeq	r0, [r0], -r8

Disassembly of section .data:

000000c0 <int_1w1>:
  c0:	0000006f 	andeq	r0, r0, pc, rrx

000000c4 <int_1w2>:
  c4:	000000de 	ldrdeq	r0, [r0], -lr

000000c8 <int_1w3>:
  c8:	0000014d 	andeq	r0, r0, sp, asr #2

000000cc <int_1w4>:
  cc:	000001bc 			; <UNDEFINED> instruction: 0x000001bc

000000d0 <int_1w5>:
  d0:	0000022b 	andeq	r0, r0, fp, lsr #4

000000d4 <float_1w>:
  d4:	448ae71c 	strmi	lr, [sl], #1820	; 0x71c

000000d8 <double_2w>:
  d8:	886594af 	stmdahi	r5!, {r0, r1, r2, r3, r5, r7, sl, ip, pc}^
  dc:	40aa0ae3 	adcmi	r0, sl, r3, ror #21

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <double_2w+0x10d0c4c>
   4:	4e472820 	cdpmi	8, 4, cr2, cr7, cr0, {1}
   8:	72412055 	subvc	r2, r1, #85	; 0x55
   c:	6d45206d 	stclvs	0, cr2, [r5, #-436]	; 0xfffffe4c
  10:	64646562 	strbtvs	r6, [r4], #-1378	; 0xfffffa9e
  14:	54206465 	strtpl	r6, [r0], #-1125	; 0xfffffb9b
  18:	636c6f6f 	cmnvs	ip, #444	; 0x1bc
  1c:	6e696168 	powvsez	f6, f1, #0.0
  20:	2e303120 	rsfcssp	f3, f0, f0
  24:	30322d33 	eorscc	r2, r2, r3, lsr sp
  28:	312e3132 			; <UNDEFINED> instruction: 0x312e3132
  2c:	31202930 			; <UNDEFINED> instruction: 0x31202930
  30:	2e332e30 	mrccs	14, 1, r2, cr3, cr0, {1}
  34:	30322031 	eorscc	r2, r2, r1, lsr r0
  38:	38303132 	ldmdacc	r0!, {r1, r4, r5, r8, ip, sp}
  3c:	28203432 	stmdacs	r0!, {r1, r4, r5, sl, ip, sp}
  40:	656c6572 	strbvs	r6, [ip, #-1394]!	; 0xfffffa8e
  44:	29657361 	stmdbcs	r5!, {r0, r5, r6, r8, r9, ip, sp, lr}^
	...

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00002b41 	andeq	r2, r0, r1, asr #22
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	00000021 	andeq	r0, r0, r1, lsr #32
  10:	2d453705 	stclcs	7, cr3, [r5, #-20]	; 0xffffffec
  14:	0d06004d 	stceq	0, cr0, [r6, #-308]	; 0xfffffecc
  18:	02094d07 	andeq	r4, r9, #448	; 0x1c0
  1c:	01140412 	tsteq	r4, r2, lsl r4
  20:	03170115 	tsteq	r7, #1073741829	; 0x40000005
  24:	011a0118 	tsteq	sl, r8, lsl r1
  28:	0122061e 			; <UNDEFINED> instruction: 0x0122061e
```
## Discussion
I will disscus these parts below：
1. global variables - `.data` & `.mytext` section
2. how does function passing arguments & return values
    - callee_int() - `.mytext <reset_handler>` & `.mytext <callee_int>`
    - callee_float() - `.mytext <reset_handler>` & `.mytext <callee_float>`
    - callee_double() - `.mytext <reset_handler>` & `.mytext <callee_double>`
### Global variables
The value of global variables are saved in `.data` section from `c0` to `dc`.
The address of global variables are saved in the end of `.mytext` section from `a4` to `bc`.
**main.c**
```c
int int_1w1 = 111;
int int_1w2 = 222;
int int_1w3 = 333;
int int_1w4 = 444;
int int_1w5 = 555;
float float_1w = 1111.2222;
double double_2w = 3333.4444;
...
```
objdump - **.data**
```Thumb2
Disassembly of section .data:

000000c0 <int_1w1>://0x6f = 1111
  c0:	0000006f 	andeq	r0, r0, pc, rrx 

000000c4 <int_1w2>://0xde = 222
  c4:	000000de 	ldrdeq	r0, [r0], -lr 

000000c8 <int_1w3>://0x14d = 333
  c8:	0000014d 	andeq	r0, r0, sp, asr #2 

000000cc <int_1w4>://0x1bc = 444       
  cc:	000001bc 			; <UNDEFINED> instruction: 0x000001bc

000000d0 <int_1w5>://0x22b = 555
  d0:	0000022b 	andeq	r0, r0, fp, lsr #4

000000d4 <float_1w>:
  d4:	448ae71c 	strmi	lr, [sl], #1820	; 0x71c

000000d8 <double_2w>:
  d8:	886594af 	stmdahi	r5!, {r0, r1, r2, r3, r5, r7, sl, ip, pc}^
  dc:	40aa0ae3 	adcmi	r0, sl, r3, ror #21
```
objdump - **.mytext <reset_handler>**
```Thumb2
  a4:	000000c0 	andeq	r0, r0, r0, asr #1
  a8:	000000c4 	andeq	r0, r0, r4, asr #1
  ac:	000000c8 	andeq	r0, r0, r8, asr #1
  b0:	000000cc 	andeq	r0, r0, ip, asr #1
  b4:	000000d0 	ldrdeq	r0, [r0], -r0	; <UNPREDICTABLE>
  b8:	000000d4 	ldrdeq	r0, [r0], -r4
  bc:	000000d8 	ldrdeq	r0, [r0], -r8
```

### function - callee_int()
**main.c**
```c
int int_1w1 = 111;
int int_1w2 = 222;
int int_1w3 = 333;
int int_1w4 = 444;
int int_1w5 = 555;
...

int callee_int(int i1, int i2, int i3, int i4, int i5)
{
    return (i1 + i2 + i3 + i4 + i5);
}
...

void reset_handler(void)
{
    int temp1;
    ...
    temp1 = callee_int(int_1w1, int_1w2, int_1w3, int_1w4, int_1w5);
    ...
    while (1);
}
```
objdump - **.mytext <reset_handler>**
```Thumb2
  66:	4b0f      	ldr	r3, [pc, #60]	; (a4 <reset_handler+0x44>)
  68:	6818      	ldr	r0, [r3, #0]    
  6a:	4b0f      	ldr	r3, [pc, #60]	; (a8 <reset_handler+0x48>)
  6c:	6819      	ldr	r1, [r3, #0]
  6e:	4b0f      	ldr	r3, [pc, #60]	; (ac <reset_handler+0x4c>)
  70:	681a      	ldr	r2, [r3, #0]
  72:	4b0f      	ldr	r3, [pc, #60]	; (b0 <reset_handler+0x50>)
  74:	681c      	ldr	r4, [r3, #0]
  76:	4b0f      	ldr	r3, [pc, #60]	; (b4 <reset_handler+0x54>)
  78:	681b      	ldr	r3, [r3, #0]
  7a:	9300      	str	r3, [sp, #0]
  7c:	4623      	mov	r3, r4
  7e:	f7ff ffc3 	bl	8 <callee_int>
  82:	60f8      	str	r0, [r7, #12]
```
R0 to R3 is used to input parameters. if over 4 parameters, the remaining parameters would be stored in stack. 
`66` - `78`：Load global variable `int_1w1`, `int_1w2`, `int_1w3`, `int_1w4`, `int_1w5` to R0, R1, R2, R4, R3.
`7a`：Store R3 to stack
`7c`：Move R4 to R3
The five parameters (i.e. `int_1w1` to `int_1w5`) are respectively saved in R0, R1, R2, R3 and end of stack.
`7e`：Call function callee_int(). After return from callee_int(), the return value is in R0.
`82`：[r7, #12] catch the return value from R0


objdump - **.mytext <callee_int>**
```Thumb2
00000008 <callee_int>:
   8:	b480      	push	{r7}
   a:	b085      	sub	sp, #20
   c:	af00      	add	r7, sp, #0
   e:	60f8      	str	r0, [r7, #12]
  10:	60b9      	str	r1, [r7, #8]
  12:	607a      	str	r2, [r7, #4]
  14:	603b      	str	r3, [r7, #0]
  16:	68fa      	ldr	r2, [r7, #12]
  18:	68bb      	ldr	r3, [r7, #8]
  1a:	441a      	add	r2, r3
  1c:	687b      	ldr	r3, [r7, #4]
  1e:	441a      	add	r2, r3
  20:	683b      	ldr	r3, [r7, #0]
  22:	441a      	add	r2, r3
  24:	69bb      	ldr	r3, [r7, #24]
  26:	4413      	add	r3, r2
  28:	4618      	mov	r0, r3
  2a:	3714      	adds	r7, #20
  2c:	46bd      	mov	sp, r7
  2e:	bc80      	pop	{r7}
  30:	4770      	bx	lr
```
`08` - `0c`：Reserves 5 word space in stack. R7 is a base to store and load arguments in stack.
`0c`：R7 = Address of SP. R7 is a base to store and load arguments.
`0e` - `14`：Store arguments `int_1w1` ~ `int_1w4` to stack.
`16` - `22`：Load arguments `int_1w1` ~ `int_1w4` and do some operation between R2 & R3.
`24`：Load the 5th arguments `int_1w5` from stack.
`26`：The final result is in R3.
`28`：Function return value is store in R0, so move the final result from R3 to R0.
`2a` - `30`：Restore stack and return to reset_handler.


### function - callee_float()
**main.c**
```c
...
float float_1w = 1111.2222;
...
float callee_float(float f){return f;}
...
void reset_handler(void)
{
    ...
    float temp2;
    ...
    temp2 = callee_float(float_1w);
    ...
    while (1);
}
```

objdump - **.mytext <reset_handler>**
```Thumb2
  84:	4b0c      	ldr	r3, [pc, #48]	; (b8 <reset_handler+0x58>)
  86:	681b      	ldr	r3, [r3, #0]
  88:	4618      	mov	r0, r3
  8a:	f7ff ffd2 	bl	32 <callee_float>
  8e:	60b8      	str	r0, [r7, #8]
```
`84` - `86`：Load global variable `float_1w` to R3.
`88`：Pass argument to R0.
`8a`：Call function callee_float(). After return from callee_float(), the return value is in R0.
`8e`：[r7, #8] catch the return value from R0.

objdump - **.mytext <callee_float>**
```Thumb2
00000032 <callee_float>:
  32:	b480      	push	{r7}
  34:	b083      	sub	sp, #12
  36:	af00      	add	r7, sp, #0
  38:	6078      	str	r0, [r7, #4]
  3a:	687b      	ldr	r3, [r7, #4]
  3c:	4618      	mov	r0, r3
  3e:	370c      	adds	r7, #12
  40:	46bd      	mov	sp, r7
  42:	bc80      	pop	{r7}
  44:	4770      	bx	lr
```
`32` - `36`：Reserves 3 word space in stack. R7 is a base to store and load arguments in stack.
`38`：Store arguments `float_1w` into stack.
`3a`：Load arguments `float_1w` to R3.
`3c`：Function return value is store in R0, so move the final result from R3 to R0.
`3e` - `44`：Restore stack and return to reset_handler.



### function - callee_double()
**main.c**
```c
...
double double_2w = 3333.4444;
..
double callee_double(double d){return d;}

void reset_handler(void)
{
    ...
    double temp3;
    ...
    temp3 = callee_double(double_2w);
    while (1);
}
```

objdump - **.mytext <reset_handler>**
```Thumb2
  90:	4b0a      	ldr	r3, [pc, #40]	; (bc <reset_handler+0x5c>)
  92:	e9d3 2300 	ldrd	r2, r3, [r3]
  96:	4610      	mov	r0, r2
  98:	4619      	mov	r1, r3
  9a:	f7ff ffd4 	bl	46 <callee_double>
  9e:	e9c7 0100 	strd	r0, r1, [r7]
  a2:	e7fe      	b.n	a2 <reset_handler+0x42>
```
`90` - `92`：Load global variable `double_2w` toR2 & R3.
`96` - `98`：Pass argument to R0 & R1.
`9a`：Call function callee_double(). After return from callee_double(), the return value is in R0 & R1.
`9e`：[r7] catch the return value from R0 & R1.

objdump - **.mytext <callee_double>**
```Thumb2
  46:	b480      	push	{r7}
  48:	b083      	sub	sp, #12
  4a:	af00      	add	r7, sp, #0
  4c:	e9c7 0100 	strd	r0, r1, [r7]
  50:	e9d7 2300 	ldrd	r2, r3, [r7]
  54:	4610      	mov	r0, r2
  56:	4619      	mov	r1, r3
  58:	370c      	adds	r7, #12
  5a:	46bd      	mov	sp, r7
  5c:	bc80      	pop	{r7}
  5e:	4770      	bx	lr
```
`46` - `4a`：Reserves 3 word space in stack. R7 is a base to store and load arguments in stack.
`4c`：Store arguments `double_1w` into stack.
`50`：Load arguments `double_1w` to R2 & R3.
`54` - `56`：Function return value is store in R0 & R1, so move the final result from R2 & R3 to R0 & R1.
`58` - `5e`：Restore stack and return to reset_handler.

## Conclusion
>**<The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors>**
>**8.1.2 Exception handlers in C**
>
>`R0` - `R3`, `R12`, `LR`, and `PSR` are called **“caller saved registers.”** The program code that calls a subroutine needs to save these register contents into memory (e.g., stack) before the function call if these values will still be needed after the function call. Register values that are not required after the function call don’t have to be saved.
>
>`R4` - `R11` are called **“callee-saved registers.”** The subroutine or function being called needs to make sure the contents of these registers are unaltered at the end of the function (same value as when the function is entered). The values of these registers could change in the middle of the function execution, but need to be restored to their original values before function exit.
![](https://i.imgur.com/7zzbZYc.png)
### Passing arguments & return values flow
1. **caller() calls callee()**
    - **R0, R1, R2, R3 and stack (if more than 4 parameters) is used to pass arguments to function.** Before calling function, load global variable to R3. Via R3, store global variable to R0, R1, R2, R3 and stack sequentially.
    -  call function and branch to the address of function with instr. `bl`. At the same time save the return address which value must +1 in Thumb state to LR.
2.  **in callee()**
    - R7 refers to arguments from R0, R1, R2 and R3.
    - Do the operation of function between R2 & R3. Finally, the result would be stored in R3. If the result is more than 1 word, it would be splitted into consecutive registers R2 & R3.
    - **R0 & R1 is used to return value.** Store the result from R2 & R3 to R0 & R1, than return to the address that is stored in LR.
3. **return to caller()**
    - R7 refers to return value from R0.
## Reference
- [Chapter 6 AAPCS Arm ABI and Runtime view](https://embeddedsecurity.io/sec-arm-abi)
- The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors
# Embedded - HW03 repo
###### tags: `2019_ES Embedded System Development and OS Design`
## Requirements
1. **How do C functions pass and return parameters?** Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.
2. Modify **main.c** to observe what you found.
3. You have to state how you designed the observation (code), and how you performed it.Just like how you did in HW02.
4. If there are any official data that define the rules, you can also use them as references.
5. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

**Keywords:**
Calling Conventions
AAPCS - Procedure Call Standard for Arm Architecture

> 參數(Parameter)：建立函式時，預設帶入的變數。
> 引數(Argument)：呼叫函式時，傳給該函式的參數的值。

## Methods
### main.c
```c=
int int_1w1 = 111;
int int_1w2 = 222;
int int_1w3 = 333;
int int_1w4 = 444;
int int_1w5 = 555;
float float_1w = 1111.2222;
double double_2w = 3333.4444;


int callee_int(int i1, int i2, int i3, int i4, int i5)
{
    return (i1 + i2 + i3 + i4 + i5);
}
float callee_float(float f){return f;}
double callee_double(double d){return d;}


void reset_handler(void)
{
    int temp1;
    float temp2;
    double temp3;

    temp1 = callee_int(int_1w1, int_1w2, int_1w3, int_1w4, int_1w5);
    temp2 = callee_float(float_1w);
    temp3 = callee_double(double_2w);
    while (1);
}
```
## Result
### The result from objdump
```
Disassembly of section .mytext:

00000000 <callee_int-0x8>:
   0:	20000100 	andcs	r0, r0, r0, lsl #2
   4:	00000061 	andeq	r0, r0, r1, rrx

00000008 <callee_int>:
   8:	b480      	push	{r7}
   a:	b085      	sub	sp, #20
   c:	af00      	add	r7, sp, #0
   e:	60f8      	str	r0, [r7, #12]
  10:	60b9      	str	r1, [r7, #8]
  12:	607a      	str	r2, [r7, #4]
  14:	603b      	str	r3, [r7, #0]
  16:	68fa      	ldr	r2, [r7, #12]
  18:	68bb      	ldr	r3, [r7, #8]
  1a:	441a      	add	r2, r3
  1c:	687b      	ldr	r3, [r7, #4]
  1e:	441a      	add	r2, r3
  20:	683b      	ldr	r3, [r7, #0]
  22:	441a      	add	r2, r3
  24:	69bb      	ldr	r3, [r7, #24]
  26:	4413      	add	r3, r2
  28:	4618      	mov	r0, r3
  2a:	3714      	adds	r7, #20
  2c:	46bd      	mov	sp, r7
  2e:	bc80      	pop	{r7}
  30:	4770      	bx	lr

00000032 <callee_float>:
  32:	b480      	push	{r7}
  34:	b083      	sub	sp, #12
  36:	af00      	add	r7, sp, #0
  38:	6078      	str	r0, [r7, #4]
  3a:	687b      	ldr	r3, [r7, #4]
  3c:	4618      	mov	r0, r3
  3e:	370c      	adds	r7, #12
  40:	46bd      	mov	sp, r7
  42:	bc80      	pop	{r7}
  44:	4770      	bx	lr

00000046 <callee_double>:
  46:	b480      	push	{r7}
  48:	b083      	sub	sp, #12
  4a:	af00      	add	r7, sp, #0
  4c:	e9c7 0100 	strd	r0, r1, [r7]
  50:	e9d7 2300 	ldrd	r2, r3, [r7]
  54:	4610      	mov	r0, r2
  56:	4619      	mov	r1, r3
  58:	370c      	adds	r7, #12
  5a:	46bd      	mov	sp, r7
  5c:	bc80      	pop	{r7}
  5e:	4770      	bx	lr

00000060 <reset_handler>:
  60:	b590      	push	{r4, r7, lr}
  62:	b087      	sub	sp, #28
  64:	af02      	add	r7, sp, #8
  66:	4b0f      	ldr	r3, [pc, #60]	; (a4 <reset_handler+0x44>)
  68:	6818      	ldr	r0, [r3, #0]
  6a:	4b0f      	ldr	r3, [pc, #60]	; (a8 <reset_handler+0x48>)
  6c:	6819      	ldr	r1, [r3, #0]
  6e:	4b0f      	ldr	r3, [pc, #60]	; (ac <reset_handler+0x4c>)
  70:	681a      	ldr	r2, [r3, #0]
  72:	4b0f      	ldr	r3, [pc, #60]	; (b0 <reset_handler+0x50>)
  74:	681c      	ldr	r4, [r3, #0]
  76:	4b0f      	ldr	r3, [pc, #60]	; (b4 <reset_handler+0x54>)
  78:	681b      	ldr	r3, [r3, #0]
  7a:	9300      	str	r3, [sp, #0]
  7c:	4623      	mov	r3, r4
  7e:	f7ff ffc3 	bl	8 <callee_int>
  82:	60f8      	str	r0, [r7, #12]
  84:	4b0c      	ldr	r3, [pc, #48]	; (b8 <reset_handler+0x58>)
  86:	681b      	ldr	r3, [r3, #0]
  88:	4618      	mov	r0, r3
  8a:	f7ff ffd2 	bl	32 <callee_float>
  8e:	60b8      	str	r0, [r7, #8]
  90:	4b0a      	ldr	r3, [pc, #40]	; (bc <reset_handler+0x5c>)
  92:	e9d3 2300 	ldrd	r2, r3, [r3]
  96:	4610      	mov	r0, r2
  98:	4619      	mov	r1, r3
  9a:	f7ff ffd4 	bl	46 <callee_double>
  9e:	e9c7 0100 	strd	r0, r1, [r7]
  a2:	e7fe      	b.n	a2 <reset_handler+0x42>
  a4:	000000c0 	andeq	r0, r0, r0, asr #1
  a8:	000000c4 	andeq	r0, r0, r4, asr #1
  ac:	000000c8 	andeq	r0, r0, r8, asr #1
  b0:	000000cc 	andeq	r0, r0, ip, asr #1
  b4:	000000d0 	ldrdeq	r0, [r0], -r0	; <UNPREDICTABLE>
  b8:	000000d4 	ldrdeq	r0, [r0], -r4
  bc:	000000d8 	ldrdeq	r0, [r0], -r8

Disassembly of section .data:

000000c0 <int_1w1>:
  c0:	0000006f 	andeq	r0, r0, pc, rrx

000000c4 <int_1w2>:
  c4:	000000de 	ldrdeq	r0, [r0], -lr

000000c8 <int_1w3>:
  c8:	0000014d 	andeq	r0, r0, sp, asr #2

000000cc <int_1w4>:
  cc:	000001bc 			; <UNDEFINED> instruction: 0x000001bc

000000d0 <int_1w5>:
  d0:	0000022b 	andeq	r0, r0, fp, lsr #4

000000d4 <float_1w>:
  d4:	448ae71c 	strmi	lr, [sl], #1820	; 0x71c

000000d8 <double_2w>:
  d8:	886594af 	stmdahi	r5!, {r0, r1, r2, r3, r5, r7, sl, ip, pc}^
  dc:	40aa0ae3 	adcmi	r0, sl, r3, ror #21

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <double_2w+0x10d0c4c>
   4:	4e472820 	cdpmi	8, 4, cr2, cr7, cr0, {1}
   8:	72412055 	subvc	r2, r1, #85	; 0x55
   c:	6d45206d 	stclvs	0, cr2, [r5, #-436]	; 0xfffffe4c
  10:	64646562 	strbtvs	r6, [r4], #-1378	; 0xfffffa9e
  14:	54206465 	strtpl	r6, [r0], #-1125	; 0xfffffb9b
  18:	636c6f6f 	cmnvs	ip, #444	; 0x1bc
  1c:	6e696168 	powvsez	f6, f1, #0.0
  20:	2e303120 	rsfcssp	f3, f0, f0
  24:	30322d33 	eorscc	r2, r2, r3, lsr sp
  28:	312e3132 			; <UNDEFINED> instruction: 0x312e3132
  2c:	31202930 			; <UNDEFINED> instruction: 0x31202930
  30:	2e332e30 	mrccs	14, 1, r2, cr3, cr0, {1}
  34:	30322031 	eorscc	r2, r2, r1, lsr r0
  38:	38303132 	ldmdacc	r0!, {r1, r4, r5, r8, ip, sp}
  3c:	28203432 	stmdacs	r0!, {r1, r4, r5, sl, ip, sp}
  40:	656c6572 	strbvs	r6, [ip, #-1394]!	; 0xfffffa8e
  44:	29657361 	stmdbcs	r5!, {r0, r5, r6, r8, r9, ip, sp, lr}^
	...

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00002b41 	andeq	r2, r0, r1, asr #22
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	00000021 	andeq	r0, r0, r1, lsr #32
  10:	2d453705 	stclcs	7, cr3, [r5, #-20]	; 0xffffffec
  14:	0d06004d 	stceq	0, cr0, [r6, #-308]	; 0xfffffecc
  18:	02094d07 	andeq	r4, r9, #448	; 0x1c0
  1c:	01140412 	tsteq	r4, r2, lsl r4
  20:	03170115 	tsteq	r7, #1073741829	; 0x40000005
  24:	011a0118 	tsteq	sl, r8, lsl r1
  28:	0122061e 			; <UNDEFINED> instruction: 0x0122061e
```
## Discussion
I will disscus these parts below：
1. global variables - `.data` & `.mytext` section
2. how does function passing arguments & return values
    - callee_int() - `.mytext <reset_handler>` & `.mytext <callee_int>`
    - callee_float() - `.mytext <reset_handler>` & `.mytext <callee_float>`
    - callee_double() - `.mytext <reset_handler>` & `.mytext <callee_double>`
### Global variables
The value of global variables are saved in `.data` section from `c0` to `dc`.
The address of global variables are saved in the end of `.mytext` section from `a4` to `bc`.
**main.c**
```c
int int_1w1 = 111;
int int_1w2 = 222;
int int_1w3 = 333;
int int_1w4 = 444;
int int_1w5 = 555;
float float_1w = 1111.2222;
double double_2w = 3333.4444;
...
```
objdump - **.data**
```Thumb2
Disassembly of section .data:

000000c0 <int_1w1>://0x6f = 1111
  c0:	0000006f 	andeq	r0, r0, pc, rrx 

000000c4 <int_1w2>://0xde = 222
  c4:	000000de 	ldrdeq	r0, [r0], -lr 

000000c8 <int_1w3>://0x14d = 333
  c8:	0000014d 	andeq	r0, r0, sp, asr #2 

000000cc <int_1w4>://0x1bc = 444       
  cc:	000001bc 			; <UNDEFINED> instruction: 0x000001bc

000000d0 <int_1w5>://0x22b = 555
  d0:	0000022b 	andeq	r0, r0, fp, lsr #4

000000d4 <float_1w>:
  d4:	448ae71c 	strmi	lr, [sl], #1820	; 0x71c

000000d8 <double_2w>:
  d8:	886594af 	stmdahi	r5!, {r0, r1, r2, r3, r5, r7, sl, ip, pc}^
  dc:	40aa0ae3 	adcmi	r0, sl, r3, ror #21
```
objdump - **.mytext <reset_handler>**
```Thumb2
  a4:	000000c0 	andeq	r0, r0, r0, asr #1
  a8:	000000c4 	andeq	r0, r0, r4, asr #1
  ac:	000000c8 	andeq	r0, r0, r8, asr #1
  b0:	000000cc 	andeq	r0, r0, ip, asr #1
  b4:	000000d0 	ldrdeq	r0, [r0], -r0	; <UNPREDICTABLE>
  b8:	000000d4 	ldrdeq	r0, [r0], -r4
  bc:	000000d8 	ldrdeq	r0, [r0], -r8
```

### function - callee_int()
**main.c**
```c
int int_1w1 = 111;
int int_1w2 = 222;
int int_1w3 = 333;
int int_1w4 = 444;
int int_1w5 = 555;
...

int callee_int(int i1, int i2, int i3, int i4, int i5)
{
    return (i1 + i2 + i3 + i4 + i5);
}
...

void reset_handler(void)
{
    int temp1;
    ...
    temp1 = callee_int(int_1w1, int_1w2, int_1w3, int_1w4, int_1w5);
    ...
    while (1);
}
```
objdump - **.mytext <reset_handler>**
```Thumb2
  66:	4b0f      	ldr	r3, [pc, #60]	; (a4 <reset_handler+0x44>)
  68:	6818      	ldr	r0, [r3, #0]    
  6a:	4b0f      	ldr	r3, [pc, #60]	; (a8 <reset_handler+0x48>)
  6c:	6819      	ldr	r1, [r3, #0]
  6e:	4b0f      	ldr	r3, [pc, #60]	; (ac <reset_handler+0x4c>)
  70:	681a      	ldr	r2, [r3, #0]
  72:	4b0f      	ldr	r3, [pc, #60]	; (b0 <reset_handler+0x50>)
  74:	681c      	ldr	r4, [r3, #0]
  76:	4b0f      	ldr	r3, [pc, #60]	; (b4 <reset_handler+0x54>)
  78:	681b      	ldr	r3, [r3, #0]
  7a:	9300      	str	r3, [sp, #0]
  7c:	4623      	mov	r3, r4
  7e:	f7ff ffc3 	bl	8 <callee_int>
  82:	60f8      	str	r0, [r7, #12]
```
R0 to R3 is used to input parameters. if over 4 parameters, the remaining parameters would be stored in stack. 
`66` - `78`：Load global variable `int_1w1`, `int_1w2`, `int_1w3`, `int_1w4`, `int_1w5` to R0, R1, R2, R4, R3.
`7a`：Store R3 to stack
`7c`：Move R4 to R3
The five parameters (i.e. `int_1w1` to `int_1w5`) are respectively saved in R0, R1, R2, R3 and end of stack.
`7e`：Call function callee_int(). After return from callee_int(), the return value is in R0.
`82`：[r7, #12] catch the return value from R0


objdump - **.mytext <callee_int>**
```Thumb2
00000008 <callee_int>:
   8:	b480      	push	{r7}
   a:	b085      	sub	sp, #20
   c:	af00      	add	r7, sp, #0
   e:	60f8      	str	r0, [r7, #12]
  10:	60b9      	str	r1, [r7, #8]
  12:	607a      	str	r2, [r7, #4]
  14:	603b      	str	r3, [r7, #0]
  16:	68fa      	ldr	r2, [r7, #12]
  18:	68bb      	ldr	r3, [r7, #8]
  1a:	441a      	add	r2, r3
  1c:	687b      	ldr	r3, [r7, #4]
  1e:	441a      	add	r2, r3
  20:	683b      	ldr	r3, [r7, #0]
  22:	441a      	add	r2, r3
  24:	69bb      	ldr	r3, [r7, #24]
  26:	4413      	add	r3, r2
  28:	4618      	mov	r0, r3
  2a:	3714      	adds	r7, #20
  2c:	46bd      	mov	sp, r7
  2e:	bc80      	pop	{r7}
  30:	4770      	bx	lr
```
`08` - `0c`：Reserves 5 word space in stack. R7 is a base to store and load arguments in stack.
`0c`：R7 = Address of SP. R7 is a base to store and load arguments.
`0e` - `14`：Store arguments `int_1w1` ~ `int_1w4` to stack.
`16` - `22`：Load arguments `int_1w1` ~ `int_1w4` and do some operation between R2 & R3.
`24`：Load the 5th arguments `int_1w5` from stack.
`26`：The final result is in R3.
`28`：Function return value is store in R0, so move the final result from R3 to R0.
`2a` - `30`：Restore stack and return to reset_handler.


### function - callee_float()
**main.c**
```c
...
float float_1w = 1111.2222;
...
float callee_float(float f){return f;}
...
void reset_handler(void)
{
    ...
    float temp2;
    ...
    temp2 = callee_float(float_1w);
    ...
    while (1);
}
```

objdump - **.mytext <reset_handler>**
```Thumb2
  84:	4b0c      	ldr	r3, [pc, #48]	; (b8 <reset_handler+0x58>)
  86:	681b      	ldr	r3, [r3, #0]
  88:	4618      	mov	r0, r3
  8a:	f7ff ffd2 	bl	32 <callee_float>
  8e:	60b8      	str	r0, [r7, #8]
```
`84` - `86`：Load global variable `float_1w` to R3.
`88`：Pass argument to R0.
`8a`：Call function callee_float(). After return from callee_float(), the return value is in R0.
`8e`：[r7, #8] catch the return value from R0.

objdump - **.mytext <callee_float>**
```Thumb2
00000032 <callee_float>:
  32:	b480      	push	{r7}
  34:	b083      	sub	sp, #12
  36:	af00      	add	r7, sp, #0
  38:	6078      	str	r0, [r7, #4]
  3a:	687b      	ldr	r3, [r7, #4]
  3c:	4618      	mov	r0, r3
  3e:	370c      	adds	r7, #12
  40:	46bd      	mov	sp, r7
  42:	bc80      	pop	{r7}
  44:	4770      	bx	lr
```
`32` - `36`：Reserves 3 word space in stack. R7 is a base to store and load arguments in stack.
`38`：Store arguments `float_1w` into stack.
`3a`：Load arguments `float_1w` to R3.
`3c`：Function return value is store in R0, so move the final result from R3 to R0.
`3e` - `44`：Restore stack and return to reset_handler.



### function - callee_double()
**main.c**
```c
...
double double_2w = 3333.4444;
..
double callee_double(double d){return d;}

void reset_handler(void)
{
    ...
    double temp3;
    ...
    temp3 = callee_double(double_2w);
    while (1);
}
```

objdump - **.mytext <reset_handler>**
```Thumb2
  90:	4b0a      	ldr	r3, [pc, #40]	; (bc <reset_handler+0x5c>)
  92:	e9d3 2300 	ldrd	r2, r3, [r3]
  96:	4610      	mov	r0, r2
  98:	4619      	mov	r1, r3
  9a:	f7ff ffd4 	bl	46 <callee_double>
  9e:	e9c7 0100 	strd	r0, r1, [r7]
  a2:	e7fe      	b.n	a2 <reset_handler+0x42>
```
`90` - `92`：Load global variable `double_2w` toR2 & R3.
`96` - `98`：Pass argument to R0 & R1.
`9a`：Call function callee_double(). After return from callee_double(), the return value is in R0 & R1.
`9e`：[r7] catch the return value from R0 & R1.

objdump - **.mytext <callee_double>**
```Thumb2
  46:	b480      	push	{r7}
  48:	b083      	sub	sp, #12
  4a:	af00      	add	r7, sp, #0
  4c:	e9c7 0100 	strd	r0, r1, [r7]
  50:	e9d7 2300 	ldrd	r2, r3, [r7]
  54:	4610      	mov	r0, r2
  56:	4619      	mov	r1, r3
  58:	370c      	adds	r7, #12
  5a:	46bd      	mov	sp, r7
  5c:	bc80      	pop	{r7}
  5e:	4770      	bx	lr
```
`46` - `4a`：Reserves 3 word space in stack. R7 is a base to store and load arguments in stack.
`4c`：Store arguments `double_1w` into stack.
`50`：Load arguments `double_1w` to R2 & R3.
`54` - `56`：Function return value is store in R0 & R1, so move the final result from R2 & R3 to R0 & R1.
`58` - `5e`：Restore stack and return to reset_handler.

## Conclusion
>**<The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors>**
>**8.1.2 Exception handlers in C**
>
>`R0` - `R3`, `R12`, `LR`, and `PSR` are called **“caller saved registers.”** The program code that calls a subroutine needs to save these register contents into memory (e.g., stack) before the function call if these values will still be needed after the function call. Register values that are not required after the function call don’t have to be saved.
>
>`R4` - `R11` are called **“callee-saved registers.”** The subroutine or function being called needs to make sure the contents of these registers are unaltered at the end of the function (same value as when the function is entered). The values of these registers could change in the middle of the function execution, but need to be restored to their original values before function exit.
![](https://i.imgur.com/7zzbZYc.png)
### Passing arguments & return values flow
1. **caller() calls callee()**
    - **R0, R1, R2, R3 and stack (if more than 4 parameters) is used to pass arguments to function.** Before calling function, load global variable to R3. Via R3, store global variable to R0, R1, R2, R3 and stack sequentially.
    -  call function and branch to the address of function with instr. `bl`. At the same time save the return address which value must +1 in Thumb state to LR.
2.  **in callee()**
    - R7 refers to arguments from R0, R1, R2 and R3.
    - Do the operation of function between R2 & R3. Finally, the result would be stored in R3. If the result is more than 1 word, it would be splitted into consecutive registers R2 & R3.
    - **R0 & R1 is used to return value.** Store the result from R2 & R3 to R0 & R1, than return to the address that is stored in LR.
3. **return to caller()**
    - R7 refers to return value from R0.
## Reference
- [Chapter 6 AAPCS Arm ABI and Runtime view](https://embeddedsecurity.io/sec-arm-abi)
- The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors

