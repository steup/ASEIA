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
using std::vector;
using std::hex;
using std::to_string;

ostream& operator<<(ostream& o, const ValueType& t) {
  o << (t.hasUncertainty()?"uncertain ":"") << id::type::name(t.typeId()) << "[" << t.rows();
  if(t.cols()!=1)
    o << ", " << t.cols();
  return o << "]";
}

ostream& operator<<(ostream& o, const ScaleType& t) {
 o << "(" << t.reference() << ") ";
 bool isNumeric = false;
  switch(t.num()) {
    case(1): break;
    case(100): o << "h"; break;
    case(1000): o << "K"; break;
    case(1000000): o << "M"; break;
    case(1000000000): o << "G"; break;
    default: o << t.num(); isNumeric = true;
  }
  switch(t.denom()) {
    case(1): break;
    case(10): o << "d"; break;
    case(100): o << "c"; break;
    case(1000): o << "m"; break;
    case(1000000): o << "u"; break;
    case(1000000000): o << "n"; break;
    default: o << (!isNumeric?to_string(t.num()):"") << "/"<< t.denom();
  }
  return o;
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
  return o << id::attribute::name(t.id()) << "(" << t.id() << "): " << t.value() << " " << t.scale() << t.unit();
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

ostream& operator<<(ostream& o, const vector<uint8_t>& v) {
  auto flags = o.flags();
  o << "[" << hex;
  size_t i=0;
  for(uint8_t elem : v)
    o << "0x" << elem << (++i==v.size()?"":" ,");
  o.setf(flags);
  return o << "]";
}
