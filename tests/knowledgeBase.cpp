#include <KnowledgeBase.h>
#include <AttributeType.h>
#include <ratio>

namespace test {

 using namespace ::id::attribute;
 using std::ratio;
 using std::cout;
 using std::endl;
 using std::vector;

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

    struct HetTrans1 : public Transformation {
      HetTrans1() : Transformation(EventID({Test2::value()})) {}
      virtual std::size_t arity() const  { return 1; }
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ EventID({Test3::value()}) });
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided) const  {
        vector<EventType> result;
        return result;
      }
      virtual bool check(const EventType& out, const EventTypes& in) const  { return true; }
      virtual TransPtr create(const EventType& out, const EventTypes& in) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Heterogeneus Transform 1"; }
    } het0;

    struct HetTrans2 : public Transformation {
      HetTrans2() : Transformation(EventID({Test0::value()})) {}
      virtual std::size_t arity() const  { return 2; }
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ EventID({Test1::value()}), EventID({Test2::value()})});
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided) const  {
        vector<EventType> result;
        return result;
      }
      virtual bool check(const EventType& out, const EventTypes& in) const  { return true; }
      virtual TransPtr create(const EventType& out, const EventTypes& in) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Heterogeneus Transform 2"; }
    } het1;

    struct HomTrans1 : public Transformation {
      HomTrans1() : Transformation(EventID::any) {}
      virtual std::size_t arity() const  { return 1; }
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ goal });
      }
      virtual bool check(const EventType& out, const EventTypes& in) const  {
        for(const auto& a : out) {
          const AttributeType* b = in[0]->attribute(a.id());
          if(!b) return false;
          if(a.value() != b->value() || a.unit() != b->unit() || a.scale().reference() !=b->scale().reference())
            return false;
        }
        return true;
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided) const  {
        vector<EventType> result;
        return result;
      }
      virtual TransPtr create(const EventType& out, const EventTypes& in) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Homogeneus Transform 1"; }
    } hom0;



    struct HomTrans2 : public Transformation {
      HomTrans2() : Transformation(EventID::any) {}
      virtual std::size_t arity() const  { return 2; }
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ goal, EventID({Test4::value()})});
      }
      virtual bool check(const EventType& out, const EventTypes& in) const {
        for(const auto& a : out) {
          const AttributeType* b = in[0]->attribute(a.id());
          if(!b) return false;
          if(a.value() != b->value() || a.unit() != b->unit() ||
             a.scale().num() != b->scale().num() || a.scale().denom() != b->scale().denom() )
            return false;
        }

        uint32_t inRef = in[0]->attribute(Test1::value())->scale().reference();
        uint32_t outRef = out.attribute(Test1::value())->scale().reference();
        return in[1]->attribute(Test4::value())->scale().reference() != outRef &&
               in[1]->attribute(Test1::value())->scale().reference() != inRef;
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided) const  {
        vector<EventType> result;
        return result;
      }
      virtual TransPtr create(const EventType& out, const EventTypes& in) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Homogeneus Transform 2"; }
    } hom1;

    struct HomTrans3 : public Transformation {
      HomTrans3() : Transformation(EventID::any) {}
      virtual std::size_t arity() const  { return 1; }
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ goal });
      }
      virtual vector<EventType> in(const EventType& goal,  const EventType& provided) const  {
        vector<EventType> result;
        return result;
      }
      virtual bool check(const EventType& out, const EventTypes& in) const  {
        for(const auto& a : out) {
          const AttributeType* b = in[0]->attribute(a.id());
          if(!b) return false;
          if(a.value().cols() != b->value().cols() || a.value().rows() != b->value().rows() ||
             a.unit() != b->unit() || a.scale() !=b->scale())
            return false;
        }
        return true;
      }
      virtual TransPtr create(const EventType& out, const EventTypes& in) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Homogeneus Transform 3"; }
    } hom2;


    EventType eT0, eT1, eT2, eT3, eT4, eT5, eT6, eT7;

    KnowledgeBaseTestSuite() {
      ValueType v(id::type::Float::value(), 1, 1, false);
      ValueType v2(id::type::UInt64::value(), 1, 1, false);
      KnowledgeBase::clear();
      KnowledgeBase::registerTransformation(het0);
      KnowledgeBase::registerTransformation(het1);
      KnowledgeBase::registerTransformation(hom0);
      KnowledgeBase::registerTransformation(hom1);
      KnowledgeBase::registerTransformation(hom2);
      eT0.add(AttributeType(Test0::value(), v, Scale<>(), Dimensionless()));
      eT1.add(AttributeType(Test1::value(), v, Scale<>(), Dimensionless()));
      eT2.add(AttributeType(Test2::value(), v, Scale<>(), Dimensionless()));
      eT3.add(AttributeType(Test3::value(), v, Scale<>(), Dimensionless()));
      eT4.add(AttributeType(Test3::value(), v, Scale<ratio<1, 1000>>(), Dimensionless()));
      eT5.add(AttributeType(Test1::value(), v, Scale<ratio<1, 1>, 1>(), Dimensionless()));
      eT6.add(AttributeType(Test1::value(), v, Scale<ratio<1, 1>, 0>(), Dimensionless()));
      eT6.add(AttributeType(Test4::value(), v, Scale<ratio<1, 1>, 1>(), Dimensionless()));
      eT7.add(AttributeType(Test3::value(), v2, Scale<ratio<1, 1000>>(), Dimensionless()));
    }

    ~KnowledgeBaseTestSuite() {
      KnowledgeBase::unregisterTransformation(het0);
      KnowledgeBase::unregisterTransformation(het1);
      KnowledgeBase::unregisterTransformation(hom0);
      KnowledgeBase::unregisterTransformation(hom1);
    }

    using  Transformations = KnowledgeBase::Transformations;

    bool checkResult(const Transformations& ts, const Transformation & t) {
      auto pred = [&t](const ConfiguredTransformation& cT) {
        return cT.trans() == t;
      };
      return any_of(ts.begin(), ts.end(), pred);
    }
  };

  TEST_F(KnowledgeBaseTestSuite, singleHeterogeneusTransform) {
    EXPECT_EQ(EventID(eT2), het0.out()) << "Wrong Output ID";
    EXPECT_EQ(het0.arity(), 1U) << "Wrong arity";
    ASSERT_EQ(het0.in(eT3).size(), 1U) << "Wrong number of inputs";
    EXPECT_EQ(EventID(eT3), het0.in(eT2).front()) << "Wrong input ID";
  }

  TEST_F(KnowledgeBaseTestSuite, singleHomogeneusTransform) {
    EXPECT_EQ(EventID::any, hom0.out()) << "Wrong Output ID";
    EXPECT_EQ(hom0.arity(), 1U) << "Wrong arity";
    ASSERT_EQ(hom0.in(eT3).size(), 1U) << "Wrong number of inputs";
    EXPECT_EQ(EventID(eT4), hom0.in(eT3).front()) << "Wrong input ID";
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
    for(const ConfiguredTransformation& t : ts)
      cout << t << endl;
    EXPECT_TRUE(checkResult(ts, hom2)) << "Searched Transform not found in list";
  }

  TEST_F(KnowledgeBaseTestSuite, fullTree) {
    KnowledgeBase::registerEventType(eT7);
    KnowledgeBase::registerEventType(eT4);
    KnowledgeBase::registerEventType(eT5);
    KnowledgeBase::registerEventType(eT6);
    Transformations ts = KnowledgeBase::findTransforms(eT0);
    ASSERT_GE(ts.size(), 1U) << "Wrong number of Transformations found";
    EXPECT_TRUE(checkResult(ts, hom2)) << "Searched Transform not found in list";
  }
}
