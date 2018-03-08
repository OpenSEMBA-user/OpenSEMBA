################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/physicalModel/volume/Anisotropic.cpp \
../src/core/physicalModel/volume/AnisotropicCrystal.cpp \
../src/core/physicalModel/volume/AnisotropicFerrite.cpp \
../src/core/physicalModel/volume/Classic.cpp \
../src/core/physicalModel/volume/Dispersive.cpp \
../src/core/physicalModel/volume/PML.cpp \
../src/core/physicalModel/volume/Volume.cpp 

OBJS += \
./src/core/physicalModel/volume/Anisotropic.o \
./src/core/physicalModel/volume/AnisotropicCrystal.o \
./src/core/physicalModel/volume/AnisotropicFerrite.o \
./src/core/physicalModel/volume/Classic.o \
./src/core/physicalModel/volume/Dispersive.o \
./src/core/physicalModel/volume/PML.o \
./src/core/physicalModel/volume/Volume.o 

CPP_DEPS += \
./src/core/physicalModel/volume/Anisotropic.d \
./src/core/physicalModel/volume/AnisotropicCrystal.d \
./src/core/physicalModel/volume/AnisotropicFerrite.d \
./src/core/physicalModel/volume/Classic.d \
./src/core/physicalModel/volume/Dispersive.d \
./src/core/physicalModel/volume/PML.d \
./src/core/physicalModel/volume/Volume.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/physicalModel/volume/%.o: ../src/core/physicalModel/volume/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


