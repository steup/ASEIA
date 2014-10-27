#include <MetaValueImplementation.h>

#include <iostream>

using namespace std;

using UInt = MetaValueImplementation<uint64_t>;

int main() {
  MetaFactory& instance = MetaFactory::instance();
  MetaValue test = instance.create(id::type::UInt16::value(), 1, false);
  test = MetaValueImplementation<uint16_t>({300});
  MetaValue test2 = instance.convert(ValueType(id::type::UInt8::value(), 1, false), test);
  cout << (ValueType)test << ": " << test << endl;
  cout << (ValueType)test2 << ": " << test2 << endl;
  return 0;
}
