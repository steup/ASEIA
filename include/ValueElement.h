#pragma once

#include <boost/numeric/interval.hpp>
#include <ID.h>

template<typename T, bool useUncertainty=true>
class ValueElement{
  private:
  public:
    using DataType = typename boost::numeric::interval<T>;
    using TypeID   = typename id::type::getTypeID<T>::type;
    using BaseType = T;
  private:
    DataType data;
  public:
    ValueElement() : data(0,0){}
    ValueElement(T v, T u=0) : data(v-u,v+u){}
    ValueElement(const DataType& data) : data(data){}

    T value() const{return (data.lower()+data.upper())/2;}
    void value(const T& v){
      T u = uncertainty();
      data.lower(v-u);
      data.upper(v+u);
    }
    T uncertainty() const{return (data.upper()-data.lower())/2;}
    void uncertainty(const T& u){
      T v = value();
      data.lower(v-u);
      data.upper(v+u);
    }

    ValueElement operator+=(const ValueElement& a){
      data+=a.data;
      return *this;
    }

    ValueElement operator-=(const ValueElement& a){
      data-=a.data;
      return *this;
    }

    ValueElement operator*=(const ValueElement& a){
      data*=a.data;
      return *this;
    }

    ValueElement operator/=(const ValueElement& a){
      data/=a.data;
      return *this;
    }

    ValueElement operator+(const ValueElement& a) const{
      return ValueElement(data)+=a.data;
    }

    ValueElement operator-(const ValueElement& a) const{
      return ValueElement(data)-=a.data;
    }

    ValueElement operator*(const ValueElement& a) const{
      return ValueElement(data)*=a.data;
    }

    ValueElement operator/(const ValueElement& a) const{
      return ValueElement(data)/=a.data;
    }

    constexpr static std::size_t size() noexcept {return sizeof(ValueElement);}
    constexpr bool hasUncertainty()     noexcept {return true;}
};

template<typename T>
class ValueElement<T, false>
{
  private:
    T data;
  public:
    using TypeID   = typename id::type::getTypeID<T>::type;
    using DataType = T;
    using BaseType = T;
    ValueElement() : data(0){}
    ValueElement(const T& v) : data(v){}

    const T value() const{return data;}
    void value(const T& v){data=v;}
    T uncertainty() const{return 0;}
    void uncertainty(const T& u){
      static_assert(hasUncertainty(), "Uncertainty disabled at compile time");
    }
    
    ValueElement operator+=(const ValueElement& a){
      data+=a.data;
      return *this;
    }

    ValueElement operator-=(const ValueElement& a){
      data-=a.data;
      return *this;
    }

    ValueElement operator*=(const ValueElement& a){
      data*=a.data;
      return *this;
    }

    ValueElement operator/=(const ValueElement& a){
      data/=a.data;
      return *this;
    }

    ValueElement operator+(const ValueElement& a) const{
      return ValueElement(data+a.data);
    }

    ValueElement operator-(const ValueElement& a)const{
      return ValueElement(data-a.data);
    }

    ValueElement operator*(const ValueElement& a)const{
      return ValueElement(data*a.data);

    }

    ValueElement operator/(const ValueElement& a)const{
      return ValueElement(data/a.data);
    }

    constexpr static std::size_t size() noexcept {return sizeof(BaseType);}
    constexpr bool hasUncertainty()     noexcept {return false;}
};
