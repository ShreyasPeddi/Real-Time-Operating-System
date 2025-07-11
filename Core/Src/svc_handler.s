.syntax unified
.cpu cortex-m4
.thumb
.global curr_SP
.global next_SP
.global svc_number
.global updateSP
.global exitSP
.global enterSP
.global SVC_Handler
.thumb_func
SVC_Handler:
	TST lr, #4
	ITE EQ
	MRSEQ r0, MSP
	MRSNE r0, PSP
	B SVC_Handler_Main
.global ThreadStart
.thumb_func
ThreadStart:
	// LDR R0, =stackptr
	LDR R1, [R0]
	LDMIA R1!, {R4-R11}
	MSR PSP, R1
	MOV LR, #0xFFFFFFFD
	BX LR
.global handleOSYield
.thumb_func
handleOSYield:
	MRS R0, PSP
	STMDB R0!, {R4-R11}
	MSR PSP, R0
	BL updateSP
	LDR R0, =curr_thread_ptr  // Changed from next_thread_ptr to curr_thread_ptr
	LDR R1, [R0]
	LDMIA R1!, {R4-R11}
	MSR PSP, R1
	MOV LR, #0xFFFFFFFD
	BX LR
.global handleOSStart
.thumb_func
handleOSStart:
	LDR R3, =curr_thread_ptr
	LDR R2, [R3]
	LDMIA R2!, {R4-R11}
	MSR PSP, R2
	BL enterSP
	MOV LR, #0xFFFFFFFD
	BX LR
.global handleOSExit
.thumb_func
handleOSExit:
	BL exitSP
	LDR R0, =curr_thread_ptr  // Changed from next_thread_ptr to curr_thread_ptr
	LDR R1, [R0]
	LDMIA R1!, {R4-R11}
	MSR PSP, R1
	MOV LR, #0xFFFFFFFD
	BX LR
.global PendSV_Handler
.thumb_func
PendSV_Handler:
	LDR R0, =svc_number    // address of svc_number
	LDR R0, [R0]           // value of svc_number
	CMP R0, #1
	BEQ handleOSStart      // If svc_number == 1, branch to handleOSStart
	CMP R0, #2
	BEQ handleOSYield      // If svc_number == 2, branch to handleOSYield
	CMP R0, #3
	BEQ handleOSExit       // If svc_number == 3, branch to handleOSExit
