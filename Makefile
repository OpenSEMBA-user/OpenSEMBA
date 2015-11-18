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
#      compiler = {intel, gnu}
# ==================== Default values =========================================
target = release
compiler = gnu

USER=`whoami`
BUILD_COMMAND=make clobber ; make -j `nproc` -s target=$(target) 
FILES=./src ./Makefile ./*.mk

APP_VERSION=\"0.10\"
DEFINES += APP_VERSION=$(APP_VERSION)
# ==================== Debugging options ======================================
ifeq ($(target),debug)
	DEFINES += HDF5 
endif
# ==================== Intel Compiler =========================================
ifeq ($(compiler),intel) 
	CC = icc
	CXX = icpc
	CCFLAGS +=
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
	CXXFLAGS +=-O0 -g3 -Wall -Wno-write-strings # -Wconversion #-fprofile-arcs -ftest-coverage
endif
ifeq ($(target),release)
   	CXXFLAGS +=-O2 
endif
# =============================================================================
# -------------------- Paths to directories -----------------------------------
BINDIR = ./bin/
OBJDIR = ./obj/
LIBDIR = ./external/
SRCDIR = ./src/
SRC_GEOMETRY_DIR = core/math/ \
 core/base/class/  core/base/error/ core/base/group/ \
 core/geometry/ core/geometry/elements/ core/geometry/graphs/ \
 core/geometry/coordinates/ core/geometry/layers/ core/geometry/pixels/ 
SRC_CORE_DIR = core/ $(SRC_GEOMETRY_DIR) \
 core/options/ \
 core/physicalModel/ \
 core/sources/ core/sources/ports/ core/sources/magnitude/ \
 core/outRq/ core/solver/ core/parser/ core/exporter/ 
SRC_PARSER_DIR = parser/gid/ parser/stl/ 
SRC_EXPORTER_DIR = exporter/gid/ exporter/vtk/ 
SRC_SOLVER_DIR = solver/dgtd/
  
LIB_DIR = $(BINDIR)gidpost/lib/ $(BINDIR)gidpost/include/ 

INCLUDES += src/ src/core/ external/

# =============================================================================
.NOTPARALLEL:
# -------------------- RULES --------------------------------------------------
default: all
	@echo "======>>>>> Done <<<<<======"

all: semba

create_dirs:
	@echo 'Creating directories to store binaries and intermediate objects'
	-mkdir -p $(BINDIR) $(LIB_DIR)
	-mkdir -p $(OBJDIR)

cudg3d: gidpost check
	$(MAKE) -f ./src/apps/cudg3d/cudg3d.mk order=1
	
libopensemba: check gidpost 
	$(MAKE) -f ./src/apps/libopensemba/libopensemba.mk

testSemba: check
	$(MAKE) -f ./src/apps/test/test.mk

gidpost: create_dirs check
	$(MAKE) -C ./external/gidpost/ -f gidpost.mk
ifeq ($(target),debug)
	cp ./external/gidpost/debug/gidpost.so $(BINDIR)/gidpost/lib/libgidpost.so
	cp ./external/gidpost/debug/libgidpost.a $(BINDIR)/gidpost/lib/libgidpost.a
endif
ifeq ($(target),release)
	cp ./external/gidpost/release/gidpost.so $(BINDIR)/gidpost/lib/libgidpost.so
	cp ./external/gidpost/release/libgidpost.a $(BINDIR)/gidpost/lib/libgidpost.a
endif
	cp ./external/gidpost/gidpost.h $(BINDIR)/gidpost/include/gidpost.h
 

repeat: clean default

clean:
	rm -rf *.err *.o *.mod *.d $(OBJDIR)
	find ./src -name "*.gch" -exec rm {} \;
	$(MAKE) -C ./external/gidpost/ -f gidpost.mk clean

clobber: clean
	rm -rf $(BINDIR) 

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
