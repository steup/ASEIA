#include <ValueElement.h>
#include <IO.h>
#include <Eigen/Core>

#include <iostream>

using std::cout;
using std::endl;

namespace Eigen {
	template<typename T, bool U>
	struct NumTraits<ValueElement<T, U>> : public NumTraits<T> {}
}

template<typename T, bool U>
const ValueElement<T, U>& conj(const ValueElement<T, U>& x)  { return x; }
template<typename T, bool U>
const ValueElement<T, U>& real(const ValueElement<T, U>& x)  { return x; }
template<typename T, bool U>
ValueElement<T, U> imag(const ValueElement<T, U>&)    { return 0.; }
template<typename T, bool U>
ValueElement<T, U> abs(const ValueElement<T, U>&  x)  { return fabs(x); }
template<typename T, bool U>
ValueElement<T, U> abs2(const ValueElement<T, U>& x)  { return x*x; } 

using Vector3f = Eigen::Matrix<ValueElement<double, false>, 3, 1>;
using Matrix33f = Eigen::Matrix<ValueElement<double, false>, 3, 3>;


int main(){
	Vector3f a, b, ab;
	Matrix33f m;
	a << 1, 2, 3;
	b << 4, 5, 6;
	m  << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	ab = a+b;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "aa = " << ab << endl;
	cout << "ma = " << m*a << endl;
	return 0;
}
