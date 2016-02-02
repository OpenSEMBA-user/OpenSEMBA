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

# -- USAGE --------------------------------------------------------------------
# make target     = {debug, release}
#      compiler = {intel, gnu, ...}
# ==================== Default values =========================================
target   = release
compiler = gnu
APP_VERSION=\"0.11\"

DEFINES += APP_VERSION=$(APP_VERSION)
# ==================== Intel Compiler =========================================
ifeq ($(compiler),intel) 
	CC       = icc
	CXX      = icpc
	CCFLAGS  +=
	CXXFLAGS +=
endif # end of If choosing Intel compiler.
#===================== GNU Compiler ===========================================
ifeq ($(compiler),gnu)
	CC = gcc 
	CXX = g++
	CCFLAGS +=
	CXXFLAGS += -std=c++0x -static -pthread #-fopenmp
endif # endif choosing the GNU compiler.
#===================== MinGW32 Compiler =======================================
ifeq ($(compiler),mingw32)
	CC = i686-w64-mingw32-gcc
	CXX = i686-w64-mingw32-g++
	CCFLAGS +=
	CXXFLAGS += -std=c++0x -static
endif # endif choosing the MinGW32 compiler.
#===================== MinGW64 Compiler =======================================
ifeq ($(compiler),mingw64)
	CC = x86_64-w64-mingw32-gcc
	CXX = x86_64-w64-mingw32-g++
	CCFLAGS +=
	CXXFLAGS += -std=c++0x -static
endif # endif choosing the MinGW64 compiler.
# ================= Optimization target =======================================
ifeq ($(target),debug)
	CXXFLAGS +=-O0 -g3 -Wall -Wno-write-strings 
	# Other options: -Wconversion -fprofile-arcs -ftest-coverage
endif
ifeq ($(target),release)
   	CXXFLAGS +=-O2 
endif
# =============================================================================
# -------------------- Paths to directories -----------------------------------
BUILD_DIR = ./build/
OBJ_DIR = ./obj/
SRC_DIR = ./src/
EXTERNAL_DIR = ./external/

BIN_DIR = $(BUILD_DIR)bin/
LIB_DIR = $(BUILD_DIR)lib/

# =============================================================================
.NOTPARALLEL:
# -------------------- RULES --------------------------------------------------
default: all
	@echo "======>>>>> Done <<<<<======"

all: check libopensemba cudg3d test 

create_dirs:
	@echo 'Creating directories to store binaries and intermediate objects'
	-mkdir -p $(OBJ_DIR)

cudg3d: gidpost check
	$(MAKE) -f ./src/apps/cudg3d/cudg3d.mk order=1
	
libopensemba: check gidpost
	-mkdir -p $(LIB_DIR)/opensemba/lib/ $(LIB_DIR)/opensemba/include/  
	$(MAKE) -f ./src/apps/libopensemba/libopensemba.mk

testSemba: check libopensemba
	-mkdir -p  $(BIN_DIR)/test/  
	$(MAKE) -f ./src/apps/test/test.mk

gidpost: create_dirs check
	$(MAKE) -C $(EXTERNAL_DIR)/gidpost/ -f gidpost.mk
	-mkdir -p $(LIB_DIR)/gidpost/lib/ $(LIB_DIR)/gidpost/include/
	-mkdir -p $(OBJ_DIR)$(EXTERNAL_DIR)/gidpost/
	mv $(EXTERNAL_DIR)/gidpost/$(target)/gidpost.so $(LIB_DIR)/gidpost/lib/libgidpost.so
	mv $(EXTERNAL_DIR)/gidpost/$(target)/libgidpost.a $(LIB_DIR)/gidpost/lib/libgidpost.a
	cp ./external/gidpost/gidpost.h $(LIB_DIR)/gidpost/include/gidpost.h
	mv $(EXTERNAL_DIR)/gidpost/$(target)/*.o $(OBJ_DIR)$(EXTERNAL_DIR)/gidpost/
	rm -r $(EXTERNAL_DIR)/gidpost/$(target)

repeat: clean default

clean:
	rm -rf *.err *.o *.mod *.d $(OBJDIR)
	find ./src -name "*.gch" -exec rm {} \;
	$(MAKE) -C ./external/gidpost/ -f gidpost.mk clean

clobber: clean
	rm -rf $(BUILD_DIR) 

check:
ifneq ($(target),release) 
ifneq ($(target),debug) 
	@echo "Invalid build target."  
	@echo "Please use 'make target=release' or 'make target=debug'"  
	@exit 1
endif 
endif 
ifneq ($(compiler),intel) 
ifneq ($(compiler),gnu) 
ifneq ($(compiler),mingw32) 
ifneq ($(compiler),mingw64) 
	@echo "Invalid build compiler"  
	@echo "Please use 'make compiler= intel|gnu|mingw32|mingw64'" 
	@exit 2
endif 
endif 
endif 
endif 

# Exports current variables when other makefiles are called.
export
