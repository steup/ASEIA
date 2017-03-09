#pragma once

#include <IDIO.h>

#include <Event.h>
#include <EventID.h>
#include <FormatID.h>
#include <Attribute.h>
#include <ValueElement.h>
#include <Value.h>
#include <Foreach.h>

#include <ostream>
#include <ratio>

class ValueType;
class AttributeType;
class UnitType;
class ScaleType;
class EventType;
class MetaValueBaseImplementation;
class MetaAttribute;

template<typename T, bool u>
std::ostream& operator<<(std::ostream& o, const ValueElement<T, u>& e)
{ 
  if(u)
    o << "[" << e.value() << " +- " << e.uncertainty() << "]";
  else
    o << e.value();
  return o;
}

template<bool u>
std::ostream& operator<<(std::ostream& o, const ValueElement<bool, u>& e) {
  if(u)
    o << "[" << (e.value()?"true ":"false") << " +- " << (e.uncertainty()?"true ":"false") << "]";
  else
    o << (e.value()?"true ":"false");
  return o;
}

namespace {
  struct UnitOutputHelper {
    std::ostream& o;
    id::unit::ID i = 0;
    UnitOutputHelper(std::ostream& o) : o(o) {}
    template<typename Value>
    void operator()(Value& v) {
      if(Value::value) {
        o << id::unit::shortName(i);
        if(Value::value != 1)
          o << "^" << (int16_t)Value::value;
      }
      i++;
    }
  };
}

template<typename Dimensions>
std::ostream& operator<<(std::ostream& o, const Unit<Dimensions>& u) {
  UnitOutputHelper h(o);
  foreach<Dimensions>(h);
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const std::milli& r) {
  return o << "m";
}

inline std::ostream& operator<<(std::ostream& o, const std::nano& r) {
  return o << "n";
}

inline std::ostream& operator<<(std::ostream& o, const std::kilo& r) {
  return o << "k";
}

inline std::ostream& operator<<(std::ostream& o, const std::mega& r) {
  return o << "M";
}

inline std::ostream& operator<<(std::ostream& o, const std::giga& r) {
  return o << "G";
}

inline std::ostream& operator<<(std::ostream& o, const std::ratio<1,1>& r) {
  return o;
}

template<std::intmax_t N, std::intmax_t D>
std::ostream& operator<<(std::ostream& o, const std::ratio<N,D>& r) {
  return o << r.num << "/" << r.den << " ";
}

template<typename ratio, uint32_t ref>
std::ostream& operator<<(std::ostream& o, const Scale<ratio, ref>& s) {
  return o << s.ratio() << "(" << s.reference() << ")";  
}

inline std::ostream& operator<<(std::ostream& o, const Dimensionless& u) {
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const uint8_t& v) {
  return o << (uint16_t)v;
}

inline std::ostream& operator<<(std::ostream& o, const int8_t& v) {
  return o << (int16_t)v;
}

template<typename ID, typename V, typename S, typename U>
std::ostream& operator<<(std::ostream& o, const Attribute<ID,V,S,U>& a)
{
  return o << id::attribute::name(ID().value()) << ": " << a.value() << " " << a.scale() << "" << a.unit();
}

namespace {
    template<typename O, typename E>
    struct OutputEvent{
      const E& e;
      O& o;
      OutputEvent(O& o, const E& e) : e(e), o(o){}
      template<typename Attr>
      void operator()(Attr& a){
        o << "\t" << e.attribute(a.id()) << std::endl;
      }
    };
}

template<Endianess end, typename... Attributes>
std::ostream& operator<<(std::ostream& o, const Event<end, Attributes...>& e){
  using ThisEvent = Event<end, Attributes...>;
  o << "Event:" << std::endl;
  OutputEvent<std::ostream, ThisEvent> out(o,e);
  foreach<typename ThisEvent::AttributeList>(out);
  return o;
}

std::ostream& operator<<(std::ostream& o, const ValueType& t);

std::ostream& operator<<(std::ostream& o, const ScaleType& t);

std::ostream& operator<<(std::ostream& o, const UnitType& t);

std::ostream& operator<<(std::ostream& o, const AttributeType& t);

std::ostream& operator<<(std::ostream& o, const EventType& t);

inline std::ostream& operator<<(std::ostream& o, EventID eID) {
  return o << eID.value();
}

inline std::ostream& operator<<(std::ostream& o, FormatID fID) {
  return o << fID.value();
}

template<typename T, int32_t R, int32_t C, bool U>
std::ostream& operator<<(std::ostream& o, const Value<T, R, C, U>& v) {
  for(std::size_t r = 0; r<v.rows(); r++) {
    o << "(";
    for(std::size_t c = 0; c<v.cols(); c++)
      o << "\t" << v(r, c);
    o << ")";
  }
  return o;
}
