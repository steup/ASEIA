#include <Attribute.h>
#include <ID.h>

namespace tests {

namespace attributeTestSuite {

using ::testing::StaticAssertTypeEq;

template<typename T, int32_t rows, int32_t cols, bool U>
void exactlyEqual(const Value<T, rows, cols, U>& a, const Value<T, rows, cols, U>& b) {
  EXPECT_EQ(a.rows(), b.rows()) << "different number of rows";
  EXPECT_EQ(a.cols(), b.cols()) << "different number of cols";
  for(unsigned int i = 0; i < a.rows(); i++)
    for(unsigned int j = 0; j < a.cols(); j++) {
      EXPECT_NEAR(a(i, j).value()     , b(i,j).value()      , 0.000001f) 
        << " at value of element (" << i << ", " << j << ")";
      EXPECT_NEAR(a(i,j).uncertainty(), b(i,j).uncertainty(), 0.000001f)
        << " at uncertainty of element (" << i << ", " << j << ")";
    }
}

TEST(AttributeTestSuite, scaleTest) {
   using AttrID       = ::id::attribute::Position;
   using AttrValue    = Value<float, 3, 1, true>;
   using AttrUnit     = Meter;
   using AttrScale    = std::milli;
   using AttrNewScale = std::ratio<1,1>;
   using Attr         = Attribute<AttrID, AttrValue, AttrUnit, AttrScale>;
   using NewAttr      = Attribute<AttrID, AttrValue, AttrUnit, AttrNewScale>;
   Attr attr({{{0, 1000}}, {{1000, 1000}}, {{2000, 1000}}});
   NewAttr newAttr({{{0, 1}}, {{1, 1}}, {{2, 1}}});

   auto result = attr * std::kilo();
   StaticAssertTypeEq<decltype(result), NewAttr>();
   exactlyEqual(result.value(), newAttr.value());
}

}}
