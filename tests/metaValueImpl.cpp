#include <MetaValueImplementation.h>

namespace tests {

namespace metaValueImplSuite {

TEST(MetaValueImplSuite, UInt8Test) {
	MetaValueImplementation<uint8_t> v0={{{13, 37}, {1, 0}, {3, 7}}};
	MetaValueImplementation<uint8_t> v2={{{73, 1}, {3, 1}, {7, 1}}};
	EXPECT_EQ(v0, v0) << "Equally assigned values are not equal";
	EXPECT_NE(v0, v2) << "Unequally assigned values are equal";
	EXPECT_FALSE(v0==v2) << "Unequally assigned values are equal";
	EXPECT_FALSE(v0!=v0) << "Equally assigned values are not equal";
}

}}
