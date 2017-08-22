#include <KnowledgeBase.h>
#include <EventTypeHelpers.h>
#include <TransformationGraph.h>
#include <AbstractRegistry.h>

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
     *  \param it OutputIterator of the partially complete vector
     *  \return the modified Output Iterator
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
    OutIt closeTrans(const CompositeTransformation& cT, const EventIDs& ids,
                                  const TransStorage& trans, OutIt it) const {
      if(cT.in().empty())
        return it;

      EventTypes todo = unfitEvents(cT, ids);

      if( !todo.empty()) {
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
                  if( newCT.add(tPtr, v, eT, provided).second)
                    *it++ = newCT;
                }
              }
            }
          }
        }
      }
      return it;
    }

    /** \brief find homogeneus transforms leading directly to goal
     *  \param goal the goal EventType
     *  \param ids the clist of currently published ids
     *  \param it the OutputIterator to the CompositeTransformation storage
     *  \todo implement
     **/
    void generateHomTrans(const EventType& goal, const MetaFilter& filter, const EventIDs& ids, OutIt it) const {
      for(TransformationPtr t : mHomTrans) {
        CompositeTransformation cT(t, goal, EventType(), filter);
        if(!cT.in().empty())
          *it++ = cT;
      }
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
          CompositeTransformation cT(t, goal, in);
          auto result = findGoalEventType(goal, cT.in());

          if(result.second && result.first - in < in - goal )
            *it++ = cT;
        }
    }

  public:
    EventTypes findCompatible(const EventType& eT) {
      EventTypes result;
      for(const EventType& curr : mTypes)
        if(eT<=curr)
          result.push_back(curr);
      return result;
    }
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
     *  \param filter the filter expression stated by the subscriber
     *  \return a list of ConfigureTransformation
     **/
    Transformations find(const EventType& goal, const MetaFilter& filter) {
      EventIDs ids = mTypes.ids();
      sort(ids.begin(), ids.end(), EventID::comp); //<< Sort EventIDs ascending

      Transformations result;

      mHetTrans.generate(goal, ids, back_inserter(result));
      generateHomTrans(goal, filter, ids, back_inserter(result));
      generateAttTrans(goal, ids, back_inserter(result));

      /** \TODO: start dirty hack including homogeneus transforms as final trans **/
      for(const Transformation* homTrans: mHomTrans)
        for(CompositeTransformation& cT: result) {
          CompositeTransformation homCT(homTrans, cT.out(), EventType(), filter);
          EventTypes inETs =homCT.in();
          if(inETs.empty()) continue;
          swap(cT, homCT);
          cT.add(move(homCT), homCT.root(), cT.out());
        }
      /** \TODO: end dirty hack including homogeneus transforms as final trans **/

      auto close = [&result, &ids, this](const TransStorage& trans){
        Transformations temp = result;
        do {
          Transformations curr;
          auto fold=[this, &ids, &trans](OutIt it, const CompositeTransformation& cT) {
            return closeTrans(cT, ids, trans, it);
          };
          accumulate(temp.begin(), temp.end(), back_inserter(curr), fold);
          temp = move(curr);
          copy(temp.begin(), temp.end(), back_inserter(result));
        }while(!temp.empty());
      };

      close(mAttNTrans);
      close(mAtt1Trans);

      auto check = [this, &ids](const CompositeTransformation& t){ return !unfitEvents(t, ids).empty() || !t.check(); };
      result.erase(remove_if(result.begin(), result.end(), check), result.end());

      return result;
    }

    void clear() {
      mTypes.clear();
      mHetTrans.clear();
      mHomTrans.clear();
      mAtt1Trans.clear();
      mAttNTrans.clear();
    }

    void print(ostream& o) const {
      o << mHetTrans;
      o << "Homogeneus Transforms: " << mHomTrans.size() << endl;
      for(const TransformationPtr& t : mHomTrans)
        o << *t << endl;
      o << "Unary Attribute Transforms: " << mAtt1Trans.size() << endl;
      for(const TransformationPtr& t : mAtt1Trans)
        o << *t << endl;
      o << "N-Ary Attribute Transforms: " << mAttNTrans.size() << endl;
      for(const TransformationPtr& t : mAttNTrans)
        o << *t << endl;
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

vector<EventType> KnowledgeBase::findCompatible(const EventType& eT) {
  return KB::instance().findCompatible(eT);
}

KnowledgeBase::Transformations KnowledgeBase::findTransforms(const EventType& goal, const MetaFilter& filter) {
  return KB::instance().find(goal, filter);
}

void KnowledgeBase::clear() {
  KB::instance().clear();
}

void KnowledgeBase::print(ostream& o) {
  KB::instance().print(o);
}
