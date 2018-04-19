################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f4xx.S 

OBJS += \
./startup/startup_stm32f4xx.o 

S_UPPER_DEPS += \
./startup/startup_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F4DISCOVERY -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/Utilities" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/StdPeriph_Driver/inc" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/inc" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/CMSIS/device" -I"C:/Users/N0tE/Documents/GitHub/2018_iDrink/Test/iDrink/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


