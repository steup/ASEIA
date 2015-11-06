#include <ValueType.h>
#include <IO.h>

#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

using namespace std;

int main(){
  array<uint8_t, ValueType::size()> buffer;

  Value<float, 3, 1, true> v3f;
  ValueType out(v3f);

  cout << "ValueType out: " << out << endl;

  Serializer<decltype(buffer.data())> s(buffer.data());
  s << out;

  cout << "Binarystream: " << hex << setfill('0') << setw(2);
  for(auto b : buffer)
    cout << "0x" << b << " ";
  cout << endl;

  ValueType in;

  DeSerializer<decltype(buffer.cbegin())> d(buffer.cbegin(), buffer.cend());
  d >> in;

  cout << (in==out?"Correct":"Error: not identical") << endl;

  return 0;
}

