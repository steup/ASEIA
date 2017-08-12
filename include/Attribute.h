#pragma once

#include <Unit.h>
#include <Scale.h>
#include <Value.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <AttributeType.h>

#include <ratio>

template<typename AttributeID, typename Value, typename Unit=Dimensionless, typename S=Scale<std::ratio<1>, 0>>
class Attribute
{
  public:
    using ValueType = Value;
    using IDType    = AttributeID;
    using ScaleType = S;
    using UnitType  = Unit;
    using InitType  = typename Value::InitType;

  private:
    ValueType v;

    template<typename ratio>
    struct mult {
      using newRatio = typename std::ratio_multiply<typename ScaleType::Ratio, ratio>::type;
      using newScale = Scale<newRatio, ScaleType().reference()>;
      using type = Attribute<AttributeID, Value, Unit, newScale>;
    };

  public:
    Attribute(){}
    Attribute(InitType l) : v(l){}

    const ValueType& value() const {return v;}
    ValueType& value(){return v;}
    void value(const ValueType& v){this->v=v;}

    template<typename ratio>
    auto operator*(Scale<ratio, ScaleType::Reference::value> dummy) const
      -> typename mult<ratio>::type {
      typename mult<ratio>::type temp;
      temp.value() = this->value() / ratio();
      return temp;
    }

    Attribute& operator+=(const Attribute& b) {
      value()+=b.value();
      return *this;
    }

    Attribute& operator-=(const Attribute& b) {
      value()-=b.value();
      return *this;
    }

    bool operator>(const Attribute& b) const {
      return (value() > b.value()).prod();
    }

    bool operator<(const Attribute& b) const {
      return (value() < b.value()).prod();
    }

    bool operator==(const Attribute& b) const {
      return (value() == b.value()).prod();
    }

    bool operator!=(const Attribute& b) const {
      return (value() != b.value()).prod();
    }

    bool operator>=(const Attribute& b) const {
      return (value() >= b.value()).prod();
    }

    bool operator<=(const Attribute& b) const {
      return (value() <= b.value()).prod();
    }

    constexpr const IDType id() const noexcept {return IDType();}
    constexpr const ScaleType scale() const noexcept {return ScaleType();}
    constexpr const UnitType unit() const noexcept {return UnitType();}
		explicit operator AttributeType() const {
			return AttributeType(id().value(), (::ValueType)value(), scale(), unit());
		}

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
