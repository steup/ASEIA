#pragma once

#include <ID.h>
#include <ValueElement.h>
#include <Serializer.h>
#include <DeSerializer.h>

class ValueElementType{
  public:
    using IDType = id::type::Base::IDType;

  private:
    IDType mId;
    std::uint8_t mHasUncertainty;

  public:
    ValueElementType() : mId(0), mHasUncertainty(false){}

    template<typename T, bool u>
    ValueElementType(ValueElement<T, u> value) : mHasUncertainty(u){
      using DataType = typename id::type::getTypeID<T>::type;
      mId = DataType().value();
    }

    IDType id() const {return mId;}
    bool hasUncertainty() const {return mHasUncertainty;}

    bool operator==(const ValueElementType& b){
      return mId == b.mId && mHasUncertainty == b.mHasUncertainty;
    }

    static constexpr unsigned int size(){return sizeof(IDType)+sizeof(std::uint8_t);}

    template<typename I> friend DeSerializer<I>& operator>>(DeSerializer<I>&, ValueElementType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const ValueElementType& value){
  return s << value.id() << value.hasUncertainty();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, ValueElementType& value){
  return d >> value.mId >> value.mHasUncertainty;
}
