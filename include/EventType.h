#pragma once

#include <AttributeType.h>
#include <Event.h>
#include <Foreach.h>

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

    class Parser{
      private:
        StorageType& mStorage;
      public:
        Parser(StorageType& storage);
        template<typename Attr>
        void operator()(Attr attr){
          AttributeType aT(attr);
          mStorage.insert(PairType(aT.id(), aT));
        }
    };

  public:
    class const_iterator : public StorageType::const_iterator {
      public:
        const_iterator(StorageType::const_iterator i);
        const ValueType& operator*() const;
    };

    const_iterator begin() const;
    const_iterator end() const;

    EventType() = default;

    template<Endianess end=hostEndianess, typename... Attributes>
    EventType(const Event<end, Attributes...>& e){
      Parser p(mStorage);
      foreach<typename Event<end, Attributes...>::AttributeList>(p);
    }

    bool add(const AttributeType& aT) {
      if(mStorage.count(aT.id()))
        return false;
      mStorage.insert(std::make_pair(aT.id(), aT));
      return true;
    }

    const AttributeType* attribute(KeyType key) const;
    
    bool operator==(const EventType& b) const;
    bool operator!=(const EventType& b) const { return !(*this==b); }

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
