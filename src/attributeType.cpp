#include <MetaAttributeType.h>
#include <IO.h>

#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

#include <boost/units/systems/si/length.hpp>

using boost::units::si::length;
using boost::units::si::seconds;
using namespace std;

using Vector3iu=Value<float,3,true>;

using Position=Attribute<id::attribute::Position, Vector3iu, boost::units::si::length, std::ratio<1,10>>;

int main(){
  array<uint8_t, MetaAttributeType::size()> buffer;

  Position pos;
  MetaAttributeType out(pos);

  cout << "AttributeType out: " << out <<  endl;

  Serializer<decltype(buffer.begin())> s(buffer.begin(), buffer.end());
  s << out;

  cout << "Binarystream: " << hex << setfill('0') << setw(2);
  for(auto b : buffer)
    cout << "0x" << b << " ";
  cout << endl;

  MetaAttributeType in;

  DeSerializer<decltype(buffer.cbegin())> d(buffer.cbegin(), buffer.cend());
  d >> in;

  cout << (in==out?"Correct":"Error: not identical") << endl;

  return 0;
}

