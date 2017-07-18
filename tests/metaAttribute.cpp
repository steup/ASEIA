
#include <gtest/gtest.h>
#include <MetaAttribute.h>
#include <ID.h>
#include <ratio>

namespace tests {

namespace metaAttributeSuite {

using namespace ::id::attribute;
using namespace ::id::type;
using namespace std;

class MetaAttributeSuite : public ::testing::Test {
  protected:
    MetaAttribute a = Position::value();
    MetaAttribute b = Position::value();
  public:
    MetaAttributeSuite() {
      a.value() = {{{0,1000}, {1000,1000}, {2000,1000}}};
      a.unit()  = Meter();
      a.scale() = Scale<std::milli>();
      b.value() = MetaValue({{{1}, {10}, {20}}}, UInt16(), 1, 3, false);
      b.unit()  = Meter();
      b.scale() = Scale<std::deci>();
    }
};

TEST_F(MetaAttributeSuite, scaleTest) {
  MetaAttribute scaled = Position::value();
  MetaAttribute ref = Position::value();

  scaled = a * Scale<kilo>();
  ref = a;

  ref.value() = {{{0,1}, {1,1}, {2,1}}};
  ref.scale() = Scale<>();

  EXPECT_EQ(scaled, ref);
  /**\todo test for exact equality */
}
TEST_F(MetaAttributeSuite, certainscaleTest) {
  MetaAttribute scaled = Position::value();
  MetaAttribute ref = Position::value();

  scaled = b / Scale<deca>();
  ref = b;

  ref.value() = MetaValue({{{10}, {100}, {200}}}, UInt16());
  ref.scale() = Scale<centi>();
  EXPECT_EQ(scaled, ref);
  /**\todo test for exact equality */
}

TEST_F(MetaAttributeSuite, operatorTest) {
  MetaAttribute resPlus = Position::value();
  resPlus.value() = {{{0.1, 1000}, {2000, 1000}, {4000, 1000}}};
  resPlus.scale() = Scale<std::milli>();
  resPlus.unit() = Meter();
  EXPECT_EQ(a+b, resPlus);
  MetaAttribute resMinus = Position::value();
  resMinus.value() = {{{-0.1,1000}, {0, 1000}, {0, 1000}}};
  resMinus.scale() = Scale<std::milli>();
  resMinus.unit() = Meter();
  EXPECT_EQ(a-b, resMinus);
  MetaAttribute resTimes = Position::value();
  resTimes.value() = {{{1000,1000}, {10000, 1000}, {40000, 1000}}};
  resTimes.scale() = Scale<std::ratio<1, 10000>>();
  resTimes.unit() = Meter();
  EXPECT_EQ(a*b, resTimes);
  MetaAttribute resQuot = Position::value();
  resQuot.value() = {{{0,1000}, {100, 1000}, {100, 1000}}};
  resQuot.scale() = Scale<std::centi>();
  resQuot.unit() = Meter();
  EXPECT_EQ(a/b, resQuot);
}

TEST_F(MetaAttributeSuite, compTest) {
  EXPECT_EQ(a<b, MetaValue({{{false}, {true}, {true}}}, Bool()));
  EXPECT_EQ(a>b, MetaValue({{{false}, {false}, {false}}}, Bool()));
  EXPECT_EQ(a<=b, MetaValue({{{true}, {true}, {true}}}, Bool()));
  EXPECT_EQ(a>=b, MetaValue({{{true}, {false}, {false}}}, Bool()));
  EXPECT_EQ(a==b, MetaValue({{{true}, {false}, {false}}}, Bool()));
  EXPECT_EQ(a!=b, MetaValue({{{false}, {true}, {true}}}, Bool()));
}

}}
