################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/argument/Argument.cpp \
../src/core/argument/Formatter.cpp \
../src/core/argument/Group.cpp \
../src/core/argument/GroupBase.cpp \
../src/core/argument/MultiSwitch.cpp \
../src/core/argument/MutuallyExclusiveGroup.cpp \
../src/core/argument/OptionBase.cpp \
../src/core/argument/Parser.cpp \
../src/core/argument/Switch.cpp \
../src/core/argument/ValueMulti.cpp \
../src/core/argument/ValueSingle.cpp 

OBJS += \
./src/core/argument/Argument.o \
./src/core/argument/Formatter.o \
./src/core/argument/Group.o \
./src/core/argument/GroupBase.o \
./src/core/argument/MultiSwitch.o \
./src/core/argument/MutuallyExclusiveGroup.o \
./src/core/argument/OptionBase.o \
./src/core/argument/Parser.o \
./src/core/argument/Switch.o \
./src/core/argument/ValueMulti.o \
./src/core/argument/ValueSingle.o 

CPP_DEPS += \
./src/core/argument/Argument.d \
./src/core/argument/Formatter.d \
./src/core/argument/Group.d \
./src/core/argument/GroupBase.d \
./src/core/argument/MultiSwitch.d \
./src/core/argument/MutuallyExclusiveGroup.d \
./src/core/argument/OptionBase.d \
./src/core/argument/Parser.d \
./src/core/argument/Switch.d \
./src/core/argument/ValueMulti.d \
./src/core/argument/ValueSingle.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/argument/%.o: ../src/core/argument/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


