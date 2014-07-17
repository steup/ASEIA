#pragma once

#include <ValueElement.h>

class MetaValueElement{
  private:
    id::type::ID id = id::type::Base::value();
    uint8_t mStorage[sizeof(ValueElement<uint64_t>)];

  public:
    MetaValueElement() = default;
    
    template<typename T, bool u>
    MetaValueElement(const ValueElement<T,u>& v){
      id = id::type::id(T());
      for(uint8_t i=0; i<sizeof(v);i++)
        mStorage[i]=reinterpret_cast<uint8_t*>(v)[i];
    }

    template<typename T, bool u>
    bool extract(ValueElement<T,u>& v){
      if(id!=id::type::id(T()))
        return false;
      v=*reinterpret_cast<ValueElement<T,u>*>(mStorage);
      return true;
    }

    bool convert(id::type::ID newType){
      if(oldType == id::type::Int8::value()  ||
         oldType == id::type::Int16::value() ||
         oldType == id::type::Int32::value() ||
         oldType == id::type::Int64::value()){
        switch(newType){
          case()
        }
        return true;
      }
      if(oldType == id::type::UInt8::value()  ||
         oldType == id::type::UInt16::value() ||
         oldType == id::type::UInt32::value() ||
         oldType == id::type::UInt64::value()){

        return true;
      }
      if(oldType == id::type::Float::value()  ||
         oldType == id::type::Double::value()){

        return true;
      }
      return false;
    }
};
