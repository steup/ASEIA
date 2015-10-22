#include <EventType.h>
#include <BaseEvent.h>
#include <IO.h>

#include <iostream>
#include <iomanip>

#include <vector>

using namespace id::attribute;
using namespace std;

struct EventConfig : public BaseConfig
{
  using PositionValueType    = Value<int16_t, 2>;
  using PublisherIDValueType = Value<uint16_t, 1, false>;
  using ValidityValueType    = Value<uint8_t, 1, false>;
  using PositionScale        = std::ratio<1,1000>;
  using ValidityScale        = std::ratio<1,100>;
};

using DistanceAttribute    = Attribute<Distance, Value<int16_t, 1>, Meter, std::ratio<1,1000>>;
using DistanceEvent        = BaseEvent<EventConfig>::append<DistanceAttribute>::type;

int main(){
  DistanceEvent e;
  EventType out(e);

  vector<uint8_t> buffer;
  buffer.resize(out.size());

  cout << "Size: " << out.size() << endl;

  cout << "out: " << out <<  endl;

  Serializer<decltype(back_inserter(buffer))> s(back_inserter(buffer));
  s << out;

  cout << "Binarystream: " << hex << setfill('0') << setw(2);
  for(auto b : buffer)
    cout << "0x" << b << " ";
  cout << dec << endl;

  EventType in;

  DeSerializer<decltype(buffer.cbegin())> d(buffer.cbegin(), buffer.cend());
  d >> in;

  cout << "in: " << in << endl;

  cout << (in==out?"Correct":"Error: not identical") << endl;
  return 0;
}
