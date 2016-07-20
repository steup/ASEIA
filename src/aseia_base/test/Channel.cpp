#include <Channel.h>

namespace test {

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

struct ChannelTestSuite : public ::testing::Test{
  using EventTypes = Transformation::EventTypes;
  using TransID = TransformationFactory::TransID;
  TransID sET;
  EventType out;
  EventType in;
  EventTypes inList = { &in };

  struct TestChannel : public Channel {
    TestChannel(const EventType& out, const EventTypes& in, TransID trans)
      : Channel(out, in, trans)
    { }

    MOCK_CONST_METHOD1(publishEvent, void(const MetaEvent& e));
    void handleEvent() { Channel::handleEvent(MetaEvent()); }
    Transformation* trans() { return mTrans.get(); }
  };

  struct SingleEventTransform : public Transformation {
    SingleEventTransform(const EventType& out, const EventTypes& inList) {
      mValid = true;
    }
    MOCK_CONST_METHOD1(check, bool(const Events& events));
    MOCK_METHOD1(call, MetaEvent(const Events& events));
    virtual MetaEvent operator()(const Events& events) { return call(events); }
    MOCK_CONST_METHOD1(print, void(std::ostream& o));
  };

  ChannelTestSuite() {
    sET = TransformationFactory::instance().registerTransformation<SingleEventTransform>();
  }
};

TEST_F(ChannelTestSuite, failedSingleTransformTest) {
  TestChannel c(out, inList, sET);
  ASSERT_TRUE(c.trans());
	EXPECT_CALL(dynamic_cast<SingleEventTransform&>(*c.trans()), call(_))
		.Times(0);
  EXPECT_CALL(dynamic_cast<const SingleEventTransform&>(*c.trans()), check(_))
    .Times(AtLeast(1))
    .WillRepeatedly(Return(false));
  EXPECT_CALL(c, publishEvent(_))
    .Times(0);
  c.handleEvent();
}

TEST_F(ChannelTestSuite, succededTransformTest) {
	MetaEvent e;
	MetaAttribute a;
	e.add(a);
  TestChannel c(out, inList, sET);
  ASSERT_TRUE(c.trans());
	EXPECT_CALL(dynamic_cast<SingleEventTransform&>(*c.trans()), call(_))
		.Times(1)
		.WillRepeatedly(Return(e));
  EXPECT_CALL(dynamic_cast<const SingleEventTransform&>(*c.trans()), check(_))
    .Times(AtLeast(1))
    .WillRepeatedly(Return(true));
  EXPECT_CALL(c, publishEvent(_))
    .Times(1);
  c.handleEvent();
}

}
