################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/apps/test/core/argument/ArgumentTest.cpp \
../src/apps/test/core/argument/ObjectTest.cpp \
../src/apps/test/core/argument/OptionBaseTest.cpp \
../src/apps/test/core/argument/OptionsSolverTest.cpp \
../src/apps/test/core/argument/ParserTest.cpp 

OBJS += \
./src/apps/test/core/argument/ArgumentTest.o \
./src/apps/test/core/argument/ObjectTest.o \
./src/apps/test/core/argument/OptionBaseTest.o \
./src/apps/test/core/argument/OptionsSolverTest.o \
./src/apps/test/core/argument/ParserTest.o 

CPP_DEPS += \
./src/apps/test/core/argument/ArgumentTest.d \
./src/apps/test/core/argument/ObjectTest.d \
./src/apps/test/core/argument/OptionBaseTest.d \
./src/apps/test/core/argument/OptionsSolverTest.d \
./src/apps/test/core/argument/ParserTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/apps/test/core/argument/%.o: ../src/apps/test/core/argument/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


