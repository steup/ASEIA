#include <IO.h>

#include <ValueType.h>
#include <UnitType.h>
#include <ScaleType.h>
#include <AttributeType.h>
#include <EventType.h>
#include <FormatID.h>
#include <MetaAttribute.h>

using std::ostream;
using std::endl;

ostream& operator<<(ostream& o, const ValueType& t) {
  o << (t.hasUncertainty()?"uncertain ":"") << id::type::name(t.typeId()) << "[" << t.rows();
  if(t.cols()!=1)
    o << ", " << t.cols();
  return o << "]";
}

ostream& operator<<(ostream& o, const ScaleType& t) {
  if(t.num()!=1 && t.denom()!=1)
    o << t.num() << "/" << t.denom() << " ";
  if(t.num()!=1)
    o << t.num() << " ";
  return o << " (" << t.reference() << ")";
}

ostream& operator<<(ostream& o, const UnitType& t) {
  uint8_t id=0;
  bool neg = false;
  for(int8_t dim : t) {
    if(dim < 0)
      neg = true;
    else
      switch(dim){
        case(0): break;
        case(1): o << id::unit::shortName(id);
                 break;
        default: o << id::unit::shortName(id) << "^" << dim;
      }
    id++;
  }
  if (neg) {
    id=0;
    o << "/(";
    for(int8_t dim : t) {
      if(dim < 0) {
        if(dim == -1)
          o << id::unit::shortName(id);
        else
          o << id::unit::shortName(id) << "^" << -dim;
      }
      id++;
    }
    o << ")";
  }
  return o;
}

ostream& operator<<(ostream& o, const AttributeType& t) {
  return o << id::attribute::name(t.id()) << ": " << t.value() << " " << t.scale() << t.unit();
}

ostream& operator<<(ostream& o, const EventType& t) {
  o << "EventType: " << endl;
  for(const auto& aT : t)
    o << "\t" << aT << endl;
  return o;
}

//ostream& operator<<(ostream& o, const FormatID& id) {
//  return o << id.node() << (id.direction()==FormatID::Direction::publisher?"P":"S") << id.nr();
//}

