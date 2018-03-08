################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/physicalModel/predefined/PEC.cpp \
../src/core/physicalModel/predefined/PMC.cpp \
../src/core/physicalModel/predefined/Predefined.cpp \
../src/core/physicalModel/predefined/SMA.cpp 

OBJS += \
./src/core/physicalModel/predefined/PEC.o \
./src/core/physicalModel/predefined/PMC.o \
./src/core/physicalModel/predefined/Predefined.o \
./src/core/physicalModel/predefined/SMA.o 

CPP_DEPS += \
./src/core/physicalModel/predefined/PEC.d \
./src/core/physicalModel/predefined/PMC.d \
./src/core/physicalModel/predefined/Predefined.d \
./src/core/physicalModel/predefined/SMA.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/physicalModel/predefined/%.o: ../src/core/physicalModel/predefined/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


