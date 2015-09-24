# ==================== Default variables ======================================
BINDIR = bin/
OBJDIR = obj/
LIBDIR = lib/
SRCDIR = src/
# =============================================================================
CXXFLAGS += -fopenmp
# =============================================================================
INCLUDES += test/
LIBRARIES +=
LIBS += gtest

ifeq ($(mode),debug)
	DEFINES +=_DEBUG
endif

DEFINES += 
ifeq ($(compiler),$(filter $(compiler),mingw32 mingw64))
	OUT := $(addsuffix .exe,$(OUT))
endif
# =============================================================================
ifeq ($(mode),debug)
	OUT = test
else 
	OUT = test
endif
# =============================================================================
# -------------------- Paths to directories -----------------------------------
DIR = $(SRC_COMMON_DIR) $(SRC_MESHER_DIR) $(SRC_SOLVER_DIR)

SOURCE_DIR = $(addprefix $(SRCDIR), ${DIR}) $(addprefix $(SRCDIR)/apps/test/, ${DIR}) $(addprefix $(LIBDIR), ${LIB_DIR})

IGNORES := OptionsSolverDGTD.cpp
EXCLUDE := $(shell find $(SOURCE_DIR) -type f \( -name MathMatrix.cpp $(addprefix -o -name , $(IGNORES)) \) 2>/dev/null )

SRCS_CXX := $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name "*.cpp")
SRCS_CXX := $(filter-out $(EXCLUDE), $(SRCS_CXX)) 
OBJS_CXX := $(addprefix $(OBJDIR), $(SRCS_CXX:.cpp=.o))

SRCS_C := $(shell find $(SOURCE_DIR) -maxdepth 1 -type f -name "*.c")
SRCS_C := $(filter-out $(EXCLUDE), $(SRCS_C)) 
OBJS_C := $(addprefix $(OBJDIR), $(SRCS_C:.c=.o))

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

$(OUT): $(OBJS_CXX) $(OBJS_C)
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
