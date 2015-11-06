#include <Value.h>

namespace tests {

namespace valueSuite {

TEST(ValueSuite, UInt8Test) {
	Value<uint8_t, 1, 3, true> v0={{{13, 37}, {1, 1}, {3, 7}}};
	Value<uint8_t, 1, 3, true> v2={{{73, 1}, {3, 1}, {7, 1}}};
	EXPECT_EQ(v0, v0) << "Equally assigned values are not equal";
	EXPECT_NE(v0, v2) << "Unequally assigned values are equal";
	EXPECT_FALSE(v0==v2) << "Unequally assigned values are equal";
}

}}
