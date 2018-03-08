################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/solver/subdir.mk
-include src/parser/stl/subdir.mk
-include src/parser/json/subdir.mk
-include src/parser/gid/subdir.mk
-include src/mesher/openfoam/subdir.mk
-include src/mesher/subdir.mk
-include src/exporter/vtk/subdir.mk
-include src/exporter/gid/subdir.mk
-include src/core/util/subdir.mk
-include src/core/source/port/subdir.mk
-include src/core/source/magnitude/subdir.mk
-include src/core/source/subdir.mk
-include src/core/solver/subdir.mk
-include src/core/physicalModel/wire/subdir.mk
-include src/core/physicalModel/volume/subdir.mk
-include src/core/physicalModel/surface/subdir.mk
-include src/core/physicalModel/predefined/subdir.mk
-include src/core/physicalModel/multiport/subdir.mk
-include src/core/physicalModel/gap/subdir.mk
-include src/core/physicalModel/bound/subdir.mk
-include src/core/physicalModel/subdir.mk
-include src/core/parser/subdir.mk
-include src/core/outputRequest/subdir.mk
-include src/core/math/vector/subdir.mk
-include src/core/math/util/subdir.mk
-include src/core/math/simplex/subdir.mk
-include src/core/math/matrix/subdir.mk
-include src/core/math/function/subdir.mk
-include src/core/math/axis/subdir.mk
-include src/core/geometry/mesh/subdir.mk
-include src/core/geometry/layer/subdir.mk
-include src/core/geometry/graph/subdir.mk
-include src/core/geometry/element/subdir.mk
-include src/core/geometry/coordinate/subdir.mk
-include src/core/filesystem/subdir.mk
-include src/core/exporter/subdir.mk
-include src/core/argument/subdir.mk
-include src/core/subdir.mk
-include src/apps/test/parser/stl/subdir.mk
-include src/apps/test/parser/json/subdir.mk
-include src/apps/test/parser/gid/subdir.mk
-include src/apps/test/core/source/port/subdir.mk
-include src/apps/test/core/source/subdir.mk
-include src/apps/test/core/physicalModel/volume/subdir.mk
-include src/apps/test/core/physicalModel/subdir.mk
-include src/apps/test/core/math/vector/subdir.mk
-include src/apps/test/core/math/util/subdir.mk
-include src/apps/test/core/math/simplex/subdir.mk
-include src/apps/test/core/math/matrix/subdir.mk
-include src/apps/test/core/math/function/subdir.mk
-include src/apps/test/core/math/axis/subdir.mk
-include src/apps/test/core/group/subdir.mk
-include src/apps/test/core/geometry/mesh/subdir.mk
-include src/apps/test/core/geometry/layer/subdir.mk
-include src/apps/test/core/geometry/graph/subdir.mk
-include src/apps/test/core/geometry/element/subdir.mk
-include src/apps/test/core/geometry/coordinate/subdir.mk
-include src/apps/test/core/geometry/subdir.mk
-include src/apps/test/core/filesystem/subdir.mk
-include src/apps/test/core/argument/subdir.mk
-include src/apps/test/subdir.mk
-include external/gidpost/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: OpenSEMBA

# Tool invocations
OpenSEMBA: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++  -o "OpenSEMBA" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(C_DEPS)$(CPP_DEPS) OpenSEMBA
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
