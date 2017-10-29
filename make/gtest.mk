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

${GTEST_HEADER}:
	@echo "Fetchin Dependancy GTest"
	@git submodule update --init gtest

${BTEST}/gtest.o : ${GTEST_HEADER} ${MAKEFILE_LIST} | ${BTEST}
	@echo "Building Dependancy GTest $@ <- $< "
	@$(CXX) -MMD -MT $@ -MF $@.d -I${GTEST_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GTEST_CODE} -o $@

${BTEST}/gmock.o : ${GTEST_HEADER} ${MAKEFILE_LIST} | ${BTEST}
	@echo "Building Dependancy GMock $@ <- $< "
	@$(CXX) -MMD -MT $@ -MF $@.d -I${GMOCK_DIR} $(GTEST_FLAGS) $(GTEST_INCLUDES) -c ${GMOCK_CODE} -o $@

${LIB}/libgtest.a : ${BTEST}/gtest.o ${BTEST}/gmock.o ${MAKEFILE_LIST} | ${LIB}
	@echo "Linking Dependancy GTest and GMock $@ <- [$<]"
	@${AR} ${ARFLAGS} $@ $^ > /dev/null
	@${RANLIB} $@
