################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/parser/stl/Parser.cpp 

OBJS += \
./src/parser/stl/Parser.o 

CPP_DEPS += \
./src/parser/stl/Parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/parser/stl/%.o: ../src/parser/stl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


