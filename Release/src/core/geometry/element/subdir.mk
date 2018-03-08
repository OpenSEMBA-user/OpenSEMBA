################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/geometry/element/Element.cpp \
../src/core/geometry/element/LineConformal.cpp \
../src/core/geometry/element/Polygon.cpp \
../src/core/geometry/element/Polyhedron.cpp \
../src/core/geometry/element/Tetrahedron.cpp \
../src/core/geometry/element/Tetrahedron10.cpp \
../src/core/geometry/element/Tetrahedron4.cpp \
../src/core/geometry/element/Triangle.cpp \
../src/core/geometry/element/Triangle3.cpp \
../src/core/geometry/element/Triangle6.cpp 

OBJS += \
./src/core/geometry/element/Element.o \
./src/core/geometry/element/LineConformal.o \
./src/core/geometry/element/Polygon.o \
./src/core/geometry/element/Polyhedron.o \
./src/core/geometry/element/Tetrahedron.o \
./src/core/geometry/element/Tetrahedron10.o \
./src/core/geometry/element/Tetrahedron4.o \
./src/core/geometry/element/Triangle.o \
./src/core/geometry/element/Triangle3.o \
./src/core/geometry/element/Triangle6.o 

CPP_DEPS += \
./src/core/geometry/element/Element.d \
./src/core/geometry/element/LineConformal.d \
./src/core/geometry/element/Polygon.d \
./src/core/geometry/element/Polyhedron.d \
./src/core/geometry/element/Tetrahedron.d \
./src/core/geometry/element/Tetrahedron10.d \
./src/core/geometry/element/Tetrahedron4.d \
./src/core/geometry/element/Triangle.d \
./src/core/geometry/element/Triangle3.d \
./src/core/geometry/element/Triangle6.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/geometry/element/%.o: ../src/core/geometry/element/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


