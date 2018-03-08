################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mesher/openfoam/Boundary.cpp \
../src/mesher/openfoam/Exporter.cpp \
../src/mesher/openfoam/Parser.cpp 

OBJS += \
./src/mesher/openfoam/Boundary.o \
./src/mesher/openfoam/Exporter.o \
./src/mesher/openfoam/Parser.o 

CPP_DEPS += \
./src/mesher/openfoam/Boundary.d \
./src/mesher/openfoam/Exporter.d \
./src/mesher/openfoam/Parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/mesher/openfoam/%.o: ../src/mesher/openfoam/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


