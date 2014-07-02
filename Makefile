PROFILING        ?= 0
DEBUG            ?= 0

INCLUDES         :=
LDPATHS          :=
SYMBOLS          :=
CXXFLAGS         := -std=gnu++11 -Wall -ffunction-sections -fno-threadsafe-statics
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

BUILD := ./build
SRC   := ./src
INC   := ./include
BIN   := ./bin
DOC   := ./doc


TARGETS  := $(notdir $(basename $(wildcard ${SRC}/*.cpp)))
LIBS     := $(addprefix -l, ${LIBS})
LDPATHS  := $(addprefix -L, ${LDPATHS})
INCLUDES := $(addprefix -I, ${INCLUDES} ${INC})
DEPS     := $(wildcard ${BUILD}/*.d)



.PHONY: all clean run_% debug_% doc

all: ${TARGETS}

${TARGETS}: %: ${BIN}/%
	
$(addprefix ${BIN}/, ${TARGETS}): ${BIN}/%: ${BUILD}/%.o | ${BIN}
	${CXX} ${LDFLAGS} -o $@ $^ ${LDPATHS} ${LIBS}

${BIN} ${BUILD}: %:
	@mkdir -p $@

${BUILD}/%.o: ${SRC}/%.cpp Makefile | ${BUILD}
	${CXX} -MMD -c ${CXXFLAGS} $< -o $@ ${INCLUDES}

$(addprefix run_,${TARGETS}): run_%: %
	@./${BIN}/$< ${ARGS}

$(addprefix debug_,${TARGETS}): debug_%: %
	@${DBG} ./${BIN}/$< -ex start ${ARGS}

clean:
	@rm -rf ${BIN} ${BUILD} ${DOC}/html

doc:
	@doxygen ${DOC}/doxyfile

-include ${DEPS}
