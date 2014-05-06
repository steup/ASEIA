#pragma once

#include <Event.h>
#include <Attribute.h>
#include <ValueElement.h>
#include <Value.h>

#include <ostream>
#include <ratio>

#include <boost/units/io.hpp>
#include <boost/mpl/for_each.hpp>

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

template<typename ID, typename V, typename S, typename U>
std::ostream& operator<<(std::ostream& o, const Attribute<ID,V,S,U>& a)
{
  return o << a.id().name() << ": " << a.value() << " " << a.scale() << "" << a.unit();
}

std::ostream& operator<<(std::ostream& o, const std::milli& r){
  return o << "m";
}

std::ostream& operator<<(std::ostream& o, const std::nano& r){
  return o << "n";
}

std::ostream& operator<<(std::ostream& o, const std::kilo& r){
  return o << "k";
}

std::ostream& operator<<(std::ostream& o, const std::mega& r){
  return o << "M";
}

std::ostream& operator<<(std::ostream& o, const std::giga& r){
  return o << "G";
}

std::ostream& operator<<(std::ostream& o, const std::ratio<1,100>& r){
  return o << "c";
}

std::ostream& operator<<(std::ostream& o, const std::ratio<1,1>& r){
  return o;
}

template<std::intmax_t N, std::intmax_t D>
std::ostream& operator<<(std::ostream& o, const std::ratio<N,D>& r){
  return o << N << "/" << D;
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
