#include <ValueElement.h>

namespace tests {

namespace valueElementSuite {

template<typename T>
bool exactlyEqual(const ValueElement<T, true>& a, const ValueElement<T, true>& b) {
  return a.value() == b.value() && a.uncertainty() == b.uncertainty();
}

#define testComp(a, b, eq, ne, le, ge, lt, gt) { \
    EXPECT_EQ(a==b, eq) << a << " == " << b << " = " << (a==b) << " != " << eq; \
    EXPECT_EQ(a!=b, ne) << a << " != " << b << " = " << (a!=b) << " != " << ne; \
    EXPECT_EQ(a<=b, le) << a << " <= " << b << " = " << (a<=b) << " != " << le; \
    EXPECT_EQ(a>=b, ge) << a << " >= " << b << " = " << (a>=b) << " != " << ge; \
    EXPECT_EQ(a<b,  lt) << a << " <  " << b << " = " << (a<b)  << " != " << lt; \
    EXPECT_EQ(a>b,  gt) << a << " >  " << b << " = " << (a>b)  << " != " << gt; \
}

#define testOp(a, b, add, sub, mul, div) { \
    EXPECT_TRUE(exactlyEqual(a+b, add)) << a << " + " << b << " = " << (a+b) << " != " << add; \
    EXPECT_TRUE(exactlyEqual(a-b, sub)) << a << " - " << b << " = " << (a-b) << " != " << sub; \
    EXPECT_TRUE(exactlyEqual(a*b, mul)) << a << " * " << b << " = " << (a*b) << " != " << mul; \
    EXPECT_TRUE(exactlyEqual(a/b, div)) << a << " / " << b << " = " << (a/b) << " != " << div; \
}

TEST(ValueElementSuite, UInt8Test) {
  using V=ValueElement<uint8_t, true>;
	V e0 = {0, 0};
	V e1 = {0, 255};
	V e2 = {255, 0};
	V e3 = {255, 255};
	V a={13, 37};
	V b={73, 1};
	V c={3, 2};
	testComp(a, b, false, true , true , false, true , false);
	testComp(a, c, true , false, true , true , false, false);
	testComp(b, c, false, true , false, true , false, true );
	testOp(a, e0, a, a, V({0, 0}), V({0, 255}));
	testOp(a, e1, V({13, 255}) , V({13, 255}), V({0, 255})  , V({0, 255}));
	testOp(a, e2, V({255, 255}), V({0, 255}) , V({255, 255}), V({0, 1})  );
	testOp(a, e3, V({255, 255}), V({0, 255}) , V({255, 255}), V({0, 255}));
	testOp(a, b, V({86, 38}), V({0, 255}), V({255, 255}), V({0, 1}));
	testOp(a, c, V({16, 39}), V({10, 39}), V({65, 185}), V({13, 37}));
}

TEST(ValueElementSuite, Int8Test) {
  using V=ValueElement<int8_t, true>;
	V e0 = {0, 0};
	V e1 = {0, 255};
	V e2 = {127, 0};
	V e3 = {127, 255};
	V e4 = {-128, 0};
	V e5 = {-128, 255};
	V a={13, 37};
	V b={-73, 1};
	V c={3, 2};
	testComp(a, b, false, true , false, true , false, true );
	testComp(a, c, true , false, true , true , false, false);
	testComp(b, c, false, true , true , false, true , false);
	testOp(a, e0, a, a, V({0, 0}), V({0, 255}));
	testOp(a, e1, V({13, 255}) , V({13, 255}), V({0, 255})  , V({0, 255}));
	testOp(a, e2, V({127, 255}), V({-114, 37}) , V({127, 255}), V({0, 1})  );
	testOp(a, e3, V({127, 255}), V({-114, 255}) , V({127, 255}), V({0, 255}));
	testOp(a, b, V({-60, 38}), V({86, 38}), V({-128, 255}), V({0, 1}));
	testOp(a, c, V({16, 39}), V({10, 39}), V({65, 185}), V({13, 37}));
}

}}
