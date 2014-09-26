#pragma once

#include <AttributeType.h>
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

    class iterator{
      private:
        StorageType::iterator i;
      public:
        iterator(StorageType::iterator i);
        iterator&   operator++();
        ValueType& operator*();
        bool operator==(const iterator& b);
        bool operator!=(const iterator& b);
    };

    iterator begin();
    iterator end();

    class Parser{
      private:
        StorageType& mStorage;
      public:
        Parser(StorageType& storage);
        template<typename Attr>
        void operator()(Attr attr){
          AttributeType aT(attr);
          mStorage.insert(PairType(aT.attributeId(), aT));
        }
    };

  public:
    class const_iterator{
      protected:
        StorageType::const_iterator i;
      public:
        const_iterator(StorageType::const_iterator i);
        const_iterator&   operator++();
        const ValueType& operator*() const;
        bool operator==(const const_iterator& b);
        bool operator!=(const const_iterator& b);
    };

    const_iterator begin() const;
    const_iterator end() const;

    EventType() = default;

    template<typename Event>
    EventType(const Event& e){
      Parser p(mStorage);
      foreach<typename Event::AttributeList>(p);
    }

    const AttributeType* attribute(KeyType key) const;
    
    bool operator==(const EventType& b) const;

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
    t.mStorage.insert(EventType::PairType(aT.attributeId(), aT));
  }
  return d;
}
