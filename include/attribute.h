#pragma once

#include <iostream>

#include <scale.h>
#include <unit.h>
#include <value.h>
#include <valueDescription.h>
#include <attributeID.h>
#include <exception.h>

#include <iostream>

class None{};

template<AttributeID i, typename T, std::size_t n, typename BoostUnit=None>
class Attribute
{
  public:
    using ValueType = Value<T, n, true>;
    using ValueElementType = typename ValueType::value_type;
  private:
    DynamicScale s;
    ValueType v;
  public:
    Attribute() : v(){}

    const ValueType& value() const {return v;}
    ValueType& value(){return v;}
    void value(const ValueType& v){this->v=v;}
    static constexpr AttributeID id(){return i;}
    const DynamicScale& scale() const{return s;}
    DynamicScale& scale(){return s;}
    const Unit unit() const{
      BoostUnit u;
      return u;
    }
};

template<AttributeID i, typename T, std::size_t n>
class Attribute<i,T,n, None>
{
  public:
    using ValueType = Value<T, n, false>;
    using ValueElementType = typename ValueType::value_type;
  private:
    ValueType v;
  public:
    Attribute() : v(){}

    const ValueType& value() const {return v;}
    ValueType& value(){return v;}
    void value(const ValueType& v){this->v=v;}
    static constexpr AttributeID id(){return i;}
};

template<typename PacketBufferIterator, AttributeID I, typename T, std::size_t n, typename U>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const Attribute<I,T,n,U>& q)
{
  return it << q.id() << q.unit() << ValueDescription(q.value()) << q.scale();
}

template<typename PacketBufferIterator, AttributeID I, typename T, std::size_t n>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const Attribute<I,T,n, None>& q)
{
  return it << q.id() << ValueDescription(q.value());
}

template<typename PacketBufferConstIterator, AttributeID I, typename T, std::size_t n, typename U>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, Attribute<I,T,n,U>& q)
{
  AttributeID id;
  Unit u;
  ValueDescription d;
  it >> id >> u >> d;
  if(id!=q.id())
    throw IDException();
  if(d!=q.value())
    throw TypeException();
  if(u!=q.unit())
    throw UnitException();
  return it >> q.scale();
}

template<typename PacketBufferConstIterator, AttributeID I, typename T, std::size_t n>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, Attribute<I,T,n, None>& q)
{
  AttributeID id;
  ValueDescription d;
  it >> id >> d;
  if(id!=q.id())
    throw IDException();
  if(d!=q.value())
    throw TypeException();
  return it;
}

template<AttributeID I, typename T, std::size_t n, typename U>
std::ostream& operator<<(std::ostream& o, const Attribute<I,T,n,U>& a)
{
  return o << a.id() << ": " << a.value() << " * " << a.scale() << " " << a.unit();
}

template<AttributeID I, typename T, std::size_t n>
std::ostream& operator<<(std::ostream& o, const Attribute<I,T,n, None>& a)
{
  return o << a.id() << ": " << a.value();
}
