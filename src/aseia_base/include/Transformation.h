#pragma once

#include <EventID.h>

#include <iosfwd>
#include <list>
#include <memory>

class MetaEvent;
class EventType;

class Transformation {
  public:
    using EventTypes = std::list<const EventType*>;
    using Events     = std::list<const MetaEvent*>;

    class Transformer {
      public:
        virtual ~Transformer() = default;
        virtual bool check(const Events& events) const =0;
        virtual MetaEvent operator()(const Events& events) =0;
        virtual void print(std::ostream& o) const = 0;
    };

    using TransPtr = std::unique_ptr<Transformer>;
    Transformation();
    virtual ~Transformation() = delete;
    virtual std::size_t arity() const=0;
    virtual EventID in(std::size_t i) const=0;
    virtual EventID out() const=0;
    virtual bool check(const EventTypes& types) const =0;
    virtual TransPtr create(const EventType& out, const EventTypes& in) const =0;
    virtual void print(std::ostream& o) const = 0;
};
