################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/geometry/layer/Layer.cpp 

OBJS += \
./src/core/geometry/layer/Layer.o 

CPP_DEPS += \
./src/core/geometry/layer/Layer.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/geometry/layer/%.o: ../src/core/geometry/layer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


