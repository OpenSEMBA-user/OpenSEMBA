################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../external/gidpost/gidpost.c \
../external/gidpost/gidpostHDF5.c \
../external/gidpost/gidpostHash.c \
../external/gidpost/gidpostInt.c \
../external/gidpost/hashtab.c \
../external/gidpost/lookupa.c \
../external/gidpost/recycle.c \
../external/gidpost/zlibint1.c \
../external/gidpost/zlibint2.c 

OBJS += \
./external/gidpost/gidpost.o \
./external/gidpost/gidpostHDF5.o \
./external/gidpost/gidpostHash.o \
./external/gidpost/gidpostInt.o \
./external/gidpost/hashtab.o \
./external/gidpost/lookupa.o \
./external/gidpost/recycle.o \
./external/gidpost/zlibint1.o \
./external/gidpost/zlibint2.o 

C_DEPS += \
./external/gidpost/gidpost.d \
./external/gidpost/gidpostHDF5.d \
./external/gidpost/gidpostHash.d \
./external/gidpost/gidpostInt.d \
./external/gidpost/hashtab.d \
./external/gidpost/lookupa.d \
./external/gidpost/recycle.d \
./external/gidpost/zlibint1.d \
./external/gidpost/zlibint2.d 


# Each subdirectory must supply rules for building sources it contributes
external/gidpost/%.o: ../external/gidpost/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


