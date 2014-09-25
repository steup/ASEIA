#include <MetaFactory.h>

#include <iostream>

using namespace std;

int main() {
  cout << "Start" << endl;
  MetaFactory& m = MetaFactory::instance();
  MetaValue test = m.create(ValueType(Value<float, 4, false>()));
  cout << "MetaEvent(float, 4, false): " << test << endl;
  cout << "Kill" << endl;
  m.~MetaFactory();
  cout << "End" << endl;
  return 0;
}
