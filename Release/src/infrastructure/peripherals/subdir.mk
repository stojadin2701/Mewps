################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/infrastructure/peripherals/Accelerometer.cpp \
../src/infrastructure/peripherals/Camera.cpp \
../src/infrastructure/peripherals/CommProtocolThreadsafe.cpp \
../src/infrastructure/peripherals/DistanceSensor.cpp \
../src/infrastructure/peripherals/Microphones.cpp \
../src/infrastructure/peripherals/Motors.cpp \
../src/infrastructure/peripherals/PowerSupply.cpp \
../src/infrastructure/peripherals/Speaker.cpp \
../src/infrastructure/peripherals/Magnetometer.cpp

OBJS += \
./src/infrastructure/peripherals/Accelerometer.o \
./src/infrastructure/peripherals/Camera.o \
./src/infrastructure/peripherals/CommProtocolThreadsafe.o \
./src/infrastructure/peripherals/DistanceSensor.o \
./src/infrastructure/peripherals/Microphones.o \
./src/infrastructure/peripherals/Motors.o \
./src/infrastructure/peripherals/PowerSupply.o \
./src/infrastructure/peripherals/Speaker.o \
./src/infrastructure/peripherals/Magnetometer.o

CPP_DEPS += \
./src/infrastructure/peripherals/Accelerometer.d \
./src/infrastructure/peripherals/Camera.d \
./src/infrastructure/peripherals/CommProtocolThreadsafe.d \
./src/infrastructure/peripherals/DistanceSensor.d \
./src/infrastructure/peripherals/Microphones.d \
./src/infrastructure/peripherals/Motors.d \
./src/infrastructure/peripherals/PowerSupply.d \
./src/infrastructure/peripherals/Speaker.d \
./src/infrastructure/peripherals/Magnetometer.d


# Each subdirectory must supply rules for building sources it contributes
src/infrastructure/peripherals/%.o: ../src/infrastructure/peripherals/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


