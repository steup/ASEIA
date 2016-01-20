#include <BaseEvent.h>
#include <IO.h>

#include <iostream>
#include <chrono>

using namespace id::attribute;
using namespace std;
using namespace chrono;

struct EventConfig : public BaseConfig
{
  using PositionValueType    = Value<int16_t, 2>;
  using PublisherIDValueType = Value<uint16_t, 1, 1, false>;
  using PositionScale        = std::ratio<1,1000>;
	using TimeScale            = std::ratio<1,1000000000>;
};

using DistanceAttribute    = Attribute<Distance, Value<int16_t, 1>, Meter, std::ratio<1,1000>>;
using DistanceEvent        = BaseEvent<EventConfig>::append<DistanceAttribute>::type;

int main()
{
  DistanceEvent e;
	high_resolution_clock clock;
	using TimeScale = DistanceEvent::findAttribute<Time>::type::ScaleType;
	duration<int64_t, TimeScale> now = clock.now().time_since_epoch();
  e.attribute(Position()).value()    = {{{1500, 100}},
																				{{3200, 200}}};
  e.attribute(Time()).value()        = {{{now.count(),1LL}}};
  e.attribute(PublisherID()).value() = {{{1337}}};
  e.attribute(Distance()).value()    = {{{1000, 300}}};

  cout << e;
  cout << "Size: " << DistanceEvent::size() << endl;
  return 0;
}
