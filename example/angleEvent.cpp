#include <BaseEvent.h>
#include <IO.h>

#include <iostream>
#include <ctime>

using namespace id::attribute;

struct AngleEventConfig : public BaseConfig
{
  using PositionValueType    = Value<int16_t, 2>;
  using PublisherIDValueType = Value<uint16_t, 1, 1, false>;
  using ValidityValueType    = Value<uint8_t, 1, 1, false>;
  using PositionScale        = std::ratio<1, 100>;
  using ValidityScale        = std::ratio<1, 100 >;
    
};

using AngleAttribute = Attribute<Angle, Value<int16_t, 1>, Radian,  std::ratio<1,100>>;
using AngleEvent = BaseEvent<AngleEventConfig>::append<AngleAttribute>::type;

int main()
{
  AngleEvent e;
  
  e.attribute(Position()).value()    = {{100, 20}, {200,20}};
  e.attribute(Time()).value()        = {{(unsigned long)std::time(nullptr), 1}};
  e.attribute(PublisherID()).value() = {{55}};
  e.attribute(Validity()).value()    = {{80}};
  e.attribute(Angle()).value()       = {{314}};
  
  std::cout << e;  
  std::cout << "Size: " << AngleEvent::size() << std::endl;

  return 0;
}
