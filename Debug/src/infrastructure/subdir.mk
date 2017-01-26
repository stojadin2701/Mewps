################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/infrastructure/EnvironmentMap.cpp \
../src/infrastructure/Task.cpp \
../src/infrastructure/TaskExecuter.cpp \
../src/infrastructure/initialization.cpp 

OBJS += \
./src/infrastructure/EnvironmentMap.o \
./src/infrastructure/Task.o \
./src/infrastructure/TaskExecuter.o \
./src/infrastructure/initialization.o 

CPP_DEPS += \
./src/infrastructure/EnvironmentMap.d \
./src/infrastructure/Task.d \
./src/infrastructure/TaskExecuter.d \
./src/infrastructure/initialization.d 


# Each subdirectory must supply rules for building sources it contributes
src/infrastructure/%.o: ../src/infrastructure/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


