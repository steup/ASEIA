#include <iostream>

#include <sensorEventBaseScheme.h>
#include <packetBuffer.h>

using namespace std;
using DistanceAttribute   = Attribute<AttributeID::Distance, uint16_t, 1, boost::units::si::length>;
using DistanceEventScheme = SensorBaseEventScheme<>::extent<DistanceAttribute>;

int main()
{
  DistanceEventScheme des;
  using Event = DistanceEventScheme::EventType;
  Event de = des.createEvent();
  Event check = des.createEvent();
  DynamicPacketBuffer buffer;

  auto& posValue=de.attribute<AttributeID::Position>().value();
  posValue={{{1.0f,0.0f},{2.0f,0.0f},{3.0f,0.0f}}};

  auto& timeValue=de.attribute<AttributeID::Time>().value();
  timeValue={{{10.0f,0.0001f}}};

  de.attribute<AttributeID::PublisherID>().value({{1337}});

  de.attribute<AttributeID::Validity>().value({{0.1}});

  de.attribute<AttributeID::Distance>().value({{{128, 5}}});

  cout << de;

  auto insertionIter = std::back_inserter(buffer);
  insertionIter << de;
//  insertionIter << sbes;

  cout << buffer << endl;

  auto parseIter = buffer.cbegin();
  parseIter >> check;

  cout << check;

  return 0;
}
