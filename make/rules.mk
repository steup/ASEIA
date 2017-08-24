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

ifeq (${EMBEDDED},1)
	CXXFLAGS := -Os -fno-threadsafe-statics ${CXXFLAGS}
else
	CXXFLAGS := -fPIC ${CXXFLAGS}
endif

ifeq (${PROFILING},1)
	CXXFLAGS :=${CXXFLAGS} -pg
	LDFLAGS  :=${LDFLAGS} -pg
endif

BASEDIR   := $(abspath $(dir $(lastword ${MAKEFILE_LIST}))/..)

SRC       := src
INC       := include
DOC       := doc
TESTS     := tests
RUN_TESTS := run_tests

HTML      := ${DOC}/html
BIN       := bin
BUILD     := build
BLIB      := ${BUILD}/lib
BTEST     := ${BUILD}/tests
LIB       := lib
CMAKE     := cmake
PKG       := pkgconfig
LOG       := log

DIRS      := ${BIN} ${BLIB} ${BTEST} ${BUILD} ${LIB} ${CMAKE} ${PKG} ${LOG}
GARBAGE   := $(wildcard ${DOC}/*.dot) $(wildcard ${DOC}/*.svg) ${HTML} ${DIRS}

CMAKEFILE := ${CMAKE}/aseiaConfig.cmake
PKGFILE   := ${PKG}/aseia.pc
BASE_CMAKEFILE := ${CMAKE}/aseiaBaseConfig.cmake
BASE_PKGFILE   := ${PKG}/aseia_base.pc
IO_CMAKEFILE := ${CMAKE}/aseiaIOConfig.cmake
IO_PKGFILE   := ${PKG}/aseia_io.pc
CONFIGS   := ${PKGFILE} ${CMAKEFILE}
BASE_CONFIGS   := ${BASE_PKGFILE} ${BASE_CMAKEFILE}
IO_CONFIGS   := ${IO_PKGFILE} ${IO_CMAKEFILE}
BASE_LIBNAME := ${LIBNAME}_base
IO_LIBNAME := ${LIBNAME}_io
DYNLIB    := ${LIB}/lib${LIBNAME}.${DLIBEXT}
STATLIB   := ${LIB}/lib${LIBNAME}.${SLIBEXT}
DYNBASELIB    := ${LIB}/lib${BASE_LIBNAME}.${DLIBEXT}
STATBASELIB   := ${LIB}/lib${BASE_LIBNAME}.${SLIBEXT}
DYNIOLIB    := ${LIB}/lib${IO_LIBNAME}.${DLIBEXT}
STATIOLIB   := ${LIB}/lib${IO_LIBNAME}.${SLIBEXT}
TARGETS   := ${DYNLIB} ${STATLIB} ${STATBASELIB}

vpath %.mk ${BASEDIR}/make

.PHONY: all ${EXAMPLES} clean run_% debug_% tests run_tests doc dot

all: ${DYNLIB} ${STATLIB} ${DYNBASELIB} ${DYNIOLIB}

include ${BASEDIR}/make/smhasher.mk

LIBS     += ${SMHASHER_LIBS}
LDPATHS  += ${LIB}
LDFLAGS  += -Wl,--as-needed -Wl,--rpath=$(abspath ${LIB})

PKG_INCLUDE := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-I))
PKG_CFLAGS  := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-other))
PKG_LIBS    := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-l))
PKG_LDPATHS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-L))
PKG_LDFLAGS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-other))

BASE_OBJECTS := AttributeType EventID EventType FormatID Prime
IO_OBJECTS   := IO IDIO
OBJECTS      := $(wildcard ${SRC}/*.cpp)

SYMBOLS  := $(addprefix -D, ${SYMBOLS})
TESTOBJS := $(addprefix ${BTEST}/, $(addsuffix .o, $(notdir $(basename $(wildcard ${TESTS}/*.cpp)))))
BASE_OBJECTS  := $(addprefix ${BLIB}/, $(addsuffix .o, ${BASE_OBJECTS}))
IO_OBJECTS  := $(addprefix ${BLIB}/, $(addsuffix .o, ${IO_OBJECTS}))
OBJECTS  := $(addprefix ${BLIB}/, $(addsuffix .o, $(notdir $(basename ${OBJECTS}))))
OBJECTS  := $(filter-out ${BASE_OBJECTS} ${IO_OBJECTS}, ${OBJECTS})
LIBS     := $(addprefix -l, ${LIBS}) ${PKG_LIBS}
LDPATHS  := $(addprefix -L, ${LDPATHS}) ${PKG_LDPATHS}
INCLUDES := $(addprefix -I, ${INCLUDES} ${INC} ${SMHASHER_INCLUDES}) ${PKG_INCLUDE}
CXXFLAGS := ${CXXFLAGS} ${PKG_CFLAGS}
LDFLAGS  := ${LDFLAGS} ${PKG_LDFLAGS}
DEPS     := $(wildcard ${BUILD}/*/*.o.d)
GRAPHS   := $(foreach graph, $(basename $(wildcard ${DOC}/*.dot)), ${graph}.svg)

BASE_OBJECTS  := ${BASE_OBJECTS} ${SMHASHER_OBJECTS}

include ${BASEDIR}/make/gtest.mk

tests: ${BIN}/${RUN_TESTS}

run_tests: ${BIN}/${RUN_TESTS}
	@./$<

${TESTOBJS}: ${BTEST}/%.o: ${TESTS}/%.cpp ${MAKEFILE_LIST} ${GTEST} | ${BTEST}
	@echo "Building unit tests $@ <- $<"
	@${CXX} -MP -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} -I${TESTS} ${GTEST_FLAGS} $< -o $@ ${INCLUDES} ${TEST_INCLUDES} ${GTEST_INCLUDES}

${BIN}/${RUN_TESTS}: ${TESTOBJS} ${MAKEFILE_LIST} ${GTEST} | ${BIN} ${DYNLIB}
	@echo "Linking unit tests $@ <- [${TESTOBJS}]"
	@${CXX} ${LDFLAGS} ${GTEST_LDFLAGS} ${TESTOBJS} -o $@ ${LDPATHS} ${LIBS} -L ${LIB} -l${LIBNAME} -l${IO_LIBNAME} -l${BASE_LIBNAME} ${GTEST_LDPATHS} ${GTEST_LIBS}

${DYNLIB}: ${OBJECTS} | ${DYNBASELIB} ${DYNIOLIB} ${CONFIGS}
	@echo "Linking dynamic library: $@ <- [$^]"
	@g++ --shared -o $@ $^ -L${LIB} -l${BASE_LIBNAME} -l${IO_LIBNAME}

${STATLIB}: ${OBJECTS} | ${LIB} ${CONFIGS}
	@echo "Building static library: $@ <- [$^]"
	@${AR} ${ARFLAGS} $@ $^
	@${RANLIB} $@

${DYNBASELIB}: ${BASE_OBJECTS} | ${LIB} ${BASE_CONFIGS}
	@echo "Linking basic dynamic library: $@ <- [$^]"
	@g++ --shared -o $@ $^

${DYNIOLIB}: ${IO_OBJECTS} | ${DYNBASELIB} ${IO_CONFIGS}
	@echo "Linking IO dynamic library: $@ <- [$^]"
	@g++ --shared -o $@ $^ -L${LIB} -l${BASE_LIBNAME}

${CMAKEFILE}: ${MAKEFILE_LIST}  | ${CMAKE}
	@echo 'set(aseia_BASE_DIR ${BASEDIR})' > $@
	@echo 'set(aseia_DEFINITIONS  ${CXXFLAGS} ${PKG_CFLAGS})' >> $@
	@echo 'set(aseia_INCLUDE_DIRS $${aseia_BASE_DIR}/${INC} $(subst -I,,${PKG_INCLUDE}))' >> $@
	@echo 'set(aseia_LIBRARIES    $${aseia_BASE_DIR}/${DYNLIB} $${aseia_BASE_DIR}/${DYNIOLIB} $${aseia_BASE_DIR}/${DYNBASELIB})' >> $@
	@echo 'include(FindPackageHandleStandardArgs)' >> $@
	@echo 'find_package_handle_standard_args(aseia DEFAULT_MSG aseia_LIBRARIES aseia_INCLUDE_DIRS)' >> $@
	@echo 'mark_as_advanced(aseia_INCLUDE_DIR aseia_LIBRARIES)' >> $@

${IO_CMAKEFILE}: ${MAKEFILE_LIST}  | ${CMAKE}
	@echo 'set(aseiaIO_BASE_DIR ${BASEDIR})' > $@
	@echo 'set(aseiaIO_DEFINITIONS  ${CXXFLAGS} ${PKG_CFLAGS})' >> $@
	@echo 'set(aseiaIO_INCLUDE_DIRS $${aseiaIO_BASE_DIR}/${INC} $(subst -I,,${PKG_INCLUDE}))' >> $@
	@echo 'set(aseiaIO_LIBRARIES    $${aseiaIO_BASE_DIR}/${DYNIOLIB} $${aseiaIO_BASE_DIR}/${DYNBASELIB})' >> $@
	@echo 'include(FindPackageHandleStandardArgs)' >> $@
	@echo 'find_package_handle_standard_args(aseiaIO DEFAULT_MSG aseiaIO_LIBRARIES aseiaIO_INCLUDE_DIRS)' >> $@
	@echo 'mark_as_advanced(aseiaIO_INCLUDE_DIR aseiaIO_LIBRARIES)' >> $@

${BASE_CMAKEFILE}: ${MAKEFILE_LIST}  | ${CMAKE}
	@echo 'set(aseiaBase_BASE_DIR ${BASEDIR})' > $@
	@echo 'set(aseiaBase_DEFINITIONS  ${CXXFLAGS} ${PKG_CFLAGS})' >> $@
	@echo 'set(aseiaBase_INCLUDE_DIRS $${aseiaBase_BASE_DIR}/${INC} $(subst -I,,${PKG_INCLUDE}))' >> $@
	@echo 'set(aseiaBase_LIBRARIES    $${aseiaBase_BASE_DIR}/${DYNBASELIB})' >> $@
	@echo 'include(FindPackageHandleStandardArgs)' >> $@
	@echo 'find_package_handle_standard_args(aseiaBase DEFAULT_MSG aseiaBase_LIBRARIES aseiaBase_INCLUDE_DIRS)' >> $@
	@echo 'mark_as_advanced(aseiaBase_INCLUDE_DIR aseiaBase_LIBRARIES)' >> $@

${PKGFILE}: ${MAKEFILE_LIST} | ${PKG}
	@echo 'prefix=${BASEDIR}' > $@
	@echo 'exec_prefix=$${prefix}' >> $@
	@echo 'libdir=$${prefix}/${LIB}' >> $@
	@echo 'includedir=$${prefix}/${INC}' >> $@
	@echo "" >> $@
	@echo 'Name: ${LIBNAME}' >> $@
	@echo 'Description: Abstract Sensor Event Interface Architecture Full Library'>> $@
	@echo 'Version: 0.0.1' >> $@
	@echo 'Libs: -L$${libdir} -l${LIBNAME} -l${IO_LIBNAME} -l${BASE_LIBNAME} ${LDFLAGS}' >> $@
	@echo 'CFlags: -I$${includedir} ${CXXFLAGS}' >> $@

${IO_PKGFILE}: ${MAKEFILE_LIST} | ${PKG}
	@echo 'prefix=${BASEDIR}' > $@
	@echo 'exec_prefix=$${prefix}' >> $@
	@echo 'libdir=$${prefix}/${LIB}' >> $@
	@echo 'includedir=$${prefix}/${INC}' >> $@
	@echo "" >> $@
	@echo 'Name: ${IO_LIBNAME}' >> $@
	@echo 'Description: Abstract Sensor Event Interface Architecture Base + Print'>> $@
	@echo 'Version: 0.0.1' >> $@
	@echo 'Libs: -L$${libdir} -l${IO_LIBNAME} -l${BASE_LIBNAME} ${LDFLAGS}' >> $@
	@echo 'CFlags: -I$${includedir} ${CXXFLAGS}' >> $@

${BASE_PKGFILE}: ${MAKEFILE_LIST} | ${PKG}
	@echo 'prefix=${BASEDIR}' > $@
	@echo 'exec_prefix=$${prefix}' >> $@
	@echo 'libdir=$${prefix}/${LIB}' >> $@
	@echo 'includedir=$${prefix}/${INC}' >> $@
	@echo "" >> $@
	@echo 'Name: ${BASE_LIBNAME}' >> $@
	@echo 'Description: Abstract Sensor Event Interface Architecture Base Library'>> $@
	@echo 'Version: 0.0.1' >> $@
	@echo 'Libs: -L$${libdir} -l${BASE_LIBNAME} ${LDFLAGS}' >> $@
	@echo 'CFlags: -I$${includedir} ${CXXFLAGS}' >> $@

${DIRS}: %:
	@echo "Creating $@"
	@mkdir -p $@

${BLIB}/%.o: ${SRC}/%.cpp ${MAKEFILE_LIST} | ${BLIB}
	@echo "Compiling lib file $@ <- $<"
	@${CXX} -MMD -MT $@ -MF $@.d -c ${CXXFLAGS} ${SYMBOLS} $< -o $@ ${INCLUDES}

${BIN}/%: ${BEX}/%.o | ${DYNLIB} ${BIN}
	@echo "Linking exampple $@ <- $<"
	@${CXX} $< ${LDFLAGS} ${LDPATHS} ${LIBS} -o $@

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
	@doxygen ${DOC}/doxyfile 2> doc/error.log 1> doc/output.log

dot: ${GRAPHS}

${GRAPHS}: ${DOC}/%.svg: ${DOC}/%.dot
	@dot -Tsvg $< -o $@

-include ${DEPS}
