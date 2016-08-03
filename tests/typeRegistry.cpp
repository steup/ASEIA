#include <TypeRegistry.h>

#include <BaseEvent.h>

namespace tests {
namespace typeRegistryTests {

class TypeRegistryTestSuite : public ::testing::Test {
	public:
		EventType none, base=(EventType)BaseEvent<>();
		TypeRegistry& tR = TypeRegistry::instance();
		TypeRegistryTestSuite() {
			tR.registerType(base);
		}
};

TEST_F(TypeRegistryTestSuite, findTest) {
	EXPECT_TRUE(tR.contains(base)) << "Known EventType not found";
}

TEST_F(TypeRegistryTestSuite, notFoundTest) {
	EXPECT_FALSE(tR.contains(none)) << "Unknown EventType found";
}

TEST_F(TypeRegistryTestSuite, retrieveTest) {
	auto r=tR.find(base);
	ASSERT_NE(r.begin(), r.end()) << "Stored EventType not found";
	EXPECT_EQ(r.front(), base) << "Stored EventType not found";
}

}}
