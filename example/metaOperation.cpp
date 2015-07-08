#include <MetaValueImplementation.h>

#include <iostream>

using namespace std;

int main() {
  {
    MetaFactory& instance = MetaFactory::instance();
    MetaValue a = instance.create({{-5.1},  {2.2}, {3.3}});
		MetaValue b = instance.create({{1}, {6}, {-3}});
    cout << "a: " << ValueType(a) << a << endl;
    cout << "b: " << ValueType(b) << b << endl;
		cout << "a+b: " << ValueType(a+b) << (a+b) << endl;
  }
  return 0;
}
