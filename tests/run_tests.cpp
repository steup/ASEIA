#include <gtest/gtest.h>

#include <id.cpp>
#include <serializer.cpp>
#include <deserializer.cpp>
#include <valueElement.cpp>
#include <value.cpp>
#include <metaValueImpl.cpp>
#include <metaEvent.cpp>

/** \namespace tests
 *  \brief %Unit Test Suites
 **/

/** \brief Execution of all %Unit Test Suites
 *  \param argc Number of arguments passed
 *  \param argv Array of arguments
 *  \return 0 on success
 **/
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}	
