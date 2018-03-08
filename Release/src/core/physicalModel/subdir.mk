################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/physicalModel/PhysicalModel.cpp 

OBJS += \
./src/core/physicalModel/PhysicalModel.o 

CPP_DEPS += \
./src/core/physicalModel/PhysicalModel.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/physicalModel/%.o: ../src/core/physicalModel/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


