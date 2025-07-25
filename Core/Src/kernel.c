#include "common.h"
#include "k_task.h"
#include "k_mem.h"
#include <stdio.h>

TCB task_list[MAX_TASKS];

task_t current_TID = 0;
task_t next_TID = 0;

uint32_t* next_stack_high = NULL;

uint32_t* curr_thread_ptr = NULL;
uint32_t* next_thread_ptr = NULL;

uint32_t* MSP_INIT_VAL;
uint32_t THREAD_SIZE;

extern int context_switch_required;

task_t task_count = 0;
unsigned int svc_number;

// global system time in Ms
volatile uint32_t g_system_time = 0;

extern uint32_t _Min_Stack_Size;

int kernel_init = 0;
int kernel_running = 0;

void null_task(void*arg) {
	while (1) {}
}

// EDF
int getEarliestDeadlineTask(void) {
    task_t earliest_task = TID_NULL;
    uint32_t earliest_deadline = 0xFFFFFFFF; // is this correct???

    for (int i = 1; i < MAX_TASKS; i++) {
        if (task_list[i].state == READY && task_list[i].time_remaining < earliest_deadline) {
            earliest_deadline = task_list[i].time_remaining;
            earliest_task = i;
        }
    }

    return earliest_task;  // will be TID_NULL if no ready tasks found
}

void osKernelInit(void) {

    MSP_INIT_VAL = *(uint32_t**)0x0;
    uint32_t* stackptr = (uint32_t)(MSP_INIT_VAL) - MAIN_STACK_SIZE;

    if (kernel_init) { return; }

    kernel_init = 1;
    if (k_mem_init() == RTX_ERR) {
    	kernel_init = 0;
    	return;
    }


    void* stack_mem = k_mem_alloc(MAIN_STACK_SIZE);
    if (stack_mem == NULL) {
        return RTX_ERR;
    }

    // initialize null task
    task_list[0].ptask = null_task;
    task_list[0].stack_high = (uint32_t*)((uint32_t)stack_mem + MAIN_STACK_SIZE);
    task_list[0].tid = TID_NULL;
    task_list[0].state = RUNNING;
    task_list[0].stack_size = MAIN_STACK_SIZE;
    // ask maran what to put in here
    task_list[0].deadline = 0xFFFFFFFF; // max val???
    task_list[0].time_remaining = 0xFFFFFFFF; // max val???

    current_TID = TID_NULL;


    uint32_t* stack_location_ptr = (uint32_t*)task_list[0].stack_high;
    *(--stack_location_ptr) = 1 << 24;  // xPSR (Thumb state)
    *(--stack_location_ptr) = (uint32_t)task_list[0].ptask; // PC (entry point)
    for (int i = 0; i < 14; i++) {
        *(--stack_location_ptr) = 0xA;  // General-purpose registers
    }

    task_list[0].thread_psp_ptr = stack_location_ptr;
    task_count++; // increment task count

    // make all other tasks as DORMANT
    for (int i = 1; i < MAX_TASKS; i++) {
        task_list[i].ptask = NULL;
        task_list[i].stack_high = 0x0;
        task_list[i].tid = TID_NULL;
        task_list[i].state = DORMANT;
        task_list[i].stack_size = 0x0;
        task_list[i].deadline = 0;
        task_list[i].time_remaining = 0;
    }

    g_system_time = 0;

    SHPR3 = (SHPR3 & ~(0xFFU << 24)) | (0xF0U << 24);//SysTick is lowest priority (highest number)
    SHPR3 = (SHPR3 & ~(0xFFU << 16)) | (0xE0U << 16);//PendSV is in the middle
    SHPR2 = (SHPR2 & ~(0xFFU << 24)) | (0xD0U << 24);//SVC is highest priority (lowest number)
}

int osCreateTask(TCB* task) {
    // just do osCreateDeadlineTask(5, task)
    return osCreateDeadlineTask(5, task);
}


task_t new_TID = TID_NULL;
int osCreateDeadlineTask(int deadline, TCB* task) {

    // Check if deadline is valid
    if (deadline <= 0) {
        return RTX_ERR;
    }

    // check if task is NULL, stack size is less than minimum, or task count is at max
    if (task == NULL || task->stack_size < STACK_SIZE || task_count >= MAX_TASKS) {
        return RTX_ERR;
    }

    // find free task ID
    for (int i = 1; i < MAX_TASKS; i++) {
        if (task_list[i].state == DORMANT) {
            new_TID = i;
            break;
        }
    }


    if (new_TID == TID_NULL) {
        return RTX_ERR; // => no free task slots
    }


    void* stack_mem = k_mem_alloc(task->stack_size);
    if (stack_mem == NULL) {
        return RTX_ERR;
    }

    task->tid = new_TID;
    task->stack_high = (uint32_t*)((uint32_t)stack_mem + task->stack_size);
    task_list[new_TID].ptask = task->ptask;
    task_list[new_TID].stack_high = task->stack_high;
    task_list[new_TID].tid = new_TID;
    task_list[new_TID].state = READY;
    task_list[new_TID].stack_size = task->stack_size;
    task_list[new_TID].deadline = deadline;
    task_list[new_TID].time_remaining = deadline;

    uint32_t* stack_location_ptr = (uint32_t*)task_list[new_TID].stack_high;
    *(--stack_location_ptr) = 1 << 24;  // xPSR (Thumb state)
    *(--stack_location_ptr) = (uint32_t)task_list[new_TID].ptask; // PC (entry point)
    for (int i = 0; i < 14; i++) {
        *(--stack_location_ptr) = 0xA;  // General-purpose registers
    }

    task_list[new_TID].thread_psp_ptr = stack_location_ptr;
    task_count++;

    // TODO new task has an earlier deadline than the current task => context switch
    if (kernel_running && current_TID != TID_NULL &&
        task_list[new_TID].time_remaining < task_list[current_TID].time_remaining) {

		next_TID = new_TID;

		svc_number = 2;
		// need to osYield on current task
		// trigger context switch => osYield
		SCB->ICSR |= (0x1 << 28);
		__asm("isb");

    }


    return RTX_OK;
}

int osKernelStart(void) {
    if (!kernel_init || kernel_running) {
        return RTX_ERR;
    }

    // reset system time
    g_system_time = 0;

    task_t first_task = getEarliestDeadlineTask();

    // set the first task to running
    current_TID = first_task;
    task_list[current_TID].state = RUNNING;
    kernel_running = 1;


    if (current_TID != TID_NULL) { // if the first task is not the null task, then we need to load the PSP
        curr_thread_ptr = task_list[current_TID].thread_psp_ptr;
        next_thread_ptr = 0;
        // handleOSStart
        __asm("SVC #1");
    }

    return RTX_OK;
}

void osYield(void) {
    // reset time_remaining to deadline
    task_list[current_TID].time_remaining = task_list[current_TID].deadline;

    next_TID = getEarliestDeadlineTask();

    task_list[current_TID].state = READY;

    // handleOSYield
    __asm("SVC #2");
}

void osSleep(int timeInMs) {
    if (timeInMs <= 0 || !kernel_running) {
        return;
    }

    task_list[current_TID].time_remaining = timeInMs;
    task_list[current_TID].state = SLEEPING;

    next_TID = getEarliestDeadlineTask();

    __asm("SVC #2");
}

void osPeriodYield(void) {
    int required_sleep_time = task_list[current_TID].time_remaining;
	osSleep(required_sleep_time);
}


int osSetDeadline(int deadline, task_t TID) {
    if (deadline <= 0 || TID < 1 || TID >= MAX_TASKS || task_list[TID].state == DORMANT) {
        return RTX_ERR;
    }


    task_list[TID].deadline = deadline;
    task_list[TID].time_remaining = deadline;

    if (task_list[TID].state == READY &&
        task_list[TID].time_remaining < task_list[current_TID].time_remaining) {
        // set next task to be the one with updated deadline
        next_TID = TID;

		svc_number = 2;
		// need to osYield on current task
		// trigger context switch => osYield
		SCB->ICSR |= (0x1 << 28);
		__asm("isb");

    }

    return RTX_OK;
}

int osTaskInfo(task_t TID, TCB* task_copy) {
    if (TID < 0 || TID >= MAX_TASKS || task_copy == NULL) {
        return RTX_ERR;
    }

    if (task_list[TID].tid != TID || task_list[TID].state == DORMANT) {
        return RTX_ERR;
    }

    task_copy->tid = task_list[TID].tid;
    task_copy->ptask = task_list[TID].ptask;
    task_copy->stack_high = task_list[TID].stack_high;
    task_copy->state = task_list[TID].state;
    task_copy->stack_size = task_list[TID].stack_size;
    task_copy->deadline = task_list[TID].deadline;
    task_copy->time_remaining = task_list[TID].time_remaining;

    return RTX_OK;
}

int osTaskExit(void) {
    if (!kernel_running) {
        return RTX_ERR;
    }

    // free the task's stack memory
    k_mem_dealloc((void *)((uint32_t)task_list[current_TID].stack_high - task_list[current_TID].stack_size));

    task_count--;
    task_list[current_TID].state = DORMANT;

    next_TID = getEarliestDeadlineTask();

    __asm("SVC #3");
    return RTX_OK;
}


// on first task (osKernelStart)
void enterSP(void) {
    task_list[current_TID].thread_psp_ptr = __get_PSP(); // save the PSP of the current task
}

// on last task (osExit)
void exitSP(void) {
	if (current_TID != next_TID) {
		// save the PSP of the current task and set the next task to running
		// set the current task to dormant
	    task_list[current_TID].state = DORMANT;
		next_thread_ptr = task_list[next_TID].thread_psp_ptr;
		current_TID = next_TID;
		curr_thread_ptr = next_thread_ptr;
	}
}

void updateSP(void) {
	if (current_TID != next_TID) {
//		task_list[current_TID].state = READY;
		task_list[current_TID].thread_psp_ptr = __get_PSP();

		// in handleOSYield, we need to load this address into our PSP
		next_thread_ptr = task_list[next_TID].thread_psp_ptr;
		current_TID = next_TID;
		curr_thread_ptr = next_thread_ptr;
	}
}

task_t osGetTID(void) {
    if (!kernel_running) {
        return TID_NULL;
    }
    return current_TID;
}


void SVC_Handler_Main(unsigned int *svc_args)
{
    svc_number = ((char*)svc_args[6])[-2];
    switch(svc_number)
    {
        case 0:  /* EnablePrivilegedMode */
            __set_CONTROL(__get_CONTROL() & ~CONTROL_nPRIV_Msk);
            break;
        case 1: /* handleOSStart */
            curr_thread_ptr = task_list[current_TID].thread_psp_ptr;
            SCB->ICSR |= (0x1 << 28);
            __asm("isb");
            break;
        case 2: /* handleOSYield */
            SCB->ICSR |= (0x1 << 28);
            __asm("isb");
            break;
        case 3: /* handleOSExit */
            SCB->ICSR |= (0x1 << 28);
            __asm("isb");
            break;
        default:    /* unknown SVC */
            break;
    }
}
