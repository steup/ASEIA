
#include <gtest/gtest.h>
#include <ValueElement.h>

namespace tests {

namespace valueElementSuite {

template<typename T, bool U>
bool exactlyEqual(const ValueElement<T, U>& a, const ValueElement<T, U>& b) {
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

TEST(ValueElementSuite, certainBoolTest) {
  using V=ValueElement<bool, false>;
	V e0 = {false};
	V e1 = { true};
	testComp(e0, e1, false, true , true, false, true , false);
	testOp(e0, e1, e1, e0, e0, e0);
}

TEST(ValueElementSuite, uncertainBoolTest) {
  using V=ValueElement<bool, false>;
	V e0 = {false, false};
	V e1 = { true, false};
	V e2 = {false, true};
	V e3 = { true, true};
	testComp(e0, e1, false, true , true , false, true , false);
	testOp(e0, e1, e1, e0, e0, e0);
	testComp(e0, e2, true , false, true , true , false, false);
	testOp(e0, e2, e2, e2, e0, e2);
	testComp(e1, e3, true , false, true , true , false, false);
	testOp(e1, e3, e3, e2, e3, e2);
}

TEST(ValueElementSuite, certainUInt8Test) {
  using V=ValueElement<uint8_t, false>;
	V e0 = {  0};
	V e1 = {255};
	V  a = { 13};
	V  b = { 73};
	V  c = {  3};
	testComp(a, b, false, true , true , false, true , false);
	testComp(a, c, false, true , false, true , false, true );
	testComp(b, c, false, true , false, true , false, true );
	testOp(a, e0,        a,       a, V({  0}), V({ 0}));
	testOp(a, e1, V({255}), V({ 0}), V({255}), V({ 0}));
	testOp(a,  b, V({ 86}), V({ 0}), V({255}), V({ 0}));
	testOp(a,  c, V({ 16}), V({10}), V({ 39}), V({ 4}));
}

TEST(ValueElementSuite, certainInt8Test) {
  using V=ValueElement<int8_t, false>;
	V e0 = {   0};
	V e1 = { 127};
	V e2 = {-128};
	V a  = {  13};
	V b  = {- 73};
	V c  = {   3};

	testComp(a, b, false, true , false, true , false, true );
	testComp(a, c, false, true , false, true , false, true );
	testComp(b, c, false, true , true , false, true , false);

	testOp(a, e0,         a,         a, V({   0}), V({ 0}));
	testOp(a, e1, V({ 127}), V({-114}), V({ 127}), V({ 0}));
	testOp(a, e2, V({-115}), V({ 127}), V({-128}), V({ 0}));
	testOp(a,  b, V({- 60}), V({  86}), V({-128}), V({ 0}));
	testOp(a,  c, V({  16}), V({  10}), V({  39}), V({ 4}));
}

TEST(ValueElementSuite, uncertainUInt8Test) {
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

TEST(ValueElementSuite, uncertainInt8Test) {
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

	testOp(a, e0,              a,              a, V({   0,   0}), V({ 0, 255}));
	testOp(a, e1, V({  13, 255}), V({  13, 255}), V({   0, 255}), V({ 0, 255}));
	testOp(a, e2, V({ 127, 255}), V({-114,  37}), V({ 127, 255}), V({ 0,   1}));
	testOp(a, e3, V({ 127, 255}), V({-114, 255}), V({ 127, 255}), V({ 0, 255}));
	testOp(a, e4, V({-115,  37}), V({ 127, 255}), V({-128, 255}), V({ 0,   1}));
	testOp(a, e5, V({-115, 255}), V({ 127, 255}), V({-128, 255}), V({ 0, 255}));
	testOp(a,  b, V({- 60,  38}), V({  86,  38}), V({-128, 255}), V({ 0,   1}));
	testOp(a,  c, V({  16,  39}), V({  10,  39}), V({  65, 185}), V({13,  37}));
}

TEST(ValueElementSuite, castTest) {
  using I8 = ValueElement<int8_t, true>;
  using U8 = ValueElement<uint8_t, true>;
  using U16 = ValueElement<uint16_t, true>;
  using U32 = ValueElement<uint32_t, true>;
  using F = ValueElement<float, true>;
  using D = ValueElement<double, true>;

  I8 i8 = {-127, 0};
  U8 u8 = {255, 0};
  U32 u32 = {1234, 1};
  F  f  = {1234.5678, 0};
  EXPECT_EQ(U8(i8).value(), 0);
  EXPECT_EQ(U8(i8).uncertainty(), 127);
  EXPECT_EQ(I8(u8).value(), 127);
  EXPECT_EQ(I8(u8).uncertainty(), 128);
  EXPECT_EQ(U16(f).value(), 1234);
  EXPECT_EQ(U16(f).uncertainty(), 1);
  EXPECT_EQ(U32(f).value(), 1234);
  EXPECT_EQ(U32(f).uncertainty(), 1);
  EXPECT_EQ(D(u32).value(), 1234);
  EXPECT_EQ(D(u32).uncertainty(), 1);
}

}}
