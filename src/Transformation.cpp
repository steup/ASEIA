#include <Transformation.h>

#include <KnowledgeBase.h>
#include <EventType.h>
#include <IO.h>

#include <iostream>

using namespace std;

bool Transformer::operator==(const ConfiguredTransformation& t) const {
  return mTrans == *t.trans() && mOut == t.out() && mIn == t.in();
}

bool Transformer::operator==(const CompositeTransformation& t) const {
  return mOut == t.out() && mIn == t.in();
}

Transformation::Transformation(Type type, size_t arity, const EventID& out)
  : mOut(out), mType(type), mArity(arity) {
  KnowledgeBase::registerTransformation(*this);
}

Transformation::~Transformation() {
  KnowledgeBase::unregisterTransformation(*this);
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
