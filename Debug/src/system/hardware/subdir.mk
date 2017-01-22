################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/system/hardware/battery_reader.c \
../src/system/hardware/serial_comm.c 

OBJS += \
./src/system/hardware/battery_reader.o \
./src/system/hardware/serial_comm.o 

C_DEPS += \
./src/system/hardware/battery_reader.d \
./src/system/hardware/serial_comm.d 


# Each subdirectory must supply rules for building sources it contributes
src/system/hardware/%.o: ../src/system/hardware/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=gnu11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


