#include <MetaFactory.h>
#include <MetaValue.h>
#include <IO.h>
#include <iostream>

using namespace std;

using VU3 = Value<unsigned int, 3, true>;
using VLU3 = Value<uint64_t, 3, true>;
using VLI3 = Value<int64_t, 3, true>;
using VF3 = Value<float, 3, true>;

using namespace id::type;

int main() {
	MetaFactory& instance = MetaFactory::instance();

	ValueType vu3 (UInt32::value(), 3, 1, true);
	ValueType vlu3( Int32::value(), 3, 1, true);
	ValueType vli3( Int64::value(), 3, 1, true);
	ValueType vf3 ( Float::value(), 3, 1, true);

	MetaValue test = instance.create({{{-5.1, 0.0},  {2.2, 0.0}, {3.3, 0.0}}});
	cout << "test : " << (ValueType)test << ":" << endl << test << endl;

	MetaValue test2(instance.convert(vu3, test));
	cout << "test2: " << (ValueType)test2 << ":" << endl << test2 << endl;

	MetaValue test3 = instance.convert(vli3, test);
	cout << "test2: " << (ValueType)test3 << ":" << endl << test3 << endl;

	MetaValue test4 = instance.convert(vf3, test);
	cout << "test2: " << (ValueType)test4 << ":" << endl << test4 << endl;

	MetaValue test5 = instance.convert(vlu3, test2);
	cout << "test5: " << (ValueType)test5 << ":" << endl << test5 << endl;
  return 0;
}
