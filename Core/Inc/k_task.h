/*
 * k_task.h
 *
 *  Created on: Jan 5, 2024
 *      Author: nexususer
 *
 *      NOTE: any C functions you write must go into a corresponding c file that you create in the Core->Src folder
 */

#ifndef INC_K_TASK_H_
#define INC_K_TASK_H_
#include "stm32f4xx_hal.h"
#define EnablePrivilegedMode() __asm("SVC #0")
#define StartThread() __asm("SVC #1")
extern uint32_t* stackptr;

#define SHPR2 *(uint32_t*)0xE000ED1C // bits 24-31 are the priority of SVC
#define SHPR3 *(uint32_t*)0xE000ED20 // bits 16-23 are the priority of PendSV

typedef unsigned int U32;
typedef unsigned short U16;
typedef char U8;
typedef unsigned int task_t;

typedef struct task_control_block{
    void (*ptask)(void* args); // entry address
    uint32_t* stack_high;      // starting address of stack (high address) - this is the PSP
    task_t tid;                // task ID
    U8 state;                  // task's state
    U16 stack_size;            // stack size
    uint32_t* thread_psp_ptr;  // thread psp pointer
    uint32_t deadline;         // Initial deadline in ms
    uint32_t time_remaining;   // Time remaining until deadline
}TCB;

// Kernel Functions - Lab 1
void osKernelInit(void);
int osCreateTask(TCB* task);
int osKernelStart(void);
void osYield(void);
int osTaskInfo(task_t TID, TCB* task_copy);
int osTaskExit(void);

void osSleep(int timeInMs);
void osPeriodYield(void);
int osSetDeadline(int deadline, task_t TID);
int osCreateDeadlineTask(int deadline, TCB* task);



#endif /* INC_K_TASK_H_ */
