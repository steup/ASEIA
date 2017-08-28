#include <EventStorage.h>

#include <numeric>

using namespace std;

EventStorage::const_iterator::const_iterator(const Storage& storage, const vector<size_t>& currIndices)
  : mFactor(0), mCurrIndicesPtr(&currIndices), mStoragePtr(&storage)  {
  for(size_t currIndex : currIndices)  {
    size_t temp=1;
    for(size_t i=0; i<mStoragePtr->size(); i++) {
      size_t size=1;
      if(i!=currIndex) {
        size=(*mStoragePtr)[i].size();
        if(count(mCurrIndicesPtr->begin(), mCurrIndicesPtr->end(), i))
          size-=1;
      }
      temp*=size;
    }
    mFactor+=temp;
  }
  if(mFactor)
    mIndex = 0;
}

vector<const MetaEvent*> EventStorage::const_iterator::operator*() const {
  vector<const MetaEvent*> result;
  size_t index = mIndex;
  size_t factor = mFactor;
  factor/=mCurrIndicesPtr->size();
  size_t currIndex = (*mCurrIndicesPtr)[index/factor];
  index/=mCurrIndicesPtr->size();
  for(size_t i=0;i<mStoragePtr->size();i++) {
    size_t mod=count(mCurrIndicesPtr->begin(), mCurrIndicesPtr->end(), i)?1:0;
    const auto& subStorage = (*mStoragePtr)[i];
    if(i==currIndex)
      result.push_back(&subStorage.back());
    else {
      factor/=subStorage.size()-mod;
      result.push_back(&subStorage[index/factor]);
      index/=subStorage.size()-mod;
    }
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
  mCurrIndices.clear();
  for(size_t i=0;i<mInTypes.size();i++)
    if(((EventType)e).isCompatible(mInTypes[i])) {
      mStorage[i].push_front(e);
      mCurrIndices.push_back(i);
    }
}
