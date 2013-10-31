#pragma once

#include <iostream>

#include <scale.h>
#include <unit.h>
#include <value.h>
#include <valueDescription.h>
#include <exception.h>

#include <iostream>

class None{};

template<typename AttributeID, typename T, std::size_t n, typename BoostUnit=None>
class Attribute
{
  public:
    using ValueType = Value<T, n, true>;
    using ValueElementType = typename ValueType::value_type;
    using ID = AttributeID;
  private:
    DynamicScale s;
    ValueType v;
  public:
    Attribute() : v(){}

    const ValueType& value() const {return v;}
    ValueType& value(){return v;}
    void value(const ValueType& v){this->v=v;}
    constexpr ID id(){return ID();}
    const DynamicScale& scale() const{return s;}
    DynamicScale& scale(){return s;}
    const Unit unit() const{
      BoostUnit u;
      return u;
    }
};

template<typename AttributeID, typename T, std::size_t n>
class Attribute<AttributeID,T,n, None>
{
  public:
    using ValueType = Value<T, n, false>;
    using ValueElementType = typename ValueType::value_type;
    using ID = AttributeID;
  private:
    ValueType v;
  public:
    Attribute() : v(){}

    const ValueType& value() const {return v;}
    ValueType& value(){return v;}
    void value(const ValueType& v){this->v=v;}
    constexpr ID id(){return ID();}
};

template<typename PacketBufferIterator, typename ID, typename T, std::size_t n, typename U>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const Attribute<ID,T,n,U>& q)
{
  return it << q.id().value() << q.unit() << ValueDescription(q.value()) << q.scale();
}

template<typename PacketBufferIterator, typename ID, typename T, std::size_t n>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const Attribute<ID,T,n, None>& q)
{
  return it << q.id().value() << ValueDescription(q.value());
}

template<typename PacketBufferConstIterator, typename ID, typename T, std::size_t n, typename U>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, Attribute<ID,T,n,U>& q)
{
  typename ID::Type id;
  Unit u;
  ValueDescription d;
  it >> id >> u >> d;
  if(q.id().value()!=id)
    throw IDException();
  if(d!=q.value())
    throw TypeException();
  if(u!=q.unit())
    throw UnitException();
  return it >> q.scale();
}

template<typename PacketBufferConstIterator, typename ID, typename T, std::size_t n>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it, Attribute<ID,T,n, None>& q)
{
  typename ID::Type id;
  ValueDescription d;
  it >> id >> d;
  if(q.id().value()!=id)
    throw IDException();
  if(d!=q.value())
    throw TypeException();
  return it;
}

template<typename ID, typename T, std::size_t n, typename U>
std::ostream& operator<<(std::ostream& o, const Attribute<ID,T,n,U>& a)
{
  return o << a.id().name() << ": " << a.value() << " * " << a.scale() << " " << a.unit();
}

template<typename ID, typename T, std::size_t n>
std::ostream& operator<<(std::ostream& o, const Attribute<ID,T,n, None>& a)
{
  return o << a.id().name() << ": " << a.value();
}
