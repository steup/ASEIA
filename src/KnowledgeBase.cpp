#include <KnowledgeBase.h>
#include <TypeRegistry.h>
#include <EventTypeHelpers.h>

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


    /** \brief add compatible EventTypes to list
     *  \param id the EventID the EventTypes need to be compatible to
     *  \param it Output iterator to store EventTypes
     *  \return the Output iterator after insertion
     **/
    TypeOutIt addValidTypes(TypeOutIt it, EventID id) const {
      return copy(mTypes.find(id).first, mTypes.find(id).second, it);
    }
    /** \todo find all compatible types not just direct fits **/
    bool mapTypes(const vector<EventType>& types) const {
      EventTypes used;
      for(const EventType& eT : types) {
        auto range = mTypes.find(eT);
        if(range.empty())
          return false;
      }
      return true;
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
          CompositeTransformation cT(t, goal, EventType());
          if( !cT.in().empty() && mapTypes(cT.in()) )
            *it++ = cT;
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

      EventIDs comp = extractCompatibleIDs(EventID(goal), ids);

      EventTypes provided;
      accumulate(comp.begin(), comp.end(), back_inserter(provided),
                 [this](TypeOutIt it, EventID id) { return addValidTypes(it, id); }
                );

      TransStorage trans = mAttNTrans;
      copy(mAtt1Trans.begin(), mAtt1Trans.end(), back_inserter(trans));
      Transformations temp;

      for(const EventType& in : provided)
        for(TransformationPtr t : trans) {

          EventTypes inList = t->in(goal, in);
          auto result = findCompatibleEventType(EventID(goal), inList);

          if(result.second && result.first - in < in - goal ) {

            if(mapTypes(inList)) {
              temp.emplace_back(t, goal, result.first);
              temp.back().in(inList);
            }
            else
              *it++ = CompositeTransformation(t, goal, result.first);
          }
        }

      for(CompositeTransformation& cT : temp) {
          TransStorage trans1;
          copy_if(mAtt1Trans.begin(), mAtt1Trans.end(), back_inserter(trans1),
                  [cT](const TransformationPtr& t){ return cT.contains(t);}
                  );
          for(const EventType& in :  cT.in()) {
            EventIDs compIDs = extractCompatibleIDs(in, ids);
            bool found = false;
            for(EventID id : compIDs) {
              if(found)break;
              for(const EventType& provType : mTypes.find(id))
                if(in<=provType)  {
                  found=true;
                  break;
                }
            }
          } 
        if(mapTypes(cT.in()))
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
        default                                 : return;
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
        default                                 : return;
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
                          [](const CompositeTransformation& t){ return !t.check(); }
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
