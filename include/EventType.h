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

    class iterator{
      private:
        StorageType::iterator i;
      public:
        iterator(StorageType::iterator i) : i(i){}
        iterator&   operator++() { i++; return *this; }
        ValueType& operator*()  { return i->second; }
        bool operator==(const iterator& b) { return i == b.i; }
        bool operator!=(const iterator& b) { return !(*this== b); }
    };

    iterator begin(){ return iterator(mStorage.begin()); }
    iterator end(){ return iterator(mStorage.end()); }

    class Parser{
      private:
        StorageType& mStorage;
      public:
        Parser(StorageType& storage) : mStorage(storage){}
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
        const_iterator(StorageType::const_iterator i) : i(i){}
        const_iterator&   operator++() { i++; return *this; }
        const ValueType& operator*() const { return i->second; }
        bool operator==(const const_iterator& b) { return i == b.i; }
        bool operator!=(const const_iterator& b) { return !(*this== b); }
    };

    const_iterator begin() const{ return const_iterator(mStorage.cbegin()); }
    const_iterator end() const { return const_iterator(mStorage.cend()); }

    EventType() = default;

    template<typename Event>
    EventType(const Event& e){
      Parser p(mStorage);
      boost::mpl::for_each<typename Event::AttributeList>(p);
    }

    const AttributeType* attribute(KeyType key) const{
      auto i = mStorage.find(key);
      if(i == mStorage.end())
        return NULL;
      else
        return &i->second;
    }
    
    bool operator==(const EventType& b) const{
      if(mStorage.size() != b.mStorage.size())
        return false;

      for( const auto& p : mStorage ){
        auto i = b.mStorage.find(p.first);
        if( i == b.mStorage.end())
          return false;
        if( !(p.second == i->second) )
          return false;
      }
      return true;
    }

    uint8_t length() const throw() { return mStorage.size(); }

    std::size_t size() const throw() {
      return AttributeType::size()*length()+1;
    }

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
