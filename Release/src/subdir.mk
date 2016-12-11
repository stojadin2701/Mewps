################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/EnvironmentMap.cpp \
../src/Motor.cpp \
../src/Task.cpp \
../src/TaskExecuter.cpp \
../src/Vision.cpp \
../src/main.cpp 

OBJS += \
./src/EnvironmentMap.o \
./src/Motor.o \
./src/Task.o \
./src/TaskExecuter.o \
./src/Vision.o \
./src/main.o 

CPP_DEPS += \
./src/EnvironmentMap.d \
./src/Motor.d \
./src/Task.d \
./src/TaskExecuter.d \
./src/Vision.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


