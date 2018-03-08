################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/parser/Parser.cpp 

OBJS += \
./src/core/parser/Parser.o 

CPP_DEPS += \
./src/core/parser/Parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/parser/%.o: ../src/core/parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


