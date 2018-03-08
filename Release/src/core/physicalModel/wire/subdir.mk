################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/physicalModel/wire/Extremes.cpp \
../src/core/physicalModel/wire/Wire.cpp 

OBJS += \
./src/core/physicalModel/wire/Extremes.o \
./src/core/physicalModel/wire/Wire.o 

CPP_DEPS += \
./src/core/physicalModel/wire/Extremes.d \
./src/core/physicalModel/wire/Wire.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/physicalModel/wire/%.o: ../src/core/physicalModel/wire/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


