PROFILING        ?= 0
DEBUG            ?= 0

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

BUILD    := ./build
SRC      := ./src
EXAMPLE  := ./example
INC      := ./include
BIN      := ./bin
DOC      := ./doc
LIB      := ./lib

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

${DYNLIB}: ${OBJECTS} | ${LIB}
	@echo "Building dynamic library: $@ <- [$^]"
	@g++ --shared -o $@ $^

${STATLIB}: ${OBJECTS} | ${LIB}
	@echo "Building static library: $@ <- [$^]"
	@ar r $@ $^
	@ranlib $@

${EXAMPLES}: %: ${BIN}/%
	
$(addprefix ${BIN}/, ${EXAMPLES}): ${BIN}/%: ${BUILD}/%.o | ${DYNLIB} ${BIN}
	@echo "Linking Example $@ <- $<"
	@${CXX} ${LDFLAGS} -o $@ $^ ${LDPATHS} ${LIBS}

${BIN} ${BUILD} ${LIB}: %:
	@echo "Creating $@"
	@mkdir -p $@

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
	@rm -rf ${BIN} ${BUILD} ${DOC}/html ${LIB}

doc:
	@echo "Creating Documentation"
	@doxygen ${DOC}/doxyfile

-include ${DEPS}
