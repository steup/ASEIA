
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <KnowledgeBase.h>
#include <AttributeType.h>
#include <Filter.h>
#include <Attribute.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <ratio>
#include <algorithm>
#include <iostream>

namespace test {

 namespace fs = boost::filesystem;
 using namespace ::id::attribute;
 using std::ratio;
 using std::cout;
 using std::ostream;
 using std::endl;
 using std::hex;
 using std::vector;
 using std::any_of;
 using std::to_string;
 using std::string;
 using std::transform;
 using boost::filesystem::ofstream;
 using boost::filesystem::current_path;
 using boost::filesystem::path;
 using boost::vertices;

 using ::testing::Each;
 using ::testing::Contains;
 using ::testing::SizeIs;
 using ::testing::AtLeast;
 using ::testing::ResultOf;
 using ::testing::Property;
 using ::testing::UnorderedElementsAre;

  struct KnowledgeBaseTestSuite : public ::testing::Test {
    
    using  Transformations = KnowledgeBase::Transformations;

    using Test0 = ::id::attribute::AttrID<250>;
    using Test1 = ::id::attribute::AttrID<251>;
    using Test2 = ::id::attribute::AttrID<252>;
    using Test3 = ::id::attribute::AttrID<253>;
    using Test4 = ::id::attribute::AttrID<254>;
    using Test5 = ::id::attribute::AttrID<255>;
    struct HetTrans0 : public Transformation {
      const EventType& outE;
      const EventType& inE;
      const string name;
      HetTrans0(const EventType& outE, const EventType& inE, const string& name)
        : Transformation(Type::heterogeneus, 1, EventID({Test2::value()})), outE(outE), inE(inE), name(name)
      {}
      virtual EventIDs in(EventID goal, const MetaFilter& filter = MetaFilter()) const  {
        return EventIDs({ EventID({Test3::value()}) });
      }
      virtual vector<EventType> in(const EventType& goal, const EventType& provided, const MetaFilter& filter = MetaFilter()) const  {
        if(goal == outE)
          return {inE};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << name; }
    };

    struct HetTrans1 : public Transformation {
      const EventType& outE;
      const EventType& in0;
      const EventType& in1;
      const string name;
      HetTrans1(const EventType& outE, const EventType& in0, const EventType& in1, const string& name)
        : Transformation(Type::heterogeneus, 2, EventID({Test0::value()})), outE(outE), in0(in0), in1(in1), name(name)

      {}
      virtual EventIDs in(EventID goal, const MetaFilter& filter = MetaFilter()) const  {
        return EventIDs({ EventID({Test1::value()}), EventID({Test2::value()})});
      }
      virtual vector<EventType> in(const EventType& goal, const EventType& provided, const MetaFilter& filter = MetaFilter()) const  {
        if(goal == outE)
          return {in0, in1};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << name; }
    };

    struct AttrTrans0 : public Transformation {
      const EventType& outE;
      const EventType& inE;
      const EventType& p;
      const string name;
      AttrTrans0(const EventType& outE, const EventType& inE, const EventType& p, const string& name)
        : Transformation(Type::attribute, 1, EventID::any), outE(outE), inE(inE), p(p), name(name)

      {}
      virtual EventIDs in(EventID goal, const MetaFilter& filter = MetaFilter()) const  {
        return EventIDs({ goal });
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided, const MetaFilter& filter = MetaFilter()) const  {
        if(goal == outE && provided == p)
          return {inE};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << name; }
    };

    struct AttrTrans1 : public Transformation {
      const EventType& outE;
      const EventType& in0;
      const EventType& in1;
      const EventType& p;
      const string name;
      AttrTrans1(const EventType& outE, const EventType& in0, const EventType& in1, const EventType& p, const string& name)
        : Transformation(Type::attribute, 2, EventID::any), outE(outE), in0(in0), in1(in1), p(p), name(name)

      {}
      virtual EventIDs in(EventID goal, const MetaFilter& filter = MetaFilter()) const  {
        return EventIDs({ goal, EventID({Test4::value()})});
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided, const MetaFilter& filter = MetaFilter()) const  {
        if(goal == outE && provided == p)
          return {in0, in1};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << name; }
    };

    struct HomTrans1 : public Transformation {
      const EventType& inOutE;
      const EventType& in1;
      const string name;
      HomTrans1(const EventType& inOutE, const EventType& in1, const string& name)
        : Transformation(Type::homogeneus, 2, EventID::any), inOutE(inOutE), in1(in1), name(name)

      {}
      bool containsUncertaintyTest(const MetaFilter& filter) const {
        for(const auto& predicate : filter.expressions())
          for(const auto& func : predicate.first.func())
            if(func==&MetaAttribute::uncertainty)
              return true;
        return false;
      }
      virtual EventIDs in(EventID goal, const MetaFilter& filter = MetaFilter()) const  {
        if(!containsUncertaintyTest(filter))
          return {};
        return EventIDs({ goal, EventID({Test4::value()})});
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided, const MetaFilter& filter = MetaFilter()) const  {
        if(!containsUncertaintyTest(filter))
          return {};
        if(goal == inOutE)
          return {inOutE, in1};
        else
          return {};
      }
      virtual TransPtr create(const EventType& outE, const EventTypes& inE, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << name; }
    };

    EventType eT0, eT1, eT2, eT3, eT4, eT5, eT6, eT7, eT8;

    HetTrans0 het0=HetTrans0(eT2, eT3, "Het [eT3] -> eT2");
    HetTrans1 het1=HetTrans1(eT0, eT1, eT2, "Het [eT1, eT2] -> eT0");
    AttrTrans0 attr0=AttrTrans0(eT3, eT4, eT4, "Attr [eT4] -> eT3(eT4)");
    AttrTrans1 attr1=AttrTrans1(eT1, eT5, eT6, eT5, "Attr [eT5, eT6] -> eT1(eT5)");
    AttrTrans0 attr2=AttrTrans0(eT4, eT7, eT7, "Attr [eT7] -> eT4(eT7)");
    AttrTrans0 attr3=AttrTrans0(eT3, eT4, eT7, "Attr [eT4] -> eT3(eT7)");
    HomTrans1  hom0=HomTrans1(eT0, eT8, "Hom [eT0, eT8] -> eT0");


    KnowledgeBaseTestSuite() {
      ValueType v(id::type::Float::value(), 1, 1, false);
      ValueType v2(id::type::UInt32::value(), 1, 1, false);
      KnowledgeBase::clear();
      KnowledgeBase::registerTransformation(het0);
      KnowledgeBase::registerTransformation(het1);
      KnowledgeBase::registerTransformation(attr0);
      KnowledgeBase::registerTransformation(attr1);
      KnowledgeBase::registerTransformation(attr2);
      KnowledgeBase::registerTransformation(attr3);
      KnowledgeBase::registerTransformation(hom0);
      eT0.add(AttributeType(Test0::value(), v, Scale<>(), Dimensionless()));
      eT1.add(AttributeType(Test1::value(), v, Scale<>(), Dimensionless()));
      eT2.add(AttributeType(Test2::value(), v, Scale<>(), Dimensionless()));
      eT3.add(AttributeType(Test3::value(), v, Scale<>(), Dimensionless()));
      eT4.add(AttributeType(Test3::value(), v, Scale<ratio<1, 1000>>(), Dimensionless()));
      eT5.add(AttributeType(Test1::value(), v, Scale<ratio<1, 1>, 1>(), Dimensionless()));
      eT6.add(AttributeType(Test1::value(), v, Scale<ratio<1, 1>, 1>(), Dimensionless()));
      eT6.add(AttributeType(Test4::value(), v, Scale<ratio<1, 1>, 0>(), Dimensionless()));
      eT7.add(AttributeType(Test3::value(), v2, Scale<ratio<1, 1000>>(), Dimensionless()));
      eT8.add(AttributeType(Test5::value(), v, Scale<>(), Dimensionless()));
      path file = current_path()/"doc"/"kb.dot";
      ofstream out(file);
      //KnowledgeBase::print(out);
    }
  };

  using ConfiguredTransformation = CompositeTransformation::ConfiguredTransformation;

  vector<ConfiguredTransformation> getTrans(const CompositeTransformation& t) {
    vector<ConfiguredTransformation> result;
    const CompositeTransformation::Graph& g = t.graph();
    auto func = [&g](const CompositeTransformation::Vertex v){ return g[v]; };
    transform(vertices(g).first, vertices(g).second, back_inserter(result), func);
    return result;
  }

  TEST_F(KnowledgeBaseTestSuite, singleHeterogeneusTransform) {
    EXPECT_EQ(EventID(eT2), het0.out()) << "Wrong Output ID";
    EXPECT_EQ(het0.arity(), 1U) << "Wrong arity";
    ASSERT_EQ(het0.in(EventID(eT2)).size(), 1U) << "Wrong number of input IDs";
    EXPECT_EQ(EventID(eT3), het0.in(EventID(eT2)).front()) << "Wrong input ID";
    ASSERT_EQ(het0.in(eT2).size(), 1U) << "Wrong number of input Types";
    EXPECT_EQ(eT3, het0.in(eT2, EventType()).front()) << "Wrong input Type";
  }

  TEST_F(KnowledgeBaseTestSuite, singleHomogeneusTransform) {
    EXPECT_EQ(EventID::any, attr0.out()) << "Wrong Output ID";
    EXPECT_EQ(attr0.arity(), 1U) << "Wrong arity";
    ASSERT_EQ(attr0.in(EventID(eT3)).size(), 1U) << "Wrong number of input IDs";
    EXPECT_EQ(EventID(eT4), attr0.in(EventID(eT3)).front()) << "Wrong input ID";
    ASSERT_EQ(attr0.in(eT3, eT4).size(), 1U) << "Wrong number of input Types";
    EXPECT_EQ(eT4, attr0.in(eT3, eT4).front()) << "Wrong input Type";
  }

  TEST_F(KnowledgeBaseTestSuite, findSingleHeterogeneusTransform) {
    KnowledgeBase::registerEventType(eT3);
    Transformations ts = KnowledgeBase::findTransforms(eT2);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbSingleHet.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het0) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT3)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT2) ));
  }

  TEST_F(KnowledgeBaseTestSuite, findSingleAttributeTransform) {
    KnowledgeBase::registerEventType(eT4);
    Transformations ts = KnowledgeBase::findTransforms(eT3);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbSingleAttr.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr0) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT4)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT3) ));
  }

  TEST_F(KnowledgeBaseTestSuite, findMultiHeterogeneusTransform) {
    KnowledgeBase::registerEventType(eT1);
    KnowledgeBase::registerEventType(eT2);
    Transformations ts = KnowledgeBase::findTransforms(eT0);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbMultiHet.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het1) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT1, eT2)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT0) ));
  }

  TEST_F(KnowledgeBaseTestSuite, findMultiAttributeTransform) {
    KnowledgeBase::registerEventType(eT5);
    KnowledgeBase::registerEventType(eT6);
    Transformations ts = KnowledgeBase::findTransforms(eT1);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbMultiAttr.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr1) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT5, eT6)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT1) ));
  }

  TEST_F(KnowledgeBaseTestSuite, findCombinedAttributeTransform) {
    KnowledgeBase::registerEventType(eT7);
    Transformations ts = KnowledgeBase::findTransforms(eT3);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbCombinedAttr.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr2) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr3) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT7)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT3) ));
  }

  TEST_F(KnowledgeBaseTestSuite, findCombinedHeterogeneusTransform) {
    KnowledgeBase::registerEventType(eT1);
    KnowledgeBase::registerEventType(eT3);
    Transformations ts = KnowledgeBase::findTransforms(eT0);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbCombinedHet.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het0) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het1) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT1, eT3)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT0) ));
  }

  TEST_F(KnowledgeBaseTestSuite, fullTree) {
    KnowledgeBase::registerEventType(eT7);
    KnowledgeBase::registerEventType(eT5);
    KnowledgeBase::registerEventType(eT6);
    Transformations ts = KnowledgeBase::findTransforms(eT0);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbFullTree.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het0) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het1) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr1) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr2) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr3) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT7, eT5, eT6)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT0) ));
  }

  TEST_F(KnowledgeBaseTestSuite, fullTreeWithHom) {
    auto filter0 = filter::uncertainty(filter::e0[Test0()]) < Value<float, 1, 1, false>({{{ 10 }}});
    auto filterExpr = filter0(filter::s0);
    vector<uint8_t> buffer(decltype(filterExpr)::size());
    Serializer<decltype(buffer.begin())> s(buffer.begin());
    s << filterExpr;
    MetaFilter metaFilter({&eT0});
    DeSerializer<decltype(buffer.cbegin())> d(buffer.cbegin(), buffer.cend());
    EXPECT_NO_THROW(d >> metaFilter);
    KnowledgeBase::registerEventType(eT7);
    KnowledgeBase::registerEventType(eT5);
    KnowledgeBase::registerEventType(eT6);
    KnowledgeBase::registerEventType(eT8);
    Transformations ts = KnowledgeBase::findTransforms(eT0, metaFilter);
    ASSERT_THAT(ts, SizeIs(1));
    fs::path file = fs::current_path()/"doc"/("kbFullTreeWithHom.dot");
    fs::ofstream out(file);
    out << ts[0];
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het0) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &het1) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr1) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr2) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &attr3) ) ) ) );
    EXPECT_THAT(ts, Each( ResultOf(getTrans, Contains( Property(&ConfiguredTransformation::trans, &hom0) ) ) ) );
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::in, UnorderedElementsAre(eT7, eT5, eT6, eT8)) ));
    EXPECT_THAT(ts, Each( Property(&CompositeTransformation::out, eT0) ));
  }
}
