################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/core/geometry/mesh/GeometricTest.cpp \
../src/apps/test/core/geometry/mesh/MeshTest.cpp \
../src/apps/test/core/geometry/mesh/UnstructuredTest.cpp 

OBJS += \
./src/apps/test/core/geometry/mesh/GeometricTest.o \
./src/apps/test/core/geometry/mesh/MeshTest.o \
./src/apps/test/core/geometry/mesh/UnstructuredTest.o 

CPP_DEPS += \
./src/apps/test/core/geometry/mesh/GeometricTest.d \
./src/apps/test/core/geometry/mesh/MeshTest.d \
./src/apps/test/core/geometry/mesh/UnstructuredTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/core/geometry/mesh/%.o: ../src/apps/test/core/geometry/mesh/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


