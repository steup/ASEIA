#include <iostream>
#include <MetaValueImplementation.h>

using namespace std;
using Interface = MetaValueBaseImplementation;

ostream& operator<<(ostream& o, const MetaValueBaseImplementation& v){
	v.print(o);
	return o;
}

int main(){
	Interface::BinaryOp op = Interface::BinaryOp::Add;
	MetaValueImplementation<int32_t,  true> int32_0({{{-1,3}, {2,1}, {3,2}}});
	MetaValueImplementation<int32_t,  true> int32_1({{{-3, 2}, {-2,1}, {-1, 0}}});
	MetaValueImplementation<double,  true> double_({{{-1,3}, {2,1}, {3,2}}});
	cout << "a: uncertain int32_t[1,3]:" << int32_0 << endl;
	cout << "b: uncertain int32_t[1,3]:" << int32_1 << endl;
	cout << "c: uncertain double [1,3]:" << double_ << endl;
	cout << "a+=b: " << (int32_0.binaryOp(op, int32_1)) << endl;
	return 0;
}
