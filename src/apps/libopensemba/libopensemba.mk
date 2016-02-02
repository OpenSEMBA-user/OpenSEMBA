# OpenSEMBA
# Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
#                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
#                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
#                    Daniel Mateos Romero            (damarro@semba.guru)
#
# This file is part of OpenSEMBA.
#
# OpenSEMBA is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

# ==================== Default variables ======================================
CXXFLAGS += -fopenmp
# =============================================================================
  
LIB_DIR = $(BINDIR)gidpost/lib/ $(BINDIR)gidpost/include/ 

INCLUDES += src/ src/core/ external/

ifeq ($(mode),debug)
	DEFINES +=_DEBUG
endif

DEFINES += 
ifeq ($(compiler),$(filter $(compiler),mingw32 mingw64))
	OUT := $(addsuffix .exe,$(OUT))
endif
# =============================================================================
OUT = libopensemba
# =============================================================================
# -------------------- Paths to directories -----------------------------------
DIR = $(SRC_DIR) 
SRC_CORE_GEOMETRY_DIR = core/geometry/ \
 core/geometry/coordinate/ core/geometry/element/ core/geometry/graph/ \
 core/geometry/layer/ core/geometry/mesh/
SRC_CORE_MATH_DIR = core/math/ core/math/axis/ core/math/function/ \
 core/math/matrix/ core/math/simplex/ core/math/util/ core/math/vector/
SRC_CORE_SOURCE_DIR = core/source/ core/source/magnitude/ core/source/port/ \
 core/filesystem/ core/geometry/ core/group/ core/outputRequests/ \
 core/parser/ core/physicalModel/ core/solver/
SRC_PARSER_DIR = parser/gid/ parser/stl/ 
SRC_EXPORTER_DIR = exporter/gid/ exporter/vtk/ 


SOURCE_DIR = $(addprefix $(SRCDIR), ${DIR}) $(addprefix $(LIBDIR), ${LIB_DIR})

IGNORES := 
EXCLUDE := $(shell find $(SOURCE_DIR) -type f \( -name MathMatrix.cpp $(addprefix -o -name , $(IGNORES)) \) 2>/dev/null )

SRCS_CXX := $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name "*.cpp")
SRCS_CXX := $(filter-out $(EXCLUDE), $(SRCS_CXX)) 
OBJS_CXX := $(addprefix $(OBJDIR), $(SRCS_CXX:.cpp=.o))


.PHONY: default clean clobber print

default: print $(OUT)
	@echo "======================================================="
	@echo "           $(OUT) compilation finished             "
	@echo "======================================================="
		
clean:
	rm -rf *.err *.o *.d $(OBJDIR)

clobber: clean
	rm -rf $(BINDIR)

$(OBJDIR)%.o: %.cpp
	@dirname $@ | xargs mkdir -p
	@echo "Compiling:" $@
	$(CXX) $(CXXFLAGS) $(addprefix -D, $(DEFINES)) $(addprefix -I,$(INCLUDES)) -c -o $@ $<
	
$(OBJDIR)%.o: %.c
	@dirname $@ | xargs mkdir -p
	@echo "Compiling:" $@
	$(CC) $(CCFLAGS) $(addprefix -D, $(DEFINES)) $(addprefix -I,$(INCLUDES)) -c -o $@ $<

# TODO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 
$(OUT): $(OBJS_CXX) $(OBJS_C)
	@mkdir -p $(BINDIR) 
	@echo "Linking:" $@
	${CXX} $^ -o $(BINDIR)$(OUT) $(CXXFLAGS) \
	 $(addprefix -D, $(DEFINES)) \
	 $(addprefix -I, ${INCLUDES}) \
	 $(addprefix -L, ${LIBRARIES}) $(addprefix -l, ${LIBS})
# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	 
	 
print:
	@echo "======================================================="
	@echo "         ----- Compiling $(OUT) ------                 "
	@echo "Target:           " $(target)
	@echo "Compiler:         " $(compiler)
	@echo "C++ Compiler:     " `which $(CXX)`
	@echo "C++ Flags:        " $(CXXFLAGS)
	@echo "Defines:          " $(DEFINES)
	@echo "======================================================="

# ------------------------------- END ----------------------------------------
