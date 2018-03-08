################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/math/function/Gaussian.cpp 

OBJS += \
./src/core/math/function/Gaussian.o 

CPP_DEPS += \
./src/core/math/function/Gaussian.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/math/function/%.o: ../src/core/math/function/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


