#include <iostream>
#include <MetaValueImplementation.h>

using namespace std;

ostream& operator<<(ostream& o, const MetaValueBaseImplementation& v){
	v.print(o);
	return o;
}

int main(){
	MetaValueImplementation<int32_t> int32_0({{{-1,3}, {2,1}, {3,2}}});
	MetaValueImplementation<int32_t> int32_1({{{-3, 2}, {-2,1}, {-1, 0}}});
	MetaValueImplementation<double> double_({{{-1,3}, {2,1}, {3,2}}});
	cout << "a: uncertain int32_t[1,3]:" << int32_0 << endl;
	cout << "b: uncertain int32_t[1,3]:" << int32_1 << endl;
	cout << "c: uncertain double [1,3]:" << double_ << endl;
	cout << "a+=b: " << (int32_0+=int32_1) << endl;
	return 0;
}
