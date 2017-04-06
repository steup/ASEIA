
#include <gtest/gtest.h>
#include <Value.h>
#include <IO.h>

namespace tests {

namespace valueSuite {

TEST(ValueSuite, uInt8Test) {
	Value<uint8_t, 1, 3, true> v0={{{13, 37}, {1, 1}, {3, 2}}};
	Value<uint8_t, 1, 3, true> v2={{{73, 1}, {3, 0}, {7, 1}}};
	EXPECT_TRUE((v0==v0).prod()) << v0 << " == " << v0 << " = " << (v0==v0) << " != true";
	EXPECT_TRUE((v0!=v2).prod()) << v0 << " != " << v2 << " = " << (v0!=v2) << " != true";
	EXPECT_FALSE((v0==v2).prod()) << v0 << " == " << v2 << " = " << (v0==v2) << " != true";
	EXPECT_TRUE((v0<v2).prod()) << v0 << " < " << v2 << " = " << (v0<v2) << " != true";
}

}}
