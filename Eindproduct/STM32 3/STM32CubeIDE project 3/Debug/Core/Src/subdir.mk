################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/RFIDSensor.cpp \
../Core/Src/deurServo.cpp \
../Core/Src/deurServoTest.cpp \
../Core/Src/deurknop.cpp \
../Core/Src/i2cBeheer.cpp \
../Core/Src/i2cSlave.cpp \
../Core/Src/lamp.cpp \
../Core/Src/main.cpp \
../Core/Src/noodknop.cpp 

C_SRCS += \
../Core/Src/RC522.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

C_DEPS += \
./Core/Src/RC522.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 

OBJS += \
./Core/Src/RC522.o \
./Core/Src/RFIDSensor.o \
./Core/Src/deurServo.o \
./Core/Src/deurServoTest.o \
./Core/Src/deurknop.o \
./Core/Src/i2cBeheer.o \
./Core/Src/i2cSlave.o \
./Core/Src/lamp.o \
./Core/Src/main.o \
./Core/Src/noodknop.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

CPP_DEPS += \
./Core/Src/RFIDSensor.d \
./Core/Src/deurServo.d \
./Core/Src/deurServoTest.d \
./Core/Src/deurknop.d \
./Core/Src/i2cBeheer.d \
./Core/Src/i2cSlave.d \
./Core/Src/lamp.d \
./Core/Src/main.d \
./Core/Src/noodknop.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/RC522.cyclo ./Core/Src/RC522.d ./Core/Src/RC522.o ./Core/Src/RC522.su ./Core/Src/RFIDSensor.cyclo ./Core/Src/RFIDSensor.d ./Core/Src/RFIDSensor.o ./Core/Src/RFIDSensor.su ./Core/Src/deurServo.cyclo ./Core/Src/deurServo.d ./Core/Src/deurServo.o ./Core/Src/deurServo.su ./Core/Src/deurServoTest.cyclo ./Core/Src/deurServoTest.d ./Core/Src/deurServoTest.o ./Core/Src/deurServoTest.su ./Core/Src/deurknop.cyclo ./Core/Src/deurknop.d ./Core/Src/deurknop.o ./Core/Src/deurknop.su ./Core/Src/i2cBeheer.cyclo ./Core/Src/i2cBeheer.d ./Core/Src/i2cBeheer.o ./Core/Src/i2cBeheer.su ./Core/Src/i2cSlave.cyclo ./Core/Src/i2cSlave.d ./Core/Src/i2cSlave.o ./Core/Src/i2cSlave.su ./Core/Src/lamp.cyclo ./Core/Src/lamp.d ./Core/Src/lamp.o ./Core/Src/lamp.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/noodknop.cyclo ./Core/Src/noodknop.d ./Core/Src/noodknop.o ./Core/Src/noodknop.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src

