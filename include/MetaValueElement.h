#pragma once

#include <ValueElement.h>
#include <ID.h>
#include <limits>

class MetaValueElement{
  private:
    id::type::ID id = id::type::Base::value();
    union{
      ValueElement<int64_t , true> _int = 0;
      ValueElement<uint64_t, true> _uint;
      ValueElement<float   , true> _float;
      ValueElement<double  , true> _double;
    };

  public:
    MetaValueElement() = default;
    
    template<typename T>
    MetaValueElement(const ValueElement<T,true>& v){
      *this=v;
    }

    template<typename T>
    void extract(ValueElement<T,true>& v){
      convert(ValueElement<T,true>::TypeID::value());
      switch(id){
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

    template<typename T>
    MetaValueElement& operator=(const ValueElement<T,true>& v){
      id = id::type::id(T());
      switch( id ){
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

    void convert(id::type::ID newType){
      ValueElement<uint64_t, true> tempU;
      ValueElement<int64_t , true> tempI;
      ValueElement<float   , true> tempF;
      ValueElement<double  , true> tempD;

      switch(id){
          case(id::type::Int8::value()) : 
          case(id::type::Int16::value()):
          case(id::type::Int32::value()):
          case(id::type::Int64::value()):  tempU = _int;
                                           tempI = _int;
                                           tempF = _int;
                                           tempD = _int;
                                           break;
          case(id::type::UInt8::value()) : 
          case(id::type::UInt16::value()):
          case(id::type::UInt32::value()):
          case(id::type::UInt64::value()): tempU = _uint;
                                           tempI = _uint;
                                           tempF = _uint;
                                           tempD = _uint;
                                           break;

          case(id::type::Float::value()) : tempU = _float;
                                           tempI = _float;
                                           tempF = _float;
                                           tempD = _float;
                                           break;

          case(id::type::Double::value()): tempU = _double;
                                           tempI = _double;
                                           tempF = _double;
                                           tempD = _double;
                                           break;
      }

      switch(newType){
          case(id::type::Int8::value()) : 
          case(id::type::Int16::value()):
          case(id::type::Int32::value()):
          case(id::type::Int64::value()):  _int = tempI;
                                           break;
          case(id::type::UInt8::value()) : 
          case(id::type::UInt16::value()):
          case(id::type::UInt32::value()):
          case(id::type::UInt64::value()): _uint = tempU;
                                           break;

          case(id::type::Float::value()) : _float = tempF;
                                           break;

          case(id::type::Double::value()): _double= tempD;
                                           break;
      }
      id=newType;
    }
};
