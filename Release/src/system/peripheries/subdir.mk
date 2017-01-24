################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/system/peripheries/Motors.cpp \
../src/system/peripheries/PowerSupply.cpp \
../src/system/peripheries/Vision.cpp \
../src/system/peripheries/comm_protocol_threadsafe.cpp 

OBJS += \
./src/system/peripheries/Motors.o \
./src/system/peripheries/PowerSupply.o \
./src/system/peripheries/Vision.o \
./src/system/peripheries/comm_protocol_threadsafe.o 

CPP_DEPS += \
./src/system/peripheries/Motors.d \
./src/system/peripheries/PowerSupply.d \
./src/system/peripheries/Vision.d \
./src/system/peripheries/comm_protocol_threadsafe.d 


# Each subdirectory must supply rules for building sources it contributes
src/system/peripheries/%.o: ../src/system/peripheries/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


