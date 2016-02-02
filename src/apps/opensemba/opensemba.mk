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
# =============================================================================
OUT = libopensemba

# =============================================================================
LIBS = gidpost
INCLUDES += src/ src/core/ external/
# =============================================================================
# -------------------- Paths to directories -----------------------------------
SRC_CORE_DIRS     := $(shell find $(SRC_DIR)core/ -type d)
SRC_EXPORTER_DIRS := $(shell find $(SRC_DIR)exporter/ -type d)
SRC_MESHER_DIRS   := $(shell find $(SRC_DIR)mesher/ -type d)
SRC_PARSER_DIRS   := $(shell find $(SRC_DIR)parser/ -type d)

SRC_DIRS = $(SRC_CORE_DIRS) \
           $(SRC_EXPORTER_DIRS) \
           $(SRC_MESHER_DIRS) \
           $(SRC_PARSER_DIRS)

SRCS_CXX := $(shell find $(SRC_DIRS) -maxdepth 1 -type f -name "*.cpp")
OBJS_CXX := $(addprefix $(OBJDIR), $(SRCS_CXX:.cpp=.o))

.PHONY: default clean clobber print
.NOTPARALLEL:

default: .NOTPARALLEL print $(OUT) 
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
