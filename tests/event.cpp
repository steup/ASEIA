
#include <gtest/gtest.h>
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
  EventID A=(EventID)a;
  EventID B=(EventID)b;
  EventID C=(EventID)c;
	EXPECT_EQ(A.value(), 2U*3U*5U);
	EXPECT_EQ(B.value(), 2U*3U*5U*11U);
	EXPECT_EQ(C.value(), 2U*3U*5U*17U);
	EXPECT_TRUE(A<=B);
	EXPECT_TRUE(A<B);
	EXPECT_TRUE(A!=B);
	EXPECT_FALSE(A>B);
	EXPECT_FALSE(A>=B);
	EXPECT_FALSE(A==B);
	EXPECT_TRUE(B>=A);
	EXPECT_TRUE(B>A);
	EXPECT_TRUE(B!=A);
	EXPECT_FALSE(B<A);
	EXPECT_FALSE(B<=A);
	EXPECT_FALSE(B==A);
	EXPECT_TRUE(A<=C);
	EXPECT_TRUE(A<C);
	EXPECT_TRUE(A!=C);
	EXPECT_FALSE(A>C);
	EXPECT_FALSE(A>=C);
	EXPECT_FALSE(A==C);
	EXPECT_TRUE(C>=A);
	EXPECT_TRUE(C>A);
	EXPECT_TRUE(C!=A);
	EXPECT_FALSE(C<A);
	EXPECT_FALSE(C<=A);
	EXPECT_FALSE(C==A);
	EXPECT_FALSE(B<=C);
	EXPECT_FALSE(B<C);
	EXPECT_TRUE(B!=C);
	EXPECT_FALSE(B>C);
	EXPECT_FALSE(B>=C);
	EXPECT_FALSE(B==C);
	EXPECT_FALSE(C>=B);
	EXPECT_FALSE(C>B);
	EXPECT_TRUE(C!=B);
	EXPECT_FALSE(C<B);
	EXPECT_FALSE(C<=B);
	EXPECT_FALSE(C==B);
}

TEST(EventTestSuite, typeTest) {
	using EventA=BaseEvent<>;
	using Distance = Attribute<Distance, Value<float, 1>, Meter>;
	using Angle = Attribute<Angle, Value<float, 1>, Radian>;
	using EventB=EventA::append<Distance>::type;
	using EventC=EventA::append<Angle>::type;
	EventA a;
	EventB b;
	EventC c;
  EventType A=(EventType)a;
  EventType B=(EventType)b;
  EventType C=(EventType)c;
	EXPECT_TRUE(A<=B);
	EXPECT_TRUE(A<B);
	EXPECT_TRUE(A!=B);
	EXPECT_FALSE(A>B);
	EXPECT_FALSE(A>=B);
	EXPECT_FALSE(A==B);
	EXPECT_TRUE(B>=A);
	EXPECT_TRUE(B>A);
	EXPECT_TRUE(B!=A);
	EXPECT_FALSE(B<A);
	EXPECT_FALSE(B<=A);
	EXPECT_FALSE(B==A);
	EXPECT_TRUE(A<=C);
	EXPECT_TRUE(A<C);
	EXPECT_TRUE(A!=C);
	EXPECT_FALSE(A>C);
	EXPECT_FALSE(A>=C);
	EXPECT_FALSE(A==C);
	EXPECT_TRUE(C>=A);
	EXPECT_TRUE(C>A);
	EXPECT_TRUE(C!=A);
	EXPECT_FALSE(C<A);
	EXPECT_FALSE(C<=A);
	EXPECT_FALSE(C==A);
	EXPECT_FALSE(B<=C);
	EXPECT_FALSE(B<C);
	EXPECT_TRUE(B!=C);
	EXPECT_FALSE(B>C);
	EXPECT_FALSE(B>=C);
	EXPECT_FALSE(B==C);
	EXPECT_FALSE(C>=B);
	EXPECT_FALSE(C>B);
	EXPECT_TRUE(C!=B);
	EXPECT_FALSE(C<B);
	EXPECT_FALSE(C<=B);
	EXPECT_FALSE(C==B);
}


}}
