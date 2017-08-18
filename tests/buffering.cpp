#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Transformation.h>
#include <CompositeTransformation.h>
#include <ID.h>

namespace test {

  using namespace std;
  using ::testing::Contains;
  using ::testing::Return;
  using ::testing::AllOf;
  using ::testing::Pointee;
  using ::testing::UnorderedElementsAre;
  using ::testing::Each;
  using ::testing::SizeIs;
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

  EventType outET, inET0, inET1, inET2, tET0, tET1;
  const id::attribute::ID outID=250, inID0=251, inID1=252, inID2=253, tID0=254, tID1=255;

  BufferingTestSuite() {
    ValueType v(id::type::Float::value(), 1, 1, false);
    AttributeType outAT (outID, v, Scale<>(), Dimensionless());
    AttributeType inAT0 (inID0, v, Scale<>(), Dimensionless());
    AttributeType inAT1 (inID1, v, Scale<>(), Dimensionless());
    AttributeType inAT2 (inID2, v, Scale<>(), Dimensionless());
    AttributeType tAT0  (tID0,  v, Scale<>(), Dimensionless());
    AttributeType tAT1  (tID1,  v, Scale<>(), Dimensionless());
    outET.add(outAT);
    inET0.add(inAT0);
    inET1.add(inAT1);
    inET2.add(inAT2);
    tET0.add(tAT0);
    tET1.add(tAT1);
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

TEST_F(BufferingTestSuite, singleHomTest) {
  MetaEvent out(outET);
  Events in = gen({inET0, inET0, inET0, inET0});
  out.attribute(outID)->value().set(0,0,3);
  in[0].attribute(inID0)->value().set(0,0,1);
  in[1].attribute(inID0)->value().set(0,0,2);
  in[2].attribute(inID0)->value().set(0,0,10);
  in[3].attribute(inID0)->value().set(0,0,20);
  EXPECT_NE(in[0], in[2]);
  EXPECT_NE(in[1], in[3]);
  TestTransformer t("SingleHom", outET, {inET0, inET0});
  EXPECT_CALL(t, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(t, check(in[0])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(t, check(in[1])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(t, execute( UnorderedElementsAre(
                            Pointee(in[0]),
                            Pointee(in[1])
                          ))).Times(2).WillRepeatedly(Return(Events({out})));
  EXPECT_THAT(execute(t, in), Events({out, out}));
}


TEST_F(BufferingTestSuite, multiHetTest) {
  MetaEvent out(outET);
  Events in = gen({inET0, inET1, inET2});
  MetaEvent temp0(tET0), temp1(tET1);
  out.attribute(outID)->value().set(0,0,3);
  in[0].attribute(inID0)->value().set(0,0,1);
  in[1].attribute(inID1)->value().set(0,0,2);
  in[2].attribute(inID2)->value().set(0,0,10);
  temp0.attribute(tID0)->value().set(0,0,5);
  temp1.attribute(tID1)->value().set(0,0,6);
  auto nothing = [](void *){};
  TestTransformer a("a", tET0, {inET0, inET1}), b("b", tET1, {inET1, inET2}), c("c", outET, {tET0, tET1});
  TestTransformation aT("aT", 2), bT("bT", 2), cT("cT", 2);

  EXPECT_CALL(aT, in(tET0, _)).Times(1).WillOnce(Return(EventTypes({inET0, inET1})));
  EXPECT_CALL(bT, in(tET1, _)).Times(1).WillOnce(Return(EventTypes({inET1, inET2})));
  EXPECT_CALL(cT, in(outET, _)).Times(1).WillOnce(Return(EventTypes({tET0, tET1})));

  CompositeTransformation compTrans(TransformationPtr(&cT), outET, EventType());
  compTrans.add(TransformationPtr(&aT), compTrans.root(), tET0, EventType());
  compTrans.add(TransformationPtr(&bT), compTrans.root(), tET1, EventType());

  using TransPtr = Transformation::TransPtr;

  EXPECT_CALL(aT, create(tET0,EventTypes({inET0, inET1}),_))
    .Times(1).WillOnce(Return(TransPtr(&a, nothing)));
  EXPECT_CALL(bT, create(tET1,EventTypes({inET1, inET2}),_))
    .Times(1).WillOnce(Return(TransPtr(&b, nothing)));
  EXPECT_CALL(cT, create(outET,EventTypes({tET0, tET1}),_))
    .Times(1).WillOnce(Return(TransPtr(&c, nothing)));

  TransPtr tPtr = compTrans.create(AbstractPolicy());
  ASSERT_NE(tPtr, nullptr);

  EXPECT_CALL(a, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(a, check(in[0])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(a, check(in[1])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(b, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(b, check(in[1])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(b, check(in[2])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(c, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(c, check(temp0)).Times(1).WillOnce(Return(true));
  EXPECT_CALL(c, check(temp1)).Times(1).WillOnce(Return(true));
  EXPECT_CALL(a, execute( UnorderedElementsAre(
                            Pointee(in[0]),
                            Pointee(in[1])
                          ))).Times(1).WillRepeatedly(Return(Events({temp0})));
  EXPECT_CALL(b, execute( UnorderedElementsAre(
                            Pointee(in[1]),
                            Pointee(in[2])
                          ))).Times(1).WillRepeatedly(Return(Events({temp1})));
  EXPECT_CALL(c, execute( UnorderedElementsAre(
                            Pointee(temp0),
                            Pointee(temp1)
                          ))).Times(1).WillRepeatedly(Return(Events({out})));

  EXPECT_THAT(execute(*tPtr, in), Events({out}));
}

TEST_F(BufferingTestSuite, complexTest) {
  MetaEvent out(outET);
  Events in = gen({inET0, inET0, inET1, inET1});
  MetaEvent temp0(tET0), temp1(tET1);
  out.attribute(outID)->value().set(0,0,3);
  in[0].attribute(inID0)->value().set(0,0,1);
  in[1].attribute(inID0)->value().set(0,0,2);
  in[2].attribute(inID1)->value().set(0,0,10);
  in[3].attribute(inID1)->value().set(0,0,9);
  temp0.attribute(tID0)->value().set(0,0,5);
  temp1.attribute(tID1)->value().set(0,0,6);
  auto nothing = [](void *){};
  TestTransformer a("a", tET0, {inET0, inET0}), b("b", tET1, {inET1, inET1}), c("c", outET, {tET0, tET1});
  TestTransformation aT("aT", 2), bT("bT", 2), cT("cT", 2);

  EXPECT_CALL(aT, in(tET0, _)).Times(1).WillOnce(Return(EventTypes({inET0, inET0})));
  EXPECT_CALL(bT, in(tET1, _)).Times(1).WillOnce(Return(EventTypes({inET1, inET1})));
  EXPECT_CALL(cT, in(outET, _)).Times(1).WillOnce(Return(EventTypes({tET0, tET1})));

  CompositeTransformation compTrans(TransformationPtr(&cT), outET, EventType());
  compTrans.add(TransformationPtr(&aT), compTrans.root(), tET0, EventType());
  compTrans.add(TransformationPtr(&bT), compTrans.root(), tET1, EventType());

  using TransPtr = Transformation::TransPtr;

  EXPECT_CALL(aT, create(tET0,EventTypes({inET0, inET0}),_))
    .Times(1).WillOnce(Return(TransPtr(&a, nothing)));
  EXPECT_CALL(bT, create(tET1,EventTypes({inET1, inET1}),_))
    .Times(1).WillOnce(Return(TransPtr(&b, nothing)));
  EXPECT_CALL(cT, create(outET,EventTypes({tET0, tET1}),_))
    .Times(1).WillOnce(Return(TransPtr(&c, nothing)));

  TransPtr tPtr = compTrans.create(AbstractPolicy());
  ASSERT_NE(tPtr, nullptr);

  EXPECT_CALL(a, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(a, check(in[0])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(a, check(in[1])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(b, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(b, check(in[2])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(b, check(in[3])).Times(1).WillOnce(Return(true));
  EXPECT_CALL(c, check(_)).WillRepeatedly(Return(false));
  EXPECT_CALL(c, check(temp0)).Times(2).WillRepeatedly(Return(true));
  EXPECT_CALL(c, check(temp1)).Times(2).WillRepeatedly(Return(true));
  EXPECT_CALL(a, execute( UnorderedElementsAre(
                            Pointee(in[0]),
                            Pointee(in[1])
                          ))).Times(2).WillRepeatedly(Return(Events({temp0})));
  EXPECT_CALL(b, execute( UnorderedElementsAre(
                            Pointee(in[2]),
                            Pointee(in[3])
                          ))).Times(2).WillRepeatedly(Return(Events({temp1})));
  EXPECT_CALL(c, execute( UnorderedElementsAre(
                            Pointee(temp0),
                            Pointee(temp1)
                          ))).Times(4).WillRepeatedly(Return(Events({out})));

  EXPECT_THAT(execute(*tPtr, in), AllOf(SizeIs(4), Each(out)));
}

}
