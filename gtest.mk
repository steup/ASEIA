GTEST_DIR := gtest/googletest
GMOCK_DIR := gtest/googlemock

GTEST_CODE     := ${GTEST_DIR}/src/gtest-all.cc
GMOCK_CODE     := ${GMOCK_DIR}/src/gmock-all.cc
GTEST_HEADER   := ${GTEST_DIR}/include/gtest/gtest.h

GTEST_MODULES  := gtest

GTEST_INCLUDES := $(addprefix -I, ${GTEST_DIR}/include -I ${GMOCK_DIR}/include)
GTEST_FLAGS    := -pthread
GTEST_LDFLAGS  := -pthread
GTEST_LDPATHS  := 
GTEST          := $(addprefix ${LIB}/lib, $(addsuffix .a, ${GTEST_MODULES})) ${GTEST_HEADER}
GTEST_LIBS     := $(addprefix -l, ${GTEST_MODULES})

${GTEST_HEADER}: | ${LOG}
	@echo "Fetchin Dependancy GTest" | tee -a ${LOG}/gtest.log
	@echo "git submodule update --init ${GTEST_DIR}" &>> ${LOG}/gtest.log
	@git submodule update --init ${GTEST_DIR} &>> ${LOG}/gtest.log

${BTEST}/gtest.o : ${GTEST_HEADER} gtest.mk | ${BTEST} ${LOG}
	@echo "Building Dependancy GTest $@ <- $< "| tee -a ${LOG}/gtest.log
	@echo "$(CXX) -MMD -MF $@.d -I${GTEST_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GTEST_CODE} -o $@" &>>  ${LOG}/gtest.log
	@$(CXX) -MMD -MT $@ -MF $@.d -I${GTEST_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GTEST_CODE} -o $@ &>>  ${LOG}/gtest.log

${BTEST}/gmock.o : ${GTEST_HEADER} gtest.mk | ${BTEST} ${LOG}
	@echo "Building Dependancy GMock $@ <- $< "| tee -a ${LOG}/gtest.log
	@echo "$(CXX) -MMD -MF $@.d -I${GMOCK_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GMOCK_CODE} -o $@" &>>  ${LOG}/gtest.log
	@$(CXX) -MMD -MT $@ -MF $@.d -I${GMOCK_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GMOCK_CODE} -o $@ &>>  ${LOG}/gtest.log

${LIB}/libgtest.a : ${BTEST}/gtest.o ${BTEST}/gmock.o gtest.mk | ${LIB} ${LOG}
	@echo "Linking Dependancy GTest and GMock $@ <- [$<]" | tee -a ${LOG}/gtest.log
	@echo "$(AR) $(ARFLAGS) $@ $^" &>> ${LOG}/gtest.log
	@$(AR) $(ARFLAGS) $@ $^ &>> ${LOG}/gtest.log
