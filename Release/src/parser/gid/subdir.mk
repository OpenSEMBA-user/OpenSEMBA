################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/parser/gid/Parser.cpp \
../src/parser/gid/ProblemSize.cpp 

OBJS += \
./src/parser/gid/Parser.o \
./src/parser/gid/ProblemSize.o 

CPP_DEPS += \
./src/parser/gid/Parser.d \
./src/parser/gid/ProblemSize.d 


# Each subdirectory must supply rules for building sources it contributes
src/parser/gid/%.o: ../src/parser/gid/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


