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

TEST_CORE_MATH          = no#
TEST_CORE_GEOMETRY      = no#
TEST_CORE_ARGUMENT      = no#
TEST_CORE_PHYSICALMODEL = no#
TEST_CORE_SOURCE        = yes#
TEST_CORE_FILESYSTEM    = no#
TEST_PARSER_GID         = no#
TEST_PARSER_STL         = no#

# =============================================================================
SRC_APP_DIR = $(SRC_DIR)apps/test/

ifeq ($(compiler),$(filter $(compiler),mingw32 mingw64))
	OUT := $(addsuffix .exe,$(OUT))
endif
ifeq ($(FFTW3_SUPPORT),yes)
	DEFINES +=FFTW3_SUPPORT
	LIBS += fftw3
endif
ifeq ($(EIGEN_SUPPORT),yes)
	DEFINES += EIGEN_SUPPORT
endif
# =============================================================================
# --- Core ---
ifeq ($(TEST_CORE_MATH),yes)
	SRC_CORE_MATH_DIRS     := $(shell find $(SRC_DIR)core/math/ -type d)
	SRC_CORE_MATH_TESTS_DIRS := $(SRC_CORE_MATH_DIRS) \
							   $(shell find $(SRC_APP_DIR)core/math/ -type d)
endif
ifeq ($(TEST_CORE_GEOMETRY),yes)
	SRC_CORE_GEOMETRY_DIRS     := $(shell find $(SRC_DIR)core/math/ -type d) \
	 						      $(shell find $(SRC_DIR)core/geometry/ -type d)
	SRC_CORE_GEOMETRY_TESTS_DIRS := $(SRC_CORE_GEOMETRY_DIRS) \
							   $(shell find $(SRC_APP_DIR)core/geometry/ -type d)
endif
ifeq ($(TEST_CORE_ARGUMENT),yes)
	SRC_CORE_ARGUMENT_DIRS := $(shell find $(SRC_DIR)core/argument/ -type d)
	SRC_CORE_ARGUMENT_TESTS_DIRS := $(SRC_CORE_ARGUMENT_DIRS) \
								   $(shell find $(SRC_APP_DIR)core/argument/ -type d)
endif
ifeq ($(TEST_CORE_PHYSICALMODEL),yes)
	SRC_CORE_PHYSICALMODEL_DIRS = $(shell find $(SRC_DIR)core/ -type d)
	SRC_CORE_PHYSICALMODEL_TESTS_DIRS := $(SRC_CORE_PHYSICALMODEL_DIRS) \
								   $(shell find $(SRC_APP_DIR)core/physicalModel/ -type d)
endif
ifeq ($(TEST_CORE_SOURCE),yes)
	SRC_CORE_SOURCE_DIRS = $(shell find $(SRC_DIR)core/math/ -type d) \
						   $(shell find $(SRC_DIR)core/filesystem/ -type d) \
	 					   $(shell find $(SRC_DIR)core/geometry/ -type d) \
	 					   $(shell find $(SRC_DIR)core/physicalModel/ -type d) \
	 					   $(shell find $(SRC_DIR)core/source/ -type d) 
	SRC_CORE_SOURCE_TESTS_DIRS := $(SRC_CORE_SOURCE_DIRS) \
								   $(shell find $(SRC_APP_DIR)core/source/ -type d)
endif
ifeq ($(TEST_CORE_FILESYSTEM),yes)
	SRC_CORE_FILESYSTEM_DIRS = $(shell find $(SRC_DIR)core/filesystem/ -type d)
	SRC_CORE_FILESYSTEM_TESTS_DIRS := $(SRC_CORE_FILESYSTEM_DIRS) \
								   $(shell find $(SRC_APP_DIR)core/filesystem/ -type d)
endif

SRC_CORE_TESTS_DIRS = $(SRC_CORE_MATH_TESTS_DIRS) \
	                  $(SRC_CORE_GEOMETRY_TESTS_DIRS) \
	                  $(SRC_CORE_ARGUMENT_TESTS_DIRS) \
	                  $(SRC_CORE_PHYSICALMODEL_TESTS_DIRS) \
	                  $(SRC_CORE_SOURCE_TESTS_DIRS) \
	                  $(SRC_CORE_FILESYSTEM_TESTS_DIRS)
# --- Parsers ---
ifeq ($(TEST_PARSER_GID),yes)
	SRC_PARSER_GID_DIRS       := $(shell find $(SRC_DIR)core/ -type d) \
								 $(shell find $(SRC_DIR)parser/gid/ -type d)
	SRC_PARSER_GID_TESTS_DIRS = $(SRC_PARSER_GID_DIRS) \
							    $(shell find $(SRC_APP_DIR)parser/gid/ -type d)
endif
ifeq ($(TEST_PARSER_STL),yes)
	SRC_PARSER_STL_DIRS       := $(shell find $(SRC_DIR)core/ -type d) \
								 $(shell find $(SRC_DIR)parser/stl/ -type d)
	SRC_PARSER_STL_TESTS_DIRS = $(SRC_PARSER_STL_DIRS) \
							    $(shell find $(SRC_APP_DIR)parser/stl/ -type d)
endif
SRC_PARSER_TESTS_DIRS = $(SRC_PARSER_GID_TESTS_DIRS) $(SRC_PARSER_STL_TESTS_DIRS)
# ----- Gathers sources ----
SRC_DIRS := $(SRC_APP_DIR) \
			$(SRC_CORE_TESTS_DIRS) \
			$(SRC_PARSER_TESTS_DIRS)

SRCS_CXX := $(shell find $(SRC_DIRS) -maxdepth 1 -type f -name "*.cpp")
OBJS_CXX := $(addprefix $(OBJ_DIR), $(SRCS_CXX:.cpp=.o))
# =============================================================================
LIBS      += gtest
LIBRARIES +=
INCLUDES  += $(SRC_DIR) $(SRC_DIR)core/
# =============================================================================
.PHONY: default print

default: $(OUT)
	@echo "======================================================="
	@echo "           $(OUT) compilation finished"
	@echo "======================================================="

$(OBJ_DIR)%.o: %.cpp
	@dirname $@ | xargs mkdir -p
	@echo "Compiling:" $@
	$(CXX) $(CXXFLAGS) $(addprefix -D, $(DEFINES)) $(addprefix -I,$(INCLUDES)) -c -o $@ $<

$(BIN_DIR)$(OUT): $(OBJS_CXX)
	@mkdir -p $(BIN_DIR)
	@echo "Linking:" $@
	${CXX} $^ \
	-o $@ $(CXXFLAGS) \
	$(addprefix -D, $(DEFINES)) \
	$(addprefix -I, ${INCLUDES}) \
	$(addprefix -L, ${LIBRARIES}) \
	$(addprefix -l, ${LIBS})

$(OUT): $(BIN_DIR)$(OUT)

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
