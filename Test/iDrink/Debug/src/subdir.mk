################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/menu.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/tm_stm32f4_delay.c \
../src/tm_stm32f4_gpio.c \
../src/tm_stm32f4_hd44780.c \
../src/tm_stm32f4_timer_properties.c 

OBJS += \
./src/main.o \
./src/menu.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/tm_stm32f4_delay.o \
./src/tm_stm32f4_gpio.o \
./src/tm_stm32f4_hd44780.o \
./src/tm_stm32f4_timer_properties.o 

C_DEPS += \
./src/main.d \
./src/menu.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/tm_stm32f4_delay.d \
./src/tm_stm32f4_gpio.d \
./src/tm_stm32f4_hd44780.d \
./src/tm_stm32f4_timer_properties.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F4DISCOVERY -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/Utilities" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/StdPeriph_Driver/inc" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/inc" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/CMSIS/device" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


