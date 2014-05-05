#pragma once

#include <array>
#include <ValueElement.h>
#include <initializer_list>

template<typename T, std::size_t n, bool useUncertainty=true>
struct Value : public std::array<ValueElement<T, useUncertainty>,n>{
  public:
    using BaseType = typename ValueElement<T, useUncertainty>::BaseType;
    using DataType = typename ValueElement<T, useUncertainty>::DataType;
    using TypeID   = typename ValueElement<T, useUncertainty>::TypeID;

    Value(std::initializer_list<std::initializer_list<T>> l){
      std::size_t i=0;
      for(const auto& value : l){
        uint8_t j=0;
        T v=0, u=0;
        for(const T& val : value){
          if(j++==0)
            v=val;
          else
            u=val;
        }
        (*this)[i++]=ValueElement<T, useUncertainty>(v, u);
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
};

template<typename T, std::size_t n, bool useUncertainty>
std::ostream& operator<<(std::ostream& o, const Value<T,n,useUncertainty>& v)
{
  o << "(";
  bool isFirst=true;
  for(auto e : v)
    if(isFirst){
      isFirst=false;
      o << e;
    }
    else
      o << ", " << e;
  return o << ")";
}
