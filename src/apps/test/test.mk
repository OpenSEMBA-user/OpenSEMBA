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

OUT = test


# =============================================================================
SRC_APP_DIR = $(SRC_DIR)apps/test/

ifeq ($(compiler),$(filter $(compiler),mingw32 mingw64))
	OUT := $(addsuffix .exe,$(OUT))
endif

# =============================================================================
SRC_CORE_MATH_TESTS_DIRS = $(shell find $(SRC_APP_DIR)/core/math/ -type d)

SRC_DIRS := $(SRC_APP_DIR) $(SRC_CORE_MATH_TESTS_DIRS)

SRCS_CXX := $(shell find $(SRC_DIRS) -maxdepth 1 -type f -name "*.cpp")
OBJS_CXX := $(addprefix $(OBJ_DIR), $(SRCS_CXX:.cpp=.o))
# =============================================================================
LIBS = opensemba
INCLUDES += $(LIB_DIR)opensemba/include/ $(LIB_DIR)opensemba/include/core/
# =============================================================================
.PHONY: default clean clobber print

default: print $(OUT)
	@echo "======================================================="
	@echo "           $(OUT) compilation finished             "
	@echo "======================================================="
		
clean:
	rm -rf $(OBJ_DIR)

clobber: clean
	rm -rf $(BIN_DIR)

$(OBJ_DIR)%.o: %.cpp
	@dirname $@ | xargs mkdir -p
	@echo "Compiling:" $@
	$(CXX) $(CXXFLAGS) $(addprefix -D, $(DEFINES)) $(addprefix -I,$(INCLUDES)) -c -o $@ $<
	
$(OUT): $(OBJS_CXX)
	@mkdir -p $(BIN_DIR)
	@echo "Linking:" $@
	${CXX} $^ -o $(BIN_DIR)$(OUT) $(CXXFLAGS) \
	 $(addprefix -D, $(DEFINES)) \
	 $(addprefix -I, ${INCLUDES}) \
	 $(addprefix -L, ${LIBRARIES}) $(addprefix -l, ${LIBS})
	 
print:
	@echo "======================================================="
	@echo "         ----- Compiling $(OUT) ------        "
	@echo "Target:           " $(target)
	@echo "Compiler:         " $(compiler)
	@echo "C++ Compiler:     " `which $(CXX)`
	@echo "C++ Flags:        " $(CXXFLAGS)
	@echo "Defines:          " $(DEFINES)
	@echo "======================================================="

# ------------------------------- END ----------------------------------------
