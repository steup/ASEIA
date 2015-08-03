#include <ValueElement.h>
#include <IO.h>
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <Eigen/Core>
#pragma GCC diagnostic pop


#include <cmath>

#include <iostream>

using std::cout;
using std::endl;

namespace Eigen {
	template<typename T, bool U>
	struct NumTraits<ValueElement<T, U>> : public NumTraits<T> {};
}

template<typename T, bool U>
const ValueElement<T, U>& conj(const ValueElement<T, U>& x)  { return x; }
template<typename T, bool U>
const ValueElement<T, U>& real(const ValueElement<T, U>& x)  { return x; }
template<typename T, bool U>
ValueElement<T, U> imag(const ValueElement<T, U>&)    { return 0.; }
template<typename T, bool U>
ValueElement<T, U> abs(const ValueElement<T, U>&  x)  { return ValueElement<T, U>({{std::abs(x.value()), x.uncertainty()}}); }
template<typename T, bool U>
ValueElement<T, U> abs2(const ValueElement<T, U>& x)  { return x*x; } 

using Vector3f = Eigen::Matrix<ValueElement<double>, 3, 1>;
using Matrix33f = Eigen::Matrix<ValueElement<double>, 3, 3>;
using Vector3i = Eigen::Matrix<ValueElement<int>, 3, 1>;
using Matrix33i = Eigen::Matrix<ValueElement<int>, 3, 3>;


int main(){
	Vector3f fa, fb;
	Matrix33f fm;
	Vector3i ia, ib;
	Matrix33i im;
	fa << 1, 2, 3;
	fb << 4, 5, 6;
	fm  << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	ia << 1, 2, 3;
	ib << 4, 5, 6;
	im  << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	cout << "fa    = " << fa << endl;
	cout << "fb    = " << fb << endl;
	cout << "fa+fb = " << (fa+fb) << endl;
	cout << "fma   = " << (fm*fa) << endl;
	cout << "ia    = " << ia << endl;
	cout << "ib    = " << ib << endl;
	cout << "ia+ib = " << (ia+ib) << endl;
	cout << "ima   = " << (im*ia) << endl;
	return 0;
}
