#include <MetaEvent.h>
#include <MetaAttribute.h>
#include <MetaValue.h>
#include <MetaFactory.h>
#include <Serializer.h>
#include <iterator>

namespace tests {

namespace metaSerializationSuite {

using namespace std;
using namespace ::id::type;
using namespace ::id::attribute;

class MetaSerializationSuite : public ::testing::Test {
  public:
    using Buffer = vector<uint8_t>;
    using Iter   = back_insert_iterator<Buffer>;
    using S      = Serializer<Iter>;
    Buffer buffer;
    S s;
    MetaFactory& f = MetaFactory::instance();
    MetaSerializationSuite() :
     s(back_inserter(buffer))
    {
    }
    void check(const Buffer& b) {
      ASSERT_EQ(buffer.size(), b.size()) << "Serialization size does not match expected size";
      EXPECT_EQ(buffer, b) << "Serialization content does not match expected content";
    }
};

TEST_F(MetaSerializationSuite, 1value8Test) {
  MetaValue v = f.create({{{0,0}}}, UInt8::value());
  Buffer comp={0,0};
  s << v;
  check(comp);
}

TEST_F(MetaSerializationSuite, 2value8Test) {
  MetaValue v = f.create({{{0,0},{1,0}}}, UInt8::value());
  Buffer comp={0,0,1,0};
  s << v;
  check(comp);
}

TEST_F(MetaSerializationSuite, 4value8Test) {
  MetaValue v = f.create({{{0,0},{1,0}},{{2,0},{3,0}}}, UInt8::value());
  Buffer comp={0,0,2,0,1,0,3,0};
  s << v;
  check(comp);
}

TEST_F(MetaSerializationSuite, 1value64Test) {
  MetaValue v = f.create({{{1024,2048}}}, UInt64::value());
  Buffer comp={0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  s << v;
  check(comp);
}

/** \todo More Test Cases **/

TEST_F(MetaSerializationSuite, attributeTest) {
  MetaAttribute attr(Position::value());
  attr.value() = f.create({{{0,0}}}, UInt8::value());
  attr.unit() = Meter();
  attr.scale() = ratio<1, 1000>();
  Buffer comp={0,0};
  s << attr;
  check(comp);
}

TEST_F(MetaSerializationSuite, eventTest) {
  MetaEvent e;
  e.add(MetaAttribute(Position::value()));
  MetaAttribute* attrPtr = e.attribute(Position::value());
  ASSERT_NE(attrPtr, nullptr) << "The specified attribute is not contained in the event";
  attrPtr->value() = f.create({{{0,0}}}, UInt8::value());
  Buffer comp={0,0};
  s << e;
  check(comp);
}

}}
