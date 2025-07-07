//#include "main.h"
//#include <stdio.h>
//#include "common.h"
//#include "k_task.h"
//#include "k_mem.h"
//
//volatile int i_cnt;
//volatile int test_i = 0;
//
//void Task1(void *arg) {
//   while(1){
//     printf("1\r\n");
//     test_i++;
////     if (test_i == 30) {
////    	 while(1);
////     }
//     for (i_cnt = 0; i_cnt < 5000; i_cnt++);
//     osYield();
//   }
//}
//
//
//void Task2(void *arg) {
//   while(1){
//     printf("2\r\n");
//     for (i_cnt = 0; i_cnt < 5000; i_cnt++);
//     osYield();
//   }
//}
//
//
//void Task3(void *arg) {
//   while(1){
//     printf("3\r\n");
//     for (i_cnt = 0; i_cnt < 5000; i_cnt++);
//     osYield();
//   }
//}
//
//int main(void){
//    /* MCU Configuration: Don't change this or the whole chip won't work!*/
//    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//    HAL_Init();
//    /* Configure the system clock */
//    SystemClock_Config();
//    /* Initialize all configured peripherals */
//    MX_GPIO_Init();
//    MX_USART2_UART_Init();
//    /* MCU Configuration is now complete. Start writing your code below this line */
//
//    printf("Starting RTOS NEW TIME...\r\n");
//
//    osKernelInit();
//
//    TCB st_mytask;
//    st_mytask.stack_size = STACK_SIZE;
//
//    // Create tasks with adequate spacing between each allocation
//    st_mytask.ptask = &Task1;
//    if (osCreateTask(&st_mytask) != RTX_OK) {
//        printf("Failed to create Task1\r\n");
//    }
//
//    st_mytask.ptask = &Task2;
//    if (osCreateTask(&st_mytask) != RTX_OK) {
//        printf("Failed to create Task2\r\n");
//    }
//
//    st_mytask.ptask = &Task3;
//    if (osCreateTask(&st_mytask) != RTX_OK) {
//        printf("Failed to create Task3\r\n");
//    }
//
//    printf("Starting kernel...\r\n");
//    osKernelStart();
//
//    printf("back to main\r\n");
//    while (1);
//}
