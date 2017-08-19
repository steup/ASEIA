#include <Transformation.h>

#include <MetaEvent.h>
#include <EventType.h>
#include <IO.h>
#include <IDIO.h>

#include <ostream>
#include <algorithm>
#include <numeric>

using namespace std;

class ScaleTransformer : public SimpleTransformer {
  private:
    using Storage = map<id::attribute::ID, MetaScale>;
    Storage mScaleDeltas;

	public:
    using ScaleList = Storage;
    static ScaleList scaleDiff(const EventType& a, const EventType& b) {
      Storage  result;
      for(const AttributeType& aT : a) {
        const AttributeType* temp = b.attribute(aT.id());
        if(!temp || (aT.scale().num() == temp->scale().num() && aT.scale().denom() == temp->scale().denom()))
          continue;
        ScaleType bS(temp->scale().num(), temp->scale().denom(), aT.scale().reference());
				MetaScale mod=aT.scale();
				mod/=bS;
        result.insert(make_pair(aT.id(), mod));
      }
      return result;
    }

    ScaleTransformer(const EventType& out, const EventType& in)
			: SimpleTransformer(out, in) {
      mScaleDeltas = scaleDiff(out, in);
    }

    virtual void print(ostream& o) const {
      o << "Rescale " << EventID(mOut) << ": \n";
      for(const Storage::value_type& scaleOp : mScaleDeltas)
        o << "\t" << id::attribute::name(scaleOp.first) << ": " << scaleOp.second << "\n";
    }

    protected:

    virtual bool check(const MetaEvent& e) const { return true; }

    virtual MetaEvent execute(const MetaEvent& event) const {
      MetaEvent result = event;
      for(MetaAttribute& a : result) {
        auto it = mScaleDeltas.find(a.id());
        if(it != mScaleDeltas.end() && (it->second.num() != 1 || it->second.denom() != 1))
          a*=it->second;
      }
      return result;
    }
};

class ScaleTransformation : public Transformation {
  public:
		ScaleTransformation()
			: Transformation(Transformation::Type::attribute, 1, EventID::any)
		{	}

    virtual EventIDs in(EventID goal, const MetaFilter& = MetaFilter()) const {
      return {goal};
    }

    virtual vector<EventType> in(const EventType& goal, const EventType& provided = EventType(), const MetaFilter& = MetaFilter()) const {
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

    virtual TransPtr create(const EventType& out, const EventTypes& in, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const {
			if(in.size()!=1)
				return TransPtr();
			else
				return TransPtr(new ScaleTransformer(out, in[0]));
		}

    virtual void print(ostream& o) const {
			o << "Rescale Transformation";
		}
} rescaleObj;

const Transformation& rescale=rescaleObj;
