
#include <gtest/gtest.h>
#include <MetaEvent.h>
#include <MetaFactory.h>
#include <ID.h>
#include <IO.h>
#include <Scale.h>
#include <BaseEvent.h>

namespace tests {

namespace metaEventSuite {

using namespace ::id::attribute;
using namespace ::id::type;
using namespace std;

	class MetaEventSuite : public ::testing::Test {
    private:
      using Config = BaseConfig;
		public:
      EventType eT;
      MetaEvent a, b, c;
			MetaEventSuite() : eT(BaseEvent<Config>()), a(eT), b(eT), c(eT) {
				a[Position()].value()    = MetaValue({{{0,1}}, {{5,1}}, {{10,1}}}, Float());
				a[Time()].value()        = MetaValue({{{123,1}}}, UInt32());
				a[PublisherID()].value() = MetaValue({{{1234}}}, UInt32(), 1, 1, false);
				b[Position()].value()    = MetaValue({{{10,1}}, {{5,1}}, {{0,1}}}, Float());
				b[Time()].value()        = MetaValue({{{124,1}}}, UInt32());
				b[PublisherID()].value() = MetaValue({{{1230}}}, UInt32(), 1, 1, false);
				c[Position()].value()    = MetaValue({{{5,1}}, {{5,1}}, {{5,1}}}, Float());
				c[Time()].value()        = MetaValue({{{123,1}}}, UInt32());
				c[PublisherID()].value() = MetaValue({{{1232}}}, UInt32(), 1, 1, false);
			}
	};

	TEST_F(MetaEventSuite, attributeAccessTest) {
		MetaAttribute posTest = MetaAttribute(eT[Position()]);
		MetaAttribute timeTest = MetaAttribute(eT[Time()]);
		MetaAttribute pubTest = MetaAttribute(eT[PublisherID()]);
		posTest.value()  = {{{0,1}}, {{5,1}}, {{10,1}}};
		timeTest.value() = {{{123,1}}};
		pubTest.value()  = {{{1234}}};
    const MetaAttribute* posPtr = a.attribute(Position::value());
    ASSERT_NE(posPtr, nullptr) << "Position attribute could not be retrieved";
    const MetaAttribute& pos = *posPtr;
		EXPECT_EQ(pos, posTest) << "Position attribute not stored correctly";
		EXPECT_EQ(a[Position()], posTest) << "Shorthand extraction failed";
    const MetaAttribute* timePtr = a.attribute(Time::value());
    ASSERT_NE(timePtr, nullptr) << "Time attribute could not be retrieved";
    const MetaAttribute& time = *timePtr;
		EXPECT_EQ(time, timeTest) << "Time attribute not stored correctly";
		EXPECT_EQ(a[Time()], timeTest) << "Time: Shorthand extraction failed";
    const MetaAttribute* pubPtr = a.attribute(PublisherID::value());
    ASSERT_NE(pubPtr, nullptr) << "PublisherID attribute could not be retrieved";
    const MetaAttribute& pub = *pubPtr;
		EXPECT_EQ(pub, pubTest) << "PublisherID attribute not stored correctly";
		EXPECT_EQ(a[PublisherID()], pubTest) << "PublisherID: shorthand extraction failed";
    EXPECT_EQ(a.attribute(Object::value()), nullptr) << "Object attribute wrongly considered contained";
	}

	TEST_F(MetaEventSuite, typeTest) {
		EXPECT_EQ((EventType)a, eT) << "Inferred MetaEvent-Type is wrong: " << endl << ((EventType)a) << endl << "<->" << endl << eT;
	}

  TEST_F(MetaEventSuite, operationTest) {
    MetaEvent res = (a+b)/2;
   EXPECT_EQ(res, c) << "Average of Events is not valid (\n" << a << "\n+\n"  << b << ")/2 ==\n" << c;
   EXPECT_NE(res, a) << "Average of Events is not valid (\n" << a << "\n+\n"  << b << ")/2 !=\n" << a;
   EXPECT_NE(res, b) << "Average of Events is not valid (\n" << a << "\n+\n"  << b << ")/2 !=\n" << b;
  }
}}
