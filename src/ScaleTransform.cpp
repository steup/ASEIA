#include <Transformation.h>

#include <MetaEvent.h>
#include <EventType.h>
#include <IO.h>
#include <IDIO.h>

#include <ostream>
#include <algorithm>
#include <numeric>

using namespace std;

class ScaleTransformer : public Transformer {
  private:
    using Storage = map<id::attribute::ID, MetaScale>;
    Storage mScaleDeltas;

	public:
    using ScaleList = Storage;
    static ScaleList scaleDiff(const EventType& a, const EventType& b) {
      Storage  result;
      for(const AttributeType& aT : a) {
        const AttributeType* temp = b.attribute(aT.id());
        if(!temp)
          continue;
        ScaleType bS(temp->scale().num(), temp->scale().denom(), aT.scale().reference());
				MetaScale mod=aT.scale();
				mod/=bS;
        result.insert(make_pair(aT.id(), mod));
      }
      return result;
    }

    ScaleTransformer(const Transformation* t, const EventType& out, const EventTypes& in)
			: Transformer(t, out, in) {

      if(in.size()!=1 || !in.front())
        return;

      const EventType& b = *in.front();
      mScaleDeltas = scaleDiff(out, b);
    }

    virtual bool check(const Events& events) const {
      return true;
    }

    virtual MetaEvent operator()(const Events& events) {
      if (events.size() != 1 || !events.front() || !mScaleDeltas.size())
        return MetaEvent(mOut);

      MetaEvent e = *events.front();
      for(MetaAttribute& a : e) {
        auto it = mScaleDeltas.find(a.id());
        if(it != mScaleDeltas.end() && (it->second.num() != 1 || it->second.denom() != 1))
          a*=it->second;
      }
      return e;
    }

    virtual void print(ostream& o) const {
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

      if(EventID(out) != EventID(b))
        return false;

      ScaleTransformer::ScaleList scaleDiff = ScaleTransformer::scaleDiff(out, b);
      auto check = [](const pair<id::attribute::ID, MetaScale>& v){
        return v.second.num() != 1 || v.second.denom() != 1;
      };
      return any_of(scaleDiff.begin(), scaleDiff.end(), check);
    }

    virtual EventIDs in(EventID goal) const {
      return EventIDs({goal});
    }
    
    virtual vector<EventType> in(const EventType& goal, const EventType& provided) const {
      ScaleTransformer::ScaleList scaleDiff = ScaleTransformer::scaleDiff(goal, provided);
      auto apply = [](EventType eT, const pair<id::attribute::ID, MetaScale>& v){
        EventType result = eT;
        AttributeType& a = *result.attribute(v.first);
        MetaScale aS = a.scale();
        aS /= v.second;
        a.scale() = aS;
        return result;
      };
      return {accumulate(scaleDiff.begin(), scaleDiff.end(), goal, apply)};
    }

    virtual size_t arity() const {
      return 1;
    }

    virtual TransPtr create(const EventType& out, const EventTypes& in) const {
			if(check(out, in))
				return TransPtr(new ScaleTransformer(this, out, in));
			else
				return TransPtr();
		}

    virtual void print(ostream& o) const {
			o << "Rescale Transformation";
		}
} rescale;
