#pragma once

#include <Unit.h>
#include <Value.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <AttributeType.h>

#include <ratio>

template<typename AttributeID, typename Value, typename Unit=Dimensionless, typename Scale=std::ratio<1>>
class Attribute
{
  public:
    using ValueType = Value;
    using IDType    = AttributeID;
    using ScaleType = Scale;
    using UnitType  = Unit;
    using InitType  = typename Value::InitType;

  private:
    ValueType v;

    template<typename NewScale>
    using mult = Attribute<AttributeID, Value, Unit, std::ratio_multiply<Scale, NewScale>>;

  public:
    Attribute(){}
    Attribute(InitType l) : v(l){}

    const ValueType& value() const {return v;}
    ValueType& value(){return v;}
    void value(const ValueType& v){this->v=v;}
    
    template<typename ScaleArg>
    auto operator*(ScaleArg dummy)
      -> mult<ScaleArg> {
      mult<ScaleArg> temp;
      temp.value() = value() / ScaleArg();
      return temp;
    }
    
    constexpr const IDType id() const noexcept {return IDType();}
    constexpr const ScaleType scale() const noexcept {return ScaleType();}
    constexpr const UnitType unit() const noexcept {return UnitType();}
		explicit operator AttributeType() const {
			return AttributeType(id().value(), (::ValueType)value(), scale(), unit());
		}

    bool operator==(const Attribute& b) const { return (value()==b.value()).prod(); }

    constexpr const static std::size_t size() noexcept {return Value::staticSize();}
};

template<typename PB, typename ID, typename V, typename U, typename S>
Serializer<PB>& operator<<(Serializer<PB>& s, const Attribute<ID,V,U,S>& attr){
  return s << attr.value();
}

template<typename PB, typename ID, typename V, typename U, typename S>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, Attribute<ID,V,U,S>& attr){
  return s >> attr.value();
}
