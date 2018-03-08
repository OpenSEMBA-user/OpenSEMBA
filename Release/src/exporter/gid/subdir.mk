################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/exporter/gid/Exporter.cpp \
../src/exporter/gid/ResultGiD.cpp 

OBJS += \
./src/exporter/gid/Exporter.o \
./src/exporter/gid/ResultGiD.o 

CPP_DEPS += \
./src/exporter/gid/Exporter.d \
./src/exporter/gid/ResultGiD.d 


# Each subdirectory must supply rules for building sources it contributes
src/exporter/gid/%.o: ../src/exporter/gid/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


