#pragma once

#include <EventID.h>

#include <iosfwd>
#include <list>
#include <memory>
#include <initializer_list>

class MetaEvent;
class EventType;

/** \brief Interface of a state-full execution of a Transformation
  *
  * Subclasses implement actual Transformations. They transform single or
  * multiple incoming MetaEvents to a new output MetaEvent.
  *
  **/
class Transformer {
  public:
    using EventTypes = std::list<const EventType*>;
    using Events     = std::list<const MetaEvent*>;
  private:
    const EventType& mOut;
    const EventTypes& mIn;
  public:
    Transformer(const EventType& out, const EventTypes& in) : mOut(out), mIn(in) {}
    virtual ~Transformer() = default;
    const EventType& out() const { return mOut; }
    const EventTypes& in() const { return mIn; }
    virtual bool check(const Events& events) const =0;
    virtual MetaEvent operator()(const Events& events) =0;
    virtual void print(std::ostream& o) const = 0;
};

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
    using EventIDs   = std::list<EventID>;
    using TransPtr   = std::unique_ptr<Transformer>;

  private:
    const EventID mOut;
    const EventIDs mIn;

  public:
    Transformation(const EventID& out, const EventIDs& in) : mOut(out), mIn(in) {}
    virtual ~Transformation() = delete;
    std::size_t arity() const { return mIn.size(); };
    const EventIDs& in() const { return mIn; }
    const EventID& out() const { return mOut; }
    virtual bool check(const EventTypes& types) const =0;
    virtual TransPtr create(const EventType& out, const EventTypes& in) const =0;
    virtual void print(std::ostream& o) const = 0;
};
