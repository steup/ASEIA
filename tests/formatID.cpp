#include <FormatID.h>
#include <EventType.h>

namespace tests {

namespace formatIDTestSuite {

using ::id::attribute::Base;
using ::id::type::UInt8;
using ::id::type::UInt16;

TEST(FormatIDTestSuite, ValueCompTest) {
    ValueType vT0(UInt8::value(), 1, 1, false);
    AttributeType aT0(Base::value(), vT0, ScaleType(), Meter());
    EventType eT0;
    eT0.add(aT0);
    FormatID f0(eT0);
    ValueType vT1(UInt8::value(), 1, 1, false);
    AttributeType aT1(Base::value(), vT1, ScaleType(), Meter());
    EventType eT1;
    eT1.add(aT1);
    FormatID f1(eT1);
    ValueType vT2(UInt16::value(), 1, 1, false);
    AttributeType aT2(Base::value(), vT2, ScaleType(), Meter());
    EventType eT2;
    eT2.add(aT2);
    FormatID f2(eT2);
    ValueType vT3(UInt8::value(), 2, 1, false);
    AttributeType aT3(Base::value(), vT3, ScaleType(), Meter());
    EventType eT3;
    eT3.add(aT3);
    FormatID f3(eT3);
    ValueType vT4(UInt8::value(), 1, 2, false);
    AttributeType aT4(Base::value(), vT4, ScaleType(), Meter());
    EventType eT4;
    eT4.add(aT4);
    FormatID f4(eT4);
    ValueType vT5(UInt8::value(), 1, 1, true);
    AttributeType aT5(Base::value(), vT5, ScaleType(), Meter());
    EventType eT5;
    eT5.add(aT5);
    FormatID f5(eT5);
    EXPECT_EQ(f0, f1) << "FormatID of same event types is not the same";
    EXPECT_EQ(eT0-eT1, 0U) << "EventTypes difference of same event types is not 0";
    EXPECT_NE(f0, f2) << "FormatID of events with different value types are the same";
    EXPECT_NE(eT0-eT2, 0U) << "EventTypes difference of different event types is 0";
    EXPECT_NE(f0, f3) << "FormatID of events with different value types are the same";
    EXPECT_NE(eT0-eT3, 0U) << "EventTypes difference of different event types is 0";
    EXPECT_NE(f0, f4) << "FormatID of events with different value types are the same";
    EXPECT_NE(eT0-eT4, 0U) << "EventTypes difference of different event types is 0";
    EXPECT_NE(f0, f5) << "FormatID of events with different value types are the same";
    EXPECT_NE(eT0-eT5, 0U) << "EventTypes difference of different event types is 0";
}

}}
