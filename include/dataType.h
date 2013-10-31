#pragma once

#include <typeCode.h>
#include <array>
#include <valarray>
#include <unit.h>
#include <exception.h>
#include <endianess.h>

template<typename T, std::size_t n>
class DataType
{
  public:
    using StorageType=std::array<T, n>;

  private:
    static const TypeCode code = getCode<T>::code;
    const StorageType defaultValue;
    const Unit unit;

  public:

    DataType() : defaultValue(){}

    DataType(std::initializer_list<T> l, Unit u) : defaultValue(), unit(u)
    {
      auto i=const_cast<StorageType&>(defaultValue).begin();
      auto o=const_cast<StorageType&>(defaultValue).end();
      for(auto v : l)
      {
        if(i==o)
          throw LengthException();
        *i++=v;
      }
      if(i!=o)
          throw LengthException();

    }
 
    template<typename PacketBufferConstIterator>
    void deserialize(PacketBufferConstIterator& p)
    {
        if((TypeCode)*p++!=code)
          throw TypeException();
        if(*p++!=defaultValue.size())
          throw LengthException();
        for(auto& v : const_cast<StorageType&>(defaultValue))
          for(uint8_t i=0;i<sizeof(T);i++)
            reinterpret_cast<uint8_t*>(&v)[i]=*p++;
        const_cast<Unit&>(unit).deserialize(p);
    }

    template<typename PacketBufferIterator>
    void serialize(PacketBufferIterator& p) const
    {
      *p++=(uint8_t)code;
      *p++=(uint8_t)defaultValue.size();
      for(auto v : defaultValue)
        for(unsigned int i=0;i<sizeof(T);i++)
          *p++=reinterpret_cast<uint8_t*>(&v)[i];
      unit.serialize(p);
    }

    static constexpr size_t size() noexcept
    {
      return sizeof(defaultValue)+2+Unit::size();
    }

  template<typename U, std::size_t o> friend ostream& operator<<(ostream& out, DataType<U, o>& dt);
};

template<typename T, std::size_t n>
ostream& operator<<(ostream& out, DataType<T, n>& dt)
{
  out << "[";
  for(auto v : dt.defaultValue)
    out << v << ", ";
  return out << "] * " << dt.unit;
}
