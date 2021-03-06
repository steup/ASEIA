#pragma once

#include <EventID.h>
#include <EventType.h>
#include <EventStorage.h>
#include <MetaFilter.h>

#include <iosfwd>
#include <vector>
#include <memory>

class MetaEvent;
class Transformation;


class AbstractConfiguredTransformation {
  public:
    using EventTypes = std::vector<EventType>;
    using Events     = std::vector<MetaEvent>;
  protected:
    EventType mOut;
    EventTypes mIn;
    MetaFilter mFilter;
  public:
    const EventType& out() const { return mOut; }
    const EventTypes& in() const { return mIn; }
    const MetaFilter& filter() const { return mFilter; }
    bool operator==(const AbstractConfiguredTransformation& b) const;
};

/** \brief Interface of a state-full execution of a Transformation
  *
  * Subclasses implement actual Transformations. They transform single or
  * multiple incoming MetaEvents to a new output MetaEvent.
  **/
class Transformer : public AbstractConfiguredTransformation {
  public:
    Transformer(const EventType& out, const EventTypes& in) {
      mOut = out;
      mIn = in;
    }
    virtual ~Transformer() = default;
    virtual bool check(const MetaEvent& e) const =0;
    virtual Events operator()(const MetaEvent& event) =0;
    virtual void print(std::ostream& o) const = 0;
};

class SimpleTransformer : public Transformer {
  protected:
    virtual MetaEvent execute(const MetaEvent& e) const =0;
  public:
    SimpleTransformer(const EventType& out, const EventType& in)
      : Transformer(out, {in}) {}
    virtual Events operator()(const MetaEvent& event) {
      if(mIn.size() == 1 && mIn[0].isCompatible(EventType(event)) && check(event))
        return {execute(event)};
      else
        return {};
    }
};

class BufferedTransformer : public Transformer {
  protected:
    EventStorage mStorage;
    using EventPtrs = std::vector<const MetaEvent*>;
    virtual Events execute(const EventPtrs& inputs) const =0;
  public:
    BufferedTransformer(const EventType& out, const EventTypes& in, const AbstractPolicy& policy)
      :Transformer(out, in), mStorage(mIn, policy) {}

    virtual Events operator()(const MetaEvent& event) {
      Events result;
      EventType eT = (EventType)event;
      auto checkType = [&eT](const EventType& b){ return eT.isCompatible(b); };
      if( any_of(mIn.begin(), mIn.end(), checkType) && check(event)) {
        mStorage.addEvent(event);
        for(const EventPtrs& input : mStorage) {
          Events temp = execute(input);
          move(temp.begin(), temp.end(), back_inserter(result));
        }
      }
      return result;
    };
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
  **/
class Transformation {
  public:
    using EventTypes = Transformer::EventTypes;
    using EventIDs   = std::vector<EventID>;
    using TransPtr   = std::shared_ptr<Transformer>;
    static const Transformation& invalid;
    enum class Type{
      invalid,
      heterogeneus,
      homogeneus,
      attribute,
      composite
    };
  private:
    const EventID mOut;
    const Type mType = Type::invalid;
    const std::size_t mArity=0;
  public:
    Transformation(Type type, std::size_t arity, const EventID& out);

    std::size_t arity() const { return mArity; }
    const EventID& out() const { return mOut; }
    Type type() const { return mType; }

    virtual ~Transformation();
    virtual std::vector<EventType> in(const EventType& goal, const  EventType& provided = EventType(), const MetaFilter& filter = MetaFilter()) const =0;

    virtual EventIDs in(EventID goal, const MetaFilter& filter = MetaFilter()) const = 0;
    virtual TransPtr create(const EventType& out, const EventTypes& in, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const =0;
    virtual void print(std::ostream& o) const = 0;

    bool operator==(const Transformation& b) const { return this == &b; }
    bool operator!=(const Transformation& b) const { return this != &b; }
};

inline std::ostream& operator<<(std::ostream& o, const Transformation& t) {
  t.print(o);
  return o;
}

using TransformationPtr = const Transformation*;
