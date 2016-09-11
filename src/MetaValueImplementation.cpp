#include <MetaValueImplementation.h>
#include <MetaScale.h>
#include <IO.h>

#include <memory>
#include <ostream>

using namespace std;
using Interface = MetaValueBaseImplementation::Interface;
using Ptr = MetaValueBaseImplementation::Ptr;
using Data = MetaValueBaseImplementation::Data;
using UnaryOp = MetaValueBaseImplementation::UnaryOp;
using BinaryOp = MetaValueBaseImplementation::BinaryOp;
using BinaryConstOp = MetaValueBaseImplementation::BinaryConstOp;

template<typename T, bool U>
using MVI = MetaValueImplementation<T, U>;

using Bool = MetaValueImplementation<bool, true>;

template<typename T, bool U>
const uint8_t* MetaValueImplementation<T, U>::begin() const{
  return reinterpret_cast<const uint8_t*>(mData.data());
}

template<typename T, bool U>
const uint8_t* MetaValueImplementation<T, U>::end() const{
  return reinterpret_cast<const uint8_t*>(mData.data()+mData.rows()*mData.cols());

}

template<typename T, bool U>
uint8_t* MetaValueImplementation<T, U>::begin(){
  return reinterpret_cast<uint8_t*>(mData.data());

}

template<typename T, bool U>
uint8_t* MetaValueImplementation<T, U>::end(){
  return reinterpret_cast<uint8_t*>(mData.data()+mData.rows()*mData.cols());

}

template<typename T, bool U>
Ptr MetaValueImplementation<T, U>::factoryCreate(std::size_t rows, std::size_t cols, bool u) {
	return Ptr(new MetaValueImplementation<T, U>(rows, cols));
}

		
template<typename T, bool U>
MetaValueImplementation<T, U>::MetaValueImplementation(std::size_t rows, std::size_t cols) : mData(rows, cols) {

}

template<typename T, bool U>
MetaValueImplementation<T, U>::MetaValueImplementation(typename Base::InitType values) : mData(values) { 

}

template<typename T, bool U>
MetaValueImplementation<T, U>::MetaValueImplementation(const typename MVI<T, U>::Base& data) : mData(data) { 

}

template<typename T, bool U>
Interface& MetaValueImplementation<T, U>::operator=( Interface&& movee) {
 mData = move(reinterpret_cast<MVI<T, U>&&>(movee).mData);
 return *this;
}

template<typename T, bool U>
Ptr MetaValueImplementation<T, U>::copy() const {
	return Ptr(new MetaValueImplementation(*this));
}

template<typename T, bool U>
Data MetaValueImplementation<T, U>::get( Attributes a ) const {
	Data res;
	switch(a) {
  	case(Attributes::TypeID): 				res.typeID = id::type::id(T());
																			break;
		case(Attributes::Cols): 					res.cols = mData.cols();
																			break;
		case(Attributes::Rows): 					res.cols = mData.rows();
																			break;
		case(Attributes::Size): 					res.size = Elem::size()*mData.cols()*mData.rows();
																			break;
		case(Attributes::HasUncertainty): res.hasUncertainty = U;
																			break;
		default:													return Interface::get(a);
	}
	return res;
}
template<typename T, bool U>
ValueElement<double, true> MetaValueImplementation<T, U>::get(size_t row, size_t col) const {
  if(row < (size_t)mData.rows() && col < (size_t)mData.cols())
    return mData(row, col);
  else
    return ValueElement<double, true>();
}

template<typename T, bool U>
bool MetaValueImplementation<T, U>::set(size_t row, size_t col, const ValueElement<double, true>& v) {
  if(row < (size_t)mData.rows() && col < (size_t)mData.cols()) {
    mData(row, col) = v;
    return true;
  }
  return false;
}

template<typename T, bool U>
bool MetaValueImplementation<T, U>::set(Attributes a, Data d) {
	switch(a) {
  	case(Attributes::TypeID): 				return false;
		case(Attributes::Cols): 					mData.conservativeResize(Eigen::NoChange_t() ,d.cols);
																			return true;
		case(Attributes::Rows): 					mData.conservativeResize(d.rows, Eigen::NoChange_t());
																			return true;
		case(Attributes::Size): 					return false;
		case(Attributes::HasUncertainty): return false;
		default:													return Interface::set(a, d);
	}
}

template<typename T, bool U>
Interface& MetaValueImplementation<T, U>::unaryOp( UnaryOp op)  {
	switch(op) {
		case(UnaryOp::Neg): mData = -mData;
												break;
		default           : return Interface::unaryOp(op);
	}
	return *this;
}

template<typename T, bool U>
Ptr MetaValueImplementation<T, U>::unaryConstOp( UnaryConstOp op) const {
  Ptr ptr;
	switch(op) {
		case(UnaryConstOp::Prod): {
      MetaValueImplementation<T, U> temp = *this;
      temp.mData.resize(1,1);
      temp.mData(0)=mData.prod();
      ptr = temp.copy(); }
			break;
		case(UnaryConstOp::Sum): {
      MetaValueImplementation<T, U> temp = *this;
      temp.mData.resize(1,1);
      temp.mData(0)=mData.sum();
      ptr = temp.copy(); }
			break;
		default           : return Interface::unaryConstOp(op);
	}
	return ptr;
}

template<typename T, bool U>
Interface& MetaValueImplementation<T, U>::binaryOp( BinaryOp op, const Interface& b)  {
	switch(op) {
      case(BinaryOp::Add): mData += reinterpret_cast<const Impl&>(b).mData;
													 break;
			case(BinaryOp::Sub): mData -= reinterpret_cast<const Impl&>(b).mData;
													 break;
			case(BinaryOp::Mul): /** \todo handle scalar multiplication and eWise product; */
													 break;
			case(BinaryOp::Div): if(b.get(Interface::Attributes::Rows).rows == 1 &&
                              b.get(Interface::Attributes::Cols).cols == 1)
                            mData /= reinterpret_cast<const Impl&>(b).mData(0,0);
                           break;
			default            : return Interface::binaryOp(op, b);
	}
	return *this;
}

template<typename T>
Ptr MetaValueImplementation<T, true>::binaryConstOp( BinaryConstOp op, const Interface& b ) const {
  const Impl& typedB = reinterpret_cast<const Impl&>(b);
	switch(op) {
   case(BinaryConstOp::Equal)      : return Ptr(new Bool(mData == typedB.mData));
   case(BinaryConstOp::NotEqual)   : return Ptr(new Bool(mData != typedB.mData));
   case(BinaryConstOp::ApproxEqual): return Ptr(new Bool(approxEqual(mData, typedB.mData)));
	 case(BinaryConstOp::Smaller)    : return Ptr(new Bool(mData < typedB.mData));
	 case(BinaryConstOp::Greater)    : return Ptr(new Bool(mData > typedB.mData));
	 case(BinaryConstOp::SmallEqual) : return Ptr(new Bool(mData <= typedB.mData));
	 case(BinaryConstOp::GreatEqual) : return Ptr(new Bool(mData >= typedB.mData));
	 default          							 : return Interface::binaryConstOp(op, b);
	}
}

template<typename T>
Ptr MetaValueImplementation<T, false>::binaryConstOp( BinaryConstOp op, const Interface& b ) const {
  const Impl& typedB = reinterpret_cast<const Impl&>(b);
  MVI<bool, false> res;
	switch(op) {
   case(BinaryConstOp::Equal)      : res = mData == typedB.mData;
   case(BinaryConstOp::NotEqual)   : res = mData != typedB.mData;
   case(BinaryConstOp::ApproxEqual): res = approxEqual(mData, typedB.mData);
	 case(BinaryConstOp::Smaller)    : res = mData < typedB.mData;
	 case(BinaryConstOp::Greater)    : res = mData > typedB.mData;
	 case(BinaryConstOp::SmallEqual) : res = mData <= typedB.mData;
	 case(BinaryConstOp::GreatEqual) : res = mData >= typedB.mData;
	 default          							 : return Interface::binaryConstOp(op, b);
	}
  return Ptr(new Bool(res.template cast<Bool>());
}

template<typename T, bool U>
Interface& MetaValueImplementation<T, U>::scale(const MetaScale& scale, bool invert) {
  if(invert) {
	  mData  *= scale.denom();
	  mData  /= scale.num();
  } else {
	  mData  *= scale.num();
	  mData  /= scale.denom();
  }
	return *this;
}

template<typename T, bool U>
ostream& MetaValueImplementation<T, U>::print(ostream& o) const {
	return o << mData;
}

template class MetaValueImplementation<uint8_t, true>;
template class MetaValueImplementation<uint16_t, true>;
template class MetaValueImplementation<uint32_t, true>;
template class MetaValueImplementation<uint64_t, true>;
template class MetaValueImplementation<int8_t, true>;
template class MetaValueImplementation<int16_t, true>;
template class MetaValueImplementation<int32_t, true>;
template class MetaValueImplementation<int64_t, true>;
template class MetaValueImplementation<float, true>;
template class MetaValueImplementation<double, true>;
template class MetaValueImplementation<bool, true>;
template class MetaValueImplementation<uint8_t, false>;
template class MetaValueImplementation<uint16_t, false>;
template class MetaValueImplementation<uint32_t, false>;
template class MetaValueImplementation<uint64_t, false>;
template class MetaValueImplementation<int8_t, false>;
template class MetaValueImplementation<int16_t, false>;
template class MetaValueImplementation<int32_t, false>;
template class MetaValueImplementation<int64_t, false>;
template class MetaValueImplementation<float, false>;
template class MetaValueImplementation<double, false>;
template class MetaValueImplementation<bool, false>;
