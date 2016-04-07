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
    Event trueEvent, falseEvent;
    using TimeAttr = Event::findAttribute<Time>::type;
    const TimeAttr c0 = {{{ 1024 }}};
    FilterTestSuite() {
      trueEvent.attribute(Time()).value()  = {{{1050}}};
      falseEvent.attribute(Time()).value() = {{{100}}};
    }
};

TEST_F(FilterTestSuite, basicFilterExpressionTest){
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

#include <IO.h>
TEST_F(FilterTestSuite, basicFilterSerializationTest) {
  auto filter0 = e0[Time()] > c0;
  auto filter1 = filter0(e0) && filter0(e1);

  std::vector<uint8_t> buffer;
	uint8_t e0Time = 0x2;
	uint8_t e1Time = 0x22;
	uint8_t gtC = 0x84;
	uint8_t lAnd = 0x9;
  std::vector<uint8_t> c1024 = {0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
  std::vector<uint8_t> c0 = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	std::vector<uint8_t> result;
	result.push_back(e0Time);
	result.push_back(gtC);
	result.insert(result.end(), c1024.begin(), c1024.end());
	result.insert(result.end(), c0.begin(), c0.end());
	result.push_back(lAnd);
	result.push_back(e1Time);
	result.push_back(gtC);
	result.insert(result.end(), c1024.begin(), c1024.end());
	result.insert(result.end(), c0.begin(), c0.end());

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
	EXPECT_EQ(buffer[1], gtC) << "Operation > with constant argument serialized wrongly";
	EXPECT_EQ(buffer[18], lAnd) << "Operation && serialized wrongly";
	EXPECT_EQ(buffer[19], e1Time) << "Placeholder event 1 time serialized wrongly";
	EXPECT_EQ(buffer[20], gtC) << "Operation > with constant argument serialized wrongly";
  EXPECT_EQ(buffer.size(), result.size()) << "Serialized size does not match";
  EXPECT_TRUE(std::equal(buffer.begin(), buffer.end(), result.begin())) << os.str();
}

}}
