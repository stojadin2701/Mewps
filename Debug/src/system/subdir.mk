################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/system/EnvironmentMap.cpp \
../src/system/Motors.cpp \
../src/system/Task.cpp \
../src/system/TaskExecuter.cpp \
../src/system/Vision.cpp \
../src/system/comm_protocol_threadsafe.cpp \
../src/system/initialization.cpp 

OBJS += \
./src/system/EnvironmentMap.o \
./src/system/Motors.o \
./src/system/Task.o \
./src/system/TaskExecuter.o \
./src/system/Vision.o \
./src/system/comm_protocol_threadsafe.o \
./src/system/initialization.o 

CPP_DEPS += \
./src/system/EnvironmentMap.d \
./src/system/Motors.d \
./src/system/Task.d \
./src/system/TaskExecuter.d \
./src/system/Vision.d \
./src/system/comm_protocol_threadsafe.d \
./src/system/initialization.d 


# Each subdirectory must supply rules for building sources it contributes
src/system/%.o: ../src/system/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


