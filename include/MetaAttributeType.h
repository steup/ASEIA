#pragma once

#include <MetaValueType.h>
#include <MetaScaleType.h>
#include <Attribute.h>

#include <Serializer.h>
#include <DeSerializer.h>

class MetaAttributeType : public MetaValueType{
  private:
    id::type::Base::IDType mAttributeId = id::attribute::Base::value();
    MetaScaleType mScale;
  public:
    MetaAttributeType() = default;
    
    template<typename I, typename V, typename U, typename S>
    MetaAttributeType(const Attribute<I,V,U,S> a) 
    : MetaValueType(a.value()),
      mAttributeId(I::value()),
      mScale(S())
  {}

    const MetaScaleType& scale() const { return mScale; }
    id::attribute::Base::IDType attributeId() const {return mAttributeId;}

//    constexpr UnitType unit() noexcept {return UnitType();}

    constexpr static std::size_t size() noexcept {return MetaValueType::size() + MetaScaleType::size();}

    bool operator==(const MetaAttributeType& b){
      return MetaValueType::operator==(b) && mScale == b.mScale && mAttributeId == b.mAttributeId;
    }

    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, MetaAttributeType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaAttributeType& attr){
  return s << attr.attributeId() << static_cast<const MetaValueType&>(attr) << attr.scale();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaAttributeType& attr){
  return d >> attr.mAttributeId >> static_cast<MetaValueType&>(attr) >> attr.mScale;
}
