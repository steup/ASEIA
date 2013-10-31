#pragma once

#include <iostream>

#include <scale.h>
#include <unit.h>
#include <value.h>
#include <attributeID.h>

template<typename T, typename U, std::size_t n>
class Attribute
{
  private:
    AttributeID id;
    Scale scale;
    Unit unit;
    Value<T, U, n> value;

  public:
    using ValueType = Value<T,U,n>::value_type;
    Quantity(AttributeID id, Unit& u, std::initializer_list<ValueType> l, Scale& s=1) : id(id), scale(s), unit(u), value(l){}

};

std::ostream& operator<<(std::ostream& o, const Attribute& a)
{
  return o << (uint16_t)id << value << " * " << scale << " " << unit;
}

template<PacketBufferInterator>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const Quantity& q)
{
  it << q.value() << q.scale() << q.unit();
}

template<PacketBufferConstInterator>
PacketBufferConstIterator& operator<<(PacketBufferIterator& it, Quantity& q)
{
  it >> q.value() >> q.scale() >> q.unit();
}
