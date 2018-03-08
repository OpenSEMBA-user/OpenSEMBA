################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/core/math/util/DTFTTest.cpp \
../src/apps/test/core/math/util/FFTTest.cpp \
../src/apps/test/core/math/util/RealTest.cpp 

OBJS += \
./src/apps/test/core/math/util/DTFTTest.o \
./src/apps/test/core/math/util/FFTTest.o \
./src/apps/test/core/math/util/RealTest.o 

CPP_DEPS += \
./src/apps/test/core/math/util/DTFTTest.d \
./src/apps/test/core/math/util/FFTTest.d \
./src/apps/test/core/math/util/RealTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/core/math/util/%.o: ../src/apps/test/core/math/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


