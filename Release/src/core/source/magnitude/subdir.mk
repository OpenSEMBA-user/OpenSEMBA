################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/source/magnitude/Magnitude.cpp \
../src/core/source/magnitude/Numerical.cpp 

OBJS += \
./src/core/source/magnitude/Magnitude.o \
./src/core/source/magnitude/Numerical.o 

CPP_DEPS += \
./src/core/source/magnitude/Magnitude.d \
./src/core/source/magnitude/Numerical.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/source/magnitude/%.o: ../src/core/source/magnitude/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


