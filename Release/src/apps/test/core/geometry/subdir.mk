################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/core/geometry/BoxTest.cpp \
../src/apps/test/core/geometry/GridTest.cpp 

OBJS += \
./src/apps/test/core/geometry/BoxTest.o \
./src/apps/test/core/geometry/GridTest.o 

CPP_DEPS += \
./src/apps/test/core/geometry/BoxTest.d \
./src/apps/test/core/geometry/GridTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/core/geometry/%.o: ../src/apps/test/core/geometry/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


