################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/exporter/Exporter.cpp 

OBJS += \
./src/core/exporter/Exporter.o 

CPP_DEPS += \
./src/core/exporter/Exporter.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/exporter/%.o: ../src/core/exporter/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


