#pragma once

#include <ValueElement.h>

#include <initializer_list>
#include <array>

template<typename T, std::size_t n, bool useUncertainty=true>
struct Value : public std::array<ValueElement<T, useUncertainty>,n>{
  public:
    using BaseType    = ValueElement<T, useUncertainty>;
    using DataType    = std::array<BaseType, n>;
    using InitType    = std::initializer_list<typename BaseType::InitType>;

    Value(){}

    Value(InitType l){
      std::size_t i=0;
      for(const auto& value : l){
        (*this)[i++]=ValueElement<T, useUncertainty>(value);
      }
    }

    Value operator+(const Value& a) const{
      return Value(*this)+=a;
    }
    Value operator-(const Value& a) const{
      return Value(*this)-=a;
    }
    Value operator*(const Value& a) const{
      return Value(*this)*=a;
    }
    Value operator/(const Value& a) const{
      return Value(*this)/=a;
    }

    Value& operator+=(const Value& a){
      std::size_t i=0;
      for(auto& value : *this)
        value+=a[i++];
      return *this;
    }
    Value operator-=(const Value& a){
      std::size_t i=0;
      for(auto& value : *this)
        value-=a[i++];
      return *this;
    }
    Value operator*=(const Value& a){
      Value temp(*this);
      std::size_t i=0;
      for(auto& value : *this)
        value*=a[i++];
      return *this;
    }
    Value operator/=(const Value& a){
      Value temp(*this);
      std::size_t i=0;
      for(auto& value : *this)
        value/=a[i++];
      return *this;
    }

    Value operator+(T a) const{
      return Value(*this)+=a;
    }
    Value operator-(T a) const{
      return Value(*this)-=a;
    }
    Value operator*(T a) const{
      return Value(*this)*=a;
    }
    Value operator/(T a) const{
      return Value(*this)/=a;
    }

    Value& operator+=(T a){
      for(auto& value : *this)
        value+=a;
      return *this;
    }
    Value operator-=(T a){
      for(auto& value : *this)
        value-=a;
      return *this;
    }
    Value operator*=(T a){
      for(auto& value : *this)
        value*=a;
      return *this;
    }
    Value operator/=(T a){
      for(auto& value : *this)
        value/=a;
      return *this;
    }

    constexpr static std::size_t size() noexcept{return n*BaseType::size();}
    constexpr bool hasUncertainty() noexcept {return (*this)[0].hasUncertainty();}
};

template<typename PB, typename T, std::size_t n, bool u>
Serializer<PB>& operator<<(Serializer<PB>& s, const Value<T,n,u>& value){
  for(const auto& v : value)
    s << v;
  return s;
}

template<typename PB, typename T, std::size_t n, bool u>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, Value<T,n,u>& value){
  for(auto& v : value)
    s >> v;
  return s;
}
