################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/source/Dipole.cpp \
../src/core/source/Generator.cpp \
../src/core/source/OnLine.cpp \
../src/core/source/PlaneWave.cpp \
../src/core/source/Source.cpp 

OBJS += \
./src/core/source/Dipole.o \
./src/core/source/Generator.o \
./src/core/source/OnLine.o \
./src/core/source/PlaneWave.o \
./src/core/source/Source.o 

CPP_DEPS += \
./src/core/source/Dipole.d \
./src/core/source/Generator.d \
./src/core/source/OnLine.d \
./src/core/source/PlaneWave.d \
./src/core/source/Source.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/source/%.o: ../src/core/source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


