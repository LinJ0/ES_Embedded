.syntax unified

.global	read_ctrl
read_ctrl:
	mrs	r0, control
	bx	lr

.global	start_user
start_user:
	movs	lr, r0
	msr	psp, r1

	movs	r3, #0b11
	msr	control, r3
	isb

	bx	lr

.type systick_handler, %function
.global systick_handler
systick_handler:
	//1. save lr (EXC_RETURN) to main stack
	push    {lr}

	//2. save r4-r11 to user stack
        //r0 store psp pointer, i.e. the last position of stack frame
	mrs	r0, psp
        //update r0 (psp) after push R4~R11
	stmdb	r0!, {r4-r11}

	//3. pass psp of curr task by r0 and get psp of the next task
	bl	sw_task
	//psp of the next task is now in r0

	//4. restore r4~r11 from stack of the next task
	ldmia   r0!, {r4-r11}

	//5. modify psp
	msr     psp, r0

	//6. restore lr (EXC_RETURN)
	pop     {lr}

	bx	lr
