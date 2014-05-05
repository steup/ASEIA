
#include <Attribute.h>
#include <IO.h>
#include <Value.h>
#include <ID.h>

#include <iostream>

#include <boost/units/systems/si/length.hpp>

using boost::units::si::length;
using boost::units::si::seconds;
using namespace std;

using Vector3iu=Value<float,3,true>;

using Position=Attribute<id::attribute::Position, Vector3iu, boost::units::si::length, std::ratio<1,10>>;

int main()
{
  Position pos{{{0.0f,0.0f}, {0.0f, 0.0f}, {1.0f,0.0f}}};

  cout << pos << endl; 
  return 0;
}
