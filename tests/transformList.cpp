#include <TransformationList.h>


namespace test {

using namespace std;

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

struct TransformListSuite : public ::testing::Test {
  struct TestTransformer : public Transformer {
    TestTransformer() : Transformer(Transformation::invalid, EventType(), {EventType()}) {}
    MOCK_CONST_METHOD1(check, bool(const Events&));
    MOCK_METHOD1(call, MetaEvent(const Events&));
    virtual MetaEvent operator()(const Events& events) { return call(events); }
    MOCK_CONST_METHOD1(print, void(std::ostream&));
  };

  struct TestTransformation : public Transformation {
    TestTransformation() : Transformation(Type::attribute, 1, EventID::any) { }

    MOCK_CONST_METHOD1(in, EventTypes(const EventType& goal));
    MOCK_CONST_METHOD2(in, EventTypes(const EventType& goal, const EventType& provided));
    MOCK_CONST_METHOD1(in, EventIDs(EventID goal));
    MOCK_CONST_METHOD2(create, TransPtr(const EventType& out, const EventTypes& in));
    MOCK_CONST_METHOD1(print, void(ostream& o));
  };

  TestTransformation a;
  TestTransformer c,d;
  const TestTransformation* b;
  TransformationList list;
  EventType goal, provided, intermediate;

  struct Test0: public ::id::attribute::Base {
    static constexpr const ::id::attribute::ID value()  { return 251; }
  };

  TransformListSuite() : list(Transformation::Type::attribute, EventID::any) {
    list.push_back(TransformationPtr(&a, [](const Transformation*){}));
    list.push_back(TransformationPtr(new TestTransformation()));
    b=dynamic_cast<const TestTransformation*>(list[1].get());

    ValueType goalVT(id::type::Float::value(), 1, 1, false);

    AttributeType goalAT(Test0::value(), goalVT, Scale<>(), Dimensionless());
    AttributeType intAT = goalAT;
    AttributeType provAT = intAT;

    intAT.scale().denom(1000);
    provAT.scale().denom(1000);
    provAT.value().typeId(id::type::Int64::value());

    goal.add(goalAT);
    intermediate.add(intAT);
    provided.add(provAT);
  }

};

TEST_F(TransformListSuite, inIDTest) {
  using EventIDs = Transformation::EventIDs;
  EXPECT_CALL(a, in(EventID(goal)))
    .Times(1).WillOnce(Return(EventIDs({EventID(intermediate)})));
  EXPECT_CALL(*b, in(EventID(intermediate)))
    .Times(1).WillOnce(Return(EventIDs({EventID(provided)})));
  auto result = list.in(EventID(goal));
  ASSERT_FALSE(result.empty());
  EXPECT_EQ(result[0], EventID(provided));
}

TEST_F(TransformListSuite, inTypeProvidedTest) {
  EXPECT_CALL(a, in(goal, provided))
    .Times(1).WillOnce(Return(EventTypes({intermediate})));
  EXPECT_CALL(*b, in(intermediate, provided))
    .Times(1).WillOnce(Return(EventTypes({provided})));
  auto result = list.in(goal, provided);
  ASSERT_FALSE(result.empty());
  EXPECT_EQ(result[0], provided);
}

TEST_F(TransformListSuite, inTypeTest) {
  EXPECT_CALL(a, in(goal))
    .Times(1).WillOnce(Return(EventTypes({intermediate})));
  EXPECT_CALL(*b, in(intermediate))
    .Times(1).WillOnce(Return(EventTypes({provided})));
  auto result = list.in(goal);
  ASSERT_FALSE(result.empty());
  EXPECT_EQ(result[0], provided);
}

TEST_F(TransformListSuite, createTest) {
  using Events = Transformer::Events;
  EXPECT_CALL(a, in(goal, provided))
    .Times(1).WillOnce(Return(EventTypes({intermediate})));
  EXPECT_CALL(*b, in(intermediate, provided))
    .Times(1).WillOnce(Return(EventTypes({provided})));
  EXPECT_CALL(a, create(goal, EventTypes({intermediate})))
    .Times(1).WillOnce(Return(TransPtr(&c, [](const Transformer*){})));
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided})))
    .Times(1).WillOnce(Return(TransPtr(&d, [](const Transformer*){})));
  TransPtr result = list.create(goal, {provided});
  MetaEvent eA(provided);
  eA.attribute(Test0::value())->value().set(0, 0, {1.1f});
  MetaEvent eB(intermediate);
  eB.attribute(Test0::value())->value().set(0, 0, {1100.0f});
  MetaEvent eC(goal);
  eC.attribute(Test0::value())->value().set(0, 0, {1100});
  EXPECT_CALL(c, call(Events({&eA})))
    .Times(1).WillOnce(Return(eB));
  EXPECT_CALL(d, call(Events{&eB}))
    .Times(1).WillOnce(Return(eC));
  EXPECT_EQ((*result)(Events({&eA})), eC);
}

}
