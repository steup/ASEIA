#include <Transformation.h>

#include <KnowledgeBase.h>
#include <EventType.h>
#include <IO.h>

#include <iostream>

using namespace std;

Transformation::Transformation(Type type, size_t arity, const EventID& out)
  : mOut(out), mType(type), mArity(arity) {
  KnowledgeBase::registerTransformation(*this);
}

bool Transformer::operator==(const ConfiguredTransformation& t) const {
  return mTrans == *t.trans() && mOut == t.out() && mIn == t.in();
}

ostream& operator<<(ostream& o, const ConfiguredTransformation& t) {
  if(!t.mTrans || !t.mOut)
    return o << "invalid configured transformation";

  o << *t.trans() << ": " << t.out() << " <- [";
  for(const EventType& eT : t.in())
    o << eT << ", ";
  return o << "]";
}

class InvalidTransformation : public Transformation {
  public:
    InvalidTransformation() : Transformation(Type::invalid, 0, EventID::any) {
      KnowledgeBase::unregisterTransformation(*this);
    }
    virtual TransPtr create(const EventType& out, const EventTypes& in) const {
      return TransPtr();
    }
    virtual void print(std::ostream& o) const {
      o << "invalid";
    }
    virtual EventIDs in(EventID goal) const {
      return EventIDs();
    }
} invalid;

const Transformation& Transformation::invalid = invalid;
