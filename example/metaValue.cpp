#include <iostream>
#include <MetaValueImplementation.h>

using namespace std;

ostream& operator<<(ostream& o, const MetaValueBaseImplementation& v){
	v.print(o);
	return o;
}

int main(){
	MetaValueImplementation<int32_t> int32_0({{-1,3}, {2,1}, {3,2}});
	MetaValueImplementation<int32_t> int32_1({{-3, 2}, {-2,1}, {-1, 0}});
	MetaValueImplementation<double> double_({{-1,3}, {2,1}, {3,2}});
	cout << "a[" << int32_0.rows() << ", " << int32_0.cols() << "]: " << endl << int32_0 << endl;
	cout << "b[" << int32_1.rows() << ", " << int32_1.cols() << "]: " << endl << int32_1 << endl;
	cout << "c[" << double_.rows() << ", " << double_.cols() << "]: " << endl << double_ << endl;
	cout << "a+=b: " << endl << (int32_0+=int32_1) << endl;
	return 0;
}
