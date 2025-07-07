//#include "main.h"
//#include <stdio.h>
//#include "common.h"
//#include "k_task.h"
//#include "k_mem.h"
//
//int main(void) {
//    HAL_Init();
//    SystemClock_Config();
//    MX_GPIO_Init();
//    MX_USART2_UART_Init();
//
//    osKernelInit();
//    k_mem_init();
//
//    void* ptrs[12];
//
//    for (int i = 0; i < 11; i++) {
//        ptrs[i] = k_mem_alloc(4);
//        printf("block %d allocated, ptr=%p\r\n", i, ptrs[i]);
//    }
//
//    printf("deallocating %p\r\n", ptrs[0]);
//    k_mem_dealloc(ptrs[0]);
//
//    printf("deallocating %p\r\n", ptrs[2]);
//    k_mem_dealloc(ptrs[2]);
//
//    printf("deallocating %p\r\n", ptrs[5]);
//    k_mem_dealloc(ptrs[5]);
//
//    printf("deallocating %p\r\n", ptrs[9]);
//    k_mem_dealloc(ptrs[9]);
//
//    ptrs[11] = k_mem_alloc(16);
//    printf("block 11 allocated, ptr=%p\r\n", ptrs[11]);
//
//    printf("deallocating %p, which is before a free block\r\n", ptrs[4]);
//    k_mem_dealloc(ptrs[4]);
//    void* test_ptr = k_mem_alloc(8);
//    printf("testing if 8 bytes can fit at %p\r\n", ptrs[4]);
//    if (test_ptr == ptrs[4]) {
//        printf("PASS: k_mem_dealloc coalesced with a free block after it\r\n");
//    } else {
//    	printf("k_mem_alloc returned unexpected pointer %p\r\n", test_ptr);
//        printf("FAIL: k_mem_dealloc did not coalesce with a free block after it\r\n");
//    }
//    k_mem_dealloc(test_ptr);
//
//    printf("deallocating %p, which is after a free block\r\n", ptrs[6]);
//    k_mem_dealloc(ptrs[6]);
//    test_ptr = k_mem_alloc(12);
//    printf("testing if 12 bytes can fit at %p\r\n", ptrs[4]);
//    if (test_ptr == ptrs[4]) {
//        printf("PASS: k_mem_dealloc coalesced with a free block before it\r\n");
//    } else {
//    	printf("k_mem_alloc returned unexpected pointer %p\r\n", test_ptr);
//        printf("FAIL: k_mem_dealloc did not coalesce with a free block before it\r\n");
//    }
//    k_mem_dealloc(test_ptr);
//
//    printf("deallocating %p, which is between two free blocks\r\n", ptrs[1]);
//    k_mem_dealloc(ptrs[1]);
//    printf("testing if 12 bytes can fit at %p\r\n", ptrs[0]);
//    test_ptr = k_mem_alloc(12);
//    if (test_ptr == ptrs[0]) {
//        printf("PASS: k_mem_dealloc coalesced with two free blocks\r\n");
//    } else {
//        printf("k_mem_alloc returned unexpected pointer %p\r\n", test_ptr);
//        printf("FAIL: k_mem_dealloc did not coalesce with two free blocks\r\n");
//    }
//    k_mem_dealloc(test_ptr);
//
//    while (1);
//}
