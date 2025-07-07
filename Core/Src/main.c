///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file           : main.c
//  * @brief          : Main program body
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2023 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include "k_task.h"
//#include <stdio.h> //You are permitted to use this library, but currently only printf is implemented. Anything else is up to you!
//
//void print_continuously(void){
//	while(1){
//		printf("Thread!\r\n");
//	}
//}
//
//extern uint32_t _Min_Stack_Size;
//uint32_t* stackptr;
//
///**
//  * @brief  The application entry point.
//  * @retval int
//  */
//int main(void)
//{
//
//  /* MCU Configuration: Don't change this or the whole chip won't work!*/
//
//  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//  HAL_Init();
//  /* Configure the system clock */
//  SystemClock_Config();
//
//  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
//  MX_USART2_UART_Init();
//  /* MCU Configuration is now complete. Start writing your code below this line */
//
//  /* Prelab 1: - Part 1*/
//  uint32_t* MSP_INIT_VAL = *(uint32_t**)0x0;
//  printf("MSP Init is: %p\r\n", MSP_INIT_VAL); //note the %p to print a pointer. It will be in hex
//
//  /* Prelab 1 - Part 2*/
////  EnablePrivilegedMode();
//
//  /* Prelab 1 - Part 3*/
//  stackptr = (uint32_t*)((uint32_t)MSP_INIT_VAL - (uint32_t)&(_Min_Stack_Size));
//
//  *(--stackptr) = 1<<24;
//  *(--stackptr) = (uint32_t) print_continuously;
//  for (int i = 0; i < 14; i++)
//  {
//      *(--stackptr) = 0xA;
//  }
//
//  StartThread();
//
//  printf("_____END MAIN_____\r\n");
//
//  /* Infinite loop */
//  /* USER CODE BEGIN WHILE */
//  while (1)
//  {
//    /* USER CODE END WHILE */
////	  printf("Hello, world!\r\n");
//    /* USER CODE BEGIN 3 */
//  }
//  /* USER CODE END 3 */
//}
//
