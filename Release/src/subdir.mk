################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/EnvironmentMap.cpp \
../src/Motors.cpp \
../src/Task.cpp \
../src/TaskExecuter.cpp \
../src/Vision.cpp \
../src/comm_protocol_threadsafe.cpp \
../src/initialization.cpp \
../src/main.cpp 

C_SRCS += \
../src/serial_comm.c 

OBJS += \
./src/EnvironmentMap.o \
./src/Motors.o \
./src/Task.o \
./src/TaskExecuter.o \
./src/Vision.o \
./src/comm_protocol_threadsafe.o \
./src/initialization.o \
./src/main.o \
./src/serial_comm.o 

CPP_DEPS += \
./src/EnvironmentMap.d \
./src/Motors.d \
./src/Task.d \
./src/TaskExecuter.d \
./src/Vision.d \
./src/comm_protocol_threadsafe.d \
./src/initialization.d \
./src/main.d 

C_DEPS += \
./src/serial_comm.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=gnu11 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


