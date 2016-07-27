#include <Transformation.h>

#include <KnowledgeBase.h>
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
    ScaleTransformer(const EventType& out, const EventTypes& in)
			: Transformer(out, in)
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
        return MetaEvent();
      MetaEvent e = *events.front();
      for(MetaAttribute& a : e) {
        const auto& it = mScaleDeltas.find(a.id());
        if(it != mScaleDeltas.end())
          a*=it->second;
      }
      return e;
    }

    virtual void print(std::ostream& o) const {
      o << "Rescale " << EventID(mOut) << ": \n";
      for(const Storage::value_type& scaleOp : mScaleDeltas)
        o << "\t" << /*id::attribute::name*/(scaleOp.first) << ": " << scaleOp.second << "\n";
    }

};

class ScaleTransformation : public Transformation {
  public:
		ScaleTransformation()
			: Transformation(EventID::any, {EventID::any})
		{	}

    virtual bool check(const EventType& out, const EventTypes& in) const {
      if(in.size()!=1 || !in.front())
        return false;
      const EventType& b = *in.front();
      if(EventID(out)!=EventID(b))
        return false;
      //TODO: check data types
      return true;
    }

    virtual TransPtr create(const EventType& out, const EventTypes& in) const {
			if(check(out, in))
				return TransPtr(new ScaleTransformer(out, in));
			else
				return TransPtr();
		}

    virtual void print(std::ostream& o) const {
			o << "Rescale Transformation";
		}
} rescale;


Transformation::Transformation(const EventID& out, const EventIDs& in)
  : mOut(out), mIn(in)
{
  KnowledgeBase::instance().registerTransformation(*this);
}
