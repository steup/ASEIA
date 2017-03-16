#pragma once

#include <EventID.h>

#include <iosfwd>
#include <vector>
#include <memory>

class MetaEvent;
class EventType;
class ConfiguredTransformation;
class Transformation;

/** \brief Interface of a state-full execution of a Transformation
  *
  * Subclasses implement actual Transformations. They transform single or
  * multiple incoming MetaEvents to a new output MetaEvent.
  *
  **/
class Transformer {
  public:
    using EventTypes = std::vector<const EventType*>;
    using Events     = std::vector<const MetaEvent*>;
  protected:
    const EventType& mOut;
    const EventTypes mIn;
    const Transformation* mTrans;
  public:
    Transformer(const Transformation* t, const EventType& out, const EventTypes& in)
      : mOut(out), mIn(in), mTrans(t) {}
    virtual ~Transformer() = default;
    const EventType& out() const { return mOut; }
    const EventTypes& in() const { return mIn; }
    virtual bool check(const Events& events) const =0;
    virtual MetaEvent operator()(const Events& events) =0;
    virtual void print(std::ostream& o) const = 0;
    const Transformation* transformation() const { return mTrans; }
    bool operator==(const Transformer& b) const { return this == &b; }
    bool operator==(const ConfiguredTransformation& t) const;
};

inline std::ostream& operator<<(std::ostream& o, const Transformer& t) {
  t.print(o);
  return o;
}

/** \brief Interface of generic Event Transformation
  *
  * Subclasses implement Transformations as content of the KnowledgeBase. The
  * subclasses define the incoming EventIDs and the outgoing EventID as well as
  * a check on the actual EventTypes provided by the Broker. The subclasses may
  * create multiple instances of subclasses of Transformer to actually do the
  * state-full transformation.
  *
  **/
class Transformation {
  public:
    using EventTypes = Transformer::EventTypes;
    using EventIDs   = std::vector<EventID>;
    using TransPtr   = std::unique_ptr<Transformer>;
    static const Transformation& invalid;
  private:
    const EventID mOut;

  public:
    Transformation(const EventID& out);
    virtual ~Transformation() = default;
    virtual std::size_t arity() const = 0;
    virtual EventIDs in(EventID goal) const = 0;
    const EventID& out() const { return mOut; }
    virtual bool check(const EventType& out, const EventTypes& in) const =0;
    virtual TransPtr create(const EventType& out, const EventTypes& in) const =0;
    virtual void print(std::ostream& o) const = 0;
    bool operator==(const Transformation& b) const { return this == &b; }
    bool operator!=(const Transformation& b) const { return this != &b; }
};

inline std::ostream& operator<<(std::ostream& o, const Transformation& t) {
  t.print(o);
  return o;
}

class ConfiguredTransformation {
  public:
  using EventTypes = Transformer::EventTypes;
  using TransPtr   = std::unique_ptr<Transformer>;
  using EventIDs   = Transformation::EventIDs;
  private:
  const Transformation* mTrans = nullptr;
  const EventType* mOut = nullptr;
  EventTypes mIn;
  public:
  ConfiguredTransformation() = default;
  ConfiguredTransformation(const Transformation& t, const EventType& out, const EventTypes in={})
    : mTrans(&t), mOut(&out), mIn(in)
  {}
  TransPtr create() const {
    if(!check())
      return TransPtr();
    else
      return mTrans->create(*mOut, mIn);
  }
  bool  check() const {
    return (mTrans && mOut && mIn.size() == mTrans->arity() &&
            mTrans->check(*mOut, mIn));
  }
  const Transformation& trans() const { return *mTrans; }
  const EventType& out() const { return *mOut; }
  const EventTypes& in() const { return mIn; }
  EventIDs inIDs() const { return (mTrans && mOut)?mTrans->in(*mOut):EventIDs(); }
  void in(const EventTypes& eTs) { mIn = eTs; }
  bool operator==(const Transformer& t) const { return t==*this; }
  friend std::ostream& operator<<(std::ostream&, const ConfiguredTransformation&);
};

std::ostream& operator<<(std::ostream& o, const ConfiguredTransformation& t);
