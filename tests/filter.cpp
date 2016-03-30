#include <BaseEvent.h>
#include <Filter.h>

namespace tests {

namespace filterTestSuite {

using namespace ::id::attribute;
using namespace filter;

class FilterTestSuite : public ::testing::Test {
  public:
    using Event = BaseEvent<>;
    Event trueEvent, falseEvent;
    using TimeAttr = Event::findAttribute<Time>::type;
    const TimeAttr c0 = {{{ 1000 }}};
    FilterTestSuite() {
      trueEvent.attribute(Time()).value()  = {{{1024}}};
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
  auto i = std::back_inserter(buffer);
  Serializer<decltype(i)> s(i);
  FilterEvent<decltype(s)> s0(0, s);
  FilterEvent<decltype(s)> s1(1);
  s << filter1(s0, s1);
  std::ostringstream os;
  for(uint8_t v : buffer)
    os << std::hex << (uint16_t)v << " ";
  EXPECT_TRUE(false) << os.str();
}

}}
