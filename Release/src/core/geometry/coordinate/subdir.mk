################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/geometry/coordinate/Conformal.cpp \
../src/core/geometry/coordinate/Coordinate.cpp \
../src/core/geometry/coordinate/Relative.cpp 

OBJS += \
./src/core/geometry/coordinate/Conformal.o \
./src/core/geometry/coordinate/Coordinate.o \
./src/core/geometry/coordinate/Relative.o 

CPP_DEPS += \
./src/core/geometry/coordinate/Conformal.d \
./src/core/geometry/coordinate/Coordinate.d \
./src/core/geometry/coordinate/Relative.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/geometry/coordinate/%.o: ../src/core/geometry/coordinate/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


