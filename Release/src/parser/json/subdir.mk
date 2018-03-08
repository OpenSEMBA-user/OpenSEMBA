################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/parser/json/Parser.cpp 

OBJS += \
./src/parser/json/Parser.o 

CPP_DEPS += \
./src/parser/json/Parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/parser/json/%.o: ../src/parser/json/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


