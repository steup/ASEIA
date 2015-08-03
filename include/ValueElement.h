#pragma once

#include <ID.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <type_traits>
#include <initializer_list>
#include <limits>
#include <iterator>

namespace {
  template<typename T>
  inline T abs(T a){
    if(a<0)
      return -a;
    else
      return a;
  }
  
	inline float satAdd(float& a, float b){
		a+=b;
		//TODO: add epsilon estimation
		return 0;
	}

  template<typename T>
  inline T satAdd(T& a, T b) {
		if( !a  || !b || (a<0 && b>0) || (a>0 && b<0))
			return a+=b;

		if( a<0) {
			T diff = std::numeric_limits<T>::min() - a;
				if( diff >  b){
					a = std::numeric_limits<T>::min();
					return diff-b;
				}
		}else{
    	T diff = std::numeric_limits<T>::max() - a;
    	if( diff < b ) {
      	a = std::numeric_limits<T>::max();
      	return b-diff;
			}
		}
    
		a += b;
    return 0;
  }

  template<typename T>
  inline T satSub(T& a, T b) {
    T diff = std::numeric_limits<T>::max() + b;
    if( diff > a ) {
      a = std::numeric_limits<T>::lowest();
      return diff-a;
    } else {
      a -= b;
      return 0;
    }
  }

  template<typename T1, typename T2>
  inline T1 modifyU(T1 u, T2 dummy){
    return u;
  }
  

  template<typename T>
  inline float modifyU(float u, T dummy){
    satAdd(u, 1.0f);
    return u;
  }

  template<typename T>
  inline double modifyU(double u, T dummy){
    satAdd(u, 1.0);
    return u;
  }

  template<>
  inline float modifyU(float u, double dummy){
    return u;
  }

  template<>
  inline double modifyU(double u, float dummy){
    satAdd(u, (double)std::numeric_limits<float>::epsilon());
    return u;
  }


  template<typename T1, typename T2>
  inline T2 checkBounds(T1& v, T1 u, T2 dummy) {
    T1 T1min = std::numeric_limits<T1>::lowest();
    T2 T2min = std::numeric_limits<T2>::lowest();
    T2 T2max = std::numeric_limits<T2>::max();
    T1 temp=0;
    if(T1min < (T1)0 && T2min == (T2)0 && v<(T1)0){
        temp = -v;
        v = (T1)0;
    }
    if(T1min < (T1)0 && T2min < (T2)0 && v<(T1)T2min) {
      v = T2min;
      temp = T2min - v;
    }
    if(v>(T1)T2max){
      v = T2max;
      temp = T2max - v;
    }
    satAdd(u, temp);

    u=modifyU(u, dummy);
    if(u<(T1)0 || u>(T1)std::numeric_limits<T2>::max())
      return std::numeric_limits<T2>::max();
    return u;
  }

  template<typename T>
  T opError(T result){
    return 0;
  }

  template<>
  float opError(float result){
    return std::numeric_limits<float>::epsilon()*result;
  }

  template<>
  double opError(double result){
    return std::numeric_limits<double>::epsilon()*result;
  }

  template<typename T>
  struct multType{
    using type  = T;
  };

  template<>
  struct multType<uint8_t>{
    using type  = int16_t;
  };

  template<>
  struct multType<uint16_t>{
    using type  = int32_t;
  };

  template<>
  struct multType<uint32_t>{
    using type  = int64_t;
  };

  template<>
  struct multType<uint64_t>{
    using type  = int64_t;
  };
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

    ValueElement operator+=(const ValueElement& a){
      mValue+=a.mValue;
      return *this;
    }

    ValueElement operator*=(const ValueElement& a){
      mValue*=a.mValue;
      return *this;
    }

    ValueElement operator-=(const ValueElement& a){
      mValue-=a.mValue;
      return *this;
    }

    ValueElement operator/=(const ValueElement& a){
      mValue/=a.mValue;
      return *this;
    }


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
      mUncertainty=checkBounds(mValue, mUncertainty, T());
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
      T e = opError(mValue);
      if(e)
        satAdd(mUncertainty, e);
      return *this;
    }

    ValueElement operator-=(const ValueElement& a){
      BaseType temp = satSub(mValue, a.mValue);
      if(temp)
        satAdd(mUncertainty, temp);
      satAdd(mUncertainty, a.mUncertainty);
      T e = opError(mValue);
      if(e)
        satAdd(mUncertainty, e);
      return *this;
    }

    ValueElement operator*=(const ValueElement& a){
      typename multType<BaseType>::type p1, m1, p2, m2, temp[4], min, max;
      min = std::numeric_limits<T>::max();
      max = std::numeric_limits<T>::min();
      p1 =   mValue +   mUncertainty;
      p2 = a.mValue + a.mUncertainty;
      m1 =   mValue -   mUncertainty;
      m2 = a.mValue - a.mUncertainty;
      temp[0] = p1 * p2;
      temp[1] = p1 * m2;
      temp[2] = m1 * p2;
      temp[3] = m1 * m2;
      for(unsigned int i = 0; i < 4; i++) {
        if(temp[i] < min)
          min = temp[i];
        if(temp[i] > max)
          max = temp[i];
      }
      mUncertainty = (max - min) / 2;
      mValue       = min + mUncertainty;
      T e = opError(mValue);
      if(e)
        satAdd(mUncertainty, e);
      return *this;
    }

    ValueElement operator/=(const ValueElement& a){
      if(a.mUncertainty >= abs(a.mValue)){
        mValue = 0;
        mUncertainty = std::numeric_limits<T>::max();
        return *this;
      }

      typename multType<BaseType>::type p1, m1, p2, m2, temp[4], min, max;
      min = std::numeric_limits<T>::max();
      max = std::numeric_limits<T>::min();
      p1 =   mValue +   mUncertainty;
      p2 = a.mValue + a.mUncertainty;
      m1 =   mValue -   mUncertainty;
      m2 = a.mValue - a.mUncertainty;
      temp[0] = p1 / p2;
      temp[1] = p1 / m2;
      temp[2] = m1 / p2;
      temp[3] = m1 / m2;
      for(unsigned int i = 0; i < 4; i++) {
        if(temp[i] < min)
          min = temp[i];
        if(temp[i] > max)
          max = temp[i];
      }
      mUncertainty = (max - min) / 2;
      mValue       = min + mUncertainty;
      T e = opError(mValue);
      if(e)
        satAdd(mUncertainty, e);
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

    ValueElement operator*(const ValueElement& a) const{
      return ValueElement(*this)*=a;    }

    ValueElement operator/(const ValueElement& a) const{
      return ValueElement(*this)/=a;
    }

    ValueElement operator+(const BaseType& a) const{
      return ValueElement(*this)+=a;
    }

    ValueElement operator-(const BaseType& a) const{
      return ValueElement(*this)-=a;
    }

    ValueElement operator*(const BaseType& a) const{
      return ValueElement(*this)*=a;    }

    ValueElement operator/(const BaseType& a) const{
      return ValueElement(*this)/=a;
    }

    explicit operator BaseType() { return mValue; }
    explicit operator ValueElement<T, false>() { return ValueElement(mValue); }

    constexpr static std::size_t size() noexcept {return sizeof(mUncertainty)+sizeof(mValue);}
    constexpr bool hasUncertainty()     noexcept {return true;}
};

template<typename T1, typename T2, bool U>
ValueElement<T1, U> operator+(const T2& a, const ValueElement<T1,U>& b) {
  T1 v,u;
  v = a+b.value();
  u = b.uncertainty();
  return ValueElement<T1, U>({v, u});
}

template<typename T1, typename T2, bool U>
ValueElement<T1, U> operator-(const T2& a, const ValueElement<T1,U>& b) {
  T1 v,u;
  v = a-b.value();
  u = b.uncertainty();
  return ValueElement<T1, U>({v, u});
}

template<typename T1, typename T2, bool U>
ValueElement<T1, U> operator*(const T2& a, const ValueElement<T1,U>& b) {
  T1 v,u;
  v = a*b.value();
  u = a*b.uncertainty();
  return ValueElement<T1, U>({v, u});
}

template<typename T1, typename T2, bool U>
ValueElement<T1, U> operator/(const T2& a, const ValueElement<T1,U>& b) {
  T1 v,u;
  v = a/b.value();
  if(!b.uncertainty())
    u = 0;
  if(b.uncertainty()>abs(b.value())) {
    v = 0;
    u = std::numeric_limits<T1>::max();
  }
  return ValueElement<T1, U>({v, u});
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
