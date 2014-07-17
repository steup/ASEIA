#pragma once

#include <ID.h>

#include <Serializer.h>
#include <DeSerializer.h>

class MetaValue{
  private:

    id::type::ID mTypeID = id::type::Base::value();
    bool mHasUncertainty = true;
    std::vector<std::pair<Data, Data>> mData;

    currentDataPair

  public:
    id::type::ID typeID() const {return mTypeID; }
    bool hasUncertainty() const {return mHasUncertainty; }

    template<typename T, std::size_t m, bool u>
    Value<T,n,u> operator Value<T,n,u>(){
      Value<T,n,u> value;
      for(uint32_t i=0;i<n;)
        std::pair<T,T> p=
        if(u)
          value[i]={{mData[i].}}
    }

  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaValue& mv){
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, const MetaValue& mv){
  return d;
}
