################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Libraries/BusDevice.cpp \
../Libraries/I2CDevice.cpp \
../Libraries/SPIDevice.cpp \
../Libraries/UartDevice.cpp 

OBJS += \
./Libraries/BusDevice.o \
./Libraries/I2CDevice.o \
./Libraries/SPIDevice.o \
./Libraries/UartDevice.o 

CPP_DEPS += \
./Libraries/BusDevice.d \
./Libraries/I2CDevice.d \
./Libraries/SPIDevice.d \
./Libraries/UartDevice.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/%.o: ../Libraries/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


