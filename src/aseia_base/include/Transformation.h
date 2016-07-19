#pragma once

#include <iosfwd>
#include <list>
#include <memory>

class MetaEvent;
class EventType;

class Transformation {
  protected:
    Transformation() = default;
		bool mValid = false;
  public:
		virtual ~Transformation() = default;
    using EventTypes = std::list<const EventType*>;
    using Events = std::list<const MetaEvent*>;
    using TransPtr = std::unique_ptr<Transformation>;

    virtual bool check(const Events& events) const =0;
    virtual MetaEvent operator()(const Events& events) =0;
    virtual void print(std::ostream& o) const = 0;
		bool valid() const { return mValid; }
    
		template<typename T>
    static TransPtr create(const EventType& out, const EventTypes& in) {
     	TransPtr t = TransPtr(new T(out, in));
			if (t->valid())
				return t;
			else
				return TransPtr();
    }
};
