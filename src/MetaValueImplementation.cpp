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

template<typename T>
using MVI = MetaValueImplementation<T>;

using Bool = MetaValueImplementation<bool>;

template<typename T>
Ptr MetaValueImplementation<T>::factoryCreate(std::size_t rows, std::size_t cols, bool u) {
	return Ptr(new MetaValueImplementation<T>(rows, cols));
}

		
template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(std::size_t rows, std::size_t cols) : mData(rows, cols) {

}

template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(typename Base::InitType values) : mData(values) { 

}

template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(const typename MVI<T>::Base& data) : mData(data) { 

}

template<typename T>
Interface& MetaValueImplementation<T>::operator=( Interface&& movee) {
 mData = move(reinterpret_cast<MVI<T>&&>(movee).mData);
 return *this;
}

template<typename T>
Ptr MetaValueImplementation<T>::copy() const {
	return Ptr(new MetaValueImplementation(*this), Interface::Deleter());
}

template<typename T>
Data MetaValueImplementation<T>::get( Attributes a ) const {
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
		case(Attributes::HasUncertainty): res.hasUncertainty = true;
																			break;
		default:													return Interface::get(a);
	}
	return res;
}
template<typename T>
ValueElement<double, true> MetaValueImplementation<T>::get(size_t row, size_t col) const {
  if(row < (size_t)mData.rows() && col < (size_t)mData.cols())
    return mData(row, col);
  else
    return ValueElement<double, true>();
}

template<typename T>
bool MetaValueImplementation<T>::set(size_t row, size_t col, const ValueElement<double, true>& v) {
  if(row < (size_t)mData.rows() && col < (size_t)mData.cols()) {
    mData(row, col) = v;
    return true;
  }
  return false;
}

template<typename T>
bool MetaValueImplementation<T>::set(Attributes a, Data d) {
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

template<typename T>
Interface& MetaValueImplementation<T>::unaryOp( UnaryOp op)  {
	switch(op) {
		case(UnaryOp::Neg): /** \todo Implement negation **/
												break;
		case(UnaryOp::Not): /** \todo Implement not **/
												break;
		default           : return Interface::unaryOp(op);
	}
	return *this;
}

template<typename T>
Interface& MetaValueImplementation<T>::binaryOp( BinaryOp op, const Interface& b)  {
	switch(op) {
      case(BinaryOp::Add): mData += reinterpret_cast<const Impl&>(b).mData;
													 break;
			case(BinaryOp::Sub): /** \todo Implement subtraction **/
													 break;
			case(BinaryOp::Mul): /** \todo Implement multiplication **/
													 break;
			case(BinaryOp::Div): /** \todo Implement division **/
													 break;
			default            : return Interface::binaryOp(op, b);
	}
	return *this;
}

template<typename T>
Ptr MetaValueImplementation<T>::binaryConstOp( BinaryConstOp op, const Interface& b ) const {
	Ptr ptr;
	switch(op) {
   case(BinaryConstOp::Equal)      : ptr = Ptr(new Bool(mData == reinterpret_cast<const Impl&>(b).mData));
	 							      							 break;
   case(BinaryConstOp::NotEqual)   : ptr = Ptr(new Bool(mData != reinterpret_cast<const Impl&>(b).mData));
	 								    							 break;
   case(BinaryConstOp::ApproxEqual): /** \todo Implement approximate equal **/
	 								    							 break;
	 case(BinaryConstOp::Smaller)    : /** \todo Implement smaller comparision **/
	 								    							 break;
	 case(BinaryConstOp::Greater)    : /** \todo Implement greater comparision **/
	 								    							 break;
	 case(BinaryConstOp::SmallEqual) : /** \todo Implement smaller or equal comparision **/
	 								    							 break;
	 case(BinaryConstOp::GreatEqual) : /** \todo Implement greater or equal comparision **/
	 								    							 break;
	 default          							 : return Interface::binaryConstOp(op, b);
	}
	return ptr;
}

template<typename T>
Interface& MetaValueImplementation<T>::scale(const MetaScale& scale) {
	mData  *= scale.num();
	mData  /= scale.denom();
	return *this;
}

template<typename T>
ostream& MetaValueImplementation<T>::print(ostream& o) const {
	return o << mData;
}

template class MetaValueImplementation<uint8_t>;
template class MetaValueImplementation<uint16_t>;
template class MetaValueImplementation<uint32_t>;
template class MetaValueImplementation<uint64_t>;
template class MetaValueImplementation<int8_t>;
template class MetaValueImplementation<int16_t>;
template class MetaValueImplementation<int32_t>;
template class MetaValueImplementation<int64_t>;
template class MetaValueImplementation<float>;
template class MetaValueImplementation<double>;
template class MetaValueImplementation<bool>;
