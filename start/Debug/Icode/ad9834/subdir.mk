################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Icode/ad9834/ad9834.c 

OBJS += \
./Icode/ad9834/ad9834.o 

C_DEPS += \
./Icode/ad9834/ad9834.d 


# Each subdirectory must supply rules for building sources it contributes
Icode/ad9834/%.o Icode/ad9834/%.su Icode/ad9834/%.cyclo: ../Icode/ad9834/%.c Icode/ad9834/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Icode-2f-ad9834

clean-Icode-2f-ad9834:
	-$(RM) ./Icode/ad9834/ad9834.cyclo ./Icode/ad9834/ad9834.d ./Icode/ad9834/ad9834.o ./Icode/ad9834/ad9834.su

.PHONY: clean-Icode-2f-ad9834

