
#include <gtest/gtest.h>
#include <MetaEvent.h>
#include <MetaAttribute.h>
#include <MetaValue.h>
#include <MetaFactory.h>
#include <DeSerializer.h>
#include <Serializer.h>
#include <Value.h>
#include <BaseEvent.h>

#include <iterator>

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
    using S      = Serializer<std::back_insert_iterator<Buffer>>;
    Buffer buffer;
    MetaFactory& f = MetaFactory::instance();

    bool compare(const MetaValue& a, const MetaValue& b){
      if(a.rows() != b.rows())
        return false;
      if(a.cols() != b.cols())
        return false;
      for(size_t i = 0; i < a.rows(); i++)
        for(size_t j = 0; j< a.cols(); j++) {
          if(a.get(i,j).value() != b.get(i,j).value())
            return false;
          if(a.get(i,j).uncertainty() != b.get(i,j).uncertainty())
            return false;
        }
      return true;
    }

    bool compare(const MetaAttribute& a, const MetaAttribute& b){
      if(!(a.scale() == b.scale()))
        return false;
      if(!(a.unit() == b.unit()))
        return false;
      if(a.id() != b.id())
        return false;
      if((ValueType)a.value()!=(ValueType)b.value())
        return false;
      return compare(a.value(), b.value());
    }

    bool compare(const MetaEvent& a, const MetaEvent& b){
      for(const MetaAttribute& attr : a) {
        auto bPtr = b.attribute(attr.id());
        if(!bPtr)
          return false;
        if(!compare(attr, *bPtr))
          return false;
      }
      return true;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
    template<typename T>
    void test(T v, T u) {
      MetaValue test = f.create(ValueType(t2Type<T>::type::value(), 1, 1, true));
      MetaValue comp = f.create({{{v, u}}}, t2Type<T>::type::value());
      buffer.resize(2*sizeof(T));
      reinterpret_cast<T*>(buffer.data())[0] = v;
      reinterpret_cast<T*>(buffer.data())[1] = u;
      D d(buffer.begin(), buffer.end());
      d >> test;
      EXPECT_FALSE(d.error()) <<  "Error occured during deserialization";
      EXPECT_TRUE(compare(test, comp)) << test << " != " <<  comp;
    }

    template<typename T>
    void test(T v0, T u0, T v1, T u1) {
      MetaValue test = f.create(ValueType(t2Type<T>::type::value(), 1, 2, true));
      MetaValue comp = f.create({{{v0, u0}, {v1, u1}}}, t2Type<T>::type::value());
      buffer.resize(4*sizeof(T));
      reinterpret_cast<T*>(buffer.data())[0] = v0;
      reinterpret_cast<T*>(buffer.data())[1] = u0;
      reinterpret_cast<T*>(buffer.data())[2] = v1;
      reinterpret_cast<T*>(buffer.data())[3] = u1;
      D d(buffer.begin(), buffer.end());
      d >> test;
      EXPECT_FALSE(d.error()) <<  "Error occured during deserialization";
      EXPECT_TRUE(compare(test, comp)) << test << " != " <<  comp;
    }

    template<typename T>
    void test(T v0, T u0, T v1, T u1, T v2, T u2, T v3, T u3) {
      MetaValue test = f.create(ValueType(t2Type<T>::type::value(), 2, 2, true));
      MetaValue comp = f.create({{{v0, u0}, {v2, u2}}, {{v1, u1}, {v3, u3}}}, t2Type<T>::type::value());
      buffer.resize(8*sizeof(T));
      reinterpret_cast<T*>(buffer.data())[0] = v0;
      reinterpret_cast<T*>(buffer.data())[1] = u0;
      reinterpret_cast<T*>(buffer.data())[2] = v1;
      reinterpret_cast<T*>(buffer.data())[3] = u1;
      reinterpret_cast<T*>(buffer.data())[4] = v2;
      reinterpret_cast<T*>(buffer.data())[5] = u2;
      reinterpret_cast<T*>(buffer.data())[6] = v3;
      reinterpret_cast<T*>(buffer.data())[7] = u3;
      D d(buffer.begin(), buffer.end());
      d >> test;
      EXPECT_FALSE(d.error()) <<  "Error occured during deserialization";
      EXPECT_TRUE(compare(test, comp)) << test << " != " <<  comp;
    }
};

#pragma GCC diagnostic pop

TEST_F(MetaDeSerializationSuite, 1value8Test) {
  uint8_t v=1,u=2;
  test(v,u);
}

TEST_F(MetaDeSerializationSuite, 1value32Test) {
  uint32_t v=1024,u=2048;
  test(v,u);
}

TEST_F(MetaDeSerializationSuite, 2value8Test) {
  uint8_t v0=1, u0=2, v1=2, u1=1;
  test(v0, u0, v1, u1);
}

TEST_F(MetaDeSerializationSuite, 4value8Test) {
  uint8_t v0=1,u0=2, v1=2,u1=1,v2=3,u2=1, v3=3, u3=2;
  test(v0, u0, v1, u1, v2, u2, v3, u3);
}

/** \todo More Test Cases **/

TEST_F(MetaDeSerializationSuite, attributeTest) {
  MetaAttribute ref(Position::value());
  ref.value() = f.create({{{1,2}}}, UInt8::value());
  ref.unit() = Meter();
  ref.scale() = Scale<std::milli>();
  MetaAttribute attr = ref;
  attr.value() = f.create(ValueType(UInt8::value(), 1, 1, true));
  buffer ={1,2};
  D d(buffer.begin(), buffer.end());
  d >> attr;
  EXPECT_TRUE(compare(attr, ref)) << attr << " != " << ref;
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
  EXPECT_TRUE(compare(e, ref)) << e << " != " << ref;
}

TEST_F(MetaDeSerializationSuite, simpleNetworkTest) {
  struct EventConfig : public BaseConfig{
    using PositionValueType    = Value<uint8_t, 1, 1, true>;
    using TimeValueType        = Value<uint8_t, 1, 1, true>;
    using PublisherIDValueType = Value<uint8_t, 1, 1, true>;
  };
	BaseEvent<EventConfig> e;
  e.attribute(Position()).value() = {{{0U, 0U}}};
  e.attribute(Time()).value() = {{{0U, 0U}}};
  e.attribute(PublisherID()).value() = {{{0U, 0U}}};
	EventType eT=(EventType)e;
	MetaEvent ref(eT);
	MetaEvent mE(eT);
	MetaAttribute* posPtr  = ref.attribute(Position::value());
	MetaAttribute* timePtr = ref.attribute(Time::value());
	MetaAttribute* pubPtr  = ref.attribute(PublisherID::value());
	ASSERT_TRUE(posPtr)  << "No position attribute in BaseEvent<>";
	ASSERT_TRUE(timePtr) << "No time attribute in BaseEvent<>";
	ASSERT_TRUE(pubPtr)  << "No publisher id in BaseEvent<>";
  posPtr->value() = f.create({{{0U, 0U}}}, UInt8::value());
  timePtr->value() = f.create({{{0U, 0U}}}, UInt8::value());
  pubPtr->value() = f.create({{{0U, 0U}}}, UInt8::value());
	EXPECT_EQ(eT, (EventType)ref) << "Referenz MetaEvent does not fit StaticEvent!";
	S s(back_inserter(buffer));
	s << e;
  EXPECT_EQ(buffer.size(), e.size());
  D d(buffer.begin(), buffer.end());
  d >> mE;
  EXPECT_TRUE(compare(mE, ref)) << mE << " != " << ref;
}

TEST_F(MetaDeSerializationSuite, complexNetworkTest) {
	BaseEvent<> e;
  e.attribute(Position()).value() = {{{0.0, 1.0}}, {{2.0, 3.0}}, {{4.0, 5.0}}};
  e.attribute(Time()).value() = {{{128U, 256U}}};
  e.attribute(PublisherID()).value() = {{{1024U}}};
	EventType eT=(EventType)e;
	MetaEvent ref = f.create(eT);
	MetaEvent mE  = f.create(eT);
	MetaAttribute* posPtr  = ref.attribute(Position::value());
	MetaAttribute* timePtr = ref.attribute(Time::value());
	MetaAttribute* pubPtr  = ref.attribute(PublisherID::value());
	ASSERT_TRUE(posPtr)  << "No position attribute in BaseEvent<>";
	ASSERT_TRUE(timePtr) << "No time attribute in BaseEvent<>";
	ASSERT_TRUE(pubPtr)  << "No publisher id in BaseEvent<>";
  posPtr->value() = f.create({{{0.0, 1.0}}, {{2.0, 3.0}}, {{4.0, 5.0}}}, Float::value());
  timePtr->value() = f.create({{{128U, 256U}}}, UInt32::value());
  pubPtr->value() = f.create({{{1024U}}}, UInt32::value(), 1, 1, false);
	EXPECT_EQ(eT, (EventType)ref) << "Referenz MetaEvent does not fit StaticEvent!";
	S s(back_inserter(buffer));
	s << e;
  EXPECT_EQ(buffer.size(), e.size());
  D d(buffer.begin(), buffer.end());
  d >> mE;
  EXPECT_TRUE(compare(mE, ref)) << mE << " != " << ref;
}
}}
