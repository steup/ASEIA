PROFILING        ?= 0
DEBUG            ?= 0
EMBEDDED         ?= 0

INCLUDES :=
LDPATHS  :=
SYMBOLS  :=
CXXFLAGS := -std=gnu++11 -ffunction-sections -fdata-sections -fvisibility-inlines-hidden ${CXXFLAGS}

LDFLAGS  := -Wl,--gc-sections ${LDFLAGS}
LIBS     := boost_system boost_filesystem
PACKAGES := eigen3
LIBNAME  ?= ASEIA
DLIBEXT  ?= so
SLIBEXT  ?= a

ifeq (${EMBEDDED},1)
	CXXFLAGS := -Os -fno-threadsafe-statics ${CXXFLAGS}
else
	CXXFLAGS := -fPIC ${CXXFLAGS}
endif

ifeq (${LTO},1)
	CXXFLAGS := -flto=4 -fuse-linker-plugin ${CXXFLAGS}
	LDFLAGS  := -flto=4 -fuse-linker-plugin ${LDFLAGS}
	AR       := gcc-ar
	RANLIB   := gcc-ranlib
else
	RANLIB   := ranlib
	AR       := ar
endif

ifeq (${DEBUG},1)
	CXXFLAGS :=-O2 -ggdb ${CXXFLAGS}
	LDFLAGS  :=-ggdb ${LDFLAGS}
else
	CXXFLAGS := -Ofast ${CXXFLAGS}
	LDFLAGS  := -Ofast ${LDFLAGS}
endif

ifeq (${PROFILING},1)
	CXXFLAGS :=${CXXFLAGS} -pg
	LDFLAGS  :=${LDFLAGS} -pg
endif

BASEDIR   := $(abspath $(dir $(lastword ${MAKEFILE_LIST}))/..)

SRC       := src
EXAMPLES  := examples
INC       := include
DOC       := doc
TESTS     := tests
RUN_TESTS := run_tests

HTML      := ${DOC}/html
BIN       := bin
BUILD     := build
BLIB      := ${BUILD}/lib
BEX       := ${BUILD}/examples
BTEST     := ${BUILD}/tests
LIB       := lib
CMAKE     := cmake
PKG       := pkgconfig
LOG       := log

DIRS      := ${BIN} ${BLIB} ${BEX} ${BTEST} ${BUILD} ${LIB} ${CMAKE} ${PKG} ${LOG}
GARBAGE   := $(wildcard ${DOC}/*.dot) $(wildcard ${DOC}/*.svg) ${HTML} ${DIRS}

CMAKEFILE := ${CMAKE}/aseiaConfig.cmake
PKGFILE   := ${PKG}/aseia.pc
CONFIGS   := ${PKGFILE} ${CMAKEFILE}
DYNLIB    := ${LIB}/lib${LIBNAME}.${DLIBEXT}
STATLIB   := ${LIB}/lib${LIBNAME}.${SLIBEXT}
TARGETS   := ${DYNLIB} ${STATLIB}

vpath %.mk ${BASEDIR}/make

.PHONY: all ${EXAMPLES} examples clean run_examples run_% debug_% tests run_tests doc dot
#.PRECIOUS: ${BEX}/%.o ${BLIB}/%.o

all: ${DYNLIB} ${STATLIB}

include ${BASEDIR}/make/smhasher.mk

LIBS     += ${LIBNAME} ${SMHASHER_LIBS}
LDPATHS  += ${LIB}
LDFLAGS  += -Wl,--as-needed -Wl,--rpath=$(abspath ${LIB})

PKG_INCLUDE := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-I))
PKG_CFLAGS  := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-other))
PKG_LIBS    := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-l))
PKG_LDPATHS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-L))
PKG_LDFLAGS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-other))


SYMBOLS  := $(addprefix -D, ${SYMBOLS})
EXEXES   := $(notdir $(basename $(wildcard ${EXAMPLES}/*.cpp)))
TESTOBJS := $(addprefix ${BTEST}/, $(addsuffix .o, $(notdir $(basename $(wildcard ${TESTS}/*.cpp)))))
OBJECTS  := $(addprefix ${BLIB}/, $(addsuffix .o, $(notdir $(basename $(wildcard ${SRC}/*.cpp)))))
LIBS     := $(addprefix -l, ${LIBS}) ${PKG_LIBS}
LDPATHS  := $(addprefix -L, ${LDPATHS}) ${PKG_LDPATHS}
INCLUDES := $(addprefix -I, ${INCLUDES} ${INC} ${SMHASHER_INCLUDES}) ${PKG_INCLUDE}
CXXFLAGS := ${CXXFLAGS} ${PKG_CFLAGS}
LDFLAGS  := ${LDFLAGS} ${PKG_LDFLAGS}
DEPS     := $(wildcard ${BUILD}/*/*.o.d)
GRAPHS   := $(foreach graph, $(basename $(wildcard ${DOC}/*.dot)), ${graph}.svg)

OBJECTS  := ${OBJECTS} ${SMHASHER_OBJECTS}

include ${BASEDIR}/make/gtest.mk

tests: ${BIN}/${RUN_TESTS}

run_tests: ${BIN}/${RUN_TESTS}
	@./$<

${TESTOBJS}: ${BTEST}/%.o: ${TESTS}/%.cpp ${MAKEFILE_LIST} ${GTEST} | ${BTEST}
	@echo "Building unit tests $@ <- $<"
	@${CXX} -MP -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} -I${TESTS} ${GTEST_FLAGS} $< -o $@ ${INCLUDES} ${TEST_INCLUDES} ${GTEST_INCLUDES}

${BIN}/${RUN_TESTS}: ${TESTOBJS} ${MAKEFILE_LIST} ${GTEST} | ${BIN} ${DYNLIB}
	@echo "Linking unit tests $@ <- [${TESTOBJS}]"
	@${CXX} ${LDFLAGS} ${GTEST_LDFLAGS} ${TESTOBJS} -o $@ ${LDPATHS} ${LIBS} ${GTEST_LDPATHS} ${GTEST_LIBS}

examples: ${EXEXES}

${EXEXES}: %: ${BIN}/%

${DYNLIB}: ${OBJECTS} | ${LIB} ${CONFIGS}
	@echo "Linking dynamic library: $@ <- [$^]"
	@g++ --shared -o $@ $^

${STATLIB}: ${OBJECTS} | ${LIB} ${CONFIGS}
	@echo "Building static library: $@ <- [$^]"
	@${AR} ${ARFLAGS} $@ $^
	@${RANLIB} $@

${CMAKEFILE}: ${MAKEFILE_LIST}  | ${CMAKE}
	@echo 'set(aseia_BASE_DIR ${BASEDIR})' > $@
	@echo 'set(aseia_DEFINITIONS  ${CXXFLAGS} ${PKG_CFLAGS})' >> $@
	@echo 'set(aseia_INCLUDE_DIRS $${aseia_BASE_DIR}/${INC} $(subst -I,,${PKG_INCLUDE}))' >> $@
	@echo 'set(aseia_LIBRARIES    $${aseia_BASE_DIR}/${DYNLIB})' >> $@
	@echo 'include(FindPackageHandleStandardArgs)' >> $@
	@echo 'find_package_handle_standard_args(aseia DEFAULT_MSG aseia_LIBRARIES aseia_INCLUDE_DIRS)' >> $@
	@echo 'mark_as_advanced(aseia_INCLUDE_DIR aseia_LIBRARIES)' >> $@

${PKGFILE}: ${MAKEFILE_LIST} | ${PKG}
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

${BLIB}/%.o: ${SRC}/%.cpp ${MAKEFILE_LIST} | ${BLIB}
	@echo "Compiling lib file $@ <- $<"
	@${CXX} -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} ${SYMBOLS} $< -o $@ ${INCLUDES}

${BEX}/%.o: ${EXAMPLES}/%.cpp ${MAKEFILE_LIST} | ${BEX}
	@echo "Compiling example $@ <- $<"
	@${CXX} -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} ${SYMBOLS} $< -o $@ ${INCLUDES}

${BIN}/%: ${BEX}/%.o | ${DYNLIB} ${BIN}
	@echo "Linking exampple $@ <- $<"
	@${CXX} $< ${LDFLAGS} ${LDPATHS} ${LIBS} -o $@

run_examples: ${EXEXES}
	@echo "Executing examples and writing output to ${LOG}"
	@for exe in ${EXEXES} ; do ./${BIN}/$${exe} > log/$${exe}.log ; done

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

dot: ${GRAPHS}

${GRAPHS}: ${DOC}/%.svg: ${DOC}/%.dot
	@dot -Tsvg $< -o $@

-include ${DEPS}
