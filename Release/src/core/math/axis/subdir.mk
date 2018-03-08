################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/math/axis/Local.cpp 

OBJS += \
./src/core/math/axis/Local.o 

CPP_DEPS += \
./src/core/math/axis/Local.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/math/axis/%.o: ../src/core/math/axis/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


