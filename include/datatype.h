#pragma once

#include <typeCode.h>
#include <array>
#include <valarray>
#include <unit.h>
#include <dataTypeException.h>

template<typename T>
struct DataType
{
  static const TypeCode code = getCode<T>::code;
  const std::valarray<T> defaultValue;
  const Unit unit;

  DataType(std::initializer_list<T>& l, Unit& u) : defaultValue(l), unit(unit){}

  DataType(PacketBuffer::iterator& p)
  {
      if(*p++!=code)
        throw DataTypeException::incompatibleTypes;
      uint8_t len=*p++;
      std::valarray<T>& defaultValueInit=const_cast<std::valarray&>(defaultValue);
      defaultValueInit.resize(len, 0);
      for(auto& v : defaultValueInit)
      {
        T temp;
        for(uint8_t i=0;i<sizeof(T);i++)
          reinterpret_cast<uint8_t*>(&temp)[i]=*p++;
        v=ntoh(temp);
      }
      const_cast<Unit&>(unit)=Unit(p);
  }

  void serialize(PacketBuffer& p)
  {
    p.push_back((uint8_t)code);
    p.push_back((uint8_t)defaultValue.size());
    for(auto v : defaultValue)
    {
      T temp=hton(v);
      for(unsigned int i=0;i<sizeof(T);i++)
        p.push_back(reinterpret_cast<uint8_t*>(&temp)[i]);
    }
    unit.serialize(p);
  }

  friend ostream& operator<<(ostream& out, DataType<T>& dt);
};

template<typename T>
ostream& operator<<(ostream& out, DataType<T>& dt)
{
  return out << dt.defaultValue << dt.unit;
}
