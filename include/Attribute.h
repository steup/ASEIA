#pragma once

#include <boost/units/systems/si/dimensionless.hpp>
#include <ratio>

template<typename AttributeID, typename Value, typename Unit=boost::units::si::dimensionless, typename Scale=std::ratio<1>>
class Attribute
{
  public:
    using ValueType = Value;
    using IDType    = AttributeID;
    using ScaleType = Scale;
    using UnitType  = Unit;
    using InitType  = typename ValueType::InitType;
  private:
    ValueType v;
  public:
    Attribute(){}
    Attribute(InitType l) : v(l){}

    const ValueType& value() const {return v;}
    ValueType& value(){return v;}
    void value(const ValueType& v){this->v=v;}
    constexpr IDType id() noexcept {return IDType();}
    constexpr ScaleType scale() noexcept {return ScaleType();}
    constexpr UnitType unit() noexcept {return UnitType();}
    constexpr bool hasUncertainty() noexcept {return v.hasUncertainty();};
    constexpr static std::size_t size() noexcept {return ValueType::size();}
};
