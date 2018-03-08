################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/math/matrix/Sparse.cpp 

OBJS += \
./src/core/math/matrix/Sparse.o 

CPP_DEPS += \
./src/core/math/matrix/Sparse.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/math/matrix/%.o: ../src/core/math/matrix/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


