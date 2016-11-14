#include <Transformation.h>

#include <KnowledgeBase.h>
#include <EventType.h>
#include <IO.h>

#include <iostream>

using namespace std;

Transformation::Transformation(const EventID& out) : mOut(out) {
  KnowledgeBase::registerTransformation(*this);
}

bool Transformer::operator==(const ConfiguredTransformation& t) const {
  return mTrans == &t.trans() && mOut == t.out() && mIn == t.in();
}

ostream& operator<<(ostream& o, const ConfiguredTransformation& t) {
  if(!t.mTrans || !t.mOut)
    return o << "invalid configured transformation";

  o << t.trans() << ": " << t.out() << " <- [";
  for(const EventType* eT : t.in())
    o << *eT << ", ";
  return o << "]";
}

class InvalidTransformation : public Transformation {
  public:
    InvalidTransformation() : Transformation(EventID::any) {
      KnowledgeBase::unregisterTransformation(*this);
    }
    virtual bool check(const EventType& out, const EventTypes& in) const {
      return false;
    }
    virtual TransPtr create(const EventType& out, const EventTypes& in) const {
      return TransPtr();
    }
    virtual std::size_t arity() const {
      return 0;
    }
    virtual void print(std::ostream& o) const {
      o << "invalid";
    }
    virtual EventIDs in(EventID goal) const {
      return EventIDs();
    }
} invalid;

const Transformation& Transformation::invalid = invalid;
