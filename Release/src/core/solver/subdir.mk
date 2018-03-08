################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/solver/Solver.cpp 

OBJS += \
./src/core/solver/Solver.o 

CPP_DEPS += \
./src/core/solver/Solver.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/solver/%.o: ../src/core/solver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


