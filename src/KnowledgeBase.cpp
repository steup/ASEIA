#include <KnowledgeBase.h>
#include <TypeRegistry.h>

#include <TransformationList.h>

#include <Singleton.h>

#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class KBImpl {
  public:
    using TransStorage = std::vector<TransformationPtr>;
    using TypeStorage  = AbstractRegistry<EventType>;
    using Transformations = KnowledgeBase::Transformations;
    using EventIDs = TypeStorage::EventIDs;
    using EventTypes = Transformation::EventTypes;

  private:
    using OutIt = back_insert_iterator<Transformations>;
    using TypeOutIt = back_insert_iterator<EventTypes>;
    TypeStorage mTypes;
    TransStorage mHetTrans, mHomTrans, mAtt1Trans, mAttNTrans;

    /** \brief extract comaptible EventIds  from published EventIds
     *  \param goal the goal EventID
     *  \param ids all published EventIds
     **/
    EventIDs extractIDs(EventID goal, const EventIDs& ids) const {
      EventIDs result;
      auto start = lower_bound(ids.begin(), ids.end(), goal);
      copy_if(start, ids.end(), back_inserter(result), [&goal](EventID id){ return id>=goal; });

      return result;
    }

    /** \brief add compatible EventTypes to list
     *  \param id the EventID the EventTypes need to be compatible to
     *  \param it Output iterator to store EventTypes
     *  \return the Output iterator after insertion
     **/
    TypeOutIt addValidTypes(TypeOutIt it, EventID id) const {
      return copy(mTypes.find(id).first, mTypes.find(id).second, it);
    }
    /** \todo find all compatible types not just direct fits **/
    pair<bool, EventTypes> mapTypes(const vector<EventType>& types) const {
      EventTypes used;
      for(const EventType& eT : types) {
        auto range = mTypes.find(eT);
        if(range.empty())
          return make_pair(false, EventTypes());
        else
          used.push_back(range.front());
      }
      return make_pair(true, used);
    }

    pair<bool, EventType> extractID(EventID goal, const EventTypes& types) const {
      EventTypes result;
      auto pred = [goal](const EventType& eT){ return EventID(eT)>=goal; };
      auto it = find_if(types.begin(), types.end(), pred);
      if(it!=types.end())
        return make_pair(true, *it);
      else
        return make_pair(false, EventType());
    }

    /** \brief create fitting partially configured composite transformations for goal EventID
     * \param goal the EventID of the goal
     * \param all existing published EventIDs
     * \param it Output iterator to output fitting partially generated transformations
     * \todo filter on input eventids
     * \todo generate composite transformations
     **/
    void generateHetTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {
      auto checkAndConvert = [&goal, &ids, this](OutIt it, TransformationPtr t){
        if (t && EventID(goal) <= t->out() ) {
          vector<EventType> inT = t->in(goal);
          auto used = mapTypes(inT);
          if( !inT.empty() && used.first )
            *it++ = ConfiguredTransformation(t, goal, used.second);
        }
        return it;
      };

      accumulate(mHetTrans.begin(), mHetTrans.end(), it, checkAndConvert);
    }

    void generateHomTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {

    }

    /** \brief find attribute transforms leading directly to goal
     *  \param goal the goal EventType
     *  \param ids available EventIDs
     *  \param it Output iterator to output partially configured transformations
     *  \todo enable composite transforms
     **/
    void generateAttTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {

      EventIDs comp = extractIDs(EventID(goal), ids);

      EventTypes provided;
      accumulate(comp.begin(), comp.end(), back_inserter(provided),
                 [this](TypeOutIt it, EventID id) { return addValidTypes(it, id); }
                );

      TransStorage trans = mAttNTrans;
      copy(mAtt1Trans.begin(), mAtt1Trans.end(), back_inserter(trans));
      vector<ConfiguredTransformation> temp;

      for(const EventType& in : provided)
        for(TransformationPtr t : trans) {

          EventTypes inList = t->in(goal, in);
          auto result = extractID(EventID(goal), inList);

          if(result.first && result.second - in < in - goal ) {

            auto fitting = mapTypes(inList);

            if(!fitting.first)
              temp.emplace_back(t, goal, inList);
            else
              *it++ = ConfiguredTransformation(t, goal, fitting.second);
          }
        }
      for(ConfiguredTransformation& cT : temp) {
        if(cT.in().size()==1) {
          const EventType& in = cT.in()[0];
          TransformationList& list = *new TransformationList(Transformation::Type::attribute, EventID(in));
          list.push_back(cT.trans());
          TransStorage trans1;
          copy_if(mAtt1Trans.begin(), mAtt1Trans.end(), back_inserter(trans1),
                  [&cT](const TransformationPtr& t){ return t != cT.trans();}
                  );
          cT.trans(TransformationPtr(&list));
          for(const EventType& in : provided)
            for(const TransformationPtr t : trans1) {
              EventTypes inList = t->in(cT.in()[0], in);
              if(inList.size()==1 && inList[0] - in < in - cT.in()[0]) {
                list.push_back(t);
                cT.in(inList);
              }
            }
        }
        auto fitting = mapTypes(cT.in());

        if(fitting.first)
          *it++ = cT;

      }
    }

  public:
    void regTrans(const Transformation& trans) {
      TransStorage* storagePtr;
      switch(trans.type()) {
        case(Transformation::Type::invalid): return;
        case(Transformation::Type::heterogeneus): storagePtr = &mHetTrans; break;
        case(Transformation::Type::homogeneus)  : storagePtr = &mHomTrans; break;
        case(Transformation::Type::attribute)   : storagePtr = (trans.arity()==1)?&mAtt1Trans:&mAttNTrans; break;
      }
      TransformationPtr tPtr(&trans, [](const Transformation*){});
      if(count(storagePtr->begin(), storagePtr->end(), tPtr))
        return;
      storagePtr->push_back(tPtr);
    }

    void unregTrans(const Transformation& trans) {
      TransStorage* storagePtr;
      switch(trans.type()) {
        case(Transformation::Type::invalid): return;
        case(Transformation::Type::heterogeneus): storagePtr = &mHetTrans; break;
        case(Transformation::Type::homogeneus)  : storagePtr = &mHomTrans; break;
        case(Transformation::Type::attribute)   : storagePtr = (trans.arity()==1)?&mAtt1Trans:&mAttNTrans; break;
      }

      TransformationPtr tPtr(&trans, [](const Transformation*){});
      auto it = remove(storagePtr->begin(), storagePtr->end(), tPtr);
      storagePtr->erase(it, storagePtr->end());
    }

    void regType(const EventType& eT) {
      mTypes.registerType(eT, eT);
    }

    void unregType(const EventType& eT) {
      mTypes.unregisterType(eT);
    }

    /** \brief Find Composite Transformations for EventType
     *  \param goal the output of the Transformations
     *  \return a list of ConfigureTransformation
     *  \todo Add homogeneus transforms
     *  \todo enable composite transforms
     **/
    Transformations find(const EventType& goal) {
      EventIDs ids = mTypes.ids();
      sort(ids.begin(), ids.end()); //<< Sort EventIDs ascending

      Transformations result;

      generateHetTrans(goal, ids, back_inserter(result));
      generateHomTrans(goal, ids, back_inserter(result));
      generateAttTrans(goal, ids, back_inserter(result));

      auto it = remove_if(result.begin(), result.end(),
                          [](const ConfiguredTransformation& t){ return !t.check(); }
                );
      result.erase(it, result.end());

      return result;
    }

    void clear() {
      mTypes.clear();
      mHetTrans.clear();
      mAtt1Trans.clear();
      mAttNTrans.clear();
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

void KnowledgeBase::clear() {
  KB::instance().clear();
}
