#include <KnowledgeBase.h>
#include <TypeRegistry.h>

#include <Singleton.h>

#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class KBImpl {
  public:
    using TransStorage = std::vector<const Transformation*>;
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
      return transform(mTypes.find(id).first, mTypes.find(id).second, it, [](const EventType& eT){ return &eT;});
    }

    /** \brief generate list of compatible EventTypes for input EventIDs
     *  \param in Input list of EventIDs
     *  \param ids  all existing published EventIds
     *  \return list of compatible EventTypes for each input EventID
     **/
    /*vector<EventTypes> generateTypeLists(const EventIDs& in, const EventIDs& ids) const{
      //Extract IDs
      vector<EventTypes> result(in.size());
      for(unsigned int i=0; i < in.size(); i++) {
        addValidTypes(in[i], back_inserter(result[i]));
        if(!result[i].size())
          break;
      }
      return result;
    }*/

    /** \brief compute kartesian product
     *  \param typeLists list of EventTypes for each required input EventID
     *  \return kartesian product of the input lists
     **/
    /*vector<EventTypes> cartesianProduct(const vector<EventTypes>& typeLists) const {
      if(typeLists.empty())
        return vector<EventTypes>();

      size_t size = accumulate(typeLists.begin(), typeLists.end(), 1UL, [](size_t size, const EventTypes& tL){ return size*tL.size(); });

      if(!size)
        return vector<EventTypes>();

      vector<EventTypes> result(size);
      for(size_t i=0; i<size; i++)
        result[i].resize(typeLists.size());

      size_t currSize=size;
      size_t oldSize = 1;
      for(size_t i=0; i < typeLists.size(); i++) {
        for(size_t m=0; m < oldSize; m++)
          for(size_t j=0; j < typeLists[i].size(); j++) {
            currSize /= typeLists[i].size();
            for(size_t k=0; k < currSize; k++)
              result[j*currSize+k][i] = typeLists[i][j];
          }
        oldSize = typeLists[i].size();
      }
      return result;
    }*/

    /** \brief generate all fully configured transformations
     *  \param trans a partially configured transformation
     *  \param ids currently published EventIDs
     *  \param it Output iterator to store resulting fully configured transformations
     **/
    /*void generateAll(const ConfiguredTransformation& trans, const EventIDs& ids, OutIt it) const {
      vector<EventTypes> usableEventTypes = generateTypeLists(trans.inIDs(), ids);
      vector<EventTypes> typeCombinations = cartesianProduct(usableEventTypes);
      auto create = [trans](const EventTypes& in){
        ConfiguredTransformation t(trans);
        t.in(in);
        return t;
      };
      transform(typeCombinations.begin(), typeCombinations.end(), it, create);
    }*/

    EventTypes mapTypes(const vector<EventType>& types) const {
      auto convert = [this](const EventType& t){
        auto range = mTypes.find(t);
        return range.empty()?nullptr:(const EventType*)range.first;
      };
      EventTypes used(types.size());
      transform(types.begin(), types.end(), used.begin(), convert);
      return used;
    }

    /** \brief create fitting partially configured composite transformations for goal EventID
     * \param goal the EventID of the goal
     * \param all existing published EventIDs
     * \param it Output iterator to output fitting partially generated transformations
     * \todo filter on input eventids
     * \todo generate composite transformations
     **/
    void generateHetTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {
      auto checkAndConvert = [&goal, &ids, this](OutIt it, const Transformation* t){
        if (t && EventID(goal) <= t->out() ) {
          vector<EventType> inT = t->in(goal);
          EventTypes used = mapTypes(inT);
          if( !inT.empty() && !count(used.begin(), used.end(), nullptr) )
            *it++ = ConfiguredTransformation(*t, goal, used);
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

      TransStorage trans = mAtt1Trans;
      copy(mAttNTrans.begin(), mAttNTrans.end(), back_inserter(trans));

      for(const EventType* in : provided)
        for(const Transformation* t : trans) {
          vector<EventType> inT = t->in(goal, *in);
          EventTypes used = mapTypes(inT);
          if( !inT.empty() && !count(used.begin(), used.end(), nullptr) )
            *it++ = ConfiguredTransformation(*t, goal, used);
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
      if(count(storagePtr->begin(), storagePtr->end(), &trans))
        return;
      storagePtr->push_back(&trans);
    }

    void unregTrans(const Transformation& trans) {
      TransStorage* storagePtr;
      switch(trans.type()) {
        case(Transformation::Type::invalid): return;
        case(Transformation::Type::heterogeneus): storagePtr = &mHetTrans; break;
        case(Transformation::Type::homogeneus)  : storagePtr = &mHomTrans; break;
        case(Transformation::Type::attribute)   : storagePtr = (trans.arity()==1)?&mAtt1Trans:&mAttNTrans; break;
      }
      auto it = remove(storagePtr->begin(), storagePtr->end(), &trans);
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
