#pragma once

#include <IDIO.h>

#include <Event.h>
#include <Attribute.h>
#include <ValueElement.h>
#include <Value.h>
#include <ValueType.h>
#include <AttributeType.h>
#include <UnitType.h>
#include <ScaleType.h>
#include <EventType.h>

#include <ostream>
#include <ratio>

#include <boost/units/io.hpp>
#include <boost/mpl/for_each.hpp>

std::ostream& operator<<(std::ostream& o, const uint8_t& v);

std::ostream& operator<<(std::ostream& o, const int8_t& v);

template<typename T, bool u>
std::ostream& operator<<(std::ostream& o, const ValueElement<T, u>& e)
{
  o << e.value();
  if(u)
    o << "+-" << e.uncertainty();
  return o;
}

template<bool u>
std::ostream& operator<<(std::ostream& o, const ValueElement<uint8_t, u>& e)
{
  o << (uint16_t)e.value();
  if(u)
    o << "+-" << e.uncertainty();
  return o;
}

template<bool u>
std::ostream& operator<<(std::ostream& o, const ValueElement<uint16_t, u>& e)
{
  o << (uint16_t)e.value();
  if(u)
    o << "+-" << e.uncertainty();
  return o;
}

template<typename T, std::size_t n, bool useUncertainty>
std::ostream& operator<<(std::ostream& o, const Value<T,n,useUncertainty>& v)
{
  o << "(";
  bool isFirst=true;
  for(auto e : v)
    if(isFirst){
      isFirst=false;
      o << e;
    }
    else
      o << ", " << e;
  return o << ")";
}

std::ostream& operator<<(std::ostream& o, const boost::units::si::dimensionless& u);

template<typename ID, typename V, typename S, typename U>
std::ostream& operator<<(std::ostream& o, const Attribute<ID,V,S,U>& a)
{
  return o << id::attribute::name(ID().value()) << ": " << a.value() << " " << a.scale() << "" << a.unit();
}

std::ostream& operator<<(std::ostream& o, const std::milli& r);

std::ostream& operator<<(std::ostream& o, const std::nano& r);

std::ostream& operator<<(std::ostream& o, const std::kilo& r);

std::ostream& operator<<(std::ostream& o, const std::mega& r);

std::ostream& operator<<(std::ostream& o, const std::giga& r);

std::ostream& operator<<(std::ostream& o, const std::ratio<1,1>& r);

template<std::intmax_t N, std::intmax_t D>
std::ostream& operator<<(std::ostream& o, const std::ratio<N,D>& r){
  return o << N << "/" << D << " ";
}

namespace helpers{
    template<typename O, typename E>
    struct OutputEvent{
      const E& e;
      O& o;
      OutputEvent(O& o, const E& e) : e(e), o(o){}
      template<typename Attr>
      void operator()(Attr a){
        o << "\t" << e.attribute(a.id()) << std::endl;
      };
    };
}

template<Endianess end, typename... Attributes>
std::ostream& operator<<(std::ostream& o, const Event<end, Attributes...>& e){
  using ThisEvent = Event<end, Attributes...>;
  o << "Event:" << std::endl;
  helpers::OutputEvent<std::ostream, ThisEvent> out(o,e);
  boost::mpl::for_each<typename ThisEvent::AttributeList>(out);
  return o;
}

std::ostream& operator<<(std::ostream& o, const ValueType& t);

std::ostream& operator<<(std::ostream& o, const ScaleType& t);

std::ostream& operator<<(std::ostream& o, const UnitType& t);

std::ostream& operator<<(std::ostream& o, const AttributeType& t);

std::ostream& operator<<(std::ostream& o, const EventType& t);
