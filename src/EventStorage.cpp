#include <EventStorage.h>

using namespace std;

/** \todo implement **/
vector<const MetaEvent*> EventStorage::const_iterator::operator*() const {
  vector<const MetaEvent*> result;
  return result;
}

/** \todo implement **/
EventStorage::EventStorage(const EventTypes& in, const AbstractPolicy& policy)
  : mPolicy(policy), mInTypes(in)  {

}

/** \todo implement **/
void EventStorage::addEvent(const MetaEvent& e) {

}
