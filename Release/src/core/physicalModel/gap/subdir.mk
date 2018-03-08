################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/physicalModel/gap/Gap.cpp 

OBJS += \
./src/core/physicalModel/gap/Gap.o 

CPP_DEPS += \
./src/core/physicalModel/gap/Gap.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/physicalModel/gap/%.o: ../src/core/physicalModel/gap/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


