################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/source/port/Port.cpp \
../src/core/source/port/TEM.cpp \
../src/core/source/port/TEMCoaxial.cpp \
../src/core/source/port/Waveguide.cpp \
../src/core/source/port/WaveguideRectangular.cpp 

OBJS += \
./src/core/source/port/Port.o \
./src/core/source/port/TEM.o \
./src/core/source/port/TEMCoaxial.o \
./src/core/source/port/Waveguide.o \
./src/core/source/port/WaveguideRectangular.o 

CPP_DEPS += \
./src/core/source/port/Port.d \
./src/core/source/port/TEM.d \
./src/core/source/port/TEMCoaxial.d \
./src/core/source/port/Waveguide.d \
./src/core/source/port/WaveguideRectangular.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/source/port/%.o: ../src/core/source/port/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


