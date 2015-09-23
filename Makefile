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

APP_VERSION=\"0.9\"
DEFINES +=APP_VERSION=$(APP_VERSION)
# ==================== Intel Compiler =========================================
ifeq ($(compiler),intel) 
	CC = icc
	CXX = icpc
	FC = ifort
	CCFLAGS +=
	CXXFLAGS +=
	FCFLAGS +=
endif # end of If choosing Intel compiler.
#===================== GNU Compiler ===========================================
ifeq ($(compiler),gnu)
	CC = gcc 
	CXX = g++
	FC = gfortran
	CCFLAGS +=
	CXXFLAGS += -std=c++0x
	FCFLAGS +=
endif # endif choosing the GNU compiler.
#===================== MinGW32 Compiler =======================================
ifeq ($(compiler),mingw32)
	CC = i686-w64-mingw32-gcc
	CXX = i686-w64-mingw32-g++
	FC = i686-w64-mingw32-gfortran
	CCFLAGS +=
	CXXFLAGS += -std=c++0x -static
	FCFLAGS +=
endif # endif choosing the MinGW32 compiler.
#===================== MinGW64 Compiler =======================================
ifeq ($(compiler),mingw64)
	CC = x86_64-w64-mingw32-gcc
	CXX = x86_64-w64-mingw32-g++
	FC = x86_64-w64-mingw32-gfortran
	CCFLAGS +=
	CXXFLAGS += -std=c++0x -static
	FCFLAGS +=
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
LIBDIR = ./lib/
SRCDIR = ./src/
SRC_DIR = ./ math/ \
 base/class/  base/error/ base/group/ \
 geometry/ geometry/elements/ geometry/maps/ \
 geometry/coordinates/ geometry/layers/ geometry/pixels/ \
 options/ \
 physicalModel/ \
 sources/ sources/ports/ sources/magnitude/ \
 outRq/ solver/ parser/ exporter/ \
 parser/ parser/gid/ parser/stl/ \
 solver/ \
 semba/
#SRC_SOLVER_DIR = 
 
LIB_DIR = ./ gidpost/

INCLUDES += src/ src/common lib/

# =============================================================================
.NOTPARALLEL:
# -------------------- RULES --------------------------------------------------
default: all
	@echo "======>>>>> Done <<<<<======"

all: semba

semba: check .NOTPARALLEL
	$(MAKE) -f ./src/apps/semba/semba.mk

create_dirs:
	@echo 'Creating directories to store binaries and intermediate objects'
	-mkdir $(BINDIR)
	-mkdir $(OBJDIR)

cudg3d: check 
	$(MAKE) -f ./src/apps/cudg3d/cudg3d.mk order=1

testSemba: check
	$(MAKE) -f ./src/apps/test/test.mk

remote-build:
	@echo "Creating directory: local-$(WORKDIR)"
	@-mkdir local-$(WORKDIR)
	@echo "Copying the following files: ${FILES}"
	@cp -rf ${FILES} ./local-$(WORKDIR)
	@echo "Removing .svn folders"
	@-find ./local-$(WORKDIR)/ -name ".svn" -exec rm -rf {} \; 2> /dev/null
	@echo "Syncing with: $(DESTINATION)"
	@rsync -rh --progress -e ssh local-$(WORKDIR)/ $(DESTINATION) 
	@echo "Building program: $(BUILD_COMMAND)"
	@ssh $(USER)@$(HOST) "cd ~/$(WORKDIR) ; $(BUILD_COMMAND)"
	@echo "Removing temporal folder: local-$(WORKDIR)"
	@rm -rf ./local-$(WORKDIR)

repeat: clean default

clean:
	rm -rf *.err *.o *.mod *.d $(OBJDIR)
	find ./src -name "*.gch" -exec rm {} \;

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
