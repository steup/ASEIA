#include <KnowledgeBase.h>

namespace test {

 using namespace ::id::attribute;

  struct KnowledgeBaseTestSuite : public ::testing::Test {

    struct Test0: public ::id::attribute::Base {
      static constexpr const ID value()  { return 251; }
    };

    struct Test1 : public ::id::attribute::Base {
      static constexpr const ID value()  { return 252; }
    };

    struct Test2 : public ::id::attribute::Base {
      static constexpr const ID value()  { return 253; }
    };

    struct Test3 : public ::id::attribute::Base {
      static constexpr const ID value()  { return 254; }
    };

    struct Test4 : public ::id::attribute::Base {
      static constexpr const ID value() { return 255; }
    };

    struct HetTrans1 : public Transformation {
      HetTrans1() : Transformation(EventID({Test2::value()})) {}
      virtual std::size_t arity() const  { return 1; }
      virtual EventIDs in(EventID goal) const  {
        return EventIDs({ EventID({Test3::value()}) });
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
      virtual TransPtr create(const EventType& out, const EventTypes& in) const { return TransPtr(); }
      virtual void print(std::ostream& o) const { o << "Homogeneus Transform 2"; }
    } hom1;

    KnowledgeBaseTestSuite() {
      KnowledgeBase::registerTransformation(het0);
      KnowledgeBase::registerTransformation(het1);
      KnowledgeBase::registerTransformation(hom0);
      KnowledgeBase::registerTransformation(hom1);
    }

    ~KnowledgeBaseTestSuite() {
      KnowledgeBase::unregisterTransformation(het0);
      KnowledgeBase::unregisterTransformation(het1);
      KnowledgeBase::unregisterTransformation(hom0);
      KnowledgeBase::unregisterTransformation(hom1);
    }
  };
}
