################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/math/util/DTFT.cpp \
../src/core/math/util/FFT.cpp \
../src/core/math/util/Real.cpp 

OBJS += \
./src/core/math/util/DTFT.o \
./src/core/math/util/FFT.o \
./src/core/math/util/Real.o 

CPP_DEPS += \
./src/core/math/util/DTFT.d \
./src/core/math/util/FFT.d \
./src/core/math/util/Real.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/math/util/%.o: ../src/core/math/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


