GTEST_DIR := gtest

GTEST_README   := ${GTEST_DIR}/README

GTEST_MODULES  := gtest

GTEST_INCLUDES := -I${GTEST_DIR}/include
GTEST_FLAGS    := -pthread
GTEST_LDFLAGS  := -pthread
GTEST_LDPATHS  := 
GTEST          := $(addprefix ${LIB}/lib, $(addsuffix .a, ${GTEST_MODULES}))
GTEST_LIBS     := $(addprefix -l, ${GTEST_MODULES})

GTEST_HEADERS  = $(wildcard ${GTEST_DIR}/include/gtest/*.h ${GTEST_DIR}/include/gtest/internal/*.h)
GTEST_SRCS     = ${GTEST_DIR}/src/gtest-all.cc $(filter-out *gtest-all.cc, $(wildcard ${GTEST_DIR}/src/*.cc ${GTEST_DIR}/src/*.h))

${GTEST_README}: | ${LOG}
	@echo "Fetchin Dependancy GTest" | tee -a ${LOG}/gtest.log
	@echo "git submodule init ${GTEST_DIR}" &>> ${LOG}/gtest.log
	@git submodule init ${GTEST_DIR} &>> ${LOG}/gtest.log
	@echo "git submodule update ${GTEST_DIR}" &>> ${LOG}/gtest.log
	@git submodule update ${GTEST_DIR} &>> ${LOG}/gtest.log

${BTEST}/gtest-all.o : ${GTEST_DIR} gtest.mk | ${GTEST_README} ${BTEST} ${LOG}
	@echo "Building Dependancy GTest" | tee -a ${LOG}/gtest.log
	@echo "$(CXX) -I${GTEST_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GTEST_DIR}/src/gtest-all.cc -o $@" &>>  ${LOG}/gtest.log
	@$(CXX) -I${GTEST_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GTEST_DIR}/src/gtest-all.cc -o $@ &>>  ${LOG}/gtest.log

${LIB}/libgtest.a : ${BTEST}/gtest-all.o | ${LIB} ${LOG}
	@echo "Linking Dependancy GTest" | tee -a ${LOG}/gtest.log
	@echo "$(AR) $(ARFLAGS) $@ $^" &>> ${LOG}/gtest.log
	@$(AR) $(ARFLAGS) $@ $^ &>> ${LOG}/gtest.log
