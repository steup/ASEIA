#include <EventStorage.h>

#include <numeric>

using namespace std;

EventStorage::const_iterator::const_iterator(const Storage& storage)
  : mStoragePtr(&storage) {
  auto acc =  [](size_t v, const boost::circular_buffer<MetaEvent>& b){ return v*b.size(); };
  mFactor = accumulate(storage.begin(), storage.end(), 1, acc);
  if(mFactor)
    mIndex = 0;
}

vector<const MetaEvent*> EventStorage::const_iterator::operator*() const {
  vector<const MetaEvent*> result;
  size_t index = mIndex;
  size_t factor = mFactor;
  for(const auto& subStorage: *mStoragePtr) {
    factor/=subStorage.size();
    result.push_back(&subStorage[index/factor]);
    index/=subStorage.size();
  }
  return result;
}

EventStorage::const_iterator& EventStorage::const_iterator::operator++() {
  if(mIndex+1 == mFactor)
    mIndex=-1;
  else
    mIndex++;
  return *this;
}

EventStorage::EventStorage(const EventTypes& in, const AbstractPolicy& policy)
  : mPolicy(policy), mInTypes(in)  {
    for(size_t i=0;i<mInTypes.size();i++)
      mStorage.emplace_back(policy.bufferSize);
}

void EventStorage::addEvent(const MetaEvent& e) {
  for(size_t i=0;i<mInTypes.size();i++)
    if(mInTypes[i]==(EventType)e)
      mStorage[i].push_front(e);
}
