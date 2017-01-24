################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/system/peripherals/Camera.cpp \
../src/system/peripherals/DistanceSensor.cpp \
../src/system/peripherals/Microphones.cpp \
../src/system/peripherals/Motors.cpp \
../src/system/peripherals/PowerSupply.cpp \
../src/system/peripherals/comm_protocol_threadsafe.cpp 

OBJS += \
./src/system/peripherals/Camera.o \
./src/system/peripherals/DistanceSensor.o \
./src/system/peripherals/Microphones.o \
./src/system/peripherals/Motors.o \
./src/system/peripherals/PowerSupply.o \
./src/system/peripherals/comm_protocol_threadsafe.o 

CPP_DEPS += \
./src/system/peripherals/Camera.d \
./src/system/peripherals/DistanceSensor.d \
./src/system/peripherals/Microphones.d \
./src/system/peripherals/Motors.d \
./src/system/peripherals/PowerSupply.d \
./src/system/peripherals/comm_protocol_threadsafe.d 


# Each subdirectory must supply rules for building sources it contributes
src/system/peripherals/%.o: ../src/system/peripherals/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


