################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ECUAL/Src/Buzzer.c \
../ECUAL/Src/DC_MOTOR.c \
../ECUAL/Src/DC_MOTOR_cfg.c \
../ECUAL/Src/HCSR04.c \
../ECUAL/Src/HCSR04_cfg.c \
../ECUAL/Src/LED.c 

OBJS += \
./ECUAL/Src/Buzzer.o \
./ECUAL/Src/DC_MOTOR.o \
./ECUAL/Src/DC_MOTOR_cfg.o \
./ECUAL/Src/HCSR04.o \
./ECUAL/Src/HCSR04_cfg.o \
./ECUAL/Src/LED.o 

C_DEPS += \
./ECUAL/Src/Buzzer.d \
./ECUAL/Src/DC_MOTOR.d \
./ECUAL/Src/DC_MOTOR_cfg.d \
./ECUAL/Src/HCSR04.d \
./ECUAL/Src/HCSR04_cfg.d \
./ECUAL/Src/LED.d 


# Each subdirectory must supply rules for building sources it contributes
ECUAL/Src/%.o ECUAL/Src/%.su ECUAL/Src/%.cyclo: ../ECUAL/Src/%.c ECUAL/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ECUAL-2f-Src

clean-ECUAL-2f-Src:
	-$(RM) ./ECUAL/Src/Buzzer.cyclo ./ECUAL/Src/Buzzer.d ./ECUAL/Src/Buzzer.o ./ECUAL/Src/Buzzer.su ./ECUAL/Src/DC_MOTOR.cyclo ./ECUAL/Src/DC_MOTOR.d ./ECUAL/Src/DC_MOTOR.o ./ECUAL/Src/DC_MOTOR.su ./ECUAL/Src/DC_MOTOR_cfg.cyclo ./ECUAL/Src/DC_MOTOR_cfg.d ./ECUAL/Src/DC_MOTOR_cfg.o ./ECUAL/Src/DC_MOTOR_cfg.su ./ECUAL/Src/HCSR04.cyclo ./ECUAL/Src/HCSR04.d ./ECUAL/Src/HCSR04.o ./ECUAL/Src/HCSR04.su ./ECUAL/Src/HCSR04_cfg.cyclo ./ECUAL/Src/HCSR04_cfg.d ./ECUAL/Src/HCSR04_cfg.o ./ECUAL/Src/HCSR04_cfg.su ./ECUAL/Src/LED.cyclo ./ECUAL/Src/LED.d ./ECUAL/Src/LED.o ./ECUAL/Src/LED.su

.PHONY: clean-ECUAL-2f-Src

