
#include <MetaEvent.h>
#include <MetaAttribute.h>
#include <MetaValue.h>
#include <MetaFactory.h>
#include <DeSerializer.h>

namespace tests {

namespace metaDeSerializationSuite {

using namespace std;
using namespace ::id::type;
using namespace ::id::attribute;

class MetaDeSerializationSuite : public ::testing::Test {
  public:
    using Buffer = vector<uint8_t>;
    using Iter   = Buffer::const_iterator;
    using D      = DeSerializer<Iter>;
    Buffer buffer;
    MetaFactory& f = MetaFactory::instance();
};

TEST_F(MetaDeSerializationSuite, 1value8Test) {
  MetaValue v = f.create(ValueType(UInt8::value(), 1, 1, true));
  MetaValue comp = f.create({{{1, 2}}}, UInt8::value());
  buffer = {1, 2};
  D d(buffer.begin(), buffer.end());
  d >> v;
  EXPECT_EQ(v, comp);
}

TEST_F(MetaDeSerializationSuite, 2value8Test) {
  MetaValue v = f.create(ValueType(UInt8::value(), 1, 2, true));
  MetaValue comp = f.create({{{1,2}, {2,1}}}, UInt8::value());
  buffer = {1, 2, 2, 1};
  D d(buffer.begin(), buffer.end());
  d >> v;
  EXPECT_EQ(v, comp);
}

TEST_F(MetaDeSerializationSuite, 4value8Test) {
  MetaValue v = f.create(ValueType(UInt8::value(), 2, 2, true));
  MetaValue comp = f.create({{{1,2}, {2,1}},{{3,1}, {3,2}}}, UInt8::value());
  buffer = {1, 2, 3,1, 2, 1, 3, 2};
  D d(buffer.begin(), buffer.end());
  d >> v;
  EXPECT_EQ(v, comp);
}

/** \todo More Test Cases **/

TEST_F(MetaDeSerializationSuite, attributeTest) {
  MetaAttribute ref(Position::value());
  ref.value() = f.create({{{1,2}}}, UInt8::value());
  ref.unit() = Meter();
  ref.scale() = ratio<1, 1000>();
  MetaAttribute attr = ref;
  attr.value() = f.create(ValueType(UInt8::value(), 1, 1, true));
  buffer ={1,2};
  D d(buffer.begin(), buffer.end());
  d >> attr;
  EXPECT_EQ(attr, ref);
}

TEST_F(MetaDeSerializationSuite, eventTest) {
  MetaEvent ref;
  ref.add(MetaAttribute(Position::value()));
  MetaAttribute* attrPtr = ref.attribute(Position::value());
  ASSERT_NE(attrPtr, nullptr) << "The specified attribute is not contained in the event";
  attrPtr->value() = f.create({{{1,2}}}, UInt8::value());
  MetaEvent e = ref;
  attrPtr = e.attribute(Position::value());
  attrPtr->value() = f.create(ValueType(UInt8::value(), 1, 1, true));
  buffer={1,2};
  D d(buffer.begin(), buffer.end());
  d >> e;
  EXPECT_EQ(e, ref);
}

}}
