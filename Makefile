PROFILING        ?= 0
DEBUG            ?= 1

CWD              := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

INCLUDES         :=
LDPATHS          :=
SYMBOLS          :=
CXXFLAGS         := -std=gnu++11 -Wall
LDFLAGS          := -O1 
LIBS             :=

ifeq (${EMBEDDED},1)
	CXXFLAGS :=${CXXFLAGS} -ffunction-sections -fno-threadsafe-statics
	LDFLAGS  :=${LDFLAGS} -Wl,--gc-sections
else
	CXXFLAGS :=${CXXFLAGS} -fPIC
endif

ifeq (${DEBUG},1)
	CXXFLAGS :=${CXXFLAGS} -O2 -ggdb
else
	CXXFLAGS :=${CXXFLAGS} -Os
endif

ifeq (${PROFILING},1)
	CXXFLAGS :=${CXXFLAGS} -pg
	LDFLAGS  :=${LDFLAGS} -pg
endif

MAKEFILE := $(lastword ${MAKEFILE_LIST})

BASEDIR  := $(dir $(abspath ${MAKEFILE}))

SRC      := src
EXAMPLE  := example
EXPERIMENTS  := experiments
INC      := include
DOC      := doc

HTML     := ${DOC}/html
BIN      := bin
BUILD    := build
BLIB     := ${BUILD}/lib
BPROG    := ${BUILD}/prog
LIB      := lib
CMAKE    := cmake
PKG      := pkgconfig
LOG      := log

DIRS     := ${BIN} ${BLIB} ${BPROG} ${BUILD} ${LIB} ${CMAKE} ${PKG} ${LOG}
GARBAGE  := ${HTML} ${DIRS}

CMAKEFILE:= ${CMAKE}/aseiaConfig.cmake
PKGFILE  := ${PKG}/aseia.pc
CONFIGS  := ${PKG}/aseia.pc ${CMAKE}/aseiaConfig.cmake
LIBNAME  := ASEIA
DYNLIB   := ${LIB}/lib${LIBNAME}.so
STATLIB  := ${LIB}/lib${LIBNAME}.a
TARGETS  := ${DYNLIB} ${STATLIB}

LIBS     += ${LIBNAME} boost_graph boost_program_options
LDPATHS  += ${LIB}
LDFLAGS  += -Wl,--rpath=$(abspath ${LIB})

EXAMPLES := $(notdir $(basename $(wildcard ${EXAMPLE}/*.cpp)))
OBJECTS  := $(addprefix ${BLIB}/, $(addsuffix .o, $(notdir $(basename $(wildcard ${SRC}/*.cpp)))))
LIBS     := $(addprefix -l, ${LIBS})
LDPATHS  := $(addprefix -L, ${LDPATHS})
INCLUDES := $(addprefix -I, ${INCLUDES} ${INC})
DEPS     := $(wildcard ${BUILD}/*.d)

.PHONY: all ${EXAMPLES} examples clean run_examples run_% debug_% doc akgs

all: ${DYNLIB} ${STATLIB} 
	
akgs:
	${MAKE} -C misc all

examples: ${EXAMPLES} | akgs

${EXAMPLES}: %: ${BIN}/%

${DYNLIB}: ${OBJECTS} | ${LIB} ${CONFIGS}
	@echo "Linking dynamic library: $@ <- [$^]"
	@g++ --shared -o $@ $^

${STATLIB}: ${OBJECTS} | ${LIB} ${CONFIGS}
	@echo "Building static library: $@ <- [$^]"
	@ar r $@ $^
	@ranlib $@

${CMAKEFILE}: ${MAKEFILE}  | ${CMAKE}
	@echo 'set(aseia_BASE_DIR ${BASEDIR})' > $@
	@echo 'set(aseia_DEFINITIONS  ${CXXFLAGS})' >> $@
	@echo 'set(aseia_INCLUDE_DIRS $${aseia_BASE_DIR}/${INC})' >> $@
	@echo 'set(aseia_LIBRARIES    $${aseia_BASE_DIR}/${DYNLIB})' >> $@
	@echo 'include(FindPackageHandleStandardArgs)' >> $@
	@echo 'find_package_handle_standard_args(aseia DEFAULT_MSG aseia_LIBRARIES aseia_INCLUDE_DIRS)' >> $@
	@echo 'mark_as_advanced(aseia_INCLUDE_DIR aseia_LIBRARIES)' >> $@

${PKGFILE}: ${MAKEFILE} | ${PKG}
	@echo 'prefix=${BASEDIR}' > $@
	@echo 'exec_prefix=$${prefix}' >> $@
	@echo 'libdir=$${prefix}/${LIB}' >> $@
	@echo 'includedir=$${prefix}/${INC}' >> $@
	@echo "" >> $@
	@echo 'Name: ${LIBNAME}' >> $@
	@echo 'Description: Abstract Sensor Event Interface Architecture'>> $@
	@echo 'Version: 0.0.1' >> $@
	@echo 'Libs: -L$${libdir} -l${LIBNAME} ${LDFLAGS}' >> $@
	@echo 'CFlags: -I$${includedir} ${CXXFLAGS}' >> $@

${DIRS}: %:
	@echo "Creating $@"
	@mkdir -p $@

${BLIB}/%.o: ${SRC}/%.cpp ${MAKEFILE} | ${BLIB}
	@echo "Compiling lib file $@ <- $<"
	@${CXX} -MMD -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BPROG}/%.o: ${EXAMPLE}/%.cpp ${MAKEFILE} | ${BPROG}
	@echo "Compiling prog $@ <- $<"
	@${CXX} -MMD -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BIN}/%: ${BPROG}/%.o ${MAKEFILE} ${DYNLIB} | ${BIN}
	@echo "Linking prog $@ <- $<"
	@${CXX} $< ${LDFLAGS} ${LDPATHS} ${LIBS} -o $@

run_examples: $(addprefix run_,${EXAMPLES})

run_%: % | ${LOG}
	@echo "Running $<"
	@./${BIN}/$< ${ARGS} &> ${LOG}/$<.log

debug_%: ${BIN}/%
	@echo "Running $< in debug mode"
	@${DBG} ./$< -ex start ${ARGS}

clean:
	@echo "Clean"
	@rm -rf ${GARBAGE}
	@${MAKE} -C misc clean

doc:
	@echo "Creating Documentation"
	@doxygen ${DOC}/doxyfile

-include ${DEPS}
