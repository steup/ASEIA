#include <MetaValueImplementation.h>

#include <iostream>

using namespace std;

using UInt = MetaValueImplementation<uint64_t>;

int main() {
  MetaFactory& instance = MetaFactory::instance();
  MetaValue test = instance.create(id::type::UInt64::value(), 1, false);
  cout << "MetaValue(uint64_t, 1, false): " << test << endl;
  return 0;
}
