#include <Transformation.h>

#include <KnowledgeBase.h>
#include <EventType.h>
#include <IO.h>

#include <iostream>

using namespace std;

/** \todo ignore sorting of input EventTypes **/
bool ConfiguredTransformationInterface::operator==(const ConfiguredTransformationInterface& b) const {
  return mOut == b.out() && mIn == b.in();
}

Transformation::Transformation(Type type, size_t arity, const EventID& out)
  : mOut(out), mType(type), mArity(arity) {
}

Transformation::~Transformation() {
}

class InvalidTransformation : public Transformation {
  public:
    InvalidTransformation() : Transformation(Type::invalid, 0, EventID::any) {
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
} invalidInstance;

const Transformation& Transformation::invalid = invalidInstance;
