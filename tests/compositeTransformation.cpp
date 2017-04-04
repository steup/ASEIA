#include <CompositeTransformation.h>


namespace test {

using namespace std;

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

struct CompositeTransformSuite : public ::testing::Test {
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

  TestTransformation a, e, f;
  TestTransformer c, d, g;
  CompositeTransformation compTrans, compTrans2;
  shared_ptr<const Transformation> bPtr;
  const TestTransformation* b=nullptr;
  EventType goal, provided, provided2, intermediate, intermediate2;

  struct Test0: public ::id::attribute::Base {
    static constexpr const ::id::attribute::ID value()  { return 251; }
  };
  struct Test1: public ::id::attribute::Base {
    static constexpr const ::id::attribute::ID value()  { return 252; }
  };

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

    bPtr.reset(new TestTransformation());
    b=dynamic_cast<const TestTransformation*>(bPtr.get());
    ASSERT_NE(b, nullptr);
    EXPECT_CALL(a, in(goal, provided))
      .Times(1).WillOnce(Return(EventTypes({intermediate})));
    EXPECT_CALL(*b, in(intermediate, provided))
      .Times(AtLeast(1)).WillRepeatedly(Return(EventTypes({provided})));
    EXPECT_CALL(e, in(goal))
      .Times(1).WillOnce(Return(EventTypes({intermediate, intermediate2})));
    EXPECT_CALL(f, in(intermediate2, provided2))
      .Times(1).WillOnce(Return(EventTypes({provided2})));
    auto r0 = compTrans.addRootTransformation(TransformationPtr(&a), goal, provided);
    ASSERT_TRUE(r0.second);
    auto r1 = compTrans.addTransformation(b, r0.first, intermediate, provided);
    ASSERT_TRUE(r1.second);
    auto r2 = compTrans2.addRootTransformation(TransformationPtr(&e), goal, EventType());
    ASSERT_TRUE(r2.second);
    auto r3 = compTrans2.addTransformation(b, r2.first, intermediate, provided);
    ASSERT_TRUE(r3.second);
    auto r4 = compTrans2.addTransformation(TransformationPtr(&f), r2.first, intermediate2, provided2);
    ASSERT_TRUE(r4.second);
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
  Vertex testEV = *v.first;
  const Transformation& testE = *g[testEV].trans();
  EXPECT_EQ(testE, e);
  auto out = out_edges(testEV, g);
  ASSERT_NE(out.first, out.second);
  Edge testBE = *out.first;
  Vertex testBV = target(testBE, g);
  const Transformation& testB = *g[testBV].trans();
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(testB, *b);
  Edge testFE = *++out.first;
  Vertex testFV = target(testFE, g);
  const Transformation& testF = *g[testFV].trans();
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(testF, f);
}

TEST_F(CompositeTransformSuite, treeInTest) {
  auto result = compTrans2.in();
  ASSERT_FALSE(result.empty());
  EXPECT_EQ(result, EventTypes({provided, provided2}));
}

TEST_F(CompositeTransformSuite, linearCreateTest) {
  using Events = Transformer::Events;
  EXPECT_CALL(a, create(goal, EventTypes({intermediate})))
    .Times(1).WillOnce(Return(TransPtr(&c, [](const Transformer*){})));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided})))
    .Times(1).WillOnce(Return(TransPtr(&d, [](const Transformer*){})));
  EXPECT_CALL(c, print(_)).Times(1);
  EXPECT_CALL(d, print(_)).Times(1);
  TransPtr result = compTrans.create();
  ASSERT_NE(result, nullptr);
  path file = current_path()/"doc"/"linearComp.dot";
  ofstream out(file);
  out << *result;
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

TEST_F(CompositeTransformSuite, treeCreateTest) {
  using Events = Transformer::Events;
  EXPECT_CALL(e, create(goal, EventTypes({intermediate, intermediate2})))
    .Times(1).WillOnce(Return(TransPtr(&c, [](const Transformer*){})));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided})))
    .Times(1).WillOnce(Return(TransPtr(&d, [](const Transformer*){})));
  EXPECT_CALL(f, create(intermediate2, EventTypes({provided2})))
    .Times(1).WillOnce(Return(TransPtr(&g, [](const Transformer*){})));
  EXPECT_CALL(c, print(_)).Times(1);
  EXPECT_CALL(d, print(_)).Times(1);
  EXPECT_CALL(g, print(_)).Times(1);
  TransPtr result = compTrans2.create();
  ASSERT_NE(result, nullptr);
  path file = current_path()/"doc"/"treeComp.dot";
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
  EXPECT_CALL(c, call(Events({&eA})))
    .Times(1).WillOnce(Return(eC));
  EXPECT_CALL(d, call(Events({&eB})))
    .Times(1).WillOnce(Return(eD));
  EXPECT_CALL(g, call(Events({&eC, &eD})))
    .Times(1).WillOnce(Return(eE));
  EXPECT_EQ((*result)(Events({&eA, &eB})), eE);
}

}
