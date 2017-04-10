
#include <gtest/gtest.h>
#include <KnowledgeBase.h>
#include <AttributeType.h>

#include <boost/filesystem.hpp>

#include <ratio>
#include <algorithm>
#include <iostream>
#include <boost/filesystem/fstream.hpp>

namespace test {

 using namespace ::id::attribute;
 using std::ratio;
 using std::cout;
 using std::endl;
 using std::vector;
 using std::any_of;
 using boost::filesystem::ofstream;
 using boost::filesystem::current_path;
 using boost::filesystem::path;


  struct KnowledgeBaseTestSuite : public ::testing::Test {

    struct Test0: public ::id::attribute::Base {
      static constexpr const ::id::attribute::ID value()  { return 251; }
    };

    struct Test1 : public ::id::attribute::Base {
      static constexpr const ::id::attribute::ID value()  { return 252; }
    };

    struct Test2 : public ::id::attribute::Base {
      static constexpr const ::id::attribute::ID value()  { return 253; }
    };

    struct Test3 : public ::id::attribute::Base {
      static constexpr const ::id::attribute::ID value()  { return 254; }
    };

    struct Test4 : public ::id::attribute::Base {
      static constexpr const ::id::attribute::ID value() { return 255; }
    };

    struct HetTrans0 : public Transformation {
      const EventType& outE;
      const EventType& inE;
      HetTrans0(const EventType& outE, const EventType& inE)
        : Transformation(Type::heterogeneus, 1, EventID({Test2::value()})), outE(outE), inE(inE)
      {}
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ EventID({Test3::value()}) });
      }
      virtual vector<EventType> in(const EventType& goal) const  {
        if(goal == outE)
          return {inE};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Heterogeneus Transform 1"; }
    };

    struct HetTrans1 : public Transformation {
      const EventType& outE;
      const EventType& in0;
      const EventType& in1;
      HetTrans1(const EventType& outE, const EventType& in0, const EventType& in1)
        : Transformation(Type::heterogeneus, 2, EventID({Test0::value()})), outE(outE), in0(in0), in1(in1)
      {}
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ EventID({Test1::value()}), EventID({Test2::value()})});
      }
      virtual vector<EventType> in(const EventType& goal) const  {
        if(goal == outE)
          return {in0, in1};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Heterogeneus Transform 2"; }
    };

    struct HomTrans0 : public Transformation {
      const EventType& outE;
      const EventType& inE;
      const EventType& p;
      HomTrans0(const EventType& outE, const EventType& inE, const EventType& p)
        : Transformation(Type::attribute, 1, EventID::any), outE(outE), inE(inE), p(p)
      {}
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ goal });
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided) const  {
        if(goal == outE && provided == p)
          return {inE};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Homogeneus Transform 1"; }
    };

    struct HomTrans1 : public Transformation {
      const EventType& outE;
      const EventType& in0;
      const EventType& in1;
      const EventType& p;
      HomTrans1(const EventType& outE, const EventType& in0, const EventType& in1, const EventType& p)
        : Transformation(Type::attribute, 2, EventID::any), outE(outE), in0(in0), in1(in1), p(p)
      {}
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ goal, EventID({Test4::value()})});
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided) const  {
        if(goal == outE && provided == p)
          return {in0, in1};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Homogeneus Transform 2"; }
    };

    EventType eT0, eT1, eT2, eT3, eT4, eT5, eT6, eT7;

    HetTrans0 het0=HetTrans0(eT2, eT3);
    HetTrans1 het1=HetTrans1(eT0, eT1, eT2);
    HomTrans0 hom0=HomTrans0(eT3, eT4, eT4);
    HomTrans1 hom1=HomTrans1(eT1, eT5, eT6, eT5);
    HomTrans0 hom2=HomTrans0(eT4, eT7, eT7);
    HomTrans0 hom3=HomTrans0(eT3, eT4, eT7);


    KnowledgeBaseTestSuite() {
      ValueType v(id::type::Float::value(), 1, 1, false);
      ValueType v2(id::type::UInt64::value(), 1, 1, false);
      KnowledgeBase::clear();
      KnowledgeBase::registerTransformation(het0);
      KnowledgeBase::registerTransformation(het1);
      KnowledgeBase::registerTransformation(hom0);
      KnowledgeBase::registerTransformation(hom1);
      KnowledgeBase::registerTransformation(hom2);
      KnowledgeBase::registerTransformation(hom3);
      eT0.add(AttributeType(Test0::value(), v, Scale<>(), Dimensionless()));
      eT1.add(AttributeType(Test1::value(), v, Scale<>(), Dimensionless()));
      eT2.add(AttributeType(Test2::value(), v, Scale<>(), Dimensionless()));
      eT3.add(AttributeType(Test3::value(), v, Scale<>(), Dimensionless()));
      eT4.add(AttributeType(Test3::value(), v, Scale<ratio<1, 1000>>(), Dimensionless()));
      eT5.add(AttributeType(Test1::value(), v, Scale<ratio<1, 1>, 1>(), Dimensionless()));
      eT6.add(AttributeType(Test1::value(), v, Scale<ratio<1, 1>, 0>(), Dimensionless()));
      eT6.add(AttributeType(Test4::value(), v, Scale<ratio<1, 1>, 1>(), Dimensionless()));
      eT7.add(AttributeType(Test3::value(), v2, Scale<ratio<1, 1000>>(), Dimensionless()));
      path file = current_path()/"doc"/"kb.dot";
      ofstream out(file);
      KnowledgeBase::print(out);
    }

    using  Transformations = KnowledgeBase::Transformations;

    bool checkResult(const Transformations& ts, const Transformation & t) {
      auto pred = [&t](const CompositeTransformation& cT) {
        return *cT.graph()[cT.root()].trans() == t;
      };
      return any_of(ts.begin(), ts.end(), pred);
    }
  };

  TEST_F(KnowledgeBaseTestSuite, singleHeterogeneusTransform) {
    EXPECT_EQ(EventID(eT2), het0.out()) << "Wrong Output ID";
    EXPECT_EQ(het0.arity(), 1U) << "Wrong arity";
    ASSERT_EQ(het0.in(EventID(eT2)).size(), 1U) << "Wrong number of input IDs";
    EXPECT_EQ(EventID(eT3), het0.in(EventID(eT2)).front()) << "Wrong input ID";
    ASSERT_EQ(het0.in(eT2).size(), 1U) << "Wrong number of input Types";
    EXPECT_EQ(eT3, het0.in(eT2).front()) << "Wrong input Type";
  }

  TEST_F(KnowledgeBaseTestSuite, singleHomogeneusTransform) {
    EXPECT_EQ(EventID::any, hom0.out()) << "Wrong Output ID";
    EXPECT_EQ(hom0.arity(), 1U) << "Wrong arity";
    ASSERT_EQ(hom0.in(EventID(eT3)).size(), 1U) << "Wrong number of input IDs";
    EXPECT_EQ(EventID(eT4), hom0.in(EventID(eT3)).front()) << "Wrong input ID";
    ASSERT_EQ(hom0.in(eT3, eT4).size(), 1U) << "Wrong number of input Types";
    EXPECT_EQ(eT4, hom0.in(eT3, eT4).front()) << "Wrong input Type";
  }

  TEST_F(KnowledgeBaseTestSuite, findSingleHeterogeneusTransform) {
    KnowledgeBase::registerEventType(eT3);
    Transformations ts = KnowledgeBase::findTransforms(eT2);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
    EXPECT_TRUE(checkResult(ts, het0)) << "Searched Transform not found";
  }

  TEST_F(KnowledgeBaseTestSuite, findSingleHomogeneusTransform) {
    KnowledgeBase::registerEventType(eT4);
    Transformations ts = KnowledgeBase::findTransforms(eT3);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
    EXPECT_TRUE(checkResult(ts, hom0)) << "Searched Transform not found";
  }

  TEST_F(KnowledgeBaseTestSuite, findMultiHeterogeneusTransform) {
    KnowledgeBase::registerEventType(eT1);
    KnowledgeBase::registerEventType(eT2);
    Transformations ts = KnowledgeBase::findTransforms(eT0);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
    EXPECT_TRUE(checkResult(ts, het1)) << "Searched Transform not found";
  }

  TEST_F(KnowledgeBaseTestSuite, findMultiHomogeneusTransform) {
    KnowledgeBase::registerEventType(eT5);
    KnowledgeBase::registerEventType(eT6);
    Transformations ts = KnowledgeBase::findTransforms(eT1);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
    EXPECT_TRUE(checkResult(ts, hom1)) << "Searched Transform not found";
  }

  TEST_F(KnowledgeBaseTestSuite, findCombinedAttributeTransform) {
    KnowledgeBase::registerEventType(eT7);
    Transformations ts = KnowledgeBase::findTransforms(eT3);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
    TransformationPtr hom2Ptr(&hom2);
    TransformationPtr hom3Ptr(&hom3);
    auto check = [hom2Ptr, hom3Ptr](const CompositeTransformation& cT){
      const CompositeTransformation::Graph& g = cT.graph();
      auto pred0 = [hom2Ptr, &g](CompositeTransformation::Vertex v){ return g[v].trans()==hom2Ptr; };
      auto pred1 = [hom3Ptr, &g](CompositeTransformation::Vertex v){ return g[v].trans()==hom3Ptr; };
      auto vertices = boost::vertices(g);
      return any_of(vertices.first, vertices.second, pred0) &&
             any_of(vertices.first, vertices.second, pred1);
    };
    EXPECT_TRUE(any_of(ts.begin(), ts.end(), check));
  }

  TEST_F(KnowledgeBaseTestSuite, findCombinedHeterogeneusTransform) {
    KnowledgeBase::registerEventType(eT1);
    KnowledgeBase::registerEventType(eT3);
    Transformations ts = KnowledgeBase::findTransforms(eT0);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
  }

  TEST_F(KnowledgeBaseTestSuite, fullTree) {
    KnowledgeBase::registerEventType(eT7);
    KnowledgeBase::registerEventType(eT5);
    KnowledgeBase::registerEventType(eT6);
    Transformations ts = KnowledgeBase::findTransforms(eT0);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
  }
}
