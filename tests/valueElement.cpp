#include <ValueElement.h>

namespace tests {

namespace valueElementSuite {

template<typename T>
bool exactlyEqual(const ValueElement<T, true>& a, const ValueElement<T, true>& b) {
  return a.value() == b.value() && a.uncertainty() == b.uncertainty();
}

TEST(ValueElementSuite, UInt8Test) {
  using V=ValueElement<uint8_t, true>;
	V a={13, 37};
	V b={13, 37};
	V c={73, 1};
	V d={3, 2};
	EXPECT_TRUE(a==b)                           << "False Negative on ==";
	EXPECT_TRUE(a!=c)                           << "False Negative on !=";
	EXPECT_FALSE(a==c)                          << "False Positive on ==";
  EXPECT_FALSE(a!=b)                          << "False Positive on !=";
 for(int16_t x=0;x<256;x+=8)
    for(int16_t y=0;y<256;y+=8)
      for(int16_t z=0;z<256;z+=8)
        for(int16_t w=0;w<256;w+=8) {
            V a = {(uint8_t)x, (uint8_t)y};
            V b = {(uint8_t)z, (uint8_t)w};
            V res;
            if(x+z>255) {
              res = {255, 255};
            } else
              res = {(uint8_t)(x+z), (uint8_t)((y+w)<256?y+w:255)};
            EXPECT_TRUE(exactlyEqual(a+b, res)) << a << " + " << b << " = " << (a+b) << " != " << res;
          }
  EXPECT_TRUE(exactlyEqual(a+b,V({26, 74})))  << "a+b is not correct";
  EXPECT_TRUE(exactlyEqual(a+c,V({86, 38})))  << "a+c is not correct";
  EXPECT_TRUE(exactlyEqual(b+a,a+b))          << "+ is not kommutative";
  EXPECT_TRUE(exactlyEqual(a-b,V({0,74})))    << "a-b is not correct";
  EXPECT_TRUE(exactlyEqual(a*d,V({65, 185}))) << "a*d is not correct"; //13+-37 * 3+-2 = [-24 * 5, 50 * 5] = [-120, 250] = (250-120) /2 +- (250--120) /2 = 65 +-185
  EXPECT_TRUE(exactlyEqual(a*d,d*a))          << "* is not kommutative";
  EXPECT_TRUE(exactlyEqual(a+(b+c),(a+b)+c))  << "+ is not asociative";
  EXPECT_TRUE(exactlyEqual(a/d,a))            << "a/d is not correct"; //13+-37 / 3+-2 = [ -24 / 1, 50 / 1] = [-24, 50] = 50-24/2 +- 50--24 / 2 = 13 +-37
}

TEST(ValueElementSuite, Int8Test) {
  using V=ValueElement<int8_t, true>;
	V a={-13, 13};
	V b={-13, 13};
	V c={73, 1};
	V d={3, 2};
	EXPECT_TRUE(a==b)                           << "False Negative on ==";
	EXPECT_TRUE(a!=c)                           << "False Negative on !=";
	EXPECT_FALSE(a==c)                          << "False Positive on ==";
  EXPECT_FALSE(a!=b)                          << "False Positive on !=";
  for(int16_t x=-128;x<128;x+=8)
    for(int16_t y=0;y<128;y+=8)
      for(int16_t z=-128;z<128;z+=8)
        for(int16_t w=0;w<128;w+=8) {
            V a = {(int8_t)x, (int8_t)y};
            V b = {(int8_t)z, (int8_t)w};
            V res = {(int8_t)(x+z), (int8_t)((y+w)<127?y+w:127)};
            if(x+z>127) {
              res = {127, 127};
            }
            if(x+z<-128) {
              res = {-128, 127};
            }
            EXPECT_TRUE(exactlyEqual(a+b, res)) << a << " + " << b << " = " << (a+b) << " != " << res;
          }
  EXPECT_TRUE(exactlyEqual(a+b,V({-26, 26})))  << "a+b is not correct";
  EXPECT_TRUE(exactlyEqual(a+c,V({60, 14})))  << "a+c is not correct";
  EXPECT_TRUE(exactlyEqual(b+a,a+b))          << "+ is not kommutative";
  EXPECT_TRUE(exactlyEqual(a-b,V({0,26})))    << "a-b is not correct";
  EXPECT_TRUE(exactlyEqual(a*d,V({-65, 65}))) << "a*d is not correct" << a*d; //-13+-13 * 3+-2 = [-26 * 5, 0 * 5] = [-130, 0] = (-130)/2 +- (130) /2 = -65 +-65
  EXPECT_TRUE(exactlyEqual(a*d,d*a))          << "* is not kommutative";
  EXPECT_TRUE(exactlyEqual(a+(b+c),(a+b)+c))  << "+ is not asociative";
  EXPECT_TRUE(exactlyEqual(a/d,a))            << "a/d is not correct"; //-13+-13 / 3+-2 = [ -26 / 1, 0 / 1] = [-26, 0] = -26/2 +- --26 / 2 = -13 +-13
}

}}
