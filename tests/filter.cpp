#include <BaseEvent.h>
#include <Filter.h>

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

  std::vector<uint8_t> buffer;
	uint8_t e0Time = ::id::attribute::Time::value();
	uint8_t gt = 0x80|::id::filterOp::GT::value;
	uint8_t lt = 0x80|::id::filterOp::LT::value;
	uint8_t ge = 0x80|::id::filterOp::GE::value;
	uint8_t le = 0x80|::id::filterOp::LE::value;
	uint8_t eq = 0x80|::id::filterOp::EQ::value;
	uint8_t ne = 0x80|::id::filterOp::NE::value;

  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);
  FilterEvent<decltype(s)> s1(1);

	uint16_t count = 0;

  s << filter0(s0);
	EXPECT_EQ(buffer[0], e0Time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[count+1], gt) << "Operation > with constant argument serialized wrongly";

	count+=2*sizeof(uint64_t)+2;

  s << filter1(s0);
	EXPECT_EQ(buffer[count+1], lt) << "Operation < with constant argument serialized wrongly";
  
	count+=2*sizeof(uint64_t)+2;

	s << filter2(s0);
	EXPECT_EQ(buffer[count+1], ge) << "Operation >= with constant argument serialized wrongly";
  
	count+=2*sizeof(uint64_t)+2;

	s << filter3(s0);
	EXPECT_EQ(buffer[count+1], le) << "Operation <= with constant argument serialized wrongly";
	
	count+=2*sizeof(uint64_t)+2;

	s << filter4(s0);
	EXPECT_EQ(buffer[count+1], eq) << "Operation == with constant argument serialized wrongly";
	
	count+=2*sizeof(uint64_t)+2;
	
	s << filter5(s0);
	EXPECT_EQ(buffer[count+1], ne) << "Operation != with constant argument serialized wrongly";

}

/*TEST_F(FilterTestSuite, extendedSerializationTest) {
  auto filter0 = e0[Time()] > e1[Time()];
  auto filter1 = e0[Time()] < e1[Time()];
  auto filter2 = e0[Time()] >= e1[Time()];
  auto filter3 = e0[Time()] <= e1[Time()];
  auto filter4 = e0[Time()] == e1[Time()];
  auto filter4 = e0[Time()] != e1[Time()];

  std::vector<uint8_t> buffer;
	uint8_t e0Time = id::attribute::Time::value();
	uint8_t e1Time = (0x1<<5)||e0Time;

  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);
  FilterEvent<decltype(s)> s1(1);

  s << filter0(s0, s1) << filter1(s0, s1) << filter2(s0, s1) << filter3(s0, s1) << filter4(s0, s1) << filter5(s0, s1);

	EXPECT_EQ(buffer[0], e0Time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[2], e1Time) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer[1], id::filterOp::GT) << "Operation > with event argument serialized wrongly";
	EXPECT_EQ(buffer[4], id::filterOp::LT) << "Operation < with event argument serialized wrongly";
	EXPECT_EQ(buffer[7], id::filterOp::GE) << "Operation >= with event argument serialized wrongly";
	EXPECT_EQ(buffer[10], id::filterOp::LE) << "Operation <= with event argument serialized wrongly";
	EXPECT_EQ(buffer[13], id::filterOp::EQ) << "Operation == with event argument serialized wrongly";
	EXPECT_EQ(buffer[16], id::filterOp::NE) << "Operation != with event argument serialized wrongly";
}

TEST_F(FilterTestSuite, compositeSerializationTest) {
  auto filter0 = e0[Time()] > c0;
  auto filter1 = filter0(e0) && filter0(e1);
	EXPECT_EQ(buffer[4], id::filterOp::LT) << "Operation < with event argument serialized wrongly";
	EXPECT_EQ(buffer[7], id::filterOp::GE) << "Operation >= with event argument serialized wrongly";
	EXPECT_EQ(buffer[10], id::filterOp::LE) << "Operation <= with event argument serialized wrongly";
	EXPECT_EQ(buffer[13], id::filterOp::EQ) << "Operation == with event argument serialized wrongly";
	EXPECT_EQ(buffer[16], id::filterOp::NE) << "Operation != with event argument serialized wrongly";
}

TEST_F(FilterTestSuite, extendedSerializationTest) {
  auto filter0 = e0[Time()] > e1[Time()];
  auto filter1 = e0[Time()] < e1[Time()];
  auto filter2 = e0[Time()] >= e1[Time()];
  auto filter3 = e0[Time()] <= e1[Time()];
  auto filter4 = e0[Time()] == e1[Time()];
  auto filter4 = e0[Time()] != e1[Time()];

  std::vector<uint8_t> buffer;
	uint8_t e0Time = 0x2;
	uint8_t e1Time = 0x22;

  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);
  FilterEvent<decltype(s)> s1(1);

  s << filter0(s0, s1) << filter1(s0, s1) << filter2(s0, s1) << filter3(s0, s1) << filter4(s0, s1) << filter5(s0, s1);

	EXPECT_EQ(buffer[0], e0Time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[2], e1Time) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer[1], id::filterOp::GT) << "Operation > with event argument serialized wrongly";
	EXPECT_EQ(buffer[4], id::filterOp::LT) << "Operation < with event argument serialized wrongly";
	EXPECT_EQ(buffer[7], id::filterOp::GE) << "Operation >= with event argument serialized wrongly";
	EXPECT_EQ(buffer[10], id::filterOp::LE) << "Operation <= with event argument serialized wrongly";
	EXPECT_EQ(buffer[13], id::filterOp::EQ) << "Operation == with event argument serialized wrongly";
	EXPECT_EQ(buffer[16], id::filterOp::NE) << "Operation != with event argument serialized wrongly";
}

TEST_F(FilterTestSuite, compositeSerializationTest) {
  auto filter0 = e0[Time()] > c0;
  auto filter1 = filter0(e0) && filter0(e1);
  auto filter2 = filter0(e0) || filter0(e1);

  std::vector<uint8_t> buffer;
	uint8_t e0Time = id::attribute::Time::value();
	uint8_t e1Time = (0x1<<5)|e0Time;

  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);
  FilterEvent<decltype(s)> s1(1);

  s << filter1(s0, s1);

	std::ostringstream os;
	for(uint8_t v : buffer)
		os <<  "0x" << std::hex << v << ", ";
	os << std::endl;
	for(uint8_t v : result)
		os <<  "0x" << std::hex << v << ", ";

	EXPECT_EQ(buffer[0], e0Time) << "Placeholder event 0 time serialized wrongly";
	EXPECT_EQ(buffer[1], 0x80|id::filterOp::GT) << "Operation > with constant argument serialized wrongly";
	EXPECT_EQ(buffer[18], id::filterOp::AND) << "Operation && serialized wrongly";
	EXPECT_EQ(buffer[19], e1Time) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer[20], id::filterOp::LE) << "Operation <= with event argument serialized wrongly";
  EXPECT_EQ(buffer.size(), sizeof()) << "Serialized size does not match";
  EXPECT_TRUE(std::equal(buffer.begin(), buffer.end(), result.begin())) << os.str();

	s.erase();
	s << filter1(s0, s1);
	EXPECT_EQ()
}*/
}}
