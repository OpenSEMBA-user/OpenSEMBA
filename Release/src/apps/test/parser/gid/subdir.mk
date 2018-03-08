################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/parser/gid/ParserTest.cpp 

OBJS += \
./src/apps/test/parser/gid/ParserTest.o 

CPP_DEPS += \
./src/apps/test/parser/gid/ParserTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/parser/gid/%.o: ../src/apps/test/parser/gid/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


