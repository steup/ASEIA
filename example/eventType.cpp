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
  using PublisherIDValueType = Value<uint16_t, 1, 1, false>;
  using ValidityValueType    = Value<uint8_t, 1, 1, false>;
  using PositionScale        = std::ratio<1,1000>;
  using ValidityScale        = std::ratio<1,100>;
};

using DistanceAttribute    = Attribute<Distance, Value<int16_t, 1>, Meter, std::ratio<1,1000>>;
using DistanceEvent        = BaseEvent<EventConfig>::append<DistanceAttribute>::type;

void printV(const vector<uint8_t>& v){
	cout << "Binary: ";
	auto formatFlags = cout.flags();
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
	cout.flags(formatFlags);
	cout << endl;
}

int main(){
  DistanceEvent e;
  EventType out(e);

  vector<uint8_t> buffer;

  cout << "Size: " << out.size() << endl;

  cout << "out: " << out <<  endl;

  Serializer<decltype(back_inserter(buffer))> s(back_inserter(buffer));
  s << out;

  printV(buffer);

  EventType in;

  DeSerializer<decltype(buffer.cbegin())> d(buffer.cbegin(), buffer.cend());
  d >> in;

  cout << "in: " << in << endl;

  cout << (in==out?"Correct":"Error: not identical") << endl;
  return 0;
}
