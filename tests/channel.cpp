#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Channel.h>

namespace test {

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

struct ChannelTestSuite : public ::testing::Test{
  using Events     = Transformer::Events;
  using EventTypes = Transformer::EventTypes;
  using TransPtr   = Transformation::TransPtr;

  struct TestTransformer : public SimpleTransformer {
    TestTransformer() : SimpleTransformer(EventType(), EventType()) {}
    MOCK_CONST_METHOD1(check, bool(const MetaEvent&));
    MOCK_CONST_METHOD1(execute, MetaEvent(const MetaEvent&));
    MOCK_CONST_METHOD1(print, void(std::ostream&));
  };

  struct TestChannel : public Channel {
    TestChannel() : Channel(TransPtr(new TestTransformer())) { }

    MOCK_CONST_METHOD1(publishEvent, void(const MetaEvent& e));
    void handleEvent() { Channel::handleEvent(MetaEvent()); }
    Transformer* trans() { return mTrans.get(); }
  } c;
};

TEST_F(ChannelTestSuite, failedSingleTransformTest) {
  ASSERT_TRUE(c.trans());
	EXPECT_CALL(dynamic_cast<TestTransformer&>(*c.trans()), execute(_))
		.Times(0);
  EXPECT_CALL(dynamic_cast<const TestTransformer&>(*c.trans()), check(_))
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
  ASSERT_TRUE(c.trans());
	EXPECT_CALL(dynamic_cast<TestTransformer&>(*c.trans()), execute(_))
		.Times(1)
		.WillRepeatedly(Return(e));
  EXPECT_CALL(dynamic_cast<const TestTransformer&>(*c.trans()), check(_))
    .Times(AtLeast(1))
    .WillRepeatedly(Return(true));
  EXPECT_CALL(c, publishEvent(_))
    .Times(1);
  c.handleEvent();
}

}
