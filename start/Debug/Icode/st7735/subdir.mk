################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Icode/st7735/st7735.c 

OBJS += \
./Icode/st7735/st7735.o 

C_DEPS += \
./Icode/st7735/st7735.d 


# Each subdirectory must supply rules for building sources it contributes
Icode/st7735/%.o Icode/st7735/%.su Icode/st7735/%.cyclo: ../Icode/st7735/%.c Icode/st7735/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Icode/st7735 -I../Icode/ad9834 -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Icode-2f-st7735

clean-Icode-2f-st7735:
	-$(RM) ./Icode/st7735/st7735.cyclo ./Icode/st7735/st7735.d ./Icode/st7735/st7735.o ./Icode/st7735/st7735.su

.PHONY: clean-Icode-2f-st7735

