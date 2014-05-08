#include <Serializer.h>
#include <ID.h>

#include <vector>
#include <iostream>
#include <iomanip>
#include <iterator>

using namespace std;

template<typename T>
void out(){
  using V=vector<uint8_t>;
  using I=back_insert_iterator<V>;
  using S=Serializer<I>;
  V v;
  S s(back_inserter(v));
  T value = sizeof(T);
  s << value;
  cout << typename id::type::getTypeID<T>::type().name() << " " << (uint16_t)value << ": ";
  for(auto byte : v)
      cout << hex << setw(2) << setfill('0') << (uint16_t)byte << " ";
  cout << endl;
}

int main(){
  out<uint8_t>();
  out<uint16_t>();
  out<uint32_t>();
  out<uint64_t>();
  out<int8_t>();
  out<int16_t>();
  out<int32_t>();
  out<int64_t>();
  out<float>();
  out<double>();
  return 0;
}
