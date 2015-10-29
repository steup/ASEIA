#include <ValueElement.h>

namespace tests {

namespace valueElementSuite {

TEST(ValueElementSuite, UInt8Test) {
	ValueElement<uint8_t, true> v0={13, 37};
	ValueElement<uint8_t, true> v2={73, 1};
	EXPECT_EQ(v0, v0) << "Equally assigned values are not equal";
	EXPECT_NE(v0, v2) << "Unequally assigned values are equal";
	EXPECT_FALSE(v0==v2) << "Unequally assigned values are equal";
}

}}
