# Modified from the provided by GiD.
SOURCE_DIR := $(SRCDIR)/gidpost
INC_FLAGS := -I${SOURCE_DIR} 

ifeq (${mode},debug)
CFLAGS   := -g2 -Wall ${CFLAGS}
CXXFLAGS := -g2 -Wall ${CXXFLAGS}
LDFLAGS  := -g2
LIB_NAME_SUFFIX := d.a
else
CFLAGS     := -O2 -W -funroll-loops ${CFLAGS}
CXXFLAGS   := -O2 -W -funroll-loops ${CXXFLAGS}
LDFLAGS    := -O2
LIB_NAME_SUFFIX := .a
endif

LIB_NAME := $(OBJDIR)/libgidpost${LIB_NAME_SUFFIX}

LST_INCLUDES = gidpost.h gidpostInt.h zlib.h zlibint1.h zlibint2.h

vpath %cpp ${SOURCE_DIR}
vpath %c ${SOURCE_DIR}
vpath %o ${OUTPUT_DIR}

CPP_SRCS = gidpost.cpp gidpostInt.cpp
C_SRCS   = zlibint1.c zlibint2.c

OBJS = ${CPP_SRCS:%.cpp=${OBJDIR}/%.o} ${C_SRCS:%.c=${OBJDIR}/%.o}

lib: check $(LIB_NAME)
	@echo "======================================================="
	@echo "GIDPOST has compiled succesfully. :-)"
	@echo "======================================================="

$(LIB_NAME): $(OBJS)
	@echo "Archiving:" $<
	@ar cr $(LIB_NAME) $(OBJS)

clean:
	rm $(OBJS)

clobber:
	rm $(LIB_NAME) $(OBJS)

depend: 
	@echo "Compiling:" $<
	@${CC} -MM -Df2cFortran ${CPP_SRCS} ${C_SRCS}

.SUFFIXES : .o .cpp .c .f

${OBJDIR}/%.o: %.c
	@echo "Compiling:" $<
	@${CC} -c $(CFLAGS) -Df2cFortran $(INC_FLAGS) -o $@ $< 2> $@.err

${OBJDIR}/%.o: %.cpp
	@echo "Compiling:" $<
	@${CXX} -c $(CXXFLAGS) $(INC_FLAGS) -o $@ $< 2> $@.err

check:
	@echo "======================================================="
	@echo "--- Compiling gidpost --- "
	@echo "-- With the following options: "
	@echo "Mode:             " $(mode)
	@echo "Compiler:         " $(compiler)
	@echo "MPI:              " $(mpi)
	@echo "-- Using the following paths tools: "
	@echo "C Compiler:       " `which $(CC)`
	@echo "C Flags:          " $(CFLAGS)
	@echo "C++ Compiler:     " `which $(CXX)`
	@echo "C++ Flags:        " $(CXXFLAGS)
	@echo "Defines:          " $(DEFINES)
	@echo "======================================================="

gidpost.o: gidpost.cpp gidpostInt.h zlib.h gidpost.h
gidpostInt.o: gidpostInt.cpp gidpostInt.h zlib.h
zlibint1.o: zlibint1.c zlib.h zlibint1.h
zlibint2.o: zlibint2.c zlibint1.h zlib.h zlibint2.h
