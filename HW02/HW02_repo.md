# Embedded - HW02 repo
###### tags: `2019_ES Embedded System Development and OS Design`
## Requirements
1. Please modify main.s to observe the push and the pop instructions:
Does the order of the registers in the push and the pop instructions affect the excution results?
For example, will push `{r0, r1, r2}` and push `{r2, r0, r1}` act in the same way?
Which register will be pushed into the stack first?
2. You have to state how you designed the observation (code), and how you performed it.
Just like how ESEmbedded_HW02_Example did.
3. If there are any official data that define the rules, you can also use them as references.
4. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

## Emulation with QEMU
**Qemu Server**
```
$ make qemu
```
![](https://i.imgur.com/b50IfAX.png)

**Qemu Client**
- Open another terminal window, start GNU Debugger：
```
$ arm-none-eabi-gdb
```
- Connect to qemu server (`IP = 本機`:`port = 1234`)：
```
$ target remote 127.0.0.1:1234
```
![](https://i.imgur.com/VUEgESV.png)

To display more information：
press `ctrl + x`, then press `2`
![](https://i.imgur.com/iIUGZnN.png)

```
(gdb) layout regs
```
![](https://i.imgur.com/0i4aQDT.png)

- `si`：step instr.

![](https://i.imgur.com/8fwDjmw.png)

full screen will shows all registers.
![](https://i.imgur.com/18KOfW3.png)

Using the following commands to observe the registers, memory values, and instructions.
- `si` or `stepi` to exe one instruction
- `x addr` to print memory addr
- `x $sp` to print memory addr of sp
- `x` to print next
- `x/20 addr` to print memory addr ~ addr + 20
- `x/20 addr - 20` to print memory addr - 20 ~ addr

## Methods
### main.s
```Thumb2=
.syntax unified

.word 0x20000100
.word _start

.global _start
.type _start, %function
_start:
        movs    r0, #0x1
        movs    r1, #0x2
        movs    r2, #0x3
        movs    r3, #0x4
push_reg:
        push    {r0, r1, r2, r3}
        push    {r3, r0, r2, r1}        //push together
        push    {r3}                    //push separate
        push    {r0}
        push    {r2}
        push    {r1}
pop_reg:
        pop     {r0, r1, r2, r3}
        pop     {r4, r5, r6, r7}
        pop     {r8, r9, r10, r11}
        bl      sleep
sleep:
        b       .
```

## Results
**Step1.** Move values to registers.
```thumb2
_start:
        movs    r0, #0x1
        movs    r1, #0x2
        movs    r2, #0x3
        movs    r3, #0x4
```

**Step2-1.** Push {r0, r1, r2, r3}
```thumb2
    push     {r0, r1, r2, r3}
```
- sp： `0x20000f0`
- gdb：
![](https://i.imgur.com/b8vtG6M.png)
- stack memory layout:
![](https://i.imgur.com/HpZQxGB.png)


**Step2-2.** Push {r3, r0, r2, r1}
```thumb2
    push    {r3, r0, r2, r1}        //push together
```
- sp： `0x20000e0`
- gdb：
![](https://i.imgur.com/OEbY1EJ.png)
- stack memory layout:
![](https://i.imgur.com/mVFcPCS.png)

**Step2-3.** Push {r3} {r0} {r2} {r1}
```thumb2
        push    {r3}                    //push separate
        push    {r0}
        push    {r2}
        push    {r1}
```
- sp： `0x20000d0`
- gdb：
![](https://i.imgur.com/vqW3SEf.png)
- stack memory layout:
![](https://i.imgur.com/skQm3zu.png)


**Step3.** Pop {r0, r1, r2, r3}
```thumb2
    pop    {r0, r1, r2, r3}
```
- sp：`0x20000e0`
- r0：`2`
- r1：`3` 
- r2：`1` 
- r3：`4`
- gdb：
![](https://i.imgur.com/Ml7uAHA.png)
- stack memory layout：
![](https://i.imgur.com/9ScXoBT.png)
## Conclusion
The result form gdb shows that the order of the registers in the push and the pop instructions does ==not== affect the excution results.
![](https://i.imgur.com/xPFTzeD.png)


We can observe the result from objdump as well, and found that instructions in address `0x10` and `0x12` is the same.

**The result from objdump**
```
Disassembly of section .text:

00000000 <_start-0x8>:
   0:	20000100 	andcs	r0, r0, r0, lsl #2
   4:	00000009 	andeq	r0, r0, r9

00000008 <_start>:
   8:	2001      	movs	r0, #1
   a:	2102      	movs	r1, #2
   c:	2203      	movs	r2, #3
   e:	2304      	movs	r3, #4

00000010 <push_reg>:
  10:	b40f      	push	{r0, r1, r2, r3}
  12:	b40f      	push	{r0, r1, r2, r3}
  14:	b408      	push	{r3}
  16:	b401      	push	{r0}
  18:	b404      	push	{r2}
  1a:	b402      	push	{r1}

0000001c <pop_reg>:
  1c:	bc0f      	pop	{r0, r1, r2, r3}
  1e:	bcf0      	pop	{r4, r5, r6, r7}
  20:	e8bd 0f00 	ldmia.w	sp!, {r8, r9, sl, fp}
  24:	f000 f800 	bl	28 <sleep>

00000028 <sleep>:
  28:	e7fe      	b.n	28 <sleep>
```
## Reference
- [How to install arm-none-eabi-gdb on Ubuntu 20.04 LTS (Focal Fossa)
](https://askubuntu.com/questions/1243252/how-to-install-arm-none-eabi-gdb-on-ubuntu-20-04-lts-focal-fossa)
- GDB指令手冊 [Debugging with GDB](https://sourceware.org/gdb/current/onlinedocs/gdb/) 
