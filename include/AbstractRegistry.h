#pragma once

#include <EventID.h>
#include <FormatID.h>

#include <utility>
#include <unordered_map>

class EventType;

template<typename T>
class AbstractRegistry {
  protected:
		struct Key  : public std::pair<EventID, FormatID> {
			Key(EventID eID, FormatID fID=FormatID())
				: std::pair<EventID, FormatID>(eID, fID)
			{}
      public:
      EventID eventID() const {
        return first;
      }
      FormatID formatID() const {
        return second;
      }
      bool operator==(const Key& b) const { return eventID() == b.eventID() && formatID() == b.formatID(); }
      bool operator!=(const Key& b) const { return !(*this == b); }
		};

    struct Hash {
      std::size_t operator()(const Key& k) const {
        return k.first;
      }
    };
    using Storage = std::unordered_multimap<Key, T, Hash>;
    Storage mStorage;

  public:
    using value_type = typename Storage::mapped_type;

    template<typename It>
    struct AbstractValueIterator : public It {
			AbstractValueIterator() = default;
      AbstractValueIterator(It t) : It(t) {}
      AbstractValueIterator& operator=(It t) {
        *this=t;
        return *this;
      }

      using value_type = typename It::value_type::second_type;
      const value_type& operator*() const { return static_cast<const It&>(*this)->second; }
      const value_type* operator->() const { return &static_cast<const It&>(*this)->second; }
			operator const value_type*() const { return &static_cast<const It&>(*this)->second; }
      AbstractValueIterator& operator++() { static_cast<It&>(*this)++; return *this;}
      AbstractValueIterator operator++(int) { 
				AbstractValueIterator temp = *this;
				++(*this);
				return temp;}
    };

    using const_iterator = AbstractValueIterator<typename Storage::const_iterator>;
    using const_local_iterator = AbstractValueIterator<typename Storage::const_local_iterator>;

    template<typename It>
    struct AbstractRange : public std::pair<It, It> {
      AbstractRange(It begin, It end)
        : std::pair<It, It>(begin, end)
      {}
      It begin() const {return this->first; }
      It end() const {return this->second; }
      bool empty() const { return this->first==this->second; }
      typename It::value_type front() { return *this->first; }
    };

    using Range = AbstractRange<const_iterator>;
    using LocalRange = AbstractRange<const_local_iterator>;

		void registerType(const EventType& eT, const T& t) {
      mStorage.insert(std::make_pair(Key(eT, eT), t));
    }

    const_iterator begin() const {
      return mStorage.begin();
    }

    const_iterator end() const {
      return mStorage.end();
    }

    Range find(EventID eID, FormatID fID) const {
      return Range(mStorage.find(Key(eID, fID)), end());
    }

    Range find(const EventType& eT) const {
      return find(eT, eT);
    }

    const_local_iterator end(EventID id) const {
      Key k(id);
      std::size_t b=mStorage.bucket(k);
      return mStorage.end(b);
    }

    LocalRange find(EventID id) const {
      Key k(id);
      std::size_t b=mStorage.bucket(k);
      return LocalRange(mStorage.begin(b), end(id));
    }

		bool contains(EventID id) const {
      return !find(id).empty();
    }

    bool contains(EventID eID, FormatID fID) const {
      return !find(eID, fID).empty();
    }

    bool contains(const EventType& eT) const {
      return contains(eT, eT);
    }
};
