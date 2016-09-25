
#include <Transformation.h>

#include <MetaEvent.h>
#include <EventType.h>
#include <IO.h>
#include <IDIO.h>
#include <MetaFactory.h>

#include <iostream>

using namespace std;

class TypeTransformer : public Transformer {
  private:
    using Storage = std::map<id::attribute::ID, ValueType>;
    Storage mTypeDeltas;

	public:
    TypeTransformer(const EventType& out, const EventTypes& in)
			: Transformer(out, in)
		{
      if(in.size()!=1 || !in.front())
        return;
      const EventType& b = *in.front();
      for(const AttributeType& aT : out) {
        const AttributeType* temp = b.attribute(aT.id());
        if(!temp || (ValueType)aT.value() == (ValueType)temp->value())
          continue;

				mTypeDeltas.insert(make_pair(aT.id(), (ValueType)aT.value()));
      }
    }

    virtual bool check(const Events& events) const {
      return true;
    }

    virtual MetaEvent operator()(const Events& events) {
      if (events.size() != 1 || !events.front())
        return MetaEvent(mOut);
      MetaEvent e = *events.front();
      for(MetaAttribute& a : e) {
        auto it = mTypeDeltas.find(a.id());
        if(it != mTypeDeltas.end())
          a.value()=MetaFactory::instance().convert(it->second, a.value());
      }
      return e;
    }

    virtual void print(std::ostream& o) const {
      o << "Cast " << EventID(mOut) << ": \n";
      for(const Storage::value_type& typeOp : mTypeDeltas)
        o << "\t" << id::attribute::name(typeOp.first) << " -> " << typeOp.second << "\n";
    }

};

class TypeTransformation : public Transformation {
  public:
		TypeTransformation()
			: Transformation(EventID::any)
		{	}

    virtual bool check(const EventType& out, const EventTypes& in) const {
      if(in.size()!=1 || !in.front())
        return false;
      const EventType& b = *in.front();
      if(EventID(out)!=EventID(b))
        return false;
      for(const AttributeType& a : out) {
        const AttributeType* b = in.front()->attribute(a.id());
        if(b && (ValueType)a.value() != (ValueType)b->value())
          return true;
      }
      return false;
    }

    virtual EventIDs in(EventID goal) const {
      return EventIDs({goal});
    }

    virtual std::size_t arity() const {
      return 1;
    }

    virtual TransPtr create(const EventType& out, const EventTypes& in) const {
			if(check(out, in))
				return TransPtr(new TypeTransformer(out, in));
			else
				return TransPtr();
		}

    virtual void print(std::ostream& o) const {
			o << "Cast Transformation";
		}
} cast;
