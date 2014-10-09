#include <MetaValueImplementation.h>

#include <iostream>

using namespace std;

using UInt = MetaValueImplementation<uint64_t>;

int main() {
  MetaValue test = UInt::create(10);
  cout << "MetaValue(uint64_t, 1, false): " << test << endl;
  return 0;
}
