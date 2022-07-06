.syntax unified

.word 0x20000100
.word _start

.global _start
.type _start, %function
_start:
	movs	r0, #0x1
	movs	r1, #0x2
	movs	r2, #0x3
	movs	r3, #0x4
push_reg:
	push	{r0, r1, r2, r3}
	push	{r3, r0, r2, r1}	//push together
	push	{r3}			//push separate
	push	{r0}
	push	{r2}
	push	{r1}
pop_reg:
	pop	{r0, r1, r2, r3}
	pop	{r4, r5, r6, r7}
	pop	{r8, r9, r10, r11}
	bl	sleep
sleep:
	b	.
