#include <KnowledgeBase.h>
#include <TypeRegistry.h>
#include <EventTypeHelpers.h>

#include <Singleton.h>

#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

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
    using VertexList = CompositeTransformation::VertexList;
    using Vertex     = CompositeTransformation::Vertex;
    using TransList  = CompositeTransformation::TransList;
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

    EventTypes extractCompatibleEventTypes(EventID goal, const EventIDs& ids) const {
      EventIDs comp = extractCompatibleIDs(goal, ids);

      EventTypes provided;
      accumulate(comp.begin(), comp.end(), back_inserter(provided),
                 [this](TypeOutIt it, EventID id) { return addValidTypes(it, id); }
                );
      return provided;
    }

    EventTypes unfitEvents(const CompositeTransformation& cT, const EventIDs& ids) const {
      EventTypes todo;
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
        if(!found)
          todo.push_back(in);
      }
      return todo;
    }
    /** \brief (partially) complete incomplete CompositeTransformations
     *  \param cT the incomplete CompositeTransformation
     *  \param ids the current sorted EventIDs available
     *  \param its a apair of OutputIterators to the result and the partially complete vectors
     *  \return the modified pair of Output Iterators
     *
     *  This method tries different attribute transformations to transform
     *  published EventTypes to input EventTypes of the given
     *  CompositeTransformation. In this process the CompositeTransformation
     *  may either be complete, when it is put to the result list or it is
     *  still incomplete but changed, when it is output to the partial list. If
     *  no transformation applied the CompositeTransformation cannot be
     *  fulfilled and is discarded. In the case multiple transformations apply
     *  additional CompositeTransformations are created and put in the partial
     *  list.
     *
     **/
    pair<OutIt, OutIt> closeTrans(const CompositeTransformation& cT, const EventIDs& ids,
                                  const TransStorage& trans, pair<OutIt, OutIt> its) const {
      if(cT.in().empty())
        return its;

      EventTypes todo = unfitEvents(cT, ids);

      if(todo.empty())
        *its.first++ = move(cT);
      else {
        for(const EventType& eT : todo) {
          VertexList unfitTrans = cT.find(eT);
          for(Vertex v : unfitTrans) {
            TransStorage tempTrans;
            TransList path = cT.path(v);
            copy_if(trans.begin(), trans.end(), back_inserter(tempTrans),
                    [&path](TransformationPtr tPtr){return std::find(path.begin(), path.end(), tPtr)==path.end(); });
            for(TransformationPtr tPtr : tempTrans) {
              for(const EventType& provided: extractCompatibleEventTypes(eT, ids)) {
                EventTypeResult result = findGoalEventType(eT, tPtr->in(eT, provided));
                if( result.second && result.first - provided < provided - eT) {
                  CompositeTransformation newCT = cT;
                  newCT.addTransformation(tPtr, v, eT, provided);
                  *its.second++ = newCT;
                }
              }
          }
          }
        }
      }
      return its;
    }


    /** \brief create fitting partially configured composite transformations for goal EventID
     * \param goal the EventID of the goal
     * \param all existing published EventIDs
     * \param it Output iterator to output fitting partially generated transformations
     * \todo filter on input eventids
     * \todo generate full composite transformation tree (Spanning Tree search)
     **/
    void generateHetTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {
      auto checkAndConvert = [&goal, &ids, this](OutIt it, TransformationPtr t){
        if (EventID(goal) <= t->out()) {
          CompositeTransformation cT(t, goal, EventType());
          if( !cT.in().empty())
            *it++ = cT;
        }
        return it;
      };

      accumulate(mHetTrans.begin(), mHetTrans.end(), it, checkAndConvert);
    }

    /** \brief find homogeneus transforms leading directly to goal
     *  \param goal the goal EventType
     *  \param ids the clist of currently published ids
     *  \param it the OutputIterator to the CompositeTransformation storage
     *  \todo implement
     **/
    void generateHomTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {

    }

    /** \brief find attribute transforms leading directly to goal
     *  \param goal the goal EventType
     *  \param ids available EventIDs
     *  \param it Output iterator to output partially configured transformations
     **/
    void generateAttTrans(const EventType& goal, const EventIDs& ids, OutIt it) const {
      EventTypes provided = extractCompatibleEventTypes(goal, ids);

      TransStorage trans = mAttNTrans;
      copy(mAtt1Trans.begin(), mAtt1Trans.end(), back_inserter(trans));

      for(const EventType& in : provided)
        for(TransformationPtr t : trans) {
          auto result = findGoalEventType(goal, t->in(goal, in));

          if(result.second && result.first - in < in - goal )
            *it++ = CompositeTransformation(t, goal, in);
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
     **/
    Transformations find(const EventType& goal) {
      EventIDs ids = mTypes.ids();
      sort(ids.begin(), ids.end()); //<< Sort EventIDs ascending

      Transformations initial;
      Transformations result;

      generateHetTrans(goal, ids, back_inserter(initial));
      generateHomTrans(goal, ids, back_inserter(initial));
      generateAttTrans(goal, ids, back_inserter(initial));

/*      bool done;
      do {
        Transformations temp;
        auto fold=[this, &ids](pair<OutIt, OutIt> its, const CompositeTransformation& cT) { return closeTrans(cT, ids, mAttNTrans, its);};
        accumulate(initial.begin(), initial.end(), make_pair(back_inserter(result), back_inserter(temp)), fold);
        done = !temp.empty();
        move(temp.begin(), temp.end(), back_inserter(initial));
      }while(done);*/

      do {
        Transformations temp;
        auto fold=[this, &ids](pair<OutIt, OutIt> its, const CompositeTransformation& cT) {
          return closeTrans(cT, ids, mAtt1Trans, its);
        };
        accumulate(initial.begin(), initial.end(), make_pair(back_inserter(result), back_inserter(temp)), fold);
        initial.clear();
        move(temp.begin(), temp.end(), back_inserter(initial));
      }while(!initial.empty());

      auto check = [](const CompositeTransformation& t){ return !t.check(); };
      result.erase(remove_if(result.begin(), result.end(), check), result.end());

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
