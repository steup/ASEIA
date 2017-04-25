#include <KnowledgeBase.h>
#include <TypeRegistry.h>
#include <EventTypeHelpers.h>
#include <TransformationGraph.h>

#include <Singleton.h>

#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <ostream>

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
    TransformationGraph mHetTrans;
    TransStorage mHomTrans, mAtt1Trans, mAttNTrans;


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
        return its;
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
      TransformationPtr tPtr(&trans);
      switch(trans.type()) {
        case(Transformation::Type::invalid): return;
        case(Transformation::Type::heterogeneus): mHetTrans.insert(tPtr); return;
        case(Transformation::Type::homogeneus)  : storagePtr = &mHomTrans; break;
        case(Transformation::Type::attribute)   : storagePtr = (trans.arity()==1)?&mAtt1Trans:&mAttNTrans; break;
        default                                 : return;
      }
      if(count(storagePtr->begin(), storagePtr->end(), tPtr))
        return;
      storagePtr->push_back(tPtr);
    }

    void unregTrans(const Transformation& trans) {
      TransStorage* storagePtr;
      TransformationPtr tPtr(&trans);
      switch(trans.type()) {
        case(Transformation::Type::invalid): return;
        case(Transformation::Type::heterogeneus): mHetTrans.remove(tPtr); return;
        case(Transformation::Type::homogeneus)  : storagePtr = &mHomTrans; break;
        case(Transformation::Type::attribute)   : storagePtr = (trans.arity()==1)?&mAtt1Trans:&mAttNTrans; break;
        default                                 : return;
      }

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
      static unsigned int i=0;
      EventIDs ids = mTypes.ids();
      sort(ids.begin(), ids.end(), EventID::comp); //<< Sort EventIDs ascending

      Transformations temp;
      Transformations result;

      mHetTrans.generate(goal, ids, back_inserter(result));
      generateHomTrans(goal, ids, back_inserter(result));
      generateAttTrans(goal, ids, back_inserter(result));


      temp = result;

      do {
        Transformations curr;
        auto fold=[this, &ids](pair<OutIt, OutIt> its, const CompositeTransformation& cT) {
          return closeTrans(cT, ids, mAttNTrans, its);
        };
        accumulate(temp.begin(), temp.end(), make_pair(back_inserter(result), back_inserter(curr)), fold);
        temp.clear();
        move(curr.begin(), curr.end(), back_inserter(temp));
        copy(temp.begin(), temp.end(), back_inserter(result));
      }while(!temp.empty());
      
      temp = result;
      
      do {
        Transformations curr;
        auto fold=[this, &ids](pair<OutIt, OutIt> its, const CompositeTransformation& cT) {
          return closeTrans(cT, ids, mAtt1Trans, its);
        };
        accumulate(temp.begin(), temp.end(), make_pair(back_inserter(result), back_inserter(curr)), fold);
        temp.clear();
        move(curr.begin(), curr.end(), back_inserter(temp));
        copy(temp.begin(), temp.end(), back_inserter(result));
      }while(!temp.empty());

      auto check = [this, &ids](const CompositeTransformation& t){ return !unfitEvents(t, ids).empty() || !t.check(); };
      result.erase(remove_if(result.begin(), result.end(), check), result.end());

      return result;
    }

    void clear() {
      mTypes.clear();
      mHetTrans.clear();
      mAtt1Trans.clear();
      mAttNTrans.clear();
    }

    void print(ostream& o) const {
      o << mHetTrans;
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

void KnowledgeBase::print(ostream& o) {
  KB::instance().print(o);
}
