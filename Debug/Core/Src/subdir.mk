################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Src/svc_handler.s 

C_SRCS += \
../Core/Src/alloc_timing_test_w25.c \
../Core/Src/backward_compatibility_test1.c \
../Core/Src/kernel.c \
../Core/Src/main.c \
../Core/Src/memory.c \
../Core/Src/memory_functional_test_w25.c \
../Core/Src/robustness_test_w25.c \
../Core/Src/simple_periodic_tasks_test.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/taskcreate_test_w25.c \
../Core/Src/test.c \
../Core/Src/util.c 

OBJS += \
./Core/Src/alloc_timing_test_w25.o \
./Core/Src/backward_compatibility_test1.o \
./Core/Src/kernel.o \
./Core/Src/main.o \
./Core/Src/memory.o \
./Core/Src/memory_functional_test_w25.o \
./Core/Src/robustness_test_w25.o \
./Core/Src/simple_periodic_tasks_test.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/svc_handler.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/taskcreate_test_w25.o \
./Core/Src/test.o \
./Core/Src/util.o 

S_DEPS += \
./Core/Src/svc_handler.d 

C_DEPS += \
./Core/Src/alloc_timing_test_w25.d \
./Core/Src/backward_compatibility_test1.d \
./Core/Src/kernel.d \
./Core/Src/main.d \
./Core/Src/memory.d \
./Core/Src/memory_functional_test_w25.d \
./Core/Src/robustness_test_w25.d \
./Core/Src/simple_periodic_tasks_test.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/taskcreate_test_w25.d \
./Core/Src/test.d \
./Core/Src/util.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o: ../Core/Src/%.s Core/Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/alloc_timing_test_w25.cyclo ./Core/Src/alloc_timing_test_w25.d ./Core/Src/alloc_timing_test_w25.o ./Core/Src/alloc_timing_test_w25.su ./Core/Src/backward_compatibility_test1.cyclo ./Core/Src/backward_compatibility_test1.d ./Core/Src/backward_compatibility_test1.o ./Core/Src/backward_compatibility_test1.su ./Core/Src/kernel.cyclo ./Core/Src/kernel.d ./Core/Src/kernel.o ./Core/Src/kernel.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/memory.cyclo ./Core/Src/memory.d ./Core/Src/memory.o ./Core/Src/memory.su ./Core/Src/memory_functional_test_w25.cyclo ./Core/Src/memory_functional_test_w25.d ./Core/Src/memory_functional_test_w25.o ./Core/Src/memory_functional_test_w25.su ./Core/Src/robustness_test_w25.cyclo ./Core/Src/robustness_test_w25.d ./Core/Src/robustness_test_w25.o ./Core/Src/robustness_test_w25.su ./Core/Src/simple_periodic_tasks_test.cyclo ./Core/Src/simple_periodic_tasks_test.d ./Core/Src/simple_periodic_tasks_test.o ./Core/Src/simple_periodic_tasks_test.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/svc_handler.d ./Core/Src/svc_handler.o ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/taskcreate_test_w25.cyclo ./Core/Src/taskcreate_test_w25.d ./Core/Src/taskcreate_test_w25.o ./Core/Src/taskcreate_test_w25.su ./Core/Src/test.cyclo ./Core/Src/test.d ./Core/Src/test.o ./Core/Src/test.su ./Core/Src/util.cyclo ./Core/Src/util.d ./Core/Src/util.o ./Core/Src/util.su

.PHONY: clean-Core-2f-Src

