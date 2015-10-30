#include <ValueElement.h>

namespace tests {

namespace valueElementSuite {

template<typename T>
bool exactlyEqual(const ValueElement<T, true>& a, const ValueElement<T, true>& b) {
  return a.value() == b.value() && a.uncertainty() == b.uncertainty();
}


template<typename V>
void testComp(V a, V b, bool eq, bool ne, bool le, bool ge, bool lt, bool gt) {
    EXPECT_EQ(a==b, eq) << a << " == " << b << " = " << (a==b) << " != " << eq;
    EXPECT_EQ(a!=b, ne) << a << " != " << b << " = " << (a!=b) << " != " << ne;
    EXPECT_EQ(a<=b, le) << a << " <= " << b << " = " << (a<=b) << " != " << le;
    EXPECT_EQ(a>=b, ge) << a << " >= " << b << " = " << (a>=b) << " != " << ge;
    EXPECT_EQ(a<b,  lt) << a << " <  " << b << " = " << (a<b)  << " != " << lt;
    EXPECT_EQ(a>b,  gt) << a << " >  " << b << " = " << (a>b)  << " != " << gt;
}

template<typename V>
void testOp(V a, V b, V add, V sub, V mul, V div) {
    EXPECT_TRUE(exactlyEqual(a+b, add)) << a << " + " << b << " = " << (a+b) << " != " << add;
    EXPECT_TRUE(exactlyEqual(a-b, sub)) << a << " - " << b << " = " << (a-b) << " != " << sub;
    EXPECT_TRUE(exactlyEqual(a*b, mul)) << a << " * " << b << " = " << (a*b) << " != " << mul;
    EXPECT_TRUE(exactlyEqual(a/b, div)) << a << " / " << b << " = " << (a/b) << " != " << div;
}

TEST(ValueElementSuite, UInt8Test) {
  using V=ValueElement<uint8_t, true>;
	V a={13, 37};
	V b={13, 37};
	V c={73, 1};
	V d={3, 2};
	testComp(a, b, true, false, true, true, false, false);
	testComp(a, c, false, true, true, false, true, false);
	testOp(a, d, V({16, 39}), V({10, 39}), V({65, 185}), V({13, 37}));
}

TEST(ValueElementSuite, Int8Test) {
  using V=ValueElement<int8_t, true>;
	V a={-13, 13};
	V b={-13, 13};
	V c={73, 1};
	V d={3, 2};
	testComp(a, b, true, false, true, true, false, false);
	testComp(a, c, false, true, true, false, true, false);
	testOp(a, d, V({-10, 15}), V({-16, 15}), V({-65, 65}), V({-13, 13}));
}

}}
