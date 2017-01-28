################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/infrastructure/peripherals/hardware/SerialCommunicator.cpp 

OBJS += \
./src/infrastructure/peripherals/hardware/SerialCommunicator.o 

CPP_DEPS += \
./src/infrastructure/peripherals/hardware/SerialCommunicator.d 


# Each subdirectory must supply rules for building sources it contributes
src/infrastructure/peripherals/hardware/%.o: ../src/infrastructure/peripherals/hardware/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


