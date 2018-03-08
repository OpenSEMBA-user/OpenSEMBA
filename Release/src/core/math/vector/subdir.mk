################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/math/vector/CVecI3Fractional.cpp \
../src/core/math/vector/Spherical.cpp 

OBJS += \
./src/core/math/vector/CVecI3Fractional.o \
./src/core/math/vector/Spherical.o 

CPP_DEPS += \
./src/core/math/vector/CVecI3Fractional.d \
./src/core/math/vector/Spherical.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/math/vector/%.o: ../src/core/math/vector/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


