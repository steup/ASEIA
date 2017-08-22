
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
using std::vector;

class FilterTestSuite : public ::testing::Test {
  public:
    using Event = BaseEvent<>;
    Event trueEvent, falseEvent, compEvent;
    EventType eventType = (EventType)trueEvent;
    using TimeAttr = Event::findAttribute<Time>::type;
    const TimeAttr c0 = {{{ 1024 }}};
    uint8_t time, pos, gt, lt, ge, le, eq, ne, cgt, clt, cge, cle, ceq, cne, nop, lOr, lAnd, unc;
    FilterTestSuite() {
      trueEvent.attribute(Time()).value()  = {{{1050}}};
      trueEvent.attribute(Position()).value()  = {{{0, 10}}, {{0, 10}}, {{0, 10}}};
      compEvent.attribute(Time())  = c0;
      compEvent.attribute(Position()).value()  = {{{0, 5}}, {{0, 5}}, {{0, 5}}};
      falseEvent.attribute(Time()).value() = {{{100}}};
      falseEvent.attribute(Position()).value()  = {{{0, 0}}, {{0, 0}}, {{0, 0}}};
      filter::Op op;
      time = ::id::attribute::Time::value();
      pos  = ::id::attribute::Position::value();
      op.code = ::id::filterOp::GT::value;
      op.constArg = 0;
      op.unary  = 0;
      op.comp = 1;
      gt = op.data;
      op.constArg = 1;
      cgt = op.data;
      op.code = ::id::filterOp::LT();
      op.constArg = 0;
      lt = op.data;
      op.constArg = 1;
      clt = op.data;
      op.constArg = 0;
      op.code = ::id::filterOp::GE();
      ge = op.data;
      op.constArg = 1;
      cge = op.data;
      op.constArg = 0;
      op.code = ::id::filterOp::LE();
      le = op.data;
      op.constArg = 1;
      cle = op.data;
      op.constArg = 0;
      op.code = ::id::filterOp::EQ();
      eq = op.data;
      op.constArg = 1;
      ceq = op.data;
      op.constArg = 0;
      op.code = ::id::filterOp::NE();
      ne = op.data;
      op.constArg = 1;
      cne = op.data;
      op.constArg = 0;
      op.comp = 0;
      op.code = ::id::filterOp::NOP();
      nop = op.data;
      lOr = ::id::filterOp::OR();
      lAnd = ::id::filterOp::AND();
      op.unary=1;
      op.code = ::id::filterOp::UNC();
      unc = op.data;
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

  std::vector<uint8_t> buffer, ref;
  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);

  s << filter0(filter::s0);
  ref = {0, time, cgt};
  Serializer<decltype(i)> sTemp = Serializer<decltype(i)>(back_inserter(ref));
  sTemp << c0 << nop;
	EXPECT_EQ(buffer, ref);
	uint16_t count = buffer.size();

	buffer.clear();
  s << filter1(filter::s0);
	EXPECT_EQ(buffer[2], clt) << "Operation < with constant argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	buffer.clear();
	s << filter2(filter::s0);
	EXPECT_EQ(buffer[2], cge) << "Operation >= with constant argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	buffer.clear();
	s << filter3(filter::s0);
	EXPECT_EQ(buffer[2], cle) << "Operation <= with constant argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	buffer.clear();
	s << filter4(filter::s0);
	EXPECT_EQ(buffer[2], ceq) << "Operation == with constant argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

	buffer.clear();
	s << filter5(filter::s0);
	EXPECT_EQ(buffer[2], cne) << "Operation != with constant argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";

}

TEST_F(FilterTestSuite, extendedSerializationTest) {
  auto filter0 = e0[Time()] >  e1[Time()];
  auto filter1 = e0[Time()] <  e1[Time()];
  auto filter2 = e0[Time()] >= e1[Time()];
  auto filter3 = e0[Time()] <= e1[Time()];
  auto filter4 = e0[Time()] == e1[Time()];
  auto filter5 = e0[Time()] != e1[Time()];

  std::vector<uint8_t> buffer, ref;
  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);

  s << filter0(filter::s0, filter::s1);
  ref = {0, time, gt, 1, time, nop};
	EXPECT_EQ(buffer, ref);
	uint16_t count = buffer.size();
  buffer.clear();

  s << filter1(filter::s0, filter::s1);
	EXPECT_EQ(buffer[2], lt) << "Operation < with event argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	buffer.clear();

	s << filter2(filter::s0, filter::s1);
	EXPECT_EQ(buffer[2], ge) << "Operation >= with event argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	buffer.clear();

	s << filter3(filter::s0, filter::s1);
	EXPECT_EQ(buffer[2], le) << "Operation <= with event argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	buffer.clear();

	s << filter4(filter::s0, filter::s1);
	EXPECT_EQ(buffer[2], eq) << "Operation == with event argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
	buffer.clear();

	s << filter5(filter::s0, filter::s1);
	EXPECT_EQ(buffer[2], ne) << "Operation != with event argument serialized wrongly";
	EXPECT_EQ(buffer.size(), count) << "Wrong length of serialized packet";
}

TEST_F(FilterTestSuite, compositeSerializationTest) {
  auto baseFilter0 = e0[Position()] != e1[Position()];
  auto baseFilter1 = e0[Time()] > e1[Time()];
  auto filter0 = baseFilter0(e0, e1) && baseFilter1(e0, e1);
  auto filter1 = baseFilter0(e0, e1) || baseFilter1(e0, e1);

	std::vector<uint8_t> buffer, ref;
  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);

  s << filter0(filter::s0, filter::s1);
  ref={0, pos, ne, 1, pos, lAnd, 0, time, gt, 1, time, nop};
	EXPECT_EQ(buffer, ref);

  buffer.clear();

	s << filter1(filter::s0, filter::s1);
  ref={0, pos, ne, 1, pos, lOr, 0, time, gt, 1, time, nop};
	EXPECT_EQ(buffer, ref);
}

TEST_F(FilterTestSuite, metaFilterBasicTest) {

	std::vector<uint8_t> buffer({0, time, gt, 1, time, nop, 0, time, lt, 1, time, nop, 0, time, eq, 1, time, nop, 0, time, ne, 1, time, nop, 0, time, ge, 1, time, nop, 0, time, le, 1, time, nop});

	DeSerializer<std::vector<uint8_t>::const_iterator> d(buffer.cbegin(), buffer.cend());
	std::vector<const EventType*> types={&eventType, &eventType};
	MetaFilter filter0(types), filter1(types), filter2(types), filter3(types), filter4(types), filter5(types);
	d >> filter0 >> filter1 >> filter2 >> filter3 >> filter4 >> filter5;

  MetaEvent trueMetaEvent(eventType), falseMetaEvent(eventType), compMetaEvent(eventType);

	std::vector<const MetaEvent*> trueEvents = {&trueMetaEvent, &compMetaEvent};
	std::vector<const MetaEvent*> falseEvents= {&falseMetaEvent, &compMetaEvent};

  buffer.clear();
  Serializer<decltype(back_inserter(buffer))> s(back_inserter(buffer));
	s << compEvent << trueEvent << falseEvent;
  d = DeSerializer<std::vector<uint8_t>::const_iterator>(buffer.cbegin(), buffer.cend());
	d >> compMetaEvent >> trueMetaEvent >> falseMetaEvent;

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

TEST_F(FilterTestSuite, metaFilterFuncTest) {

  auto filter1 = uncertainty(e0[Position()]) > e1[Position()];
	std::vector<uint8_t> compBuffer;
  auto i = back_inserter(compBuffer);
  Serializer<decltype(i)> s(i);

  s << filter1(filter::s0, filter::s1);

	std::vector<uint8_t> buffer({0, pos, unc, gt, 1, pos, nop});
  EXPECT_EQ(buffer, compBuffer);

  DeSerializer<std::vector<uint8_t>::const_iterator> d(buffer.cbegin(), buffer.cend());
	std::vector<const EventType*> types={&eventType, &eventType};
	MetaFilter filter0(types);
	d >> filter0;

  MetaEvent trueMetaEvent(eventType), falseMetaEvent(eventType), compMetaEvent(eventType);

	std::vector<const MetaEvent*> trueEvents = {&trueMetaEvent, &compMetaEvent};
	std::vector<const MetaEvent*> falseEvents= {&falseMetaEvent, &compMetaEvent};
  buffer.clear();
  s = Serializer<decltype(back_inserter(buffer))>(back_inserter(buffer));
	s << compEvent << trueEvent << falseEvent;
  d = DeSerializer<std::vector<uint8_t>::const_iterator>(buffer.cbegin(), buffer.cend());
	d >> compMetaEvent >> trueMetaEvent >> falseMetaEvent;

  EXPECT_EQ(filter0.at(0).func().at(0), &MetaAttribute::uncertainty);
	EXPECT_TRUE(filter0(trueEvents))   << "False negative:\n" << *trueEvents[0]  <<  filter0 << "\n" << *trueEvents[1] ;
  EXPECT_FALSE(filter0(falseEvents)) << "False positive:\n" << *falseEvents[0] <<  filter0 << "\n" << *falseEvents[1];
}

}}
