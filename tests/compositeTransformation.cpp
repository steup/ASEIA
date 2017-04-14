
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <CompositeTransformation.h>
#include <MetaEvent.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>


namespace test {

using namespace std;

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;
using boost::filesystem::ofstream;
using boost::filesystem::current_path;
using boost::filesystem::path;

MATCHER(EqPtrContent, "") {
  return *get<0>(arg)==*get<1>(arg);
}
struct CompositeTransformSuite : public ::testing::Test {

  using EventTypes = Transformer::EventTypes;
  using TransPtr   = Transformation::TransPtr;

  static bool comp(const MetaEvent* const a, const MetaEvent* const b) {
    return *a==*b;
  }

  struct TestTransformer : public Transformer {
    string name;
    TestTransformer(const string& name, const EventType& out, const EventTypes& in,
                    map<string, TestTransformer*>& trans)
      : Transformer(out, in), name(name) {
      trans[name]=this;
    }
    MOCK_CONST_METHOD1(check, bool(const MetaEvent&));
    MOCK_CONST_METHOD1(call, Events(const MetaEvent&));
    virtual Events operator()(const MetaEvent& e) { return call(e); }
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

  map<string, TestTransformer*> trans;
  TestTransformation a, c, d;
  CompositeTransformation compTrans, compTrans2;
  shared_ptr<const TestTransformation> b;
  EventType goal, provided, provided2, intermediate, intermediate2;

  struct Test0: public ::id::attribute::Base {
    static constexpr const ::id::attribute::ID value()  { return 251; }
  };
  struct Test1: public ::id::attribute::Base {
    static constexpr const ::id::attribute::ID value()  { return 252; }
  };

  CompositeTransformSuite()
    : a("a", 1), c("c", 2), d("d", 1), b(new TestTransformation("b", 1))
  {}

  void SetUp() {

    ValueType goalVT(id::type::Float::value(), 1, 1, false);

    AttributeType goalAT(Test0::value(), goalVT, Scale<>(), Dimensionless());
    AttributeType int2AT(Test1::value(), goalVT, Scale<std::ratio<1, 1000>>(), Dimensionless());
    AttributeType intAT = goalAT;
    AttributeType provAT = intAT;
    AttributeType prov2AT = int2AT;


    intAT.scale().denom(1000);
    provAT.scale().denom(1000);
    provAT.value().typeId(id::type::Int64::value());
    prov2AT.value().typeId(id::type::Int64::value());

    goal.add(goalAT);
    intermediate.add(intAT);
    provided.add(provAT);
    intermediate2.add(int2AT);
    provided2.add(prov2AT);

    ASSERT_NE(b, nullptr);
    EXPECT_CALL(a, in(goal, provided))
      .Times(1).WillOnce(Return(EventTypes({intermediate})));
    EXPECT_CALL(*b, in(intermediate, provided))
      .Times(AtLeast(1)).WillRepeatedly(Return(EventTypes({provided})));
    EXPECT_CALL(c, in(goal))
      .Times(1).WillOnce(Return(EventTypes({intermediate, intermediate2})));
    EXPECT_CALL(d, in(intermediate2, provided2))
      .Times(1).WillOnce(Return(EventTypes({provided2})));
    auto r0 = compTrans.addRootTransformation(&a, goal, provided);
    ASSERT_TRUE(r0.second);
    auto r1 = compTrans.addTransformation(b.get(), r0.first, intermediate, provided);
    ASSERT_TRUE(r1.second);
    ofstream out(current_path()/"doc"/"linTransformation.dot");
    out << compTrans;
    auto r2 = compTrans2.addRootTransformation(&c, goal, EventType());
    ASSERT_TRUE(r2.second);
    auto r3 = compTrans2.addTransformation(b.get(), r2.first, intermediate, provided);
    ASSERT_TRUE(r3.second);
    auto r4 = compTrans2.addTransformation(&d, r2.first, intermediate2, provided2);
    ASSERT_TRUE(r4.second);
    out.close();
    out.open(current_path()/"doc"/"treeTransformation.dot");
    out << compTrans2;
  }
};

TEST_F(CompositeTransformSuite, linearTest) {
  using Graph = CompositeTransformation::Graph;
  using Vertex = CompositeTransformation::Vertex;
  using Edge = Graph::edge_descriptor;
  const Graph& g = compTrans.graph();
  ASSERT_EQ(boost::num_vertices(g), 2U);
  auto v = boost::vertices(g);
  Vertex testAV = *v.first;
  const Transformation& testA = *g[testAV].trans();
  EXPECT_EQ(testA, a);
  auto out = out_edges(testAV, g);
  ASSERT_NE(out.first, out.second);
  Edge testBE = *out.first;
  Vertex testBV = target(testBE, g);
  const Transformation& testB = *g[testBV].trans();
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(testB, *b);
}

TEST_F(CompositeTransformSuite, linearInTest) {
  auto result = compTrans.in();
  EXPECT_EQ(result, EventTypes({provided}));
}

TEST_F(CompositeTransformSuite, treeTest) {
  using Graph = CompositeTransformation::Graph;
  using Vertex = CompositeTransformation::Vertex;
  using Edge = Graph::edge_descriptor;
  const Graph& g = compTrans2.graph();
  ASSERT_EQ(boost::num_vertices(g), 3U);
  auto v = boost::vertices(g);
  Vertex testCV = *v.first;
  const Transformation& testC = *g[testCV].trans();
  EXPECT_EQ(testC, c);
  auto out = out_edges(testCV, g);
  ASSERT_NE(out.first, out.second);
  Edge testBE = *out.first;
  Vertex testBV = target(testBE, g);
  const Transformation& testB = *g[testBV].trans();
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(testB, *b);
  Edge testDE = *++out.first;
  Vertex testDV = target(testDE, g);
  const Transformation& testD = *g[testDV].trans();
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(testD, d);
}

TEST_F(CompositeTransformSuite, treeInTest) {
  auto result = compTrans2.in();
  ASSERT_FALSE(result.empty());
  EXPECT_EQ(result, EventTypes({provided, provided2}));
}

TEST_F(CompositeTransformSuite, linearCreateTest) {
  using Events = Transformer::Events;
  EXPECT_CALL(a, create(goal, EventTypes({intermediate}), _))
    .Times(1).WillOnce(Return(TransPtr(new TestTransformer("a", goal, EventTypes({intermediate}), trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _))
    .Times(1).WillOnce(Return(TransPtr(new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  TransPtr result = compTrans.create(AbstractPolicy());
  ASSERT_NE(result, nullptr);
  path file = current_path()/"doc"/"linearTransformer.dot";
  ofstream out(file);
  out << *result;
  MetaEvent eA(provided);
  eA.attribute(Test0::value())->value().set(0, 0, {1.1f});
  MetaEvent eB(intermediate);
  eB.attribute(Test0::value())->value().set(0, 0, {1100.0f});
  MetaEvent eC(goal);
  eC.attribute(Test0::value())->value().set(0, 0, {1100});
  EXPECT_CALL(*trans["b"], call(eA))
    .Times(1).WillOnce(Return(Events{eB}));
  EXPECT_CALL(*trans["a"], call(_))
    .WillRepeatedly(Return(Events{}));
  EXPECT_CALL(*trans["a"], call(eB))
    .Times(1).WillOnce(Return(Events{eC}));
  Events res0 = (*result)(eA);
  ASSERT_GE(res0.size(), 1U) << "No events generated";
  EXPECT_EQ(res0.front(), eC);
}

TEST_F(CompositeTransformSuite, treeCreateTest) {
  using Events = Transformer::Events;
  EXPECT_CALL(c, create(goal, EventTypes({intermediate, intermediate2}), _))
    .Times(1).WillOnce(Return(TransPtr(new TestTransformer("c", goal, EventTypes({intermediate, intermediate2}), trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _))
    .Times(1).WillOnce(Return(TransPtr(new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  EXPECT_CALL(d, create(intermediate2, EventTypes({provided2}), _))
    .Times(1).WillOnce(Return(TransPtr(new TestTransformer("d", intermediate2, EventTypes({provided2}), trans))));
  TransPtr result = compTrans2.create(AbstractPolicy());
  ASSERT_NE(result, nullptr);
  path file = current_path()/"doc"/"treeTransformer.dot";
  ofstream out(file);
  out << *result;
  MetaEvent eA(provided);
  eA.attribute(Test0::value())->value().set(0, 0, {1.0f});
  MetaEvent eB(provided2);
  eB.attribute(Test1::value())->value().set(0, 0, {2.0f});
  MetaEvent eC(intermediate);
  eC.attribute(Test0::value())->value().set(0, 0, {3.0f});
  MetaEvent eD(intermediate2);
  eD.attribute(Test1::value())->value().set(0, 0, {4.0f});
  MetaEvent eE(goal);
  eE.attribute(Test0::value())->value().set(0, 0, {5.0f});
  EXPECT_CALL(*trans["b"], call(_))
    .WillRepeatedly(Return(Events{}));
  EXPECT_CALL(*trans["b"], call(eA))
    .Times(1).WillOnce(Return(Events{eC}));
  EXPECT_CALL(*trans["d"], call(_))
    .WillRepeatedly(Return(Events{}));
  EXPECT_CALL(*trans["d"], call(eB))
    .Times(1).WillOnce(Return(Events{eD}));
  EXPECT_CALL(*trans["c"], call(_))
    .WillRepeatedly(Return(Events{}));
  EXPECT_CALL(*trans["c"], call(eD))
    .Times(1);
  EXPECT_CALL(*trans["c"], call(eC))
    .Times(1).WillOnce(Return(Events{eE}));
  Events res0 = (*result)(eA);
  Events res1 = (*result)(eB);
  move(res1.begin(), res1.end(), back_inserter(res0));
  ASSERT_GE(res0.size(), 1U) << "No events generated";
  EXPECT_EQ(res0.front(), eE);
}

}
