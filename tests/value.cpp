
#include <gtest/gtest.h>
#include <Value.h>
#include <IO.h>

namespace tests {

namespace valueSuite {

TEST(ValueSuite, uInt8Test) {
  using V3U = Value<uint8_t, 1, 3, true>;
  using V3  = Value<uint8_t, 1, 3, false>;
	V3U v0={{{3, 1}, {2, 0}, {1, 1}}};
	V3U v2={{{73, 1}, {3, 0}, {7, 1}}};
	EXPECT_TRUE((v0==v0).prod()) << v0 << " == " << v0 << " = " << (v0==v0) << " != true";
	EXPECT_TRUE((v0!=v2).prod()) << v0 << " != " << v2 << " = " << (v0!=v2) << " != true";
	EXPECT_FALSE((v0==v2).prod()) << v0 << " == " << v2 << " = " << (v0==v2) << " != true";
	EXPECT_TRUE((v0<v2).prod()) << v0 << " < " << v2 << " = " << (v0<v2) << " != true";
  EXPECT_EQ(v0.norm().value(), 3) << v0*v0;
  EXPECT_EQ(v0.norm().uncertainty(), 2) << v0*v0;
  V3 resSqrtV({{{1}, {1}, {1}}});
  V3 resSqrtU({{{1}, {1}, {1}}});
  EXPECT_EQ(v0.sqrt().value(),       resSqrtV);
  EXPECT_EQ(v0.sqrt().uncertainty(), resSqrtU);
}

}}
