#include <IO.h>

std::ostream& operator<<(std::ostream& o, const ValueType& t) {
  return o << (t.hasUncertainty()?"uncertain ":"") << id::type::name(t.typeId()) << "[" << t.n() << "]";
}

std::ostream& operator<<(std::ostream& o, const ScaleType& t) {
  if(t.num()==1 && t.denom()==1)
    return o;
  if(t.denom()!=1)
    return o << t.num() << "/" << t.denom() << " ";
  else
    return o << t.num() << " ";
}

std::ostream& operator<<(std::ostream& o, const UnitType& t) {
  unsigned int i=0;
  for(const auto& v : t)
    switch(v){
      case(0): i++;
               continue;
      case(1): o << id::unit::shortName(i++);
               continue;
      default: o << id::unit::shortName(i++) << "^" << v;
               continue;
    }
  return o;
}

std::ostream& operator<<(std::ostream& o, const AttributeType& t) {
  return o << id::attribute::name(t.attributeId()) << ": " << t.value() << " " << t.scale() << t.unit();
}

std::ostream& operator<<(std::ostream& o, const EventType& t) {
  o << "EventType: " << std::endl;
  for(const auto& aT : t)
    o << "\t" << aT << std::endl;
  return o;
}

std::ostream& operator<<(std::ostream& o, const std::milli& r) {
  return o << "m";
}

std::ostream& operator<<(std::ostream& o, const std::nano& r) {
  return o << "n";
}

std::ostream& operator<<(std::ostream& o, const std::kilo& r) {
  return o << "k";
}

std::ostream& operator<<(std::ostream& o, const std::mega& r) {
  return o << "M";
}

std::ostream& operator<<(std::ostream& o, const std::giga& r) {
  return o << "G";
}

std::ostream& operator<<(std::ostream& o, const std::ratio<1,1>& r) {
  return o;
}

std::ostream& operator<<(std::ostream& o, const boost::units::si::dimensionless& u) {
  return o;
}

std::ostream& operator<<(std::ostream& o, const uint8_t& v) {
  return o << (uint16_t)v;
}

std::ostream& operator<<(std::ostream& o, const int8_t& v) {
  return o << (int16_t)v;
}
