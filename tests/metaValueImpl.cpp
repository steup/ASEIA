
#include <gtest/gtest.h>
#include <MetaValue.h>
#include <MetaFactory.h>

namespace tests {

namespace metaValueImplSuite {

using BCOp = MetaValueBaseImplementation::BinaryConstOp;

TEST(MetaValueImplSuite, uInt8Test) {
	MetaFactory& f = MetaFactory::instance();
	MetaValue v0=f.create({{{13, 37}, {1, 0}, {3, 7}}}, id::type::UInt8::value());
	MetaValue v2=f.create({{{73, 1}, {3, 1}, {7, 1}}}, id::type::UInt8::value());
	//EXPECT_TRUE(v0.binaryConstOp(BCOp::Equal, v0)->prod()) << "Equally assigned values are not equal";
	//EXPECT_TRUE(v0.binaryConstOp(BCOp::NotEqual, v2)->prod()) << "Unequally assigned values are equal";
	//EXPECT_FALSE(v0.binaryConstOp(BCOp::Equal, v2)->prod()) << "Unequally assigned values are equal";
	//EXPECT_FALSE(v0.binaryConstOp(BCOp::NotEqual, v2)->prod()) << "Equally assigned values are not equal";
}

}}
