#include <gtest/gtest.h>

#include <id.cpp>
#include <serializer.cpp>
#include <deserializer.cpp>
#include <valueElement.cpp>
#include <value.cpp>
#include <attribute.cpp>
#include <metaValueImpl.cpp>
#include <metaAttribute.cpp>
#include <metaEvent.cpp>
#include <event.cpp>
#include <filter.cpp>
#include <metaSerialization.cpp>
#include <metaDeSerialization.cpp>
#include <formatID.cpp>
#include <typeRegistry.cpp>

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
