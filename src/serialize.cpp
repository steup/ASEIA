#include <Serializer.h>
#include <Value.h>
#include <BaseEvent.h>
#include <ID.h>
#include <IO.h>

#include <vector>
#include <iostream>
#include <iomanip>
#include <iterator>

using namespace std;
using namespace boost::units;

using V=vector<uint8_t>;
using S=Serializer<V::iterator>;

template<typename T>
void podOut(){
  V v(sizeof(T));
  S s(v.begin(), v.end());
  T value = sizeof(T);
  s << value;
  cout << typename id::type::t2Type<T>::type().name() << " " << value << ": ";
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << endl << "Error: " << (s.error()?"true":"false") << dec << endl;
}

template<typename T, bool uncertainty>
void valueElementOut(){
  using VE=ValueElement<T, uncertainty>;
  V v(VE::size());
  S s(v.begin(), v.end());
  VE value = {sizeof(T), 1};
  s << value;
  cout << typename id::type::t2Type<T>::type().name() << " " << value << ": ";
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << endl << "Error: " << (s.error()?"true":"false") << dec << endl;
}

void valueOut(){
  using Val=Value<float, 3, true>;
  V v(Val::size());
  S s(v.begin(), v.end());
  Val value = {{1.1, 0.2}, {0,0.2}, {4.2,1.1}};
  s << value;
  cout << "float " << value << ": ";
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << endl << "Error: " << (s.error()?"true":"false") << dec << endl;
}

void attributeOut(){
  using Val=Value<double, 3, true>;
  using Attr=Attribute<id::attribute::Position, Val, si::length, std::ratio<1,1000>>;
  V v(Attr::size()-10);
  S s(v.begin(), v.end());
  Attr value = {{1.1, 0.2}, {0,0.2}, {4.2,1.1}};
  s << value;
  cout << "double " << value << ": ";
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << endl << "Error: " << (s.error()?"true":"false") << dec << endl;
}

void baseEventOut(){
  V v(BaseEvent<>::size());
  S s(v.begin(), v.end());
  BaseEvent<> e;
  e.attribute(id::attribute::PublisherID()).value() = {{1337}};
  e.attribute(id::attribute::Validity()).value() = {{0.9}};
  s << e;
  cout << e << ": ";
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << byte << " ";
  cout << endl << "Error: " << (s.error()?"true":"false") << dec << endl;
  cout << "size: " << BaseEvent<>::size() << " - " << v.size() << endl;
}

int main(){
  podOut<uint8_t>();
  podOut<uint16_t>();
  podOut<uint32_t>();
  podOut<uint64_t>();
  podOut<int8_t>();
  podOut<int16_t>();
  podOut<int32_t>();
  podOut<int64_t>();
  podOut<float>();
  podOut<double>();
  valueElementOut<uint8_t, true>();
  valueElementOut<uint16_t, true>();
  valueElementOut<uint32_t, true>();
  valueElementOut<uint64_t, true>();
  valueElementOut<int8_t, true>();
  valueElementOut<int16_t, true>();
  valueElementOut<int32_t, true>();
  valueElementOut<int64_t, true>();
  valueElementOut<float, true>();
  valueElementOut<double, true>();
  valueOut();
  attributeOut();
  baseEventOut();
  return 0;
}
