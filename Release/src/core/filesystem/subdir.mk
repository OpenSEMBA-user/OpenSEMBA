################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/filesystem/Project.cpp 

OBJS += \
./src/core/filesystem/Project.o 

CPP_DEPS += \
./src/core/filesystem/Project.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/filesystem/%.o: ../src/core/filesystem/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


