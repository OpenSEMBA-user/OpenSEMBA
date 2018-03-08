################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/core/geometry/graph/ConnectivitiesTest.cpp \
../src/apps/test/core/geometry/graph/VerticesTest.cpp 

OBJS += \
./src/apps/test/core/geometry/graph/ConnectivitiesTest.o \
./src/apps/test/core/geometry/graph/VerticesTest.o 

CPP_DEPS += \
./src/apps/test/core/geometry/graph/ConnectivitiesTest.d \
./src/apps/test/core/geometry/graph/VerticesTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/core/geometry/graph/%.o: ../src/apps/test/core/geometry/graph/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


