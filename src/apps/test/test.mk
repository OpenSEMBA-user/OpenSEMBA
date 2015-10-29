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


LIBS += gtest

OUT = test
ifeq ($(compiler),$(filter $(compiler),mingw32 mingw64))
	OUT := $(addsuffix .exe,$(OUT))
endif

# =============================================================================
DIR = ./ $(SRC_CORE_DIR) 

SOURCE_DIR = $(addprefix $(SRCDIR), ${DIR}) $(addprefix $(SRCDIR)/apps/test/, ${DIR}) $(addprefix $(LIBDIR), ${LIB_DIR})

SRCS_CXX := $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name "*.cpp" 2>/dev/null)
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
	
$(OUT): $(OBJS_CXX)
	@mkdir -p $(BINDIR)
	@echo "Linking:" $@
	${CXX} $^ -o $(BINDIR)$(OUT) $(CXXFLAGS) \
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
