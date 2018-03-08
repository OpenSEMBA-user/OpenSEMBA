################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mesher/Options.cpp 

OBJS += \
./src/mesher/Options.o 

CPP_DEPS += \
./src/mesher/Options.d 


# Each subdirectory must supply rules for building sources it contributes
src/mesher/%.o: ../src/mesher/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


