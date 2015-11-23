#include <MetaValueImplementation.h>

namespace tests {

namespace metaValueImplSuite {

using BCOp = MetaValueBaseImplementation::BinaryConstOp;

TEST(MetaValueImplSuite, uInt8Test) {
	
	MetaValueImplementation<uint8_t> v0={{{13, 37}, {1, 0}, {3, 7}}};
	MetaValueImplementation<uint8_t> v2={{{73, 1}, {3, 1}, {7, 1}}};
	//EXPECT_TRUE(v0.binaryConstOp(BCOp::Equal, v0)->prod()) << "Equally assigned values are not equal";
	//EXPECT_TRUE(v0.binaryConstOp(BCOp::NotEqual, v2)->prod()) << "Unequally assigned values are equal";
	//EXPECT_FALSE(v0.binaryConstOp(BCOp::Equal, v2)->prod()) << "Unequally assigned values are equal";
	//EXPECT_FALSE(v0.binaryConstOp(BCOp::NotEqual, v2)->prod()) << "Equally assigned values are not equal";
}

}}
