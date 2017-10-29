SMHASHER_DIR      := smhasher

SMHASHER_SRC      := ${SMHASHER_DIR}/src
SMHASHER_INCLUDES := ${SMHASHER_DIR}/src
BSMHASHER         := ${BUILD}/smhasher

SMHASHER_OBJECTS  := MurmurHash2

SMHASHER_FLAGS    := ${CXXFLAGS}
SMHASHER_SOURCES  := $(addprefix ${SMHASHER_SRC}/, $(addsuffix .cpp, ${SMHASHER_OBJECTS}))
SMHASHER_OBJECTS  := $(addprefix ${BSMHASHER}/, $(addsuffix .o, ${SMHASHER_OBJECTS}))

${BSMHASHER}:
	@echo "create directory $@"
	@mkdir -p $@

${SMHASHER_SRC}:
	@echo "Fetchin Dependancy SMHasher"
	@git submodule update --init ${SMHASHER_DIR}

${SMHASHER_SOURCES}: ${SMHASHER_SRC}

${BSMHASHER}/%.o : ${SMHASHER_SRC}/%.cpp make/smhasher.mk | ${BSMHASHER}
	@echo "Building Dependancy SMHasher $@ <- $< "
	@$(CXX) -MMD -MT $@ -MF $@.d $(SMHASHER_FLAGS) -I $(SMHASHER_INCLUDES) -c $< -o $@
	@file $@
