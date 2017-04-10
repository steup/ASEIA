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

    ScaleTransformer(const EventType& out, const EventTypes& in, const AbstractPolicy& policy)
			: Transformer(out, in, policy) {

      if(in.size()!=1)
        return;

      const EventType& b = in.front();
      mScaleDeltas = scaleDiff(out, b);
    }

    virtual bool check(const MetaEvent& e) const { return mIn.front() <= (EventType) e; }

    virtual Events operator()(const MetaEvent& event) {
      Events result = { event };
      for(MetaAttribute& a : result.front()) {
        auto it = mScaleDeltas.find(a.id());
        if(it != mScaleDeltas.end() && (it->second.num() != 1 || it->second.denom() != 1))
          a*=it->second;
      }
      return result;
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
			: Transformation(Transformation::Type::attribute, 1, EventID::any)
		{	}

    virtual EventIDs in(EventID goal) const {
      return {goal};
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

    virtual TransPtr create(const EventType& out, const EventTypes& in, const AbstractPolicy& policy) const {
			if(in.size()!=arity())
				return TransPtr();
			else
				return TransPtr(new ScaleTransformer(out, in, policy));
		}

    virtual void print(ostream& o) const {
			o << "Rescale Transformation";
		}
} rescaleObj;

const Transformation& rescale=rescaleObj;
