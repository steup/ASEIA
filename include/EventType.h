#pragma once

#include <AttributeType.h>
#include <map>

#include <Serializer.h>
#include <DeSerializer.h>

class EventType{
  public:
    using KeyType = id::attribute::ID;
    using ValueType = AttributeType;
  private:
    using StorageType = std::map<KeyType, ValueType>;
    using PairType    = StorageType::value_type;
    StorageType mStorage;


  public:
    class const_iterator : public std::iterator<std::forward_iterator_tag, AttributeType> {
			private:
				StorageType::const_iterator  mI;
      public:
        const_iterator(StorageType::const_iterator i);
        const ValueType& operator*() const;
        const ValueType* operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
				bool operator==(const const_iterator& b) const;
				bool operator!=(const const_iterator& b) const;
    };

    const_iterator begin() const;
    const_iterator end() const;

    EventType() = default;

    bool add(const AttributeType& aT) {
      if(mStorage.count(aT.id()))
        return false;
      mStorage.insert(std::make_pair(aT.id(), aT));
      return true;
    }

    void remove(KeyType k) { mStorage.erase(k); }

    const AttributeType* attribute(KeyType key) const;
    AttributeType* attribute(KeyType key);


    const AttributeType& operator[](KeyType key) const { return *attribute(key); }
    AttributeType& operator[](KeyType key)             { return *attribute(key); }

    std::size_t operator-(const EventType& b) const;
    bool operator==(const EventType& b) const;
    bool operator!=(const EventType& b) const { return !(*this == b);}
    bool operator<(const EventType& b) const;
    bool operator<=(const EventType& b) const;
    bool operator>(const EventType& b) const { return b < *this; }
    bool operator>=(const EventType& b) const { return b <= *this; }

    uint8_t length() const;

    std::size_t size() const throw();

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, EventType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const EventType& t){
  s << t.length();
  for(const auto& a : t)
    s << a;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, EventType& t){
  uint8_t len;
  d >> len;
  while(len--){
    AttributeType aT;
    d >> aT;
    t.add(aT);
  }
  return d;
}
