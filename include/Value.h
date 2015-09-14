#pragma once

#define EIGEN_MATRIX_PLUGIN <ValueBase.h>

#include <ValueElement.h>
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <Eigen/Core>
#pragma GCC diagnostic pop


#include <cmath>

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

template<typename T, int32_t rows, bool useUncertainty=true>
using Value = Eigen::Matrix<ValueElement<T, useUncertainty>, rows, 1>;

template<typename PB, typename T, int32_t n, bool u>
Serializer<PB>& operator<<(Serializer<PB>& s, const Value<T,n,u>& value){
  for(const auto& v : value)
		s << v;
  return s;
}

template<typename PB, typename T, int32_t n, bool u>
DeSerializer<PB>& operator>>(DeSerializer<PB>& s, Value<T,n,u>& value){
  for(auto& v : value)
    s >> v;
  return s;
}
