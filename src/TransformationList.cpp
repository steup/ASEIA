#include <TransformationList.h>
#include <MetaEvent.h>

#include <ostream>

using namespace std;

using EventIDs = Transformation::EventIDs;
using TransPtr = Transformation::TransPtr;

class TransformerList : public Transformer {
  private:
    vector<TransPtr> mTrans;
  public:
    TransformerList(const Transformation& t, const EventType& out,
                    const EventTypes in, vector<TransPtr>&& trans)
      : Transformer(t, out, in), mTrans(move(trans))
    {}

    virtual bool check(const Events& events) const {
      return false;
    }

    virtual MetaEvent operator()(const Events& events) {
      return MetaEvent();
    }

    virtual void print(std::ostream& o) const {

    }
};


TransformationList::TransformationList(InitType list)
  : Transformation(list.size()==0?Type::invalid:(*begin(list))->type(), 1, EventID::any), mTransList(list)
{}

std::vector<EventType> TransformationList::in(const EventType& goal) const {
  EventType curr = goal;
  for(const TransformationPtr& t : mTransList) {
    EventTypes temp(t->in(curr));
    if(temp.empty())
      return {};
    curr=temp[0];
  }
  return {curr};
}

std::vector<EventType> TransformationList::in(const EventType& goal, const  EventType& provided) const {
  EventType curr = goal;
  for(const TransformationPtr& t : mTransList) {
    EventTypes temp(t->in(curr, provided));
    if(temp.empty())
      return {};
    curr=temp[0];
  }
  return {curr};
}

EventIDs TransformationList::in(EventID goal) const {
  EventID curr = goal;
  for(const TransformationPtr& t : mTransList) {
    EventIDs temp(t->in(curr));
    if(temp.empty())
      return {};
    curr=temp[0];
  }
  return {curr};
}

TransPtr TransformationList::create(const EventType& out, const EventTypes& in) const {
  if(in.size() != 1)
    return TransPtr();
  vector<TransPtr> trans;
  EventType currOut = out;
  if(type()==Type::heterogeneus)
  for(const TransformationPtr& t : mTransList) {
    EventTypes currIn(t->in(currOut));
    if(currIn.size() != 1)
      return TransPtr();
    trans.emplace_back(t->create(currOut, currIn));
    currOut = currIn[0];
  }
  reverse(trans.begin(), trans.end());
  return TransPtr(new TransformerList(*this, out, in, move(trans)));
}

void TransformationList::print(std::ostream& o) const {
  o << "Transformation List: " << endl;
  for(const TransformationPtr& t : mTransList) {
    o << "\t";
    t->print(o);
    o << endl;
  }
}
