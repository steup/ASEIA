#include <iostream>

#include <attribute.h>

#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/time.hpp>

using boost::units::si::length;
using boost::units::si::seconds;
using namespace std;

template<typename T, std::size_t n>
using Position=Attribute<AttributeID::position, length, T, n, true>;

int main()
{
  using F3Position=Position<float, 3>;

  F3Position pos;
  F3Position deserialize;
  pos.value()={{{0.0f,0.0f}, {0.0f, 0.0f}, {1.0f,0.0f}}};
//  pos.value()={{0.0f, 0.0f, 1.0f}};
  pos.scale()=1.0f/1000;

  DynamicPacketBuffer b;

  cout << pos << endl;

  auto it=back_inserter(b);
  it << pos << pos.value();

  cout << b << endl;

  auto deIt=b.cbegin();
  deIt >> deserialize >> deserialize.value();
  cout << deserialize << endl;
  
  return 0;
}
