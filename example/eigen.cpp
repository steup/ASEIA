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
	struct NumTraits<ValueElement<T, U>> : public NumTraits<T> {
		enum {
			ReadCost = U?2:1 * NumTraits<T>::ReadCost,
			AddCost  = U?3:1 * NumTraits<T>::AddCost,
		  MulCost = U?5:1 * NumTraits<T>::MulCost
		};
	};
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
template<typename T, bool U>
ValueElement<T, U> ceil(const ValueElement<T, U>& x)  { return ValueElement<T, U>({{ceil(x.value()), x.uncertainty()}}); } 
template<typename T, bool U>
ValueElement<T, U> log(const ValueElement<T, U>& x)  { return ValueElement<T, U>({{log(x.value()), log(x.uncertainty())}}); } 

using Vector3f = Eigen::Matrix<ValueElement<double>, 3, 1>;
using Matrix33f = Eigen::Matrix<ValueElement<double>, 3, 3>;
using Vector3i = Eigen::Matrix<ValueElement<int>, 3, 1>;
using Matrix33i = Eigen::Matrix<ValueElement<int>, 3, 3>;
using Vectori = Eigen::Matrix<ValueElement<int>, Eigen::Dynamic, 1>;
using Matrixi = Eigen::Matrix<ValueElement<int>, Eigen::Dynamic, Eigen::Dynamic>;

int main(){
	Vector3f fa, fb;
	Matrix33f fm;
	Vector3i ia, ib;
	Matrix33i im;
	Vectori ida(3), idb(4);
	Matrixi idm(3,3);
	fa << 1, 2, 3;
	fb << 4, 5, 6;
	fm  << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	ia << 1, 2, 3;
	ib << 4, 5, 6;
	im  << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	ida << 1, 2, 3;
	idb << 4, 5, 6, 0;
	idm  << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	cout << "fa      = " << fa << endl;
	cout << "fb      = " << fb << endl;
	cout << "fa+fb   = " << (fa+fb) << endl;
	cout << "fma     = " << (fm*fa) << endl;
	cout << "ia      = " << ia << endl;
	cout << "ib      = " << ib << endl;
	cout << "ia+ib   = " << (ia+ib) << endl;
	cout << "ima     = " << (im*ia) << endl;
	cout << "ida     = " << ida << endl;
	cout << "idb     = " << idb << endl;
	cout << "ida+idb = " << (ida+idb) << endl;
	cout << "idma    = " << (idm*ida) << endl;
	return 0;
}
