#include <Event.h>
#include <Attribute.h>
#include <Value.h>
#include <IO.h>

#include <iostream>
#include <ctime>

using namespace id::attribute;
using namespace boost::units;

using std::cout;
using std::endl;

using Vector1_16u          = Value<int16_t,  1 , true>;
using Vector1_u8           = Value<uint8_t,  1, false>;
using Vector1_64u          = Value<int64_t,  1, true>;
using UUID                 = Value<uint64_t, 1, false>;
using Vector2_16u          = Value<int16_t,  2, true>;

using DistanceAttribute    = Attribute<Distance, Vector1_16u, si::length, std::ratio<1,1000>>;
using PositionAttribute    = Attribute<Position, Vector2_16u, si::length, std::ratio<1,1000>>;
using TimeAttribute        = Attribute<Time, Vector1_64u, si::time>;
using PublisherIDAttribute = Attribute<PublisherID, UUID>;
using ValidityAttribute    = Attribute<Validity, Vector1_u8>;

using DistanceEvent       = Event<hostEndianess, DistanceAttribute, PositionAttribute, TimeAttribute, PublisherIDAttribute, ValidityAttribute>;



int main()
{
  DistanceEvent e;

  e.attribute(Position()).value()    = {{1500, 100}, {3200,200}};
  e.attribute(Time()).value()        = {{std::time(nullptr),1}};
  e.attribute(Distance()).value()    = {{1000,300}};
  e.attribute(PublisherID()).value() = {{1337}};
  e.attribute(Validity()).value()    = {{(uint16_t)UINT8_MAX*9/10}};

  cout << e;
  cout << "Size: " << DistanceEvent::size() << endl;
  return 0;
}
