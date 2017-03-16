#include <Transformation.h>

#include <MetaEvent.h>
#include <EventType.h>
#include <IO.h>
#include <IDIO.h>

#include <iostream>

class ScaleTransformer : public Transformer {
  private:
    using Storage = std::map<id::attribute::ID, MetaScale>;
    Storage mScaleDeltas;

	public:
    ScaleTransformer(const Transformation* t, const EventType& out, const EventTypes& in)
			: Transformer(t, out, in)
		{
      if(in.size()!=1 || !in.front())
        return;
      const EventType& b = *in.front();
      for(const AttributeType& aT : out) {
        const AttributeType* temp = b.attribute(aT.id());
        if(!temp || aT.scale() == temp->scale())
          continue;

				MetaScale mod=aT.scale();
				mod/=temp->scale();
        mScaleDeltas.insert(std::make_pair(aT.id(), mod));
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
        auto it = mScaleDeltas.find(a.id());
        if(it != mScaleDeltas.end())
          a*=it->second;
      }
      return e;
    }

    virtual void print(std::ostream& o) const {
      o << "Rescale " << EventID(mOut) << ": \n";
      for(const Storage::value_type& scaleOp : mScaleDeltas)
        o << "\t" << id::attribute::name(scaleOp.first) << ": " << scaleOp.second << "\n";
    }

};

class ScaleTransformation : public Transformation {
  public:
		ScaleTransformation()
			: Transformation(EventID::any)
		{	}

    /** \todo fix lenient check for other subtypes **/
    virtual bool check(const EventType& out, const EventTypes& in) const {
      if(in.size()!=1 || !in.front())
        return false;
      const EventType& b = *in.front();
      if(EventID(out)!=EventID(b))
        return false;
      for(const AttributeType& a : out) {
        const AttributeType* b = in.front()->attribute(a.id());
        if(b && a.scale() != b->scale())
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
				return TransPtr(new ScaleTransformer(this, out, in));
			else
				return TransPtr();
		}

    virtual void print(std::ostream& o) const {
			o << "Rescale Transformation";
		}
} rescale;
