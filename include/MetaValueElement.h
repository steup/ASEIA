#pragma once

#include <ValueElement.h>
#include <ID.h>
#include <limits>

class MetaValueElement{
  private:
    id::type::ID mId = id::type::Base::value();
    union{
      ValueElement<int64_t , true> _int = 0;
      ValueElement<uint64_t, true> _uint;
      ValueElement<float   , true> _float;
      ValueElement<double  , true> _double;
    };

    void equalize(MetaValueElement& b);

  public:
    MetaValueElement() = default;
    MetaValueElement(const MetaValueElement& mve);

    id::type::ID id() const { return mId; }
    
    template<typename T>
    MetaValueElement(const ValueElement<T,true>& v){
      *this=v;
    }

    template<typename T>
    void extract(ValueElement<T,true>& v){
      convert(id::type::id(T()));
      switch(mId){
        case(id::type::UInt8::value()) :
        case(id::type::UInt16::value()):
        case(id::type::UInt32::value()):
        case(id::type::UInt64::value()): v = _uint;
                                         break;

        case(id::type::Int8::value())  : 
        case(id::type::Int16::value()) : 
        case(id::type::Int32::value()) : 
        case(id::type::Int64::value()) : v = _int;
                                         break;

        case(id::type::Float::value()) : v = _float;
                                         break;

        case(id::type::Double::value()): v = _double;
                                         break;
      }
    }

    void convert(id::type::ID newType);

    template<typename T>
    MetaValueElement& operator=(const ValueElement<T,true>& v){
      mId = id::type::id(T());
      switch( mId ){
        case(id::type::UInt8::value()):  
        case(id::type::UInt16::value()): 
        case(id::type::UInt32::value()): 
        case(id::type::UInt64::value()): _uint = v;
                                         break;

        case(id::type::Int8::value())  : 
        case(id::type::Int16::value()) : 
        case(id::type::Int32::value()) : 
        case(id::type::Int64::value()) : _int = v;
                                         break;

        case(id::type::Float::value()) : _float = v;
                                         break;

        case(id::type::Double::value()): _double = v;
                                         break;
      }
      return *this;
    }

    MetaValueElement& operator+=(const MetaValueElement& b);
    MetaValueElement& operator-=(const MetaValueElement& b);
    MetaValueElement& operator*=(const MetaValueElement& b);
    MetaValueElement& operator/=(const MetaValueElement& b);
    MetaValueElement operator+(const MetaValueElement& b) const;
    MetaValueElement operator-(const MetaValueElement& b) const;
    MetaValueElement operator*(const MetaValueElement& b) const;
    MetaValueElement operator/(const MetaValueElement& b) const;

    template<typename T>
    MetaValueElement& operator+=(const T& b) const{

      return *this;
    }

    template<typename T>
    MetaValueElement& operator-=(const T& b) const{

      return *this;
    }

    template<typename T>
    MetaValueElement& operator*=(const T& b) const{

      return *this;
    }

    template<typename T>
    MetaValueElement& operator/=(const T& b) const{

      return *this;
    }

    template<typename T>
    MetaValueElement operator+(const T& b) const{
      return *this;
    }

    template<typename T>
    MetaValueElement operator-(const T& b) const{
      return *this;
    }

    template<typename T>
    MetaValueElement operator*(const T& b) const{
      return *this;
    }

    template<typename T>
    MetaValueElement operator/(const T& b) const{
      return *this;
    }
    friend std::ostream& operator<<(std::ostream&, const MetaValueElement&);
};

template<typename T>
MetaValueElement operator+(const T& a, const MetaValueElement& b){
  return b;
};

template<typename T>
MetaValueElement operator-(const T& a, const MetaValueElement& b){
  return b;
}

template<typename T>
MetaValueElement operator*(const T& a, const MetaValueElement& b){
  return b;
}

template<typename T>
MetaValueElement operator/(const T& a, const MetaValueElement& b){
  return b;
}
