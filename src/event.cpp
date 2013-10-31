#include <iostream>

#include <sebs.h>
#include <packetBuffer.h>

using namespace std;

using TestAttr = Attribute<AttributeID::unspecified, bool, 1>;
using NewScheme = SensorBaseEventScheme<>::extent<TestAttr>;

int main()
{
  SensorBaseEventScheme<> sbes;
  using Event = SensorBaseEventScheme<>::EventType;
  Event sbe = sbes.createEvent();
  Event check = sbes.createEvent();
  DynamicPacketBuffer buffer;

  auto& posValue=sbe.attribute<AttributeID::position>().value();
  posValue={{{1.0f,0.0f},{2.0f,0.0f},{3.0f,0.0f}}};

  auto& timeValue=sbe.attribute<AttributeID::time>().value();
  timeValue={{{10.0f,0.0001f}}};

  sbe.attribute<AttributeID::publisherID>().value({{1337}});

  sbe.attribute<AttributeID::validity>().value({{0.1}});

  cout << sbe;

  auto insertionIter = std::back_inserter(buffer);
  insertionIter << sbe;
  insertionIter << sbes;

  cout << buffer << endl;

  auto parseIter = buffer.cbegin();
  parseIter >> check;

  cout << check;

  return 0;
}
