#pragma once

#include <ID.h>
#include <ScaleType.h>
#include <UnitType.h>
#include <ValueType.h>
#include <Attribute.h>

#include <Serializer.h>
#include <DeSerializer.h>

class AttributeType{
  public:
    using ID = id::attribute::ID;
  private:
    ID        mId    = id::attribute::Base::value();
    ValueType mValue ;
    ScaleType mScale ;
    UnitType  mUnit  ;
  public:
    AttributeType() = default;
    AttributeType(ID id, const ValueType& v, const ScaleType& s, const UnitType& u)
      : mId(id), mValue(v), mScale(s), mUnit(u) {}
    
    template<typename I, typename V, typename U, typename S>
    AttributeType(const Attribute<I,V,U,S> a)
      : AttributeType(I::value(), (ValueType)a.value(), S(), U()) {}
    
          ID            id() const;
    const UnitType&   unit() const;
    const ScaleType& scale() const;
    const ValueType& value() const;

    constexpr static std::size_t size() noexcept {
      return sizeof(id::attribute::ID) +
             ValueType::size()         +
             ScaleType::size()         +
             UnitType::size()          ;
    }

    bool operator==(const AttributeType& b) const;
    bool operator!=(const AttributeType& b) const { return !(*this==b); }

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, AttributeType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const AttributeType& attr){
  return s << attr.id() << attr.value() << attr.scale() << attr.unit();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, AttributeType& attr){
  return d >> attr.mId >> attr.mValue >> attr.mScale >> attr.mUnit;
}
