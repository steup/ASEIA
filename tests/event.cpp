#include <EventID.h>
#include <BaseEvent.h>
#include <ID.h>

namespace tests {

namespace eventTestSuite {

using namespace ::id::attribute;

TEST(EventTestSuite, idTest) {
	using EventA=BaseEvent<>;
	using Distance = Attribute<Distance, Value<float, 1>, Meter>;
	using Angle = Attribute<Angle, Value<float, 1>, Radian>;
	using EventB=EventA::append<Distance>::type;
	using EventC=EventA::append<Angle>::type;
	EventA a;
	EventB b;
	EventC c;
	EXPECT_EQ(a.id().value(), 3U*5U*7U);
	EXPECT_EQ(b.id().value(), 3U*5U*7U*13U);
	EXPECT_EQ(c.id().value(), 3U*5U*7U*19U);
	EXPECT_TRUE(a.id()<=b.id());
	EXPECT_TRUE(a.id()<b.id());
	EXPECT_TRUE(a.id()!=b.id());
	EXPECT_FALSE(a.id()>b.id());
	EXPECT_FALSE(a.id()>=b.id());
	EXPECT_FALSE(a.id()==b.id());
	EXPECT_TRUE(a.id()<=c.id());
	EXPECT_TRUE(a.id()<c.id());
	EXPECT_FALSE(b.id()<=c.id());
	EXPECT_FALSE(b.id()<c.id());
}

}}
