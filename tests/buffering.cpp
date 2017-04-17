#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Transformation.h>
#include <ID.h>

namespace test {

  using namespace std;
  using ::testing::Contains;
  using ::testing::Return;
  using ::testing::AllOf;
  using ::testing::Pointee;
  using ::testing::_;

  struct BufferingTestSuite : public ::testing::Test {

    using Events = Transformer::Events;
    using EventTypes = Transformer::EventTypes;

  struct TestTransformer : public BufferedTransformer {
    using EventPtrs = BufferedTransformer::EventPtrs;
    string name;
    TestTransformer(const string& name, const EventType& out, const EventTypes& in)
      : BufferedTransformer(out, in, AbstractPolicy()), name(name) {}
    MOCK_CONST_METHOD1(check, bool(const MetaEvent&));
    MOCK_CONST_METHOD1(execute, Events(const EventPtrs&));
    virtual void print(ostream& o) const { o << name; }
  };

  struct TestTransformation : public Transformation {
    string name;
    TestTransformation(const string& name, size_t arity)
      : Transformation(Type::attribute, arity, EventID::any), name(name) { }

    MOCK_CONST_METHOD1(in, EventTypes(const EventType& goal));
    MOCK_CONST_METHOD2(in, EventTypes(const EventType& goal, const EventType& provided));
    MOCK_CONST_METHOD1(in, EventIDs(EventID goal));
    MOCK_CONST_METHOD3(create, TransPtr(const EventType& out, const EventTypes& in, const AbstractPolicy& policy));
    virtual void print(ostream& o) const { o << name; }
  };

  Events gen(const EventTypes& eTs) const {
    Events e;
    for(const EventType& eT: eTs)
      e.emplace_back(eT);
    return e;
  }

  Events execute(Transformer& t, const Events& in) const {
    Events result;
    for(const MetaEvent& e : in) {
      Events temp = t(e);
      move(temp.begin(), temp.end(), back_inserter(result));
    }
    return result;
  }

  EventType outET, inET0, inET1;
  const id::attribute::ID outID=251, inID0=252, inID1=253;

  BufferingTestSuite() {
    ValueType v(id::type::Float::value(), 1, 1, false);
    AttributeType outAT (outID, v, Scale<>(), Dimensionless());
    AttributeType inAT0 (inID0, v, Scale<>(), Dimensionless());
    AttributeType inAT1 (inID1, v, Scale<>(), Dimensionless());
    outET.add(outAT);
    inET0.add(inAT0);
    inET1.add(inAT1);
  }

};

TEST_F(BufferingTestSuite, singleHetTest) {
  MetaEvent out(outET);
  Events in = gen({inET0, inET1, inET0, inET1});
  out.attribute(outID)->value().set(0,0,3);
  in[0].attribute(inID0)->value().set(0,0,1);
  in[1].attribute(inID1)->value().set(0,0,2);
  in[2].attribute(inID0)->value().set(0,0,10);
  in[3].attribute(inID1)->value().set(0,0,20);
  EXPECT_NE(in[0], in[2]);
  EXPECT_NE(in[1], in[3]);
  TestTransformer t("SingleHet", outET, {inET0, inET1});
  EXPECT_CALL(t, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(t, check(in[0])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(t, check(in[1])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(t, execute( AllOf(
                            Contains(Pointee(in[0])),
                            Contains(Pointee(in[1]))
                          ))).Times(1).WillOnce(Return(Events({out})));
  EXPECT_THAT(execute(t, in), Events({out}));
}

}
