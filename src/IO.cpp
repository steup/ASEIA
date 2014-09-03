#include <IO.h>

#include <ValueType.h>
#include <UnitType.h>
#include <ScaleType.h>
#include <AttributeType.h>
#include <EventType.h>
#include <FormatID.h>
#include <MetaValueElement.h>

using std::ostream;
using std::endl;

ostream& operator<<(ostream& o, const ValueType& t) {
  return o << (t.hasUncertainty()?"uncertain ":"") << id::type::name(t.typeId()) << "[" << t.n() << "]";
}

ostream& operator<<(ostream& o, const ScaleType& t) {
  if(t.num()==1 && t.denom()==1)
    return o;
  if(t.denom()!=1)
    return o << t.num() << "/" << t.denom() << " ";
  else
    return o << t.num() << " ";
}

ostream& operator<<(ostream& o, const UnitType& t) {
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

ostream& operator<<(ostream& o, const AttributeType& t) {
  return o << id::attribute::name(t.attributeId()) << ": " << t.value() << " " << t.scale() << t.unit();
}

ostream& operator<<(ostream& o, const EventType& t) {
  o << "EventType: " << endl;
  for(const auto& aT : t)
    o << "\t" << aT << endl;
  return o;
}

ostream& operator<<(ostream& o, const FormatID& id) {
  return o << id.node() << ":" << (id.direction()==FormatID::Direction::publisher?"P":"S") << id.nr();
}

ostream& operator<<(ostream& o, const MetaValueElement& mve) {
  o << "(" << id::type::name(mve.id()) << ")";
  switch(mve.id()){
    case(id::type::UInt8::value()):  
    case(id::type::UInt16::value()): 
    case(id::type::UInt32::value()): 
    case(id::type::UInt64::value()): o << mve._uint;
                                     break;

    case(id::type::Int8::value())  : 
    case(id::type::Int16::value()) : 
    case(id::type::Int32::value()) : 
    case(id::type::Int64::value()) : o << mve._int;
                                     break;

    case(id::type::Float::value()) : o << mve._float;
                                     break;

    case(id::type::Double::value()): o << mve._double;
                                     break;
  }
  return o;
}
