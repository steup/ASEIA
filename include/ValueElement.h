#pragma once

#include <ID.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <type_traits>
#include <initializer_list>
#include <limits>
#include <iterator>
#include <cmath>

namespace {
  template<typename T>
  inline T abs(T a){
    return a<0?-a:a;
  }
  
  inline bool satAdd(double& a, double b){
    a+=b;
    return false;
  }

  inline bool satAdd(float& a, float b){
    a+=b;
    return false;
  }
  
  inline bool satSub(double& a, double b){
    a-=b;
    return false;
  }

  inline bool satSub(float& a, float b){
    a-=b;
    return false;
  }

  template<typename T>
  inline bool satAdd(T& a, T b) {
		if( a> 0 && b > 0 && std::numeric_limits<T>::max() - a < b ) {
      	a=std::numeric_limits<T>::max();
        return true;
    }
		if( a< 0 && b < 0 && std::numeric_limits<T>::min() - a > b ) {
      	a=std::numeric_limits<T>::min();
        return true;
    }
    a+=b;
    return false;
  }

  template<typename T>
  inline bool satSub(T& a, T b){
		if(!std::is_signed<T>::value && a < b) {
			a =std::numeric_limits<T>::min();
			return true;
		}
    if( a> 0 && b < 0 && std::numeric_limits<T>::max() - a < -b ) {
      	a=std::numeric_limits<T>::max();
        return true;
    }
		if( a< 0 && b > 0 && std::numeric_limits<T>::min() - a > -b ) {
      	a=std::numeric_limits<T>::min();
        return true;
    }
    a-=b;
    return false;
  }

  template<typename T1, typename T2>
  inline T1 modifyU(T1 u, T2 dummy){
    return 0;
  }
  

  template<typename T>
  inline float modifyU(float u, T dummy){
    return 1.0f;
  }

  template<typename T>
  inline double modifyU(double u, T dummy){
    return 1.0;
  }
  
  template<>
  inline float modifyU(float u, float dummy){
    return 0.0f;
  }
  
  template<>
  inline double modifyU(double u, double dummy){
    return 0.0;
  }

  template<>
  inline float modifyU(float u, double dummy){
    return 0.0f;
  }

  template<>
  inline double modifyU(double u, float dummy){
    return std::numeric_limits<float>::epsilon();
  }


  template<typename From, typename To>
  inline From checkBounds(From v, To& toV) {
    To   toMin       = std::numeric_limits<To>::lowest();
    To   toMax       = std::numeric_limits<To>::max();
    bool sameSize    = std::is_integral<From>::value && std::is_integral<To>::value && sizeof(From) == sizeof(To);
    bool toSign      = !std::is_signed<From>::value && std::is_signed<To>::value;
    bool fromSign    = std::is_signed<From>::value && !std::is_signed<To>::value;
    using common     = typename std::common_type<From, To>::type;
    bool toCommon    = std::is_same<common, To>::value;

    toV = v;
    if(fromSign && v<0) {
      toV = 0;
      From fromU = -v;
      satAdd(fromU, modifyU(From(), To()));
      return fromU;
    }

    if(sameSize && toSign && (From)v > (From)toMax) {
      toV = toMax;
      From fromU = v-(From)toMax;
      satAdd(fromU, modifyU(From(), To()));
      return fromU;
    }

    if(!toCommon && (common)v>(common)toMax){
      toV = toMax;
      From fromU = v-toMax;
      satAdd(fromU, modifyU(From(), To()));
      return fromU;
    }
    
    if(!toCommon && (common)v<(common)toMin){
      toV = toMin;
      From fromU = v-toMin;
      satAdd(fromU, modifyU(From(), To()));
      return fromU;
    }

    return modifyU(From(), To());
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
  struct multType<int8_t>{
    using type  = int16_t;
  };
  
	template<>
  struct multType<int16_t>{
    using type  = int32_t;
  };
  
	template<>
  struct multType<int32_t>{
    using type  = int64_t;
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

			if(i.size()<1)
				mValue = 0;
			else
      	mValue = *iter;

			if(i.size()<2)
				mUncertainty = 0;
			else
      	mUncertainty = *std::next(iter);

      if (mUncertainty < 0)
        mUncertainty = std::numeric_limits<T>::max();
    }

    template<typename T2>
    ValueElement(const ValueElement<T2, true>& data){
      T2 u = checkBounds(data.value(), this->mValue);
      satAdd(u, data.uncertainty());
      checkBounds(u, this->mUncertainty);
    }

    ValueElement& operator=(const ValueElement& data) = default;
    ~ValueElement() = default;

    const T value() const { return mValue; }
    void value(const T& v) { mValue=v; }

    T uncertainty() const{ return mUncertainty; }
    void uncertainty(const T& u){ mUncertainty = u; }

    ValueElement& operator+=(const ValueElement& a){
      if(satAdd(mValue, a.mValue))
        mUncertainty = std::numeric_limits<T>::max();
      else {
        satAdd(mUncertainty, a.mUncertainty);
        satAdd(mUncertainty, opError(mValue));
      }
      return *this;
    }

    ValueElement operator-() const{
      ValueElement temp(*this);
      if(std::is_signed<T>::value)
        temp.mValue=-mValue;
      else{
        temp.mUncertainty=std::numeric_limits<T>::max();
        temp.mValue=(T)0;
      }
      return temp;
    }

    ValueElement operator-=(const ValueElement& a){
			if(satSub(mValue, a.mValue))
        mUncertainty = std::numeric_limits<T>::max();
      else {
        satAdd(mUncertainty, a.mUncertainty);
        satAdd(mUncertainty, opError(mValue));
      }
      return *this;
    }

    ValueElement operator*=(const ValueElement& a){
      using T2 = typename multType<BaseType>::type;

			T2 min = std::numeric_limits<T>::max();
      T2 max = std::numeric_limits<T>::min();
      const T2 p1 =   mValue +   mUncertainty;
      const T2 p2 = a.mValue + a.mUncertainty;
      const T2 m1 =   mValue -   mUncertainty;
      const T2 m2 = a.mValue - a.mUncertainty;
      const T2 temp[4] = {(T2)(p1 * p2), (T2)(p1 * m2), (T2)(m1 * p2), (T2)(m1 * m2)};

      for(unsigned int i = 0; i < 4; i++) {
        if(temp[i] < min)
          min = temp[i];
        if(temp[i] > max)
          max = temp[i];
      }

      mUncertainty = (max - min) / 2;
      mValue       = (min + max) / 2;

			if(mUncertainty > std::numeric_limits<T>::max())
				mUncertainty =  std::numeric_limits<T>::max();

			if(mValue > std::numeric_limits<T>::max()) {
				mValue       =  std::numeric_limits<T>::max();
				mUncertainty =  std::numeric_limits<T>::max();
			}

			if(mValue < std::numeric_limits<T>::min()) {
				mValue       =  std::numeric_limits<T>::min();
				mUncertainty =  std::numeric_limits<T>::max();
			}

      satAdd(mUncertainty, opError(mValue));
			
      return *this;
    }

    ValueElement operator/=(const ValueElement& a){
      if(a.mUncertainty >= abs(a.mValue)){
        mValue = 0;
        mUncertainty = std::numeric_limits<T>::max();
        return *this;
      }

      using T2 = typename multType<BaseType>::type;
			
			uint8_t min = 0;
			uint8_t max = 0;

      const T2 p1 =   mValue +   mUncertainty;
      const T2 p2 = a.mValue + a.mUncertainty;
      const T2 m1 =   mValue -   mUncertainty;
      const T2 m2 = a.mValue - a.mUncertainty;
			const T2 temp[4] = { (T2)(p1 / p2), (T2)(p1 / m2),  (T2)(m1 / p2), (T2)(m1 / m2)};
			const T2 mod[4]  = { (T2)(p1 % p2), (T2)(p1 % m2),  (T2)(m1 % p2), (T2)(m1 % m2)};

      for(unsigned int i = 0; i < 4; i++) {
        if(temp[i] < temp[min])
          min = i;
        if(temp[i] > temp[max])
          max = i;
      }
      mUncertainty = (temp[max] - temp[min]) / 2 + ((mod[min]||mod[max])?1:0);
      mValue       = (temp[min] + temp[max]) / 2;
			
			if(mUncertainty > std::numeric_limits<T>::max())
				mUncertainty =  std::numeric_limits<T>::max();

			if(mValue > std::numeric_limits<T>::max()) {
				mValue       =  std::numeric_limits<T>::max();
				mUncertainty =  std::numeric_limits<T>::max();
			}

			if(mValue < std::numeric_limits<T>::min()) {
				mValue       =  std::numeric_limits<T>::min();
				mUncertainty =  std::numeric_limits<T>::max();
			}
      satAdd(mUncertainty, opError(mValue));
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

		bool operator<(const ValueElement& a) const{
			return this->mValue+this->mUncertainty < a.mValue - a.mUncertainty;
		}
		
		bool operator>(const ValueElement& a) const{
			return this->mValue-this->mUncertainty > a.mValue + a.mUncertainty;
		}
		
		bool operator<(const BaseType& a) const{
			return this->mValue+this->mUncertainty < a;
		}
		
		bool operator>(const BaseType& a) const{
			return this->mValue-this->mUncertainty > a;
		}

    explicit operator BaseType() const { return mValue; }
    //explicit operator int() const { return (int)mValue; }
    explicit operator ValueElement<T, false>() const { return ValueElement<T, false>(mValue); }

    constexpr static std::size_t size() noexcept {return sizeof(mUncertainty)+sizeof(mValue);}
    constexpr bool hasUncertainty()     noexcept {return true;}
};

template<typename T, bool U>
bool operator<=(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return !(a>b);
}

template<typename T, bool U>
bool operator<=(const ValueElement<T, U>& a, const T& b){
	return !(a>b);
}

template<typename T, bool U>
bool operator>=(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return !(a<b);
}

template<typename T, bool U>
bool operator>=(const ValueElement<T, U>& a, const T& b){
	return !(a<b);
}

template<typename T, bool U>
bool operator==(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return a<=b && a>=b;
}

template<typename T, bool U>
bool operator==(const ValueElement<T, U>& a, const T& b){
	return a<=b && a>=b;
}

template<typename T, bool U>
bool operator!=(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return a<b || a>b;
}

template<typename T, bool U>
bool operator!=(const ValueElement<T, U>& a, const T& b){
	return a<b || a>b;
}

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
