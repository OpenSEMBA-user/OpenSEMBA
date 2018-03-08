################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/physicalModel/surface/Multilayer.cpp \
../src/core/physicalModel/surface/SIBC.cpp \
../src/core/physicalModel/surface/Surface.cpp 

OBJS += \
./src/core/physicalModel/surface/Multilayer.o \
./src/core/physicalModel/surface/SIBC.o \
./src/core/physicalModel/surface/Surface.o 

CPP_DEPS += \
./src/core/physicalModel/surface/Multilayer.d \
./src/core/physicalModel/surface/SIBC.d \
./src/core/physicalModel/surface/Surface.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/physicalModel/surface/%.o: ../src/core/physicalModel/surface/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


