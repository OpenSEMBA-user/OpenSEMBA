################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/core/math/simplex/LineTest.cpp \
../src/apps/test/core/math/simplex/TetrahedronTest.cpp \
../src/apps/test/core/math/simplex/TriangleTest.cpp 

OBJS += \
./src/apps/test/core/math/simplex/LineTest.o \
./src/apps/test/core/math/simplex/TetrahedronTest.o \
./src/apps/test/core/math/simplex/TriangleTest.o 

CPP_DEPS += \
./src/apps/test/core/math/simplex/LineTest.d \
./src/apps/test/core/math/simplex/TetrahedronTest.d \
./src/apps/test/core/math/simplex/TriangleTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/core/math/simplex/%.o: ../src/apps/test/core/math/simplex/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


