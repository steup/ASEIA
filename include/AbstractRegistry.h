#pragma once

#include <EventID.h>
#include <FormatID.h>

#include <utility>
#include <unordered_map>

class EventType;

template<typename It>
struct RegistryIterator : public std::iterator< std::forward_iterator_tag,
                                  typename It::value_type::second_type> {
  private:
    It mIt;
  public:
    using value_type = typename std::iterator<std::forward_iterator_tag, typename It::value_type::second_type>::value_type;

    RegistryIterator() = default;
    RegistryIterator(It t) : mIt(t) {}
    RegistryIterator& operator=(It t) {
      mIt=t;
      return *this;
    }
    const value_type& operator*() const { return mIt->second; }
    const value_type* operator->() const { return &mIt->second; }
    operator const value_type*() const { return &mIt->second; }
    RegistryIterator& operator++() { mIt++; return *this;}
    RegistryIterator operator++(int) {
      RegistryIterator temp = *this;
      mIt++;
      return temp;
    }
};

template<typename It>
struct RegistryRange : public std::pair<It, It> {
  using value_type = typename It::value_type;
  RegistryRange() = default;
  RegistryRange(It begin, It end)
    : std::pair<It, It>(begin, end)
  {}
  It begin() const {return this->first; }
  It end() const {return this->second; }
  bool empty() const { return this->first==this->second; }
  typename It::value_type front() { return *this->first; }
};

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

    using const_iterator = RegistryIterator<typename Storage::const_iterator>;
    using const_local_iterator = RegistryIterator<typename Storage::const_local_iterator>;

    using Range = RegistryRange<const_iterator>;
    using LocalRange = RegistryRange<const_local_iterator>;

		void registerType(const EventType& eT, const T& t) {
      mStorage.emplace(Key(eT, eT), t);
    }

    void registerType(const EventType& eT, T&& t) {
      mStorage.emplace(Key(eT, eT), std::move(t));
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

    LocalRange find(EventID id) const {
      Key k(id);
      std::size_t b=mStorage.bucket(k);
      return LocalRange(mStorage.begin(b), mStorage.end(b));
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
