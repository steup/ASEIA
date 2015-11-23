#include <MetaValueImplementation.h>
#include <IO.h>

#include <iostream>

using namespace std;

int main() {
  {
		cout << "Size of metavalue: " << sizeof(MetaValueImplementation<int>) << endl;
    MetaFactory& instance = MetaFactory::instance();
    MetaValue a = instance.create({{-5.1},  {2.2}, {3.3}});
		MetaValue b = instance.create({{1}, {6}, {-3}}, id::type::Int32::value());
		MetaValue b2 = instance.convert(ValueType(a), b);
		MetaValue a2 = instance.convert(ValueType(b), a);
    cout << "a: " << ValueType(a) << a << endl;
    cout << "b: " << ValueType(b) << b << endl;
		cout << "a+b: " << ValueType(a+b) << (a+b) << endl;
		cout << "b2: " << ValueType(b2) << b2 << endl;
		cout << "a+b2: " << ValueType(a+b2) << (a+b2) << endl;
		cout << "a2: " << ValueType(a2) << a2 << endl;
		cout << "a2+b: " << ValueType(a2+b) << (a2+b) << endl;
  }
  return 0;
}
