################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/system/peripherals/Motors.cpp \
../src/system/peripherals/PowerSupply.cpp \
../src/system/peripherals/Vision.cpp \
../src/system/peripherals/comm_protocol_threadsafe.cpp 

OBJS += \
./src/system/peripherals/Motors.o \
./src/system/peripherals/PowerSupply.o \
./src/system/peripherals/Vision.o \
./src/system/peripherals/comm_protocol_threadsafe.o 

CPP_DEPS += \
./src/system/peripherals/Motors.d \
./src/system/peripherals/PowerSupply.d \
./src/system/peripherals/Vision.d \
./src/system/peripherals/comm_protocol_threadsafe.d 


# Each subdirectory must supply rules for building sources it contributes
src/system/peripherals/%.o: ../src/system/peripherals/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


