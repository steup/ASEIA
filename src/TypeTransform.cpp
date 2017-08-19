
#include <Transformation.h>

#include <MetaEvent.h>
#include <EventType.h>
#include <IO.h>
#include <IDIO.h>
#include <MetaFactory.h>

#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

class TypeTransformer : public SimpleTransformer {
  private:
    // from -> to
    using Storage = std::map<id::attribute::ID, pair<ValueType, ValueType>>;
    Storage mTypeDeltas;

	public:
    using TypeList = Storage;
    static TypeList typeDiff(const EventType& to, const EventType&  from) {
      TypeList result;
      for(const AttributeType& aT : to) {
        const AttributeType* temp = from.attribute(aT.id());
        if(!temp || (ValueType)aT.value() == (ValueType)temp->value())
          continue;

				result.insert(make_pair(aT.id(), make_pair(temp->value(), aT.value())));
      }
      return  result;
    }

    TypeTransformer(const EventType& out, const EventType& in)
			: SimpleTransformer(out, in)
		{
      mTypeDeltas = typeDiff(out, in);
    }

    virtual void print(std::ostream& o) const {
      o << "Cast " << EventID(mOut) << ": \n";
      for(const Storage::value_type& typeOp : mTypeDeltas)
        o << "\t" << id::attribute::name(typeOp.first) << ": " <<  typeOp.second.first << " -> " << typeOp.second.second << "\n";
    }

  protected:
    virtual bool check(const MetaEvent& e) const { return true; }

    virtual MetaEvent execute(const MetaEvent& event) const {
      MetaEvent result = event;
      for(MetaAttribute& a : result) {
        auto it = mTypeDeltas.find(a.id());
        if(it != mTypeDeltas.end() && it->second.first != it->second.second)
          a.value()=MetaFactory::instance().convert(it->second.second, a.value());
      }
      return result;
    }


};

class TypeTransformation : public Transformation {
  public:
		TypeTransformation()
			: Transformation(Transformation::Type::attribute, 1, EventID::any)
		{	}

    virtual EventIDs in(EventID goal, const MetaFilter& = MetaFilter()) const {
      return EventIDs({goal});
    }

    virtual vector<EventType> in(const EventType& goal, const EventType& provided = EventType(), const MetaFilter& = MetaFilter())  const {
      TypeTransformer::TypeList typeDiff = TypeTransformer::typeDiff(goal, provided);
      auto apply = [](EventType eT, const pair<id::attribute::ID, pair<ValueType, ValueType>>& v){
        EventType result = eT;
        AttributeType& a = *result.attribute(v.first);
        a.value() = v.second.first;
        return result;
      };
      return {accumulate(typeDiff.begin(), typeDiff.end(), goal, apply)};
    }

    virtual TransPtr create(const EventType& out, const EventTypes& in, const AbstractPolicy& policy, const MetaFilter& filter = MetaFilter()) const {
      if(in.size()!=1)
        return TransPtr();
      else
        return TransPtr(new TypeTransformer(out, in[0]));
		}

    virtual void print(std::ostream& o) const {
			o << "Cast Transformation";
		}
} castObj;

const Transformation& cast=castObj;
