#include <MetaValueImplementation.h>

#include <iostream>

using namespace std;

int main() {
  {
    MetaFactory& instance = MetaFactory::instance();
    MetaValue a = instance.create({{-5.1},  {2.2}, {3.3}});
		MetaValue b = instance.create({{1}, {6}, {-3}}, id::type::Int32::value());
		MetaValue b2 = instance.convert(ValueType(a), b);
    cout << "a: " << ValueType(a) << a << endl;
    cout << "b: " << ValueType(b) << b << endl;
		cout << "a+b: " << ValueType(a+b) << (a+b) << endl;
		cout << "b2: " << ValueType(b2) << b2 << endl;
		cout << "a+b2: " << ValueType(a+b2) << (a+b2) << endl;
  }
  return 0;
}
