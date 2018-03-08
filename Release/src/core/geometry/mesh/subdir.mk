################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/geometry/mesh/Geometric.cpp \
../src/core/geometry/mesh/Mesh.cpp \
../src/core/geometry/mesh/Structured.cpp \
../src/core/geometry/mesh/Unstructured.cpp 

OBJS += \
./src/core/geometry/mesh/Geometric.o \
./src/core/geometry/mesh/Mesh.o \
./src/core/geometry/mesh/Structured.o \
./src/core/geometry/mesh/Unstructured.o 

CPP_DEPS += \
./src/core/geometry/mesh/Geometric.d \
./src/core/geometry/mesh/Mesh.d \
./src/core/geometry/mesh/Structured.d \
./src/core/geometry/mesh/Unstructured.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/geometry/mesh/%.o: ../src/core/geometry/mesh/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


