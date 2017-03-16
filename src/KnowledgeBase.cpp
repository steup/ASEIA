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
    TransStorage mHetTrans;
    TransStorage mAttrTrans;
    TypeStorage  mTypes;

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
     **/
    void addValidTypes(EventID id, TypeOutIt it) const {
      transform(mTypes.find(id).first, mTypes.find(id).second, it, [](const EventType& eT){ return &eT;});
    }

    /** \brief generate list of compatible EventTypes for input EventIDs
     *  \param in Input list of EventIDs
     *  \param ids  all existing published EventIds
     *  \return list of compatible EventTypes for each input EventID
     **/
    vector<EventTypes> generateTypeLists(const EventIDs& in, const EventIDs& ids) const{
      //Extract IDs
      vector<EventTypes> result(in.size());
      for(unsigned int i=0; i < in.size(); i++) {
        addValidTypes(in[i], back_inserter(result[i]));
        if(!result[i].size())
          break;
      }
      return result;
    }

    /** \brief compute kartesian product
     *  \param typeLists list of EventTypes for each required input EventID
     *  \return kartesian product of the input lists
     **/
    vector<EventTypes> cartesianProduct(const vector<EventTypes>& typeLists) const {
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
    }

    /** \brief generate all fully configured transformations
     *  \param trans a partially configured transformation
     *  \param ids currently published EventIDs
     *  \param it Output iterator to store resulting fully configured transformations
     **/
    void generateAll(const ConfiguredTransformation& trans, const EventIDs& ids, OutIt it) const {
      vector<EventTypes> usableEventTypes = generateTypeLists(trans.inIDs(), ids);
      vector<EventTypes> typeCombinations = cartesianProduct(usableEventTypes);
      auto create = [trans](const EventTypes& in){
        ConfiguredTransformation t(trans);
        t.in(in);
        return t;
      };
      transform(typeCombinations.begin(), typeCombinations.end(), it, create);
    }

    /** \brief create fitting partially configured composite transformations for goal EventID
     * \param goal the EventID of the goal
     * \param all existing published EventIDs
     * \param it Output iterator to output fitting partially generated transformations
     * \todo filter on input eventids
     * \todo generate composite transformations
     **/
    void findHetTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {
      auto checkAndConvert = [&goal, &ids](OutIt it, const Transformation* t){
        if (t && EventID(goal) <= t->out() )
          return *it++ = ConfiguredTransformation(*t, goal);// || !includes(tIn.begin(), tIn.end(), ids.begin(), ids.end());
        else
          return it;
      };

      accumulate(mHetTrans.begin(), mHetTrans.end(), it, checkAndConvert);
    }

    /** \brief find attribute transforms leading directly to goal
     *  \param goal the goal EventType
     *  \param ids available EventIDs
     *  \param it Output iterator to output partially configured transformations
     *  \todo enable composite transforms
     **/
    void findAttrTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {

      EventIDs comp = extractIDs(EventID(goal), ids);
      if(!comp.size())
        return;
      
      auto toConfTrans = [&goal](const Transformation* t){ return ConfiguredTransformation(*t, goal); };
      transform(mAttrTrans.begin(), mAttrTrans.end(), it, toConfTrans);
    }

  public:
    void regTrans(const Transformation& trans) {
      if(trans==Transformation::invalid)
        return;
      TransStorage& storage = (trans.out() == EventID::any)?mAttrTrans:mHetTrans;
      if(count(storage.begin(), storage.end(), &trans))
        return;
      storage.push_back(&trans);
    }

    void unregTrans(const Transformation& trans) {
      TransStorage& storage = (trans.out() == EventID::any)?mAttrTrans:mHetTrans;
      auto it = remove(storage.begin(), storage.end(), &trans);
      storage.erase(it, storage.end());
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

      Transformations temp;
      Transformations result;

      findHetTrans(goal, ids, back_inserter(temp));
      findAttrTrans(goal, ids, back_inserter(temp));

      for(const ConfiguredTransformation& confTrans : temp)
        generateAll(confTrans, ids, back_inserter(result));

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
