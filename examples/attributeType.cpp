#include <AttributeType.h>
#include <IO.h>

#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

using namespace std;

using Vector3iu=Value<float,3,1,true>;

using Position=Attribute<id::attribute::Position, Vector3iu, Meter, Scale<std::deci>>;

int main(){
  array<uint8_t, AttributeType::size()> buffer;

  Position pos;
  AttributeType out = (AttributeType)pos;

  cout << "AttributeType out: " << out <<  endl;

  Serializer<decltype(buffer.data())> s(buffer.data());
  s << out;

  cout << "Binarystream: " << hex << setfill('0') << setw(2);
  for(auto b : buffer)
    cout << "0x" << b << " ";
  cout << dec << endl;

  AttributeType in;

  DeSerializer<decltype(buffer.cbegin())> d(buffer.cbegin(), buffer.cend());
  d >> in;

  cout << "AttributeType  in: " << out <<  endl;

  cout << (in==out?"Correct":"Error: not identical") << endl;

  return 0;
}

