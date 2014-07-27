#pragma once

#include <ID.h>
#include <ScaleType.h>
#include <UnitType.h>
#include <ValueType.h>
#include <Attribute.h>

#include <Serializer.h>
#include <DeSerializer.h>

class AttributeType{
  private:
    id::attribute::ID mAttributeId = id::attribute::Base::value();
    ValueType         mValue;
    ScaleType         mScale;
    UnitType          mUnit;
  public:
    AttributeType() = default;
    
    template<typename I, typename V, typename U, typename S>
    AttributeType(const Attribute<I,V,U,S> a)
      : mAttributeId(I::value()),
        mValue(a.value()),
        mScale(S()),
        mUnit(U())
    {}
    
    id::attribute::ID attributeId() const;
    const UnitType&   unit()        const;
    const ScaleType&  scale()       const;
    const ValueType&  value()       const;

    constexpr static std::size_t size() noexcept {
      return sizeof(id::attribute::ID) +
             ValueType::size()         +
             ScaleType::size()         +
             UnitType::size()          ;
    }

    bool operator==(const AttributeType& b) const;

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, AttributeType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const AttributeType& attr){
  return s << attr.attributeId() << attr.value() << attr.scale() << attr.unit();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, AttributeType& attr){
  return d >> attr.mAttributeId >> attr.mValue >> attr.mScale >> attr.mUnit;
}
