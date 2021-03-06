
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <CompositeTransformation.h>
#include <MetaEvent.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

/** \class CompositeTransformation
 *
 *  \test CompositeTransformSuite testing creation of linear and tree-like
 *  CompositeTransformations. Additionally checks correctness of input
 *  EventTypes and created CompositeTransformers. Finally tests correct merging
 *  of two CompositeTransformations in ::tests::compTrans
 **/

namespace tests {
/** \brief CompositeTransformation Tests **/
namespace compTrans {
using namespace std;

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::UnorderedElementsAre;
using boost::filesystem::ofstream;
using boost::filesystem::current_path;
using boost::filesystem::path;

/** \brief Fixture for all CompositeTransformation UnitTests **/
struct CompositeTransformSuite : public ::testing::Test {

  /** \brief forward declaration of EventTypes **/
  using EventTypes = Transformer::EventTypes;
  /** \brief forward declaration of TransPtr **/
  using TransPtr   = Transformation::TransPtr;

  /** \brief helper function to compare MetaEvents based on their pointers
   *  \param a first MetaEvent pointer
   *  \param b second MetaEvent pointer
   *  \return true if equal according to MetaEvent::operator==, false otherwise
   **/
  static bool comp(const MetaEvent* const a, const MetaEvent* const b) {
    return *a==*b;
  }

  /// Mock-Implementation of Unary Transformer
  struct TestTransformer : public Transformer {
    /// Name of this Mock-Object
    string name;
    /** \brief Create Mock-Transformer
     *  \param name Name of this Mock-Object
     *  \param out Goal EventType
     *  \param in single elementary vector of input EventTypes
     *  \param trans Transformer lookup structure to register this instance
     **/
    TestTransformer(const string& name, const EventType& out, const EventTypes& in,
                    map<string, TestTransformer*>& trans)
      : Transformer(out, in), name(name) {
      trans[name]=this;
    }
    /// Mock-Implementation of input Event check 
    MOCK_CONST_METHOD1(check, bool(const MetaEvent&));
    /// Mock-Implementation of input Event processing 
    MOCK_CONST_METHOD1(call, Events(const MetaEvent&));
    /** \brief Forward declaration to enable Mock-Implementation
     *  \param e Input Event
     **/
    virtual Events operator()(const MetaEvent& e) { return call(e); }
    /** \brief implemented formatted output
     *  \param o ostream to output to
     **/
    virtual void print(ostream& o) const { o << name; }
  };

  /// Mock-Implementation of Unary Transformation
  struct TestTransformation : public Transformation {
    /// Name of this Mock-Object
    string name;
    /** \brief Create Mock-Transformation
     *  \param name Name of this Mock-Object
     *  \param arity Arity of transformation
     **/
    TestTransformation(const string& name, size_t arity)
      : Transformation(Type::attribute, arity, EventID::any), name(name) { }

    /// Mock-Implementation of input EventID generation 
    MOCK_CONST_METHOD3(in, EventTypes(const EventType& goal, const EventType& provided,
                                      const MetaFilter& filter));
    /// Mock-Implementation of input EventType generation 
    MOCK_CONST_METHOD2(in, EventIDs(EventID goal, const MetaFilter& filter));
    /// Mock-Implementation of TestTransformer creation 
    MOCK_CONST_METHOD4(create, TransPtr(const EventType& out, const EventTypes& in,
                                        const AbstractPolicy& policy, const MetaFilter& filter));
    /** \brief implemented formatted output
     *  \param o ostream to output to
     **/
    virtual void print(ostream& o) const { o << name; }
  };

  /// Lookup structure to map TestTransformation names to instances
  map<string, TestTransformer*> trans;
  /// Statically allocated unary TestTransformations
  TestTransformation a, c, d, e;
  /// Linear CompositeTransformation containing a and b
  CompositeTransformation compTrans;
  /// Tree-like CompositeTransformation containing c, b and d and a filter expression
  CompositeTransformation compTransNoFilter;
  /// Tree-like CompositeTransformation containing c, b and d
  CompositeTransformation compTrans2;
  /// Single CompositeTransformation containing e
  CompositeTransformation compTrans3;
  /// Dynamically allocated unary TestTransformation
  shared_ptr<const TestTransformation> b;
  /// Final goal EventType: single Attribute Test0 of certain 1x1 float
  EventType goal;
  /// Provided EventType: single Attribute Test0 of certain 1x1 int with scale 1/1000
  EventType provided;
  /// Provided EventType: single Attribute Test1 of certain 1x1 int with scale 1/1000
  EventType provided2;
  /// Intermediate EventType: single Attribute Test0 of certain 1x1 float with scale 1/1000
  EventType intermediate;
  /// Intermediate EventType: single Attribute Test1 of certain 1x1 float with scale 1/1000
  EventType intermediate2;
  /// Simple MetaFilter used to initialize CompositeTransformations
  MetaFilter metaFilter;

  /// Definition of non-static AttrID used only testing
  using Test0 = ::id::attribute::AttrID<251>;
  /// Definition of non-static AttrID used only testing
  using Test1 = ::id::attribute::AttrID<252>;

  /// Create four TestTransformations
  CompositeTransformSuite()
    : a("a", 1), c("c", 2), d("d", 1),  e("e", 2), b(new TestTransformation("b", 1))
  {}

  /// Create CompositeTransformations compTrans and compTrans2
  void SetUp() {
    ValueType goalVT(id::type::Float::value(), 1, 1, false);

    AttributeType goalAT(Test0::value(), goalVT, Scale<>(), Dimensionless());
    AttributeType int2AT(Test1::value(), goalVT, Scale<std::ratio<1, 1000>>(),
                         Dimensionless());
    AttributeType intAT = goalAT;
    AttributeType provAT = intAT;
    AttributeType prov2AT = int2AT;


    intAT.scale().denom(1000);
    provAT.scale().denom(1000);
    provAT.value().typeId(id::type::Int32::value());
    prov2AT.value().typeId(id::type::Int32::value());

    goal.add(goalAT);
    intermediate.add(intAT);
    provided.add(provAT);
    intermediate2.add(int2AT);
    provided2.add(prov2AT);

    metaFilter = MetaFilter({&goal});
    {
      auto filter0 = filter::e0[Test0()] > Value<float, 1, 1, false>({{{ 10 }}});
      vector<uint8_t> buffer;
      auto i = back_inserter(buffer);
      Serializer<decltype(i)> s(i);
      s << filter0(filter::s0);
      DeSerializer<decltype(buffer.cbegin())> d(buffer.cbegin(), buffer.cend());
      EXPECT_NO_THROW(d >> metaFilter);
    }
    ASSERT_NE(b, nullptr);
    EXPECT_CALL(a, in(goal, provided, metaFilter))
      .Times(1).WillOnce(Return(EventTypes({intermediate})));
    EXPECT_CALL(*b, in(intermediate, provided, metaFilter))
      .Times(2).WillRepeatedly(Return(EventTypes({provided})));
    EXPECT_CALL(a, in(goal, provided, MetaFilter()))
      .Times(1).WillOnce(Return(EventTypes({intermediate})));
    EXPECT_CALL(*b, in(intermediate, provided, MetaFilter()))
      .Times(1).WillRepeatedly(Return(EventTypes({provided})));
    EXPECT_CALL(c, in(goal, _, _))
      .Times(1).WillOnce(Return(EventTypes({intermediate, intermediate2})));
    EXPECT_CALL(d, in(intermediate2, provided2, _))
      .Times(1).WillOnce(Return(EventTypes({provided2})));
    EXPECT_CALL(e, in(goal, _, _))
      .Times(1).WillOnce(Return(EventTypes({goal, intermediate})));
    auto r0 = compTrans.add(&a, goal, provided, metaFilter);
    ASSERT_TRUE(r0.second);
    auto r1 = compTrans.add(b.get(), r0.first, intermediate, provided);
    ASSERT_TRUE(r1.second);
    ofstream out(current_path()/"doc"/"filteredLinTransformation.dot");
    out << compTrans;
    r0 = compTransNoFilter.add(&a, goal, provided);
    ASSERT_TRUE(r0.second);
    r1 = compTransNoFilter.add(b.get(), r0.first, intermediate, provided);
    ASSERT_TRUE(r1.second);
    out.close();
    out.open(current_path()/"doc"/"linTransformation.dot");
    out << compTrans;

    auto r2 = compTrans2.add(&c, goal, EventType(), metaFilter);
    ASSERT_TRUE(r2.second);
    auto r3 = compTrans2.add(b.get(), r2.first, intermediate, provided);
    ASSERT_TRUE(r3.second);
    auto r4 = compTrans2.add(&d, r2.first, intermediate2, provided2);
    ASSERT_TRUE(r4.second);
    out.close();
    out.open(current_path()/"doc"/"treeTransformation.dot");
    out << compTrans2;
    auto r5 = compTrans3.add(&e, goal);
    ASSERT_TRUE(r5.second);

    EXPECT_EQ(metaFilter, compTrans.filter());
    EXPECT_EQ(metaFilter, compTrans2.filter());
  }
};

/** \brief Unit-Test testing creation of a linear CompositeTransformation **/
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

/** \brief Unit-Test checking correctness of input EventTypes of a linear
 * CompositeTransformation**/
TEST_F(CompositeTransformSuite, linearInTest) {
  auto result = compTrans.in();
  EXPECT_EQ(result, EventTypes({provided}));
}

/** \brief Unit-Test testing creation of a tree-like CompositeTransformation **/
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

/** \brief Unit-Test checking correctness of input EventTypes of a tree-like
 * CompositeTransformation**/
TEST_F(CompositeTransformSuite, treeInTest) {
  auto result = compTrans2.in();
  ASSERT_FALSE(result.empty());
  EXPECT_EQ(result, EventTypes({provided, provided2}));
}

TEST_F(CompositeTransformSuite, eqTest) {
  EXPECT_FALSE(compTrans == compTrans2);
  EXPECT_FALSE(compTrans == compTrans3);
  EXPECT_FALSE(compTrans2 == compTrans3);
  EXPECT_FALSE(compTrans2 == compTrans);
  EXPECT_FALSE(compTrans3 == compTrans);
  EXPECT_FALSE(compTrans3 == compTrans2);
}

/** \brief Unit-Test checking correct creation of CompositeTransformer from
 * linear CompositeTransformation **/
TEST_F(CompositeTransformSuite, linearCreateTest) {
  using Events = Transformer::Events;
  EXPECT_CALL(a, create(goal, EventTypes({intermediate}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("a", goal, EventTypes({intermediate}), trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  TransPtr result = compTrans.create(AbstractPolicy());
  ASSERT_NE(result, nullptr);
  path file = current_path()/"doc"/"linearTransformer.dot";
  ofstream out(file);
  out << *result;
  {
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
  {
    MetaEvent eA(provided);
    eA.attribute(Test0::value())->value().set(0, 0, {0.0f});
    MetaEvent eB(intermediate);
    eB.attribute(Test0::value())->value().set(0, 0, {1.0f});
    MetaEvent eC(goal);
    eC.attribute(Test0::value())->value().set(0, 0, {0});
    EXPECT_CALL(*trans["b"], call(eA))
      .Times(1).WillOnce(Return(Events{eB}));
    EXPECT_CALL(*trans["a"], call(_))
      .WillRepeatedly(Return(Events{}));
    EXPECT_CALL(*trans["a"], call(eB))
      .Times(1).WillOnce(Return(Events{eC}));
    Events res0 = (*result)(eA);
    ASSERT_EQ(res0.size(), 0U) << "Event should be filtered" << result->filter();
  }
}

/** \brief Unit-Test checking correct creation of CompositeTransformer from
 * tree-like CompositeTransformation **/
TEST_F(CompositeTransformSuite, treeCreateTest) {
  using Events = Transformer::Events;
  EXPECT_CALL(c, create(goal, EventTypes({intermediate, intermediate2}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("c", goal, EventTypes({intermediate, intermediate2}),
                          trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  EXPECT_CALL(d, create(intermediate2, EventTypes({provided2}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("d", intermediate2, EventTypes({provided2}), trans))));
  TransPtr result = compTrans2.create(AbstractPolicy());
  ASSERT_NE(result, nullptr);
  path file = current_path()/"doc"/"treeTransformer.dot";
  ofstream out(file);
  out << *result;
  {
    MetaEvent eA(provided);
    eA.attribute(Test0::value())->value().set(0, 0, {1.0f});
    MetaEvent eB(provided2);
    eB.attribute(Test1::value())->value().set(0, 0, {2.0f});
    MetaEvent eC(intermediate);
    eC.attribute(Test0::value())->value().set(0, 0, {3.0f});
    MetaEvent eD(intermediate2);
    eD.attribute(Test1::value())->value().set(0, 0, {4.0f});
    MetaEvent eE(goal);
    eE.attribute(Test0::value())->value().set(0, 0, {50.0f});
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
  {
    MetaEvent eA(provided);
    eA.attribute(Test0::value())->value().set(0, 0, {10.0f});
    MetaEvent eB(provided2);
    eB.attribute(Test1::value())->value().set(0, 0, {20.0f});
    MetaEvent eC(intermediate);
    eC.attribute(Test0::value())->value().set(0, 0, {30.0f});
    MetaEvent eD(intermediate2);
    eD.attribute(Test1::value())->value().set(0, 0, {40.0f});
    MetaEvent eE(goal);
    eE.attribute(Test0::value())->value().set(0, 0, {0.0f});
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
    ASSERT_EQ(res0.size(), 0U) << "Event should be filtered by " << result->filter();
  }
}

/** \brief Unit-Test checking correct merging of single and tree-like
 * CompositeTransformation **/
TEST_F(CompositeTransformSuite, insertTest) {
  using Vertex = CompositeTransformation::Vertex;
  std::pair<Vertex, bool> r;
  ASSERT_NO_THROW(r = compTrans3.add(move(compTrans2)));
  ASSERT_TRUE(r.second);
  Vertex insertPoint = compTrans3.root();
  path file = current_path()/"doc"/"insertCompTrans.dot";
  ofstream out(file);
  out << compTrans3;
  ASSERT_EQ(out_degree(insertPoint, compTrans3.graph()), 1U);
  auto outEdges = boost::out_edges(insertPoint, compTrans3.graph());
  Vertex cV = boost::target(*outEdges.first, compTrans3.graph());
  ASSERT_EQ(out_degree(cV, compTrans3.graph()), 2U);
  outEdges = boost::out_edges(cV, compTrans3.graph());
  Vertex bV = boost::target(*outEdges.first, compTrans3.graph());
  Vertex dV = boost::target(*++outEdges.first, compTrans3.graph());
  EXPECT_EQ(compTrans3.graph()[cV].trans(), &c);
  EXPECT_EQ(compTrans3.graph()[bV].trans(), b.get());
  EXPECT_EQ(compTrans3.graph()[dV].trans(), &d);
  EXPECT_THAT(compTrans3.in(), UnorderedElementsAre(provided, provided2, intermediate));
}

TEST_F(CompositeTransformSuite, compTest) {
  EXPECT_CALL(a, create(goal, EventTypes({intermediate}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("a", goal, EventTypes({intermediate}), trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  TransPtr linearPtr = compTrans.create(AbstractPolicy());
  EXPECT_CALL(c, create(goal, EventTypes({intermediate, intermediate2}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("c", goal, EventTypes({intermediate, intermediate2}),
                          trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  EXPECT_CALL(d, create(intermediate2, EventTypes({provided2}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("d", intermediate2, EventTypes({provided2}), trans))));
  TransPtr treePtr = compTrans2.create(AbstractPolicy());
  EXPECT_EQ(*treePtr, *treePtr);
  EXPECT_EQ(compTrans2, compTrans2);
  EXPECT_EQ(compTrans, compTrans);
  EXPECT_EQ(*treePtr, compTrans2);
  EXPECT_EQ(*linearPtr, compTrans);
  EXPECT_FALSE(*treePtr == *linearPtr);
  EXPECT_FALSE(*treePtr == compTrans);
  EXPECT_FALSE(*linearPtr == compTrans2);
  EXPECT_FALSE(compTrans == compTrans2);
}

TEST_F(CompositeTransformSuite, compWithFilterTest) {
  EXPECT_CALL(a, create(goal, EventTypes({intermediate}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("a", goal, EventTypes({intermediate}), trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  TransPtr filteredLinearPtr = compTrans.create(AbstractPolicy());
  EXPECT_CALL(a, create(goal, EventTypes({intermediate}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("a", goal, EventTypes({intermediate}), trans))));
  ASSERT_NE(b, nullptr);
  EXPECT_CALL(*b, create(intermediate, EventTypes({provided}), _, _))
    .Times(1).WillOnce(Return(TransPtr(
      new TestTransformer("b", intermediate, EventTypes({provided}),trans))));
  TransPtr linearPtr = compTransNoFilter.create(AbstractPolicy());
  EXPECT_EQ(*filteredLinearPtr, compTrans);
  EXPECT_EQ(*linearPtr, compTransNoFilter);
  EXPECT_FALSE(*filteredLinearPtr == *linearPtr);
  EXPECT_FALSE(*filteredLinearPtr == compTransNoFilter);
  EXPECT_FALSE(*linearPtr == compTrans);
}
}}
