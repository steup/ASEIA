#pragma once

#include <ID.h>
#include <ValueElement.h>
#include <Serializer.h>
#include <DeSerializer.h>

class MetaValueElementType{
  public:
    using IDType = id::type::Base::IDType;

  private:
    IDType mTypeId = id::type::Base::value();
    std::uint8_t mHasUncertainty = false;

  public:
    MetaValueElementType() = default;

    template<typename T, bool u>
    MetaValueElementType(ValueElement<T, u> value) : mHasUncertainty(u){
      using DataType = typename id::type::getTypeID<T>::type;
      mTypeId = DataType::value();
    }

    IDType typeId() const {return mTypeId;}
    bool hasUncertainty() const {return mHasUncertainty;}

    bool operator==(const MetaValueElementType& b){
      return mTypeId == b.mTypeId && mHasUncertainty == b.mHasUncertainty;
    }

    static constexpr unsigned int size(){return sizeof(mTypeId);}

    template<typename I> friend DeSerializer<I>& operator>>(DeSerializer<I>&, MetaValueElementType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaValueElementType& value){
  union{
    uint8_t data;
    struct{
      bool uncertainty : 1;
      MetaValueElementType::IDType id : sizeof(MetaValueElementType::IDType)*8-1;
    };
  } u;
  u.id=value.typeId();
  u.uncertainty=value.hasUncertainty();
  return s << u.data;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaValueElementType& value){
  union{
    uint8_t data;
    struct{
      bool uncertainty : 1;
      MetaValueElementType::IDType id : sizeof(MetaValueElementType::IDType)*8-1;
    };
  } u;
  d >> u.data;
  value.mHasUncertainty = u.uncertainty;
  value.mTypeId = u.id;
  return d;
}
