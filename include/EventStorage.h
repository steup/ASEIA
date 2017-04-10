#pragma once

#include <MetaEvent.h>
#include <Transformation.h>

#include <vector>
#include <iterator>

class AbstractPolicy {
  
};

class EventStorage {
  public:
    using EventTypes = Transformer::EventTypes;
  private:
    using Storage = std::vector<Transformer::Events>;
    Storage mStorage;
    const AbstractPolicy& mPolicy;
    const EventTypes& mInTypes;
	public:
    class const_iterator : public std::iterator<std::forward_iterator_tag, std::vector<const MetaEvent*>> {
      private:
        long mIndex=-1;
        const Storage* mStoragePtr = nullptr;
      public:
        const_iterator() = default;
        const_iterator(const Storage& storage) : mIndex(0), mStoragePtr(&storage) {}
        std::vector<const MetaEvent*> operator*() const;
        std::vector<const MetaEvent*> operator->() const;
        bool operator==(const_iterator b) const { return mIndex == b.mIndex; };
        bool operator!=(const_iterator b) const { return mIndex != b.mIndex; }
        const_iterator& operator++() { mIndex++; return *this; };
        const_iterator operator++(int) { const_iterator temp=*this; mIndex++; return temp;};
    };
    EventStorage(EventTypes in, const AbstractPolicy& policy);
		~EventStorage() {}
		void addEvent(const MetaEvent& e);
		void purge() { mStorage.clear(); };
    const_iterator begin() const { return const_iterator(mStorage); };
    const_iterator end() const { return const_iterator(); }
};
