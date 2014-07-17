#include <DeSerializer.h>
#include <ValueElement.h>
#include <BaseEvent.h>
#include <ID.h>
#include <IO.h>

#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace boost::units;

using V=vector<uint8_t>;
using I=vector<uint8_t>::const_iterator;
using D=DeSerializer<I>;

template<typename T>
void podIn(){
  V v({8, 0, 0, 0, 0, 0, 0, 0});
  D d(v.cbegin(), v.end());
  T value;
  d >> value;
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << dec;
  cout << id::type::name(id::type::id(T())) << " " << value << ": " << endl;
}

template<>
void podIn<float>(){
  V v({0xcd, 0xcc, 0x8c, 0x3f});
  D d(v.cbegin(),v.end());
  float value;
  d >> value;
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << dec;
  cout << id::type::name(id::type::id(float())) << " " << value << ": " << endl;
}

template<>
void podIn<double>(){
  V v({0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f});
  D d(v.cbegin(),v.end());
  double value;
  d >> value;
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << dec;
  cout << id::type::name(id::type::id(double())) << " " << value << ": " << endl;
}

void valueElementIn(){
  V v({0xcd, 0xcc, 0x8c, 0x3f, 0xcd, 0xcc, 0x8c, 0x3f});
  D d(v.cbegin(),v.end());
  ValueElement<float, true> value;
  d >> value;
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << dec << "float " << value << ": " << endl;
}

void valueIn(){
  V v({0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f});
  D d(v.cbegin(),v.end());
  Value<double, 3, true> value;;
  d >> value;
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << dec << "float " << value << ": " << endl;
}

void attributeIn(){
  using Val=Value<double, 3, true>;
  V v({0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f,0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xf1, 0x3f});
  D d(v.cbegin(),v.end());
  Attribute<id::attribute::Position, Val, si::length, std::ratio<1,1000>> value;
  d >> value;
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << dec << "double " << value << ": " << endl;
}

void baseEventIn(){
 V v({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x3f});
  D d(v.cbegin(),v.end());
  BaseEvent<> e;
  d >> e;
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << endl << dec << e;
}

int main(){
  podIn<uint8_t>();
  podIn<uint16_t>();
  podIn<uint32_t>();
  podIn<uint64_t>();
  podIn<int8_t>();
  podIn<int16_t>();
  podIn<int32_t>();
  podIn<int64_t>();
  podIn<float>();
  podIn<double>();
  valueElementIn();
  valueIn();
  attributeIn();
  baseEventIn();
  return 0;
}

