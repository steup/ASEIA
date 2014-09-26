#include <MetaFactory.h>

#include <iostream>

using namespace std;

int main() {
  MetaFactory& m = MetaFactory::instance();
  MetaValue test = m.create(ValueType(Value<float, 4, false>()));
  cout << "MetaValue(float, 4, false): " << test << endl;
  return 0;
}
