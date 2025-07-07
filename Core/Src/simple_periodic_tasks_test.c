//#include "main.h"
//#include <stdio.h>
//#include "common.h"
//#include "k_task.h"
//#include "k_mem.h"
//
//int i_test = 0;
//
//int i_test2 = 0;
//
//void TaskA(void *arg) {
//   while(1){
//      printf("%d, %d\r\n", i_test, i_test2);
////      if (i_test == 10) {
////    	  while(1);
////      }
//      osPeriodYield();
//   }
//}
//
//void TaskB(void *arg) {
//   while(1){
//      i_test = i_test + 1;
//      osPeriodYield();
//   }
//}
//
//void TaskC(void *arg) {
//   while(1){
//      i_test2 = i_test2 + 1;
//      osPeriodYield();
//   }
//}
//
//
//int main(void){
//	/* MCU Configuration: Don't change this or the whole chip won't work!*/
//
//	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//	HAL_Init();
//
//	/* Configure the system clock */
//	SystemClock_Config();
//
//	/* Initialize all configured peripherals */
//	MX_GPIO_Init();
//	MX_USART2_UART_Init();
//	/* MCU Configuration is now complete. Start writing your code below this line */
//	printf("STARTING RTOS \r\n");
//	osKernelInit();
//
//	TCB st_mytask;
//	st_mytask.stack_size = STACK_SIZE;
//	st_mytask.ptask = &TaskA;
//	osCreateDeadlineTask(4, &st_mytask);
//
//	st_mytask.ptask = &TaskB;
//	osCreateDeadlineTask(4, &st_mytask);
//
//	st_mytask.ptask = &TaskC;
//	osCreateDeadlineTask(12, &st_mytask);
//
//	osKernelStart();
//
//	printf("back to main\r\n");
//	while (1);
//}
