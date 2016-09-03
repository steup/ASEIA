PROFILING        ?= 0
DEBUG            ?= 0
EMBEDDED         ?= 0


INCLUDES +=
LDPATHS  +=
SYMBOLS  +=
CXXFLAGS += -std=gnu++11
LDFLAGS  +=
LIBS     +=
PACKAGES += eigen3
LIBNAME  ?= ASEIA
DLIBEXT  ?= so
SLIBEXT  ?= a

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

BASEDIR   := $(abspath $(dir $(lastword ${MAKEFILE_LIST}))/..)

SRC       := src
EXAMPLE   := example
INC       := include
DOC       := doc
TESTS     := tests
RUN_TESTS := run_tests

HTML      := ${DOC}/html
BIN       := bin
BUILD     := build
BLIB      := ${BUILD}/lib
BPROG     := ${BUILD}/prog
BTEST     := ${BUILD}/test
LIB       := lib
CMAKE     := cmake
PKG       := pkgconfig
LOG       := log

DIRS      := ${BIN} ${BLIB} ${BPROG} ${BTEST} ${BUILD} ${LIB} ${CMAKE} ${PKG} ${LOG}
GARBAGE   := ${HTML} ${DIRS}

CMAKEFILE := ${CMAKE}/aseiaConfig.cmake
PKGFILE   := ${PKG}/aseia.pc
CONFIGS   := ${PKGFILE} ${CMAKEFILE}
DYNLIB    := ${LIB}/lib${LIBNAME}.${DLIBEXT}
STATLIB   := ${LIB}/lib${LIBNAME}.${SLIBEXT}
TARGETS   := ${DYNLIB} ${STATLIB}


include smhasher.mk

LIBS     += ${LIBNAME} ${SMHASHER_LIBS}
LDPATHS  += ${LIB}
LDFLAGS  += -Wl,--as-needed -Wl,--rpath=$(abspath ${LIB})

PKG_INCLUDE := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-I))
PKG_CFLAGS  := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --cflags-only-other))
PKG_LIBS    := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-l))
PKG_LDPATHS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-L))
PKG_LDFLAGS := $(foreach pkg, ${PACKAGES}, $(shell pkg-config ${pkg} --libs-only-other))


SYMBOLS  := $(addprefix -D, ${SYMBOLS})
EXAMPLES := $(notdir $(basename $(wildcard ${EXAMPLE}/*.cpp)))
OBJECTS  := $(addprefix ${BLIB}/, $(addsuffix .o, $(notdir $(basename $(wildcard ${SRC}/*.cpp)))))
LIBS     := $(addprefix -l, ${LIBS}) ${PKG_LIBS}
LDPATHS  := $(addprefix -L, ${LDPATHS}) ${PKG_LDPATHS}
INCLUDES := $(addprefix -I, ${INCLUDES} ${INC} ${SMHASHER_INCLUDES}) ${PKG_INCLUDE}
CXXFLAGS := ${CXXFLAGS} ${PKG_CFLAGS}
LDFLAGS  := ${LDFLAGS} ${PKG_LDFLAGS}
DEPS     := $(wildcard ${BUILD}/*/*.o.d)

OBJECTS  := ${OBJECTS} ${SMHASHER_OBJECTS}
