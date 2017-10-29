SMHASHER_DIR      := smhasher

SMHASHER_SRC      := ${SMHASHER_DIR}/src
SMHASHER_INCLUDES := ${SMHASHER_DIR}/src
BSMHASHER         := ${BUILD}/smhasher

SMHASHER_OBJECTS  := MurmurHash2

SMHASHER_FLAGS    := ${CXXFLAGS}
SMHASHER_OBJECTS  := $(addprefix ${BSMHASHER}/, $(addsuffix .o, ${SMHASHER_OBJECTS}))

${BSMHASHER}:
	@echo "create directory $@"
	@mkdir -p $@

${SMHASHER_SRC}: | ${LOG}
	@echo "Fetchin Dependancy SMHasher" | tee -a ${LOG}/smhasher.log
	@echo "git submodule update --init ${SMHASHER_DIR}" &>> ${LOG}/smhasher.log
	@git submodule update --init ${SMHASHER_DIR} &>> ${LOG}/smhasher.log

${BSMHASHER}/%.o : ${SMHASHER_SRC}/%.cpp smhasher.mk | ${SMHASHER_SRC} ${BSMHASHER} ${LOG}
	@echo "Building Dependancy SMHasher $@ <- $< "| tee -a ${LOG}/smhasher.log
	@$(CXX) -MMD -MT $@ -MF $@.d $(SMHASHER_FLAGS) $(SMHASHER_INCLUDES) -c $< -o $@ &>>  ${LOG}/smhasher.log
