#pragma once

#include <ID.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <boost/mpl/if.hpp>

#include <type_traits>
#include <initializer_list>
#include <limits>
#include <iterator>
#include <cmath>

namespace {
	struct doNothing { 
		template<typename T>
		struct apply {
			using type =  T;
		};
	};

	struct unSign{
		template<typename  T>
		struct apply {
			using type = typename std::make_unsigned<T>::type;
		};
	};
	
	template<typename T>
	struct make_unsigned{
		using func = typename boost::mpl::if_<typename std::is_floating_point<T>, doNothing, unSign>::type;
		using type = typename  func::template apply<T>::type;
	};
	
	template<>
	struct make_unsigned<bool>{
		using type = bool;
	};
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
  
  template<typename T>
  inline T opErrDiv(T aMin, T aMax, T bMin, T bMax) {
      if( aMin%bMin || aMax%bMax )
        return 1;
      else
        return 0;
  }
  
  template<>
  inline float opErrDiv(float aMin, float aMax, float bMin, float bMax) {
    return 0;
  }
  
  template<>
  inline double opErrDiv(double aMin, double aMax, double bMin, double bMax) {
    return 0;
  }

  template<typename T1, typename T2>
  inline T1 modifyU(T1 u, T2 dummy){
    return 0;
  }
  

  template<typename T>
  inline float modifyU(float u, T dummy){
    if(u!=dummy)
      return 1.0f;
    else
      return 0.0f;
  }

  template<typename T>
  inline double modifyU(double u, T dummy){
    if(u!=dummy)
      return 1.0;
    else
      return 0.0;
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
    if ((float)u != u)
      return std::numeric_limits<float>::epsilon();
    else
      return 0;
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
  typename make_unsigned<T>::type opError(T result){
    return std::numeric_limits<T>::epsilon()*result;
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

template<typename T>
class ValueElementBase {
	public:
		using VType    = T;
		using UType    = typename make_unsigned<T>::type;
		using PType    = typename multType<T>::type;
    using InitType = std::initializer_list<PType>;
    using U        = boost::mpl::bool_<false>;
    using TypeID   = typename id::type::t2Type<T>::type;
    using BaseType = T;
	protected:
		VType mValue;

	public:
		constexpr ValueElementBase() :  mValue(0) {}
		ValueElementBase(const T v) : mValue(v) {}
		ValueElementBase(const InitType& init) {
			if(init.size())
				value(*init.begin());
		}

		VType value() const { return mValue; }
		void value( VType v) { mValue = v; }
		UType uncertainty() const { return std::numeric_limits<UType>::max(); }
    void uncertainty ( T u ) {}
		
		ValueElementBase& operator+=(T a){
			satAdd(mValue, a);
			return *this;
		}

		ValueElementBase& operator-=(T a){
			if(std::is_same<T, bool>::value)
				mValue=false;
			else
				satSub(mValue, a);
			return *this;
		}

		ValueElementBase& operator*=(T a){
			PType res = (PType)this->mValue * (PType)a;
			if( res > (PType)std::numeric_limits<VType>::max())
				res = (PType)std::numeric_limits<VType>::max();
			if( res < (PType)std::numeric_limits<VType>::min())
				res = (PType)std::numeric_limits<VType>::min();
			this->mValue = (VType)res;
			return *this;
		}

		ValueElementBase& operator/=(T a){
			if(std::is_same<T, bool>::value) {
				mValue = false;
				return *this;
			}
			if(!a) {
				mValue = 0;
				if(mValue>0)
					mValue=std::numeric_limits<VType>::max();
				if(mValue<0)
					mValue=std::numeric_limits<VType>::min();
				return *this;
			}
			PType res = (PType)this->mValue / (PType)a;
			if( res > (PType)std::numeric_limits<VType>::max())
				res = (PType)std::numeric_limits<VType>::max();
			if( res < (PType)std::numeric_limits<VType>::min())
				res = (PType)std::numeric_limits<VType>::min();
			this->mValue = (VType)res;
			return *this;
		}

		constexpr const static std::size_t size() noexcept { return sizeof(VType);   }
    constexpr const        id::type::ID  id() const noexcept { return TypeID::value(); }
    constexpr const bool     hasUncertainty() const noexcept { return U::value;        }
		operator T() const {
			return mValue;
		}
    template<typename, bool> friend class ValueElement;
};
template<>
class ValueElementBase<bool> {
	public:
		using T        = bool;
		using VType    = T;
		using UType    = T;
		using PType    = T;
    using InitType = std::initializer_list<PType>;
    using U        = boost::mpl::bool_<false>;
    using TypeID   = id::type::Bool;
    using BaseType = T;
	protected:
		VType mValue;

	public:
		constexpr ValueElementBase() :  mValue(false) {}
		ValueElementBase(const VType v) : mValue(v) {}
		ValueElementBase(const InitType& init) {
			if(init.size())
				value(*init.begin());
		}

		VType value() const { return mValue; }
		void value( VType v) { mValue = v; }
		UType uncertainty() const { return std::numeric_limits<UType>::max(); }
    void uncertainty ( T u ) {}

		ValueElementBase& operator+=(T a){
			mValue = mValue || a;
			return *this;
		}

		ValueElementBase& operator-=(T a){
			mValue = false;
			return *this;
		}

		ValueElementBase& operator*=(T a){
			mValue = mValue && a;
			return *this;
		}

		ValueElementBase& operator/=(T a){
			mValue = false;
			return *this;
		}

		constexpr const static std::size_t size() noexcept { return sizeof(VType);   }
    constexpr const        id::type::ID  id() const noexcept { return TypeID::value(); }
    constexpr const bool     hasUncertainty() const noexcept { return U::value;        }
		operator T() const {
			return mValue;
		}
    template<typename, bool> friend class ValueElement;
};

template<typename T, bool U =true>
class ValueElement;

template<>
class ValueElement<bool, false> : public ValueElementBase<bool>{
  public:
		using Base = ValueElementBase<T>;
		using typename Base::InitType;
		using typename Base::VType;
    using typename Base::TypeID;
    using typename Base::BaseType;
		using typename Base::PType;
    using typename Base::U;
		using Bool = ValueElement<bool, false>;

		constexpr ValueElement() = default;
		ValueElement(const T v) : Base(v) {}
		ValueElement(const InitType& init)  : Base(init) {}

    template<typename T2, bool U>
    ValueElement(const ValueElement<T2, U>& data){
      this->mValue = data.mValue;
    }

		Bool operator<(const ValueElement& a) const{
			return this->mValue < a.mValue;
		}

		Bool operator>(const ValueElement& a) const{
			return this->mValue > a.mValue;
		}

		ValueElement operator-() const {
			return ValueElement(!mValue);
		}

		ValueElement& operator+=(const ValueElement& a) {
			Base::operator+=(a.mValue);
			return *this;
		}

		ValueElement& operator-=(const ValueElement& a) {
			Base::operator-=(a.mValue);
			return *this;
		}

		ValueElement& operator*=(const ValueElement& a) {
			Base::operator*=(a.mValue);
			return *this;
		}

		ValueElement& operator/=(const ValueElement& a) {
			Base::operator/=(a.mValue);
			return *this;
		}

};

template<>
class ValueElement<bool, true> : public ValueElementBase<bool>{
  public:
		using Base     = ValueElementBase<T>;
		using typename Base::InitType;
		using typename Base::VType;
		using typename Base::UType;
    using typename Base::TypeID;
    using typename Base::BaseType;
		using typename Base::PType;
    using U = boost::mpl::bool_<true>;
		using Bool = ValueElement<bool, true>;
  protected:
    UType mUncertainty;
  public:
    constexpr ValueElement() : mUncertainty(std::numeric_limits<T>::max()) {}
    ValueElement(VType v, UType u=0) : Base(v), mUncertainty(u) {}
    ValueElement(InitType init) {
      auto iter = init.begin();

			if(init.size()<1) {
				this->mValue = 0;
				mUncertainty = std::numeric_limits<UType>::max();
				return;
			}
      
			this->mValue = *iter;

			if(init.size()<2)
				mUncertainty = 0;
			else
      	mUncertainty = *std::next(iter);
    }

    template<typename T2>
    ValueElement(const ValueElement<T2, true>& data){
      this->mValue = data.value();
			this->mUncertainty = data.uncertainty()>(typename ValueElement<T2>::UType)(abs(data.mValue));
    }
    
    template<typename T2>
    ValueElement(const ValueElement<T2, false>& data){
      this->mValue = data.value();
			this->mUncertainty = true;
    }

    UType uncertainty() const{ return mUncertainty; }
    void uncertainty(UType u){ mUncertainty = u; }
    
		Bool operator<(const ValueElement& a) const{
			return this->mValue+this->mUncertainty < a.mValue - a.mUncertainty;
		}
		
		Bool operator>(const ValueElement& a) const{
			return this->mValue-this->mUncertainty > a.mValue + a.mUncertainty;
		}
		
		ValueElement operator-() const{
			return ValueElement(!mValue, mUncertainty);
    }

    ValueElement& operator+=(const ValueElement& a){
			mValue       = mValue       || a.mValue;
			mUncertainty = mUncertainty || a.mUncertainty;
      return *this;
    }


    ValueElement& operator-=(const ValueElement& a){
			mValue       = false;
			mUncertainty = true;
      return *this;
    }

    ValueElement& operator*=(const ValueElement& a){
			mValue       = mValue       && a.mValue;
      mUncertainty = mUncertainty || a.mUncertainty;
      return *this;
    }

    ValueElement& operator/=(const ValueElement& a){
			mValue       = false;
			mUncertainty = true;
      return *this;
    }

    constexpr const static std::size_t size() noexcept {return sizeof(VType)+sizeof(UType);}
    constexpr const bool hasUncertainty() const    noexcept {return U::value;}
    template<typename, bool> friend class ValueElement;
};

template<typename T>
class ValueElement<T, false> : public ValueElementBase<T>{
  public:
		using Base = ValueElementBase<T>;
		using typename Base::InitType;
		using typename Base::VType;
    using typename Base::TypeID;
    using typename Base::BaseType;
		using typename Base::PType;
    using typename Base::U;
		using Bool = ValueElement<bool, false>;

		constexpr ValueElement() = default;
		ValueElement(const T v) : Base(v) {}
		ValueElement(const InitType& init)  : Base(init) {}

    template<typename T2, bool U>
    ValueElement(const ValueElement<T2, U>& data){
      checkBounds(data.mValue, this->mValue);
    }
    
		Bool operator<(const ValueElement& a) const{
			return this->mValue < a.mValue;
		}

		Bool operator>(const ValueElement& a) const{
			return this->mValue > a.mValue;
		}
		
		ValueElement operator-() const {
			if(!std::is_signed<VType>::value)
				return ValueElement(std::numeric_limits<VType>::min());
			else
				return ValueElement(-this->mValue);
		}

		ValueElement& operator+=(const ValueElement& a) {
			Base::operator+=(a.mValue);
			return *this;
		}
		
		ValueElement& operator-=(const ValueElement& a) {
			Base::operator-=(a.mValue);
			return *this;
		}
		
		ValueElement& operator*=(const ValueElement& a) {
			Base::operator*=(a.mValue);
			return *this;
		}

		ValueElement& operator/=(const ValueElement& a) {
			Base::operator/=(a.mValue);
			return *this;
		}
		
};

template<typename T>
class ValueElement<T, true> : public ValueElementBase<T>{
  public:
		using Base     = ValueElementBase<T>;
		using typename Base::InitType;
		using typename Base::VType;
		using typename Base::UType;
    using typename Base::TypeID;
    using typename Base::BaseType;
		using typename Base::PType;
    using U = boost::mpl::bool_<true>;
		using Bool = ValueElement<bool, true>;
  protected:
    UType mUncertainty;
  public:
    constexpr ValueElement() : mUncertainty(std::numeric_limits<T>::max()) {}
    ValueElement(VType v, UType u=0) : Base(v), mUncertainty(u) {}
    ValueElement(InitType init) {
      auto iter = init.begin();

			if(init.size()<1) {
				this->mValue = 0;
				mUncertainty = std::numeric_limits<UType>::max();
				return;
			}
      
			this->mValue = *iter;

			if(init.size()<2)
				mUncertainty = 0;
			else
      	mUncertainty = *std::next(iter);
    }

    template<typename T2>
    ValueElement(const ValueElement<T2, true>& data){
      typename ValueElement<T2>::UType u = checkBounds(data.mValue, this->mValue);
      satAdd(u, data.uncertainty());
      checkBounds(u, this->mUncertainty);
    }
    
    template<typename T2>
    ValueElement(const ValueElement<T2, false>& data) : ValueElement() {
      checkBounds(data.mValue, this->mValue);
    }

    UType uncertainty() const{ return mUncertainty; }
    void uncertainty(UType u){ mUncertainty = u; }
    
		Bool operator<(const ValueElement& a) const{
			return this->mValue+this->mUncertainty < a.mValue - a.mUncertainty;
		}
		
		Bool operator>(const ValueElement& a) const{
			return this->mValue-this->mUncertainty > a.mValue + a.mUncertainty;
		}
		
		ValueElement operator-() const{
      ValueElement temp(*this);
			if(std::is_same<T, bool>::value) {
				temp.mValue = !temp.mValue;
				return temp;
			}

      if(std::is_signed<T>::value)
        temp.mValue=-temp.mValue;
      else{
        temp.uncertainty(std::numeric_limits<UType>::max());
        temp.mValue=0;
      }
      return temp;
    }

    ValueElement& operator+=(const ValueElement& a){
      if(satAdd(this->mValue, a.mValue))
        mUncertainty = std::numeric_limits<UType>::max();
      else {
        satAdd(mUncertainty, a.mUncertainty);
        satAdd(mUncertainty, opError(this->mValue));
      }
      return *this;
    }


    ValueElement& operator-=(const ValueElement& a){
			if(std::is_same<T, bool>::value) {
				this->mValue = false;
				mUncertainty = true;
				return *this;;
			}
			if(satSub(this->mValue, a.mValue))
        mUncertainty = std::numeric_limits<UType>::max();
      else {
        satAdd(mUncertainty, a.mUncertainty);
        satAdd(mUncertainty, opError(this->mValue));
      }
      return *this;
    }

    ValueElement& operator*=(const ValueElement& a){
      using T2 = PType;

			T2 min = std::numeric_limits<T>::max();
      T2 max = std::numeric_limits<T>::min();
      const T2 p1 = (T2)this->mValue + (T2)  mUncertainty;
      const T2 p2 = (T2)    a.mValue + (T2)a.mUncertainty;
      const T2 m1 = (T2)this->mValue - (T2)  mUncertainty;
      const T2 m2 = (T2)    a.mValue - (T2)a.mUncertainty;
      const T2 temp[4] = {(T2)(p1 * p2), (T2)(p1 * m2), (T2)(m1 * p2), (T2)(m1 * m2)};

      for(unsigned int i = 0; i < 4; i++) {
        if(temp[i] < min)
          min = temp[i];
        if(temp[i] > max)
          max = temp[i];
      }

      T2 u = (max - min) / 2;
      T2 v = (min + max) / 2;

			if(std::is_integral<T>::value && u > (T2)std::numeric_limits<UType>::max())
				u =  std::numeric_limits<UType>::max();

			if(std::is_integral<T>::value && v > (T2)std::numeric_limits<T>::max()) {
				v =  std::numeric_limits<T>::max();
				u =  std::numeric_limits<UType>::max();
			}

			if(std::is_integral<T>::value && v < (T2)std::numeric_limits<T>::min()) {
				v =  std::numeric_limits<T>::min();
				u =  std::numeric_limits<UType>::max();
			}

      mUncertainty = u;
      this->mValue = v;

      satAdd(mUncertainty, opError(this->mValue));
			
      return *this;
    }

    ValueElement& operator/=(const ValueElement& a){
			if(std::is_same<T, bool>::value) {
				this->mValue = false;
				mUncertainty = true;
				return *this;
			}
      if(a==ValueElement(0,0)){
        this->mValue = 0;
        mUncertainty = std::numeric_limits<UType>::max();
        return *this;
      }

      using T2 = PType;
			
			uint8_t min = 0;
			uint8_t max = 0;

      const T2 _1[2] = { (T2)((T2)(this->mValue) + (T2)  mUncertainty), (T2)((T2)(this->mValue) - (T2)  mUncertainty) };
      const T2 _2[2] = { (T2)((T2)(    a.mValue) + (T2)a.mUncertainty), (T2)((T2)(    a.mValue) - (T2)a.mUncertainty) };

			const T2 temp[4] = { (T2)(_1[0] / _2[0]), (T2)(_1[0] / _2[1]),  (T2)(_1[1] / _2[0]), (T2)(_1[1] / _2[1]) };

      for(unsigned int i = 0; i < 4; i++) {
        if(temp[i] < temp[min])
          min = i;
        if(temp[i] > temp[max])
          max = i;
      }
      
      T2 u = (temp[max] - temp[min]) / 2 ;
      T2 v = (temp[min] + temp[max]) / 2;

      u += opErrDiv(_1[min/2], _1[max/2], _2[min%2], _2[max%2]);
			
			if(std::is_integral<T>::value && u > (T2)std::numeric_limits<UType>::max())
				u =  std::numeric_limits<UType>::max();

			if(std::is_integral<T>::value && v > (T2)std::numeric_limits<T>::max()) {
				v =  std::numeric_limits<T>::max();
				u =  std::numeric_limits<UType>::max();
			}

			if(std::is_integral<T>::value && v < (T2)std::numeric_limits<T>::min()) {
				v =  std::numeric_limits<T>::min();
				u =  std::numeric_limits<UType>::max();
			}

      mUncertainty = u;
      this->mValue = v;

      satAdd(mUncertainty, opError(this->mValue));
      return *this;
    }

    constexpr const static std::size_t size() noexcept {return sizeof(VType)+sizeof(UType);}
    constexpr const bool hasUncertainty() const    noexcept {return U::value;}
};


template<typename T, bool U>
ValueElement<bool, U> operator<=(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return -(a>b);
}

template<typename T, bool U>
ValueElement<bool, U> operator>=(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return -(a<b);
}

template<typename T, bool U>
ValueElement<bool, U> operator==(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return a<=b && a>=b;
}

template<typename T, bool U>
ValueElement<bool, U> operator!=(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return a<b || a>b;
}

template<typename T, bool U>
ValueElement<bool, U> approxEqual(const ValueElement<T, U>& a, const ValueElement<T, U>& b){
	return a<=b || a>=b;
}


template<typename T, bool U>
ValueElement<T, U> operator+(const ValueElement<T, U>& a, const ValueElement<T, U>& b) {
	return ValueElement<T, U>(a)+=b;
}

template<typename T, bool U>
ValueElement<T, U> operator-(const ValueElement<T, U>& a, const ValueElement<T, U>& b) {
	return ValueElement<T, U>(a)-=b;
}

template<typename T, bool U>
ValueElement<T, U> operator*(const ValueElement<T, U>& a, const ValueElement<T, U>& b) {
	return ValueElement<T, U>(a)*=b;
}

template<typename T, bool U>
ValueElement<T, U> operator/(const ValueElement<T, U>& a, const ValueElement<T, U>& b) {
	return ValueElement<T, U>(a)/=b;
}

template<typename T, bool U>
ValueElement<T, U> operator+(const ValueElement<T, U>& a, T b){
	return ValueElement<T, U>(a)+=b;
}
template<typename T, bool U>
ValueElement<T, U> operator-(const ValueElement<T, U>& a, T b){
	return ValueElement<T, U>(a)-=b;
}
template<typename T, bool U>
ValueElement<T, U> operator*(const ValueElement<T, U>& a, T b){
	return ValueElement<T, U>(a)*=b;
}
template<typename T, bool U>
ValueElement<T, U> operator/(const ValueElement<T, U>& a, T b){
	return ValueElement<T, U>(a)/=b;
}

template<typename T, bool U>
ValueElement<T, U> operator+(T a, const ValueElement<T, U>& b){
	return ValueElement<T, U>(a)+=b;
}
template<typename T, bool U>
ValueElement<T, U> operator-(T a, const ValueElement<T, U>& b){
	return ValueElement<T, U>(a)-=b;
}
template<typename T, bool U>
ValueElement<T, U> operator*(T a, const ValueElement<T, U>& b){
	return ValueElement<T, U>(a)*=b;
}
template<typename T, bool U>
ValueElement<T, U> operator/(T a, const ValueElement<T, U>& b){
	return ValueElement<T, U>(a)/=b;
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
