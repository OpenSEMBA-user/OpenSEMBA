################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/core/math/matrix/DynamicTest.cpp \
../src/apps/test/core/math/matrix/StaticTest.cpp 

OBJS += \
./src/apps/test/core/math/matrix/DynamicTest.o \
./src/apps/test/core/math/matrix/StaticTest.o 

CPP_DEPS += \
./src/apps/test/core/math/matrix/DynamicTest.d \
./src/apps/test/core/math/matrix/StaticTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/core/math/matrix/%.o: ../src/apps/test/core/math/matrix/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


