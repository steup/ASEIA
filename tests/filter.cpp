
#include <gtest/gtest.h>
#include <BaseEvent.h>
#include <Filter.h>
#include <MetaFilter.h>
#include <MetaEvent.h>

#include <vector>
#include <algorithm>
#include <sstream>

namespace tests {

namespace filterTestSuite {

using namespace ::id::attribute;
using namespace filter;

class FilterTestSuite : public ::testing::Test {
  public:
    using Event = BaseEvent<>;
    Event trueEvent, falseEvent, compEvent;
    EventType eventType = (EventType)trueEvent;
    using TimeAttr = Event::findAttribute<Time>::type;
    const TimeAttr c0 = {{{ 1024 }}};
    FilterTestSuite() {
      trueEvent.attribute(Time()).value()  = {{{1050}}};
      compEvent.attribute(Time())  = c0;
      falseEvent.attribute(Time()).value() = {{{100}}};
    }
};

TEST_F(FilterTestSuite, basicExpressionTest){
  auto filter0 = e0[Time()] > c0;
  auto filter1 = e0[Time()] < c0;
  auto filter2 = e0[Time()] == c0;
  auto filter3 = e0[Time()] != c0;
  auto filter4 = e0[Time()] >= c0;
  auto filter5 = e0[Time()] <= c0;

  EXPECT_TRUE(filter0(trueEvent))  << "False negative";
  EXPECT_FALSE(filter0(falseEvent)) << "False positive";
  EXPECT_TRUE(filter1(falseEvent))  << "False negative";
  EXPECT_FALSE(filter1(trueEvent)) << "False positive";
  EXPECT_FALSE(filter2(falseEvent))  << "False positive";
  EXPECT_FALSE(filter2(trueEvent)) << "False positive";
  EXPECT_TRUE(filter3(trueEvent))  << "False negative";
  EXPECT_TRUE(filter3(falseEvent)) << "False negative";
  EXPECT_TRUE(filter4(trueEvent))  << "False negative";
  EXPECT_FALSE(filter4(falseEvent)) << "False positive";
  EXPECT_TRUE(filter5(falseEvent))  << "False negative";
  EXPECT_FALSE(filter5(trueEvent)) << "False positive";
}

TEST_F(FilterTestSuite, extendedExpressionTest){
  auto filter0 = e0[Time()] > e1[Time()];
  auto filter1 = e0[Time()] < e1[Time()];
  auto filter2 = e0[Time()] == e1[Time()];
  auto filter3 = e0[Time()] != e1[Time()];
  auto filter4 = e0[Time()] >= e1[Time()];
  auto filter5 = e0[Time()] <= e1[Time()];

  EXPECT_TRUE(filter0(trueEvent, compEvent))  << "False negative";
  EXPECT_FALSE(filter0(falseEvent, compEvent)) << "False positive";
  EXPECT_TRUE(filter1(falseEvent, compEvent))  << "False negative";
  EXPECT_FALSE(filter1(trueEvent, compEvent)) << "False positive";
  EXPECT_FALSE(filter2(falseEvent, compEvent))  << "False positive";
  EXPECT_FALSE(filter2(trueEvent, compEvent)) << "False positive";
  EXPECT_TRUE(filter3(trueEvent, compEvent))  << "False negative";
  EXPECT_TRUE(filter3(falseEvent, compEvent)) << "False negative";
  EXPECT_TRUE(filter4(trueEvent, compEvent))  << "False negative";
  EXPECT_FALSE(filter4(falseEvent, compEvent)) << "False positive";
  EXPECT_TRUE(filter5(falseEvent, compEvent))  << "False negative";
  EXPECT_FALSE(filter5(trueEvent, compEvent)) << "False positive";
}

TEST_F(FilterTestSuite, compositeExpressionTest){
  auto baseFilter = e0[Time()] > c0;
  auto filter0 = baseFilter(e0) && !baseFilter(e1);
  auto filter1 = baseFilter(e0) || baseFilter(e1);

  EXPECT_TRUE(filter0(trueEvent, falseEvent))  << "False negative";
  EXPECT_FALSE(filter0(falseEvent, falseEvent)) << "False positive";
  EXPECT_TRUE(filter1(trueEvent, falseEvent))  << "False negative";
  EXPECT_FALSE(filter1(falseEvent, falseEvent)) << "False positive";
}

TEST_F(FilterTestSuite, basicSerializationTest) {

  auto filter0 = e0[Time()] > c0;
  auto filter1 = e0[Time()] < c0;
  auto filter2 = e0[Time()] >= c0;
  auto filter3 = e0[Time()] <= c0;
  auto filter4 = e0[Time()] == c0;
  auto filter5 = e0[Time()] != c0;

	uint8_t time = ::id::attribute::Time::value();
	uint8_t gt = 0x80|::id::filterOp::GT::value;
	uint8_t lt = 0x80|::id::filterOp::LT::value;
	uint8_t ge = 0x80|::id::filterOp::GE::value;
	uint8_t le = 0x80|::id::filterOp::LE::value;
	uint8_t eq = 0x80|::id::filterOp::EQ::value;
	uint8_t ne = 0x80|::id::filterOp::NE::value;

  std::vector<uint8_t> buffer;
  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);

	uint16_t count = 0;

  s << filter0(s0);
	EXPECT_EQ(buffer[count++], time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], 0) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], gt) << "Operation > with constant argument serialized wrongly";

	count+=2*sizeof(uint32_t);
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	count+=sizeof(EventPlaceholder);
  s << filter1(s0);
	EXPECT_EQ(buffer[count++], lt) << "Operation < with constant argument serialized wrongly";

	count+=2*sizeof(uint32_t);
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	count+=sizeof(EventPlaceholder);

	s << filter2(s0);
	EXPECT_EQ(buffer[count++], ge) << "Operation >= with constant argument serialized wrongly";

	count+=2*sizeof(uint32_t);
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	count+=sizeof(EventPlaceholder);

	s << filter3(s0);
	EXPECT_EQ(buffer[count++], le) << "Operation <= with constant argument serialized wrongly";
  count+=2*sizeof(uint32_t);

	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	count+=sizeof(EventPlaceholder);

	s << filter4(s0);
	EXPECT_EQ(buffer[count++], eq) << "Operation == with constant argument serialized wrongly";

	count+=2*sizeof(uint32_t);
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	count+=sizeof(EventPlaceholder);

	s << filter5(s0);
	EXPECT_EQ(buffer[count++], ne) << "Operation != with constant argument serialized wrongly";
	count+=2*sizeof(uint32_t);
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

}

TEST_F(FilterTestSuite, extendedSerializationTest) {
  auto filter0 = e0[Time()] >  e1[Time()];
  auto filter1 = e0[Time()] <  e1[Time()];
  auto filter2 = e0[Time()] >= e1[Time()];
  auto filter3 = e0[Time()] <= e1[Time()];
  auto filter4 = e0[Time()] == e1[Time()];
  auto filter5 = e0[Time()] != e1[Time()];

	uint8_t time = ::id::attribute::Time::value();
	uint8_t gt = ::id::filterOp::GT::value;
	uint8_t lt = ::id::filterOp::LT::value;
	uint8_t ge = ::id::filterOp::GE::value;
	uint8_t le = ::id::filterOp::LE::value;
	uint8_t eq = ::id::filterOp::EQ::value;
	uint8_t ne = ::id::filterOp::NE::value;

  std::vector<uint8_t> buffer;
  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);
  FilterEvent<decltype(s)> s1(1, s);

	uint16_t count = 0;
  s << filter0(s0, s1);
	EXPECT_EQ(buffer[count++], time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], 0)    << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], gt)   << "Operation > with event argument serialized wrongly";
	EXPECT_EQ(buffer[count++], time) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer[count++], 1)    << "Placeholder event 1 time serialized wrongly";

	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

  s << filter1(s0, s1);
	EXPECT_EQ(buffer[count+=sizeof(EventPlaceholder)], lt) << "Operation < with event argument serialized wrongly";

	count+=sizeof(EventPlaceholder)+1;
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	s << filter2(s0, s1);
	EXPECT_EQ(buffer[count+=sizeof(EventPlaceholder)], ge) << "Operation >= with event argument serialized wrongly";

	count+=sizeof(EventPlaceholder)+1;
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	s << filter3(s0, s1);
	EXPECT_EQ(buffer[count+=sizeof(EventPlaceholder)], le) << "Operation <= with event argument serialized wrongly";

	count+=sizeof(EventPlaceholder)+1;
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	s << filter4(s0, s1);
	EXPECT_EQ(buffer[count+=sizeof(EventPlaceholder)], eq) << "Operation == with event argument serialized wrongly";

	count+=sizeof(EventPlaceholder)+1;
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	s << filter5(s0, s1);
	EXPECT_EQ(buffer[count+=sizeof(EventPlaceholder)], ne) << "Operation != with event argument serialized wrongly";

	count+=sizeof(EventPlaceholder)+1;
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
}

TEST_F(FilterTestSuite, compositeSerializationTest) {
  auto baseFilter0 = e0[Position()] != e1[Position()];
  auto baseFilter1 = e0[Time()] > e1[Time()];
  auto filter0 = baseFilter0(e0, e1) && baseFilter1(e0, e1);
  auto filter1 = baseFilter0(e0, e1) || baseFilter1(e0, e1);

	uint8_t time = ::id::attribute::Time::value();
	uint8_t pos  = ::id::attribute::Position::value();
	uint8_t gt   = ::id::filterOp::GT::value;
	uint8_t ne   = ::id::filterOp::NE::value;
	uint8_t lAnd = ::id::filterOp::AND::value;
	uint8_t lOr  = ::id::filterOp::OR::value;


	std::vector<uint8_t> buffer;
  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);
  FilterEvent<decltype(s)> s1(1, s);

  size_t count= 0;
  s << filter0(s0, s1);
	EXPECT_EQ(buffer[count++], pos) << "Placeholder event 0 position serialized wrongly";
	EXPECT_EQ(buffer[count++], 0) << "Placeholder event 0 position serialized wrongly";
	EXPECT_EQ(buffer[count++], ne) << "Operation != with event argument serialized wrongly";
	EXPECT_EQ(buffer[count++], pos) << "Placeholder event 1 position serialized wrongly";
	EXPECT_EQ(buffer[count++], 1) << "Placeholder event 1 position serialized wrongly";
	EXPECT_EQ(buffer[count++], lAnd) << "Operation && serialized wrongly";
	EXPECT_EQ(buffer[count++], time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], 0) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], gt) << "Operation > with event argument serialized wrongly";
	EXPECT_EQ(buffer[count++], time) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer[count++], 1) << "Placeholder event 1 time serialized wrongly";

	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	s << filter1(s0, s1);
	EXPECT_EQ(buffer[count++], pos) << "Placeholder event 0 position serialized wrongly";
	EXPECT_EQ(buffer[count++], 0) << "Placeholder event 0 position serialized wrongly";
	EXPECT_EQ(buffer[count++], ne) << "Operation != with event argument serialized wrongly";
	EXPECT_EQ(buffer[count++], pos) << "Placeholder event 1 position serialized wrongly";
	EXPECT_EQ(buffer[count++], 1) << "Placeholder event 1 position serialized wrongly";
	EXPECT_EQ(buffer[count++], lOr) << "Operation || serialized wrongly";
	EXPECT_EQ(buffer[count++], time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], 0) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count++], gt) << "Operation > with event argument serialized wrongly";
	EXPECT_EQ(buffer[count++], time) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer[count++], 1) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
}

TEST_F(FilterTestSuite, metaFilterBasicTest) {
	uint8_t time = ::id::attribute::Time::value();
	uint8_t gt = ::id::filterOp::GT::value;
	uint8_t lt = ::id::filterOp::LT::value;
	uint8_t ge = ::id::filterOp::GE::value;
	uint8_t le = ::id::filterOp::LE::value;
	uint8_t eq = ::id::filterOp::EQ::value;
	uint8_t ne = ::id::filterOp::NE::value;
	uint8_t noop = ::id::filterOp::NOOP::value;

	std::vector<uint8_t> buffer({time, 0, gt, time, 1, noop,  time, 0, lt, time, 1, noop, time, 0, eq, time, 1, noop, time, 0, ne, time, 1, noop, time, 0, ge, time, 1, noop, time, 0, le, time, 1, noop});

	DeSerializer<std::vector<uint8_t>::const_iterator> d(buffer.cbegin(), buffer.cend());
	std::vector<const EventType*> types={&eventType, &eventType};
	MetaFilter filter0(types), filter1(types), filter2(types), filter3(types), filter4(types), filter5(types);
	d >> filter0 >> filter1 >> filter2 >> filter3 >> filter4 >> filter5;

  MetaEvent trueMetaEvent(eventType), falseMetaEvent(eventType), compMetaEvent(eventType);

	std::vector<const MetaEvent*> trueEvents = {&trueMetaEvent, &compMetaEvent};
	std::vector<const MetaEvent*> falseEvents= {&falseMetaEvent, &compMetaEvent};

	buffer.clear();
	auto s0 = Serializer<decltype(back_inserter(buffer))>(back_inserter(buffer));
	s0 << compEvent;
	auto d0 = DeSerializer<std::vector<uint8_t>::const_iterator>(buffer.begin(), buffer.end());
	d0 >> compMetaEvent;
	buffer.clear();
	auto s1 = Serializer<decltype(back_inserter(buffer))>(back_inserter(buffer));
	s1 << trueEvent;
	auto d1 = DeSerializer<std::vector<uint8_t>::const_iterator>(buffer.begin(), buffer.end());
	d1 >> trueMetaEvent;
	buffer.clear();
	auto s2 = Serializer<decltype(back_inserter(buffer))>(back_inserter(buffer));
	s2 << falseEvent;
	auto d2 = DeSerializer<std::vector<uint8_t>::const_iterator>(buffer.begin(), buffer.end());
	d2 >> falseMetaEvent;

	EXPECT_TRUE(filter0(trueEvents))   << "False negative:\n" << *trueEvents[0]  <<  filter0 << "\n" << *trueEvents[1] ;
  EXPECT_FALSE(filter0(falseEvents)) << "False positive:\n" << *falseEvents[0] <<  filter0 << "\n" << *falseEvents[1];
  EXPECT_TRUE(filter1(falseEvents))  << "False negative:\n" << *falseEvents[0] <<  filter1 << "\n" << *falseEvents[1];
  EXPECT_FALSE(filter1(trueEvents))  << "False positive:\n" << *trueEvents[0]  <<  filter1 << "\n" << *trueEvents[1] ;
  EXPECT_FALSE(filter2(falseEvents)) << "False positive:\n" << *falseEvents[0] <<  filter2 << "\n" << *falseEvents[1];
  EXPECT_FALSE(filter2(trueEvents))  << "False positive:\n" << *trueEvents[0]  <<  filter2 << "\n" << *trueEvents[1] ;
  EXPECT_TRUE(filter3(trueEvents))   << "False negative:\n" << *trueEvents[0]  <<  filter3 << "\n" << *trueEvents[1] ;
  EXPECT_TRUE(filter3(falseEvents))  << "False negative:\n" << *falseEvents[0] <<  filter3 << "\n" << *falseEvents[1];
  EXPECT_TRUE(filter4(trueEvents))   << "False negative:\n" << *trueEvents[0]  <<  filter4 << "\n" << *trueEvents[1] ;
  EXPECT_FALSE(filter4(falseEvents)) << "False positive:\n" << *falseEvents[0] <<  filter4 << "\n" << *falseEvents[1];
  EXPECT_TRUE(filter5(falseEvents))  << "False negative:\n" << *falseEvents[0] <<  filter5 << "\n" << *falseEvents[1];
  EXPECT_FALSE(filter5(trueEvents))  << "False positive:\n" << *trueEvents[0]  <<  filter5 << "\n" << *trueEvents[1] ;
}
}}
