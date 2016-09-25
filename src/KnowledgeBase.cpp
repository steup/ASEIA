#include <KnowledgeBase.h>
#include <TypeRegistry.h>

#include <Singleton.h>

#include <vector>
#include <algorithm>

using namespace std;

class KBImpl {
  public:
    using TransStorage = std::vector<const Transformation*>;
    using TypeStorage  = AbstractRegistry<EventType>;
    using Transformations = KnowledgeBase::Transformations;
    using EventIDs = TypeStorage::EventIDs;
    using EventTypes = Transformation::EventTypes;

  private:
    TransStorage mTrans;
    TypeStorage  mTypes;

    void filter(vector<const Transformation*>& trans, EventID goal, const EventIDs& ids) const {
      auto useless = [&goal, &ids](const Transformation* t){
        const auto& tIn = t->in(goal);
        return goal>=t->out();// || !includes(tIn.begin(), tIn.end(), ids.begin(), ids.end());
      };
      auto endIt = remove_if(trans.begin(), trans.end(), useless);
      trans.erase(endIt, trans.end());
    }

    void addValidTypes(EventTypes& list, EventID id, const EventIDs& ids) const {
      auto it = lower_bound(ids.begin(), ids.end(), id);
      for(;it!=ids.end();it++)
        if(id<=*it)
          transform(mTypes.find(*it).first, mTypes.find(*it).second, back_inserter(list), [](const EventType& eT){ return &eT;});
    }

    vector<EventTypes> generateTypeLists(const EventIDs& in, const EventIDs& ids) const{
      vector<EventTypes> result(in.size());
      for(EventID id : in)
        addValidTypes(result.back(), id, ids);
      return result;
    }

    vector<EventTypes> generateCombinations(const vector<EventTypes>& typeLists) const {
      vector<EventTypes> result;
      if(typeLists.empty())
        return result;
      auto toList = [](const EventType* eT){ return EventTypes({eT});};
      transform(typeLists[0].begin(), typeLists[0].end(), back_inserter(result), toList);
      for(size_t i=1; i<typeLists.size(); i++) {
        auto currStart = result.begin();
        for(const EventType* b : typeLists[i]) {
          auto currEnd = result.end();
          copy(currStart, currEnd, back_inserter(result));
          for(; currStart!=currEnd; currStart++){
            currStart->push_back(b);
          }
        }
      }
      return result;
    }

    Transformations generate(vector<const Transformation*>& trans,
                  const EventType& goal, const EventIDs& ids) const {
      Transformations result;
      for(const Transformation* t : trans) {
        vector<EventTypes> usableEventTypes = generateTypeLists(t->in(goal), ids);
        vector<EventTypes> typeCombinations = generateCombinations(usableEventTypes);
        auto create = [t, &goal](const EventTypes& in){
          return ConfiguredTransformation(*t, goal, in);
        };
        transform(typeCombinations.begin(), typeCombinations.end(), back_inserter(result), create);
      }
      return result;
    }
  public:
    void regTrans(const Transformation& trans) {
      auto comp = [&trans](const Transformation* t){
        return *t == trans;
      };
      if(trans==Transformation::invalid)
        return;
      if(any_of(mTrans.begin(), mTrans.end(), comp))
        return;
      if(trans.out() == EventID::any)
        mTrans.push_back(&trans);
    }

    void unregTrans(const Transformation& trans) {
      auto comp = [&trans](const Transformation* t){
        return *t == trans;
      };
      auto it = find_if(mTrans.begin(), mTrans.end(), comp);
      if(it == mTrans.end())
        return;
      iter_swap(it, mTrans.end()-1);
      mTrans.erase(mTrans.end()-1);
    }

    void regType(const EventType& eT) {
      mTypes.registerType(eT, eT);
    }

    void unregType(const EventType& eT) {
      mTypes.unregisterType(eT);
    }

    Transformations find(const EventType& goal) {
      EventIDs ids = mTypes.ids(); //<< Get EventIDs currently registered
      sort(ids.begin(), ids.end());//<< Sort EventIDs ascending

      EventID out = (EventID)goal; //<< Get goal EventID
      std::vector<const Transformation*> trans(mTrans); //<< Copy list of transformations

      filter(trans, out, ids); //<< Remove all transformation not leading to the goal EventID or with unfullfilled dependancies

      Transformations result = generate(trans, goal, ids); //<< Generate all configurations
      auto resultEnd = remove_if(result.begin(), result.end(),
        [](const ConfiguredTransformation& t){ return !t.check();}
      ); //<< Remove invalid configurations
      result.erase(resultEnd, result.end());
      return result;
    }
};

using KB = Singleton<KBImpl>;

void KnowledgeBase::registerTransformation(const Transformation& trans) {
  KB::instance().regTrans(trans);
}

void KnowledgeBase::unregisterTransformation(const Transformation& trans) {
  KB::instance().unregTrans(trans);
}

void KnowledgeBase::registerEventType(const EventType& trans) {
  KB::instance().regType(trans);
}

void KnowledgeBase::unregisterEventType(const EventType& trans) {
  KB::instance().unregType(trans);
}

KnowledgeBase::Transformations KnowledgeBase::findTransforms(const EventType& goal) {
  return KB::instance().find(goal);
}
