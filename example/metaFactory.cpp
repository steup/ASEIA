#include <MetaValueImplementation.h>

#include <iostream>

using namespace std;

using VU3 = Value<unsigned int, 3, true>;
using VI3 = Value<int64_t, 3, true>;
using VF3 = Value<float, 3, true>;

int main() {
  MetaFactory& instance = MetaFactory::instance();
  MetaValue test = instance.create({{-5.1},  {2.2}, {3.3}});
  cout << "test : " << ValueType(test) << test << endl;
  MetaValue test2 = instance.convert(VU3(), test);
  cout << "test2: " << ValueType(test2) << test2 << endl;
  MetaValue test3 = instance.convert(VI3(), test);
  cout << "test2: " << ValueType(test3) << test3 << endl;
  MetaValue test4 = instance.convert(VF3(), test);
  cout << "test2: " << ValueType(test4) << test4 << endl;
  return 0;
}
