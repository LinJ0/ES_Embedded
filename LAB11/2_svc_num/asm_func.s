.syntax unified

.global	read_sp
read_sp:
	mov	r0, sp
	bx	lr

.global	read_msp
read_msp:
	mrs	r0, msp
	bx	lr

.global	read_psp
read_psp:
	mrs	r0, psp
	bx	lr

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

.global sys_call
sys_call:
        //int32_t sys_call(void);
        svc     #0x02 //call SVC function 0x02
        bx      lr

.global	sys_call_add
sys_call_add:
	//int32_t sys_call(int32_t a, int32_t b);
        svc     #0x0A //call SVC function 0x0A
        bx      lr

.type svc_handler, %function
.global svc_handler
svc_handler:
	mov     r0, lr
        mrs     r1, msp
        b       svc_handler_c
