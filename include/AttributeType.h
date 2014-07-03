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
    id::type::Base::IDType mAttributeId = id::attribute::Base::value();
    ValueType mValue;
    ScaleType mScale;
    UnitType mUnit;
  public:
    AttributeType() = default;
    
    template<typename I, typename V, typename U, typename S>
    AttributeType(const Attribute<I,V,U,S> a) 
    : mAttributeId(I::value()),
      mValue(a.value()),
      mScale(S()),
      mUnit(U())
  {}

    
    id::attribute::Base::IDType attributeId() const { return mAttributeId; }
    const UnitType&             unit()        const { return mUnit;        }
    const ScaleType&            scale()       const { return mScale;       }
    const ValueType&            value()       const { return mValue;       }

    constexpr static std::size_t size() noexcept {return sizeof(mAttributeId) +
                                                         ValueType::size() +
                                                         ScaleType::size() +
                                                         UnitType::size();}

    bool operator==(const AttributeType& b){
      return mAttributeId == b.mAttributeId &&
             mValue       == b.mValue       &&
             mScale       == b.mScale       &&
             mUnit        == b.mUnit;
    }

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
