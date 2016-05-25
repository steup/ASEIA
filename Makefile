PROFILING        ?= 0
DEBUG            ?= 1

CWD              := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

INCLUDES         :=
LDPATHS          :=
SYMBOLS          :=
CXXFLAGS         := -std=gnu++11 -Wall
LDFLAGS          := -O1 
LIBS             :=
PACKAGES         := eigen3

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

SRC       := src
EXAMPLE   := example
INC       := include
DOC       := doc
TESTS     := tests
RUN_TESTS := run_tests

HTML     := ${DOC}/html
BIN      := bin
BUILD    := build
BLIB     := ${BUILD}/lib
BPROG    := ${BUILD}/prog
BTEST    := ${BUILD}/test
LIB      := lib
CMAKE    := cmake
PKG      := pkgconfig
LOG      := log

DIRS     := ${BIN} ${BLIB} ${BPROG} ${BTEST} ${BUILD} ${LIB} ${CMAKE} ${PKG} ${LOG}
GARBAGE  := ${HTML} ${DIRS}

CMAKEFILE:= ${CMAKE}/aseiaConfig.cmake
PKGFILE  := ${PKG}/aseia.pc
CONFIGS  := ${PKG}/aseia.pc ${CMAKE}/aseiaConfig.cmake
LIBNAME  := ASEIA
DYNLIB   := ${LIB}/lib${LIBNAME}.so
STATLIB  := ${LIB}/lib${LIBNAME}.a
TARGETS  := ${DYNLIB} ${STATLIB}

LIBS     += ${LIBNAME}
LDPATHS  += ${LIB}
LDFLAGS  += -Wl,--rpath=$(abspath ${LIB})

PKG_INCLUDE := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-I))
PKG_CFLAGS  := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-other))
PKG_LIBS    := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-l))
PKG_LDPATHS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-L))
PKG_LDFLAGS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-other))


EXAMPLES := $(notdir $(basename $(wildcard ${EXAMPLE}/*.cpp)))
OBJECTS  := $(addprefix ${BLIB}/, $(addsuffix .o, $(notdir $(basename $(wildcard ${SRC}/*.cpp)))))
LIBS     := $(addprefix -l, ${LIBS}) ${PKG_LIBS}
LDPATHS  := $(addprefix -L, ${LDPATHS}) ${PKG_LDPATHS}
INCLUDES := $(addprefix -I, ${INCLUDES} ${INC}) ${PKG_INCLUDE}
CXXFLAGS := ${CXXFLAGS} ${PKG_CFLAGS}
LDFLAGS  := ${LDFLAGS} ${PKG_LDFLAGS}
DEPS     := $(wildcard ${BUILD}/*/*.o.d)


.PHONY: all ${EXAMPLES} examples clean run_examples run_% debug_% tests run_tests doc
.PRECIOUS: ${BPROG}/%.o ${BLIB}/%.o

all: ${DYNLIB} ${STATLIB} 
	
include gtest.mk

tests: ${BIN}/${RUN_TESTS}

run_tests: ${BIN}/${RUN_TESTS}
	@./$<

${BTEST}/${RUN_TESTS}.o: ${TESTS}/${RUN_TESTS}.cpp ${MAKEFILE} ${GTEST} | ${BTEST} 
	@echo "Building unit tests $@ <- $<"
	@${CXX} -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} -I${TESTS} ${GTEST_FLAGS} $< -o $@ ${INCLUDES} ${TEST_INCLUDES} ${GTEST_INCLUDES}

${BIN}/${RUN_TESTS}: ${BTEST}/${RUN_TESTS}.o ${MAKEFILE} ${GTEST} | ${BIN} ${DYNLIB}
	@echo "Linking unit tests $@ <- $<"
	@${CXX} ${LDFLAGS} ${GTEST_LDFLAGS} $< -o $@ ${LDPATHS} ${LIBS} ${GTEST_LDPATHS} ${GTEST_LIBS}

examples: ${EXAMPLES}

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
	@echo 'set(aseia_DEFINITIONS  ${CXXFLAGS} ${PKG_CFLAGS})' >> $@
	@echo 'set(aseia_INCLUDE_DIRS $${aseia_BASE_DIR}/${INC} $(subst -I,,${PKG_INCLUDE}))' >> $@
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
	@${CXX} -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BPROG}/%.o: ${EXAMPLE}/%.cpp ${MAKEFILE} | ${BPROG}
	@echo "Compiling prog $@ <- $<"
	@${CXX} -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

${BIN}/%: ${BPROG}/%.o ${MAKEFILE} | ${DYNLIB} ${BIN}
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

doc:
	@echo "Creating Documentation"
	@doxygen ${DOC}/doxyfile

-include ${DEPS}
