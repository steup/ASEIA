
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <AbstractRegistry.h>

#include <BaseEvent.h>

#include <iterator>
#include <algorithm>
#include <list>

namespace tests {
namespace typeRegistryTests {

using ::testing::UnorderedElementsAre;
using namespace ::id::attribute;
using namespace ::id::type;

class TypeRegistryTestSuite : public ::testing::Test {
	public:
    using AngleAttr = Attribute<Angle, Value<float, 1, 1, false>, Radian, Scale<>>;
    using AngleAttr1 = Attribute<Angle, Value<uint32_t, 3, 1, false>, Radian, Scale<>>;
    using ObjectAttr = Attribute<Object, Value<uint32_t, 1, 1, false>, Dimensionless, Scale<>>;
    using ExtendedEvent = BaseEvent<>::append<AngleAttr>::type;
    using ExtendedEvent1 = BaseEvent<>::append<AngleAttr1>::type;
    using ExtendedEvent2 = ExtendedEvent::append<ObjectAttr>::type;
		EventType none, base, extended, extended1, extended2;
		AbstractRegistry<EventType> r;
		TypeRegistryTestSuite()
      : base(BaseEvent<>()),
        extended(ExtendedEvent()),
        extended1(ExtendedEvent1()),
        extended2(ExtendedEvent2()) {
			r.registerType(base, base);
			r.registerType(extended, extended);
			r.registerType(extended1, extended1);
			r.registerType(extended2, extended2);
		}
};

TEST_F(TypeRegistryTestSuite, findTest) {
	EXPECT_TRUE(r.contains(base)) << "Known EventType not found";
}

TEST_F(TypeRegistryTestSuite, notFoundTest) {
	EXPECT_FALSE(r.contains(none)) << "Unknown EventType found";
}

TEST_F(TypeRegistryTestSuite, retrieveTest) {
	auto range=r.find(base);
	ASSERT_NE(range.begin(), range.end()) << "Stored EventType not found";
	EXPECT_EQ(range.front(), base) << "Stored EventType not found";
}

TEST_F(TypeRegistryTestSuite, iteratorTest) {
	std::list<AbstractRegistry<EventType>::value_type> list;
	std::copy(r.begin(), r.end(), std::back_inserter(list));
	ASSERT_TRUE(std::equal(r.begin(), r.end(), list.begin())) << "Iterator based copy of type registry is not equal";
}

TEST_F(TypeRegistryTestSuite, idExtractTest) {
  EXPECT_THAT(r.ids(), UnorderedElementsAre(EventID(base), EventID(extended), EventID(extended2)));
}

}}
