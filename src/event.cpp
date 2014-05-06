#include <BaseEvent.h>
#include <IO.h>

#include <iostream>
#include <ctime>

using namespace id::attribute;
using namespace boost::units;

using std::cout;
using std::endl;

struct EventConfig : public BaseConfig
{
  using PositionValueType    = Value<int16_t, 2>;
  using PublisherIDValueType = Value<uint16_t, 1, false>;
  using ValidityValueType    = Value<uint8_t, 1, false>;
  using PositionScale        = std::ratio<1,1000>;
  using ValidityScale        = std::ratio<1,100>;
};

using DistanceAttribute    = Attribute<Distance, Value<int16_t, 1>, si::length, std::ratio<1,1000>>;
using DistanceEvent        = BaseEvent<EventConfig>::append<DistanceAttribute>::type;

int main()
{
  DistanceEvent e;

  e.attribute(Position()).value()    = {{1500, 100}, {3200,200}};
  e.attribute(Time()).value()        = {{(unsigned long)std::time(nullptr),1}};
  e.attribute(PublisherID()).value() = {{1337}};
  e.attribute(Validity()).value()    = {{90}};
  e.attribute(Distance()).value()    = {{1000,300}};

  cout << e;
  cout << "Size: " << DistanceEvent::size() << endl;
  return 0;
}
