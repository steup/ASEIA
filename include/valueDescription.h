#pragma once

#include <ostream>
#include <value.h>
#include <typeCode.h>

class ValueDescription
{
  public:
    const uint8_t n=0;
    const TypeCode c=TypeCode::unknown;
    const bool uncertainty=true;
    
    explicit ValueDescription() = default;

    template<typename T, std::size_t n, bool uncertainty>
    ValueDescription(const Value<T,n,uncertainty>& v) : n(n), c(getCode<T>::code), uncertainty(uncertainty){}

    bool operator==(const ValueDescription& v) const
    {
      return (n==v.n && c==v.c && uncertainty==v.uncertainty);
    }

    bool operator!=(const ValueDescription& v) const
    {
      return !((*this)==v);
    }
};

std::ostream& operator<<(std::ostream& o, const ValueDescription& v)
{
  return o << v.c << "[" << (uint16_t)v.n << "]" << (v.uncertainty?"+-":"");
}

template<typename PacketBufferIterator>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const ValueDescription& v)
{
  return it << v.c << v.n << v.uncertainty;
}

template<typename PacketBufferConstIterator>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, ValueDescription& v)
{
  return it >> *const_cast<TypeCode*>(&v.c) >> *const_cast<uint8_t*>(&v.n) >> *const_cast<bool*>(&v.uncertainty);
}
