#include <MetaAttribute.h>
#include <MetaFactory.h>
#include <ID.h>
#include <ratio>

namespace tests {

namespace metaAttributeSuite {

using namespace ::id::attribute;
using namespace std;

class MetaAttributeSuite : public ::testing::Test {
  protected:
    MetaAttribute test = Position::value();
    MetaAttribute test2 = Position::value();
    MetaFactory&  f    = MetaFactory::instance();
  public:
    MetaAttributeSuite() {
      test.value() = f.create({{{0,1000}, {1000,1000}, {2000,1000}}});
      test.unit()  = Meter();
      test.scale() = ratio<1, 1000>();
      test2.value() = f.create({{{0}, {1000}, {2000}}}, id::type::UInt16::value(), false);
      test2.unit()  = Meter();
      test2.scale() = ratio<1, 1000>();
    }
};

TEST_F(MetaAttributeSuite, scaleTest) {
  MetaAttribute scaled = Position::value();
  MetaAttribute ref = Position::value();

  scaled = test * kilo();
  ref = test;

  ref.value() = f.create({{{0,1}, {1,1}, {2,1}}});
  ref.scale() = ratio<1,1>();
  
  EXPECT_EQ(scaled, ref);
  /**\todo test for exact equality */
}
TEST_F(MetaAttributeSuite, certainscaleTest) {
  MetaAttribute scaled = Position::value();
  MetaAttribute ref = Position::value();

  scaled = test2 * kilo();
  ref = test2;

  ref.value() = f.create({{{0}, {1}, {2}}}, id::type::UInt16::value(), false);
  ref.scale() = ratio<1,1>();
  
  EXPECT_EQ(scaled, ref);
  /**\todo test for exact equality */
}
	
}}
