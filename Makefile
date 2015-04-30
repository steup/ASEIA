PROFILING        ?= 0
DEBUG            ?= 0

CWD              := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

INCLUDES         :=
LDPATHS          :=
SYMBOLS          :=
CXXFLAGS         := -std=gnu++11 -Wall -ffunction-sections -fno-threadsafe-statics -fPIC
LDFLAGS          := -O1 -Wl,--gc-sections
LIBS             :=

ifeq (${DEBUG},1)
	CXXFLAGS :=${CXXFLAGS} -O0 -g
else
	CXXFLAGS :=${CXXFLAGS} -Os
endif

ifeq (${PROFILING},1)
	CXXFLAGS :=${CXXFLAGS} -pg
	LDFLAGS  :=${LDFLAGS} -pg
endif

SRC      := ./src
EXAMPLE  := ./example
DOC      := ./doc
INC      := ./include

BIN      := ./bin
BUILD    := ./build
LIB      := ./lib
PKG      := ./pkgconfig
CMAKE    := ./cmake

DIRS     := ${PKG} ${CMAKE} ${LIB} ${BUILD} ${BIN}
CONFIGS  := ${PKG}/aseia.pc ${CMAKE}/aseiaConfig.cmake

LIBNAME  := ASEIA
DYNLIB   := ${LIB}/lib${LIBNAME}.so
STATLIB  := ${LIB}/lib${LIBNAME}.a
TARGETS  := ${DYNLIB} ${STATLIB}

LIBS     += ${LIBNAME}
LDPATHS  += ${LIB}
LDFLAGS  += -Wl,--rpath=$(abspath ${LIB})

EXAMPLES := $(notdir $(basename $(wildcard ${EXAMPLE}/*.cpp)))
OBJECTS  := $(addprefix ${BUILD}/, $(addsuffix .o, $(notdir $(basename $(wildcard ${SRC}/*.cpp)))))
LIBS     := $(addprefix -l, ${LIBS})
LDPATHS  := $(addprefix -L, ${LDPATHS})
INCLUDES := $(addprefix -I, ${INCLUDES} ${INC})
DEPS     := $(wildcard ${BUILD}/*.d)

.PHONY: all examples clean run_% debug_% doc

vpath %.cpp ${SRC} ${EXAMPLE}

all: ${DYNLIB} ${STATLIB} 
	
examples: ${EXAMPLES}

${DIRS}: 
	mkdir -p $@

${DYNLIB}: ${OBJECTS} | ${LIB} ${CONFIGS}
	@echo "Building dynamic library: $@ <- [$^]"
	@g++ --shared -o $@ $^

${STATLIB}: ${OBJECTS} | ${LIB} ${CONFIGS}
	@echo "Building static library: $@ <- [$^]"
	@ar r $@ $^
	@ranlib $@

${PKG}/aseia.pc: | ${PKG}
	@echo "prefix=${CWD}" >> $@
	@echo "exec_prefix=${CWD}" >> $@
	@echo "libdir=${CWD}/lib" >> $@
	@echo "includedir=${CWD}/include" >> $@
	@echo "" >> $@
	@echo "Name: ASEIA" >> $@
	@echo "Description: Abstract Sensor Event Interface Architecture" >> $@
	@echo "Version: 0.0.1" >> $@
	@echo "Libs: -L${libdir} -lASEIA" >> $@
	@echo "CFlags: -I${includedir} -std=c++11" >> $@

${CMAKE}/aseiaConfig.cmake: | ${CMAKE}
	@echo "set(aseia_BASE_DIR ${CWD})" >> $@
	@echo "set(aseia_DEFINITIONS  -std=c++11)" >> $@
	@echo "set(aseia_INCLUDE_DIRS ${CWD}/include)" >> $@
	@echo "set(aseia_LIBRARIES    ${CWD}/lib/libASEIA.so)" >> $@
	@echo "include(FindPackageHandleStandardArgs)" >> $@
	@echo "find_package_handle_standard_args(aseia DEFAULT_MSG aseia_LIBRARIES aseia_INCLUDE_DIRS)" >> $@
	@echo "mark_as_advanced(aseia_INCLUDE_DIR aseia_LIBRARIES)" >> $@

${EXAMPLES}: %: ${BIN}/%
	
$(addprefix ${BIN}/, ${EXAMPLES}): ${BIN}/%: ${BUILD}/%.o | ${DYNLIB} ${BIN}
	@echo "Linking Example $@ <- $<"
	@${CXX} ${LDFLAGS} -o $@ $^ ${LDPATHS} ${LIBS}

${BUILD}/%.o: %.cpp Makefile | ${BUILD}
	@echo "Compiling $@ <- $<"
	@${CXX} -MMD -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

$(addprefix run_,${EXAMPLES}): run_%: %
	@echo "Running $<"
	@./${BIN}/$< ${ARGS}

$(addprefix debug_,${EXAMPLES}): debug_%: %
	@echo "Running $< in debug mode"
	@${DBG} ./${BIN}/$< -ex start ${ARGS}

clean:
	@echo "Clean"
	@rm -rf ${DIRS} ${DOC}/html

doc:
	@echo "Creating Documentation"
	@doxygen ${DOC}/doxyfile

-include ${DEPS}
