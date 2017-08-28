#pragma once

#include <MetaEvent.h>
#include <EventType.h>

#include <boost/circular_buffer.hpp>

#include <vector>
#include <iterator>

/** \todo define interface **/
class AbstractPolicy {
  public:
    size_t bufferSize=10;
};

class EventStorage {
  public:
    using EventTypes = std::vector<EventType>;
  private:
    using Storage = std::vector<boost::circular_buffer<MetaEvent>>;
    Storage mStorage;
    const AbstractPolicy& mPolicy;
    const EventTypes& mInTypes;
    std::vector<size_t> mCurrIndices;
	public:
    class const_iterator : public std::iterator<std::forward_iterator_tag, std::vector<const MetaEvent*>> {
      private:
        long mIndex=-1;
        long mFactor=1;
        const std::vector<size_t>* mCurrIndicesPtr = nullptr;
        const Storage* mStoragePtr = nullptr;
      public:
        const_iterator() = default;
        const_iterator(const Storage& storage, const std::vector<size_t>& currIndicesPtr);
        std::vector<const MetaEvent*> operator*() const;
        bool operator==(const_iterator b) const { return mIndex == b.mIndex; }
        bool operator!=(const_iterator b) const { return mIndex != b.mIndex; }
        const_iterator& operator++();
        const_iterator operator++(int) { auto temp=*this; ++(*this); return temp; }
    };
    EventStorage(const EventTypes& in, const AbstractPolicy& policy);
		void addEvent(const MetaEvent& e);
		void purge() { mStorage.clear(); };
    const_iterator begin() const { return const_iterator(mStorage, mCurrIndices); };
    const_iterator end() const { return const_iterator(); }
};
