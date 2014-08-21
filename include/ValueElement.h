#pragma once

#include <ID.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <initializer_list>

namespace {
  template<typename T>
  inline T satAdd(T& a, T b) {
    T diff = std::numeric_limits<T>::max() - a;
    if( diff < b ) {
      a = std::numeric_limits<T>::max();
      return b-diff;
    } else {
      a += b;
      return 0;
    }
  }

  template<typename T>
  inline T satSub(T& a, T b) {
    T diff = std::numeric_limits<T>::max() + b;
    if( diff > a ) {
      a = std::numeric_limits<T>::min();
      return diff-a;
    } else {
      a -= b;
      return 0;
    }
  }

  template<typename T1, typename T2>
  inline T2 checkBounds(T1& v, T1 u, T2 dummy) {
    if( v > std::numeric_limits<T2>::max() ) {
      T1 temp = v - std::numeric_limits<T2>::max();
      v = std::numeric_limits<T2>::max();
      satAdd( u, temp );
    }
    if( v < std::numeric_limits<T2>::min() ) {
      T1 temp = std::numeric_limits<T2>::min() - v;
      v = std::numeric_limits<T2>::min();
      satAdd( u, temp );
    }
    if(u<0 || u>std::numeric_limits<T2>::max())
      return std::numeric_limits<T2>::max();
    return u;
  }
}

template<typename T, bool U =true>
class ValueElement;

template<typename T>
class ValueElement<T, false> {
  public:
    using TypeID   = typename id::type::t2Type<T>::type;
    using BaseType = T;
    using InitType = std::initializer_list<T>;
  protected:
    T mValue;
  public:
    constexpr ValueElement() : mValue(0){}
    ValueElement(const T& v) : mValue(v){}
    ValueElement(InitType i) : mValue(*i.begin()){}

    template<typename T2>
    ValueElement(const ValueElement<T2, false>& data){
      T2 v = data.value();
      checkBounds(v, 0, T());
      mValue=(BaseType)(v);
    }
    
    ValueElement& operator=(const ValueElement& data) = default;
    ~ValueElement() = default;

    const T value() const { return mValue; }
    void value(const T& v) { mValue=v; }

    T uncertainty() const { return 0; }
    void uncertainty ( const T& u ) {
      static_assert(hasUncertainty(), "Uncertainty disabled at compile time");
    }

    operator BaseType() { return mValue; }

    constexpr static std::size_t size() noexcept {return sizeof(BaseType);}
    constexpr bool hasUncertainty()     noexcept {return false;}
};

template<typename T>
class ValueElement<T, true>{
  public:
    using TypeID   = typename id::type::t2Type<T>::type;
    using BaseType = T;
    using InitType = std::initializer_list<T>;
  protected:
    T mValue;
    T mUncertainty;
  public:
    constexpr ValueElement() : mValue(0), mUncertainty(std::numeric_limits<T>::max()){}
    ValueElement(T v, T u=0) : mValue(v), mUncertainty(u) {
      if (mUncertainty < 0)
        mUncertainty = std::numeric_limits<T>::max();
    }
    ValueElement(const ValueElement& data) : mValue(data.mValue), mUncertainty(data.mUncertainty){}
    ValueElement(InitType i) : ValueElement() {
      auto iter = i.begin();
      mValue = *iter;
      mUncertainty = *std::next(iter);
      if (mUncertainty < 0)
        mUncertainty = std::numeric_limits<T>::max();
    }

    template<typename T2>
    ValueElement(const ValueElement<T2, true>& data){
      T2 v = data.value();
      mUncertainty=checkBounds(v, data.uncertainty(), T());
      mValue=(BaseType)(v);
    }

    ValueElement& operator=(const ValueElement& data) = default;
    ~ValueElement() = default;

    const T value() const { return mValue; }
    void value(const T& v) { mValue=v; }

    T uncertainty() const{ return mUncertainty; }
    void uncertainty(const T& u){ mUncertainty = u; }

    ValueElement operator+=(const ValueElement& a){
      BaseType temp = satAdd(mValue, a.mValue);
      if(temp)
        satAdd(mUncertainty, temp);
      satAdd(mUncertainty, a.mUncertainty);
      return *this;
    }

    ValueElement operator-=(const ValueElement& a){
      BaseType temp = satSub(mValue, a.mValue);
      if(temp)
        satAdd(mUncertainty, temp);
      satAdd(mUncertainty, a.mUncertainty);
      return *this;
    }

    ValueElement operator+=(const BaseType& a){
      return *this+=ValueElement(a, 0);
    }

    ValueElement operator-=(const BaseType& a){
      return *this-=ValueElement(a, 0);
    }

    ValueElement operator*=(const BaseType& a){
      return *this*=ValueElement(a, 0);
    }

    ValueElement operator/=(const BaseType& a){
      return *this/=ValueElement(a, 0);
    }

    ValueElement operator+(const ValueElement& a) const{
      return ValueElement(*this)+=a;
    }

    ValueElement operator-(const ValueElement& a) const{
      return ValueElement(*this)-=a;
    }

    ValueElement operator+(const BaseType& a) const{
      return ValueElement(*this)+=a;
    }

    ValueElement operator-(const BaseType& a) const{
      return ValueElement(*this)-=a;
    }

    explicit operator BaseType() { return mValue; }
    explicit operator ValueElement<T, false>() { return ValueElement(mValue); }

    constexpr static std::size_t size() noexcept {return sizeof(mUncertainty)+sizeof(mValue);}
    constexpr bool hasUncertainty()     noexcept {return true;}
};

template<typename T, bool U>
ValueElement<T, U> operator+(const T& a, const ValueElement<T,U>& b) {
  return ValueElement<T,U>({a, 0})+=b;
}

template<typename T, bool U>
ValueElement<T, U> operator-(const T& a, const ValueElement<T,U>& b) {
  return ValueElement<T,U>({a, 0})-=b;
}

template<typename PB, typename T>
Serializer<PB>& operator<<(Serializer<PB>& s, const ValueElement<T, true>& value){
  return s << value.value() << value.uncertainty();
}

template<typename PB, typename T>
Serializer<PB>& operator<<(Serializer<PB>& s, const ValueElement<T, false>& value){
  return s << value.value();
}

template<typename PB, typename T>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, ValueElement<T, true>& value){
  T v,u;
  s >> v >> u;
  value.value(v);
  value.uncertainty(u);
  return s;
}

template<typename PB, typename T>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, ValueElement<T, false>& value){
  T v;
  s >> v;
  value.value(v);
  return s;
}
