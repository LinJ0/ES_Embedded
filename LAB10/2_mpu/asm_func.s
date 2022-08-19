.syntax unified

.global	read_sp
read_sp:
	//uint32_t read_sp(void);
        //function return value store in r0, then bl lr
        mov     r0, sp
        bx      lr

.global	read_msp
read_msp:
	//uint32_t read_msp(void);
        mrs     r0, msp
        bx      lr

.global	read_psp
read_psp:
	//uint32_t read_psp(void);
        mrs     r0, psp
        bx      lr

.global	read_ctrl
read_ctrl:
	//uint32_t read_ctrl(void);
        mrs     r0, control
        bx      lr

.global	start_user
start_user:
	//void start_user(uint32_t *task_addr, uint32_t *psp_init); 
        //passing parameters to r0 & r1
        //r0 --> user_task_addr  
        //r1 --> psp_init  
        sub     sp, #8 //space 2 words for r0 & r1       
        add     r7, sp, #0
        str     r0, [r7, #0]
        str     r1, [r7, #4]

        //switch privilege from privileged access level to unprivileged access level
        msr     psp, r1         //initial PSP
        mov     r2, 0b011       //FPCA = no FP extension, SPSEL = PSP, nPRIV = unprivilege
        msr     control, r2
        ISB                     //flush pipeline

        //branch to user task
        ldr     r3, [r7, #0]
        bx      r3

.global	sw_priv
sw_priv:
        //void sw_priv(void);
	//switch privilege from unprivileged access level to privileged access level
        mov     r2, 0b000       //FPCA = np FP extension, SPSEL = MSP, nPRIV = privilege
        msr     control, r2
        ISB                     //flush pipeline
        bx      lr
