#pragma once

#include <ValueElement.h>
#include <ID.h>
#include <limits>

class MetaValueElement{
  private:
    id::type::ID id = id::type::Base::value();
    union{
      int64_t  _int;
      uint64_t _uint;
      float    _float;
      double   _double;
    } value;
    union{
      int64_t  _int;
      uint64_t _uint;
      float    _float;
      double   _double;
    } uncertainty;

    template<typename T>
    bool satAdd(T& a, T b) const{
      T diff = std::numeric_limits<T>::max() - a;
      if( diff < b ) {
        a = std::numeric_limits<T>::max();
        return true;
      } else {
        a += b;
        return false;
      }
    }

    template<typename T>
    bool satSub(T& a, T b) const{
      T diff = std::numeric_limits<T>::max() + b;
      if( diff > a ) {
        a = std::numeric_limits<T>::min();
        return true;
      } else {
        a -= b;
        return false;
      }
    }

    template<typename T1, typename T2>
    void checkBounds(T1& v, T1& u, T2 dummy) const{
      if( v > std::numeric_limits<T2>::max() ) {
        v = std::numeric_limits<T2>::max();
        T1 temp = v - std::numeric_limits<T2>::max();
        satAdd( u, temp );
      }
      if( v < std::numeric_limits<T2>::min() ) {
        v = std::numeric_limits<T2>::min();
        T1 temp = std::numeric_limits<T2>::min() - v;
        satAdd( u, temp );
      }
    }

  public:
    MetaValueElement() = default;
    
    template<typename T, bool u>
    MetaValueElement(const ValueElement<T,u>& v){
      *this=v;
    }

    template<typename T, bool u>
    void extract(ValueElement<T,u>& v){
      convert(ValueElement<T,u>::TypeID::value());
      switch(id){
        case(id::type::UInt8::value()) : checkBounds(value._uint, uncertainty._uint, uint8_t());
        case(id::type::UInt16::value()): checkBounds(value._uint, uncertainty._uint, uint16_t());
        case(id::type::UInt32::value()): checkBounds(value._uint, uncertainty._uint, uint32_t());
        case(id::type::UInt64::value()): v = ValueElement<T,u>({value._uint, uncertainty._uint});
                                         break;
        case(id::type::Int8::value())  : checkBounds(value._int, uncertainty._int, int8_t());
        case(id::type::Int16::value()) : checkBounds(value._int, uncertainty._int, int16_t());
        case(id::type::Int32::value()) : checkBounds(value._int, uncertainty._int, int32_t());
        case(id::type::Int64::value()) : v = ValueElement<T,u>({value._int , uncertainty._int });
                                         break;
        case(id::type::Float::value()) : v = ValueElement<T,u>({value._float , uncertainty._float });
                                         break;
        case(id::type::Double::value()): v = ValueElement<T,u>({value._double, uncertainty._double});
                                         break;
      }
    }

    template<typename T, bool u>
    MetaValueElement& operator=(const ValueElement<T,u>& v){
      id = id::type::id(T());
      switch( id ){
        case(id::type::UInt8::value()):  
        case(id::type::UInt16::value()): 
        case(id::type::UInt32::value()): 
        case(id::type::UInt64::value()): value._uint=v.value();
                                         uncertainty._uint=v.uncertainty();
                                         break;

        case(id::type::Int8::value())  : 
        case(id::type::Int16::value()) : 
        case(id::type::Int32::value()) : 
        case(id::type::Int64::value()) : value._int=v.value();
                                         uncertainty._int=v.uncertainty();
                                         break;

        case(id::type::Float::value()) : value._float=v.value();
                                         uncertainty._float=v.uncertainty();
                                         break;

        case(id::type::Double::value()): value._double=v.value();
                                         uncertainty._double=v.uncertainty();
                                         break;
      }
      return *this;
    }

    void convert(id::type::ID newType){
      if(id >= id::type::Int8::value()  &&
         id <= id::type::Int64::value()){
        switch(newType){
          case(id::type::UInt8::value()) : 
          case(id::type::UInt16::value()):
          case(id::type::UInt32::value()):
          case(id::type::UInt64::value()): checkBounds( value._int, uncertainty._int, uint64_t() );
                                           value._uint=value._int;
                                           uncertainty._uint=uncertainty._int;
                                           break;

          case(id::type::Float::value()) : value._float=value._int;
                                           uncertainty._float=uncertainty._int;
                                           break;

          case(id::type::Double::value()): value._double=value._int;
                                           uncertainty._double=uncertainty._int;
                                           break;
        }
      }
      if(id >= id::type::UInt8::value()  &&
         id <= id::type::UInt64::value()) {
        switch(newType){
          case(id::type::Int8::value())  : 
          case(id::type::Int16::value()) :
          case(id::type::Int32::value()) :
          case(id::type::Int64::value()) : checkBounds( value._uint, uncertainty._uint, int64_t() );
                                           value._int=value._uint;
                                           uncertainty._int=uncertainty._uint;
                                           break;

          case(id::type::Float::value()) : value._float=value._uint;
                                           uncertainty._float=uncertainty._uint;
                                           break;

          case(id::type::Double::value()): value._double=value._uint;
                                           uncertainty._double=uncertainty._uint;
                                           break;
        }
      }
      if(id == id::type::Float::value()){
        switch(newType){
          case(id::type::Int8::value())  : 
          case(id::type::Int16::value()) :
          case(id::type::Int32::value()) :
          case(id::type::Int64::value()) : checkBounds( value._float, uncertainty._float, int64_t() );
                                           value._int=value._float;
                                           uncertainty._int=uncertainty._float+1;
                                           break;

          case(id::type::UInt8::value()) : 
          case(id::type::UInt16::value()):
          case(id::type::UInt32::value()):
          case(id::type::UInt64::value()): checkBounds( value._float, uncertainty._float, uint64_t() );
                                           value._uint=value._float;
                                           uncertainty._uint=uncertainty._float+1;
                                           break;
                                           
          case(id::type::Double::value()): value._double=value._float;
                                           uncertainty._double=uncertainty._float+std::numeric_limits<float>::epsilon();
                                           break;
        }
      }
      if(id == id::type::Double::value()){
        switch(newType){
          case(id::type::Int8::value())  : 
          case(id::type::Int16::value()) :
          case(id::type::Int32::value()) :
          case(id::type::Int64::value()) : checkBounds( value._double, uncertainty._double, int64_t() );
                                           value._int=value._double;
                                           uncertainty._int=uncertainty._double+1;
                                           break;

          case(id::type::UInt8::value()) : 
          case(id::type::UInt16::value()):
          case(id::type::UInt32::value()):
          case(id::type::UInt64::value()): checkBounds( value._double, uncertainty._double, uint64_t() );
                                           value._uint=value._double;
                                           uncertainty._uint=uncertainty._double + 1;
                                           break;

          case(id::type::Float::value()) : checkBounds( value._double, uncertainty._double, float() );
                                           value._float=value._double;
                                           uncertainty._float=uncertainty._double+std::numeric_limits<float>::epsilon();
                                           break;
        }
      }
      id=newType;
    }
};
