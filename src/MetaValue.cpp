#include <MetaValue.h>
#include <ValueType.h>

using namespace std;
using MVB           = MetaValueBaseImplementation;
using Data          = MVB::Data;
using Attributes    = MVB::Attributes;
using UnaryOp       = MVB::UnaryOp;
using UnaryConstOp  = MVB::UnaryConstOp;
using BinaryOp      = MVB::BinaryOp;
using BinaryConstOp = MVB::BinaryConstOp;
using Ptr           = MetaValue::Ptr;
using ID            = MetaValue::ID;

MetaValue::MetaValue() : mImpl(MVB::sInstance.copy()) {}

MetaValue::MetaValue(MetaValue::Ptr&& ptr){
		mImpl = move(ptr);
}

MetaValue::MetaValue(const MetaValue& copy) : mImpl(MetaValueBaseImplementation::sInstance.copy()){
	mImpl = copy.mImpl->copy();
}

MetaValue::MetaValue(MetaValue&& copy) : mImpl(move(copy.mImpl)) { }

bool MetaValue::resize(std::size_t rows, std::size_t cols) {
	Data d;
	d.rows=rows;
	if(!mImpl->set(Attributes::Rows, d))
		return false;
	d.cols=cols;
	return mImpl->set(Attributes::Cols, d);
}
    
bool MetaValue::hasUncertainy(bool u) {
	Data d;
	d.hasUncertainty = u;
	return mImpl->set(Attributes::HasUncertainty, d);
}

MetaValue& MetaValue::operator=(const MetaValue& copy) {
	mImpl = copy.mImpl->copy();
	return *this;
}

MetaValue& MetaValue::operator=(MetaValue&& copy) {
  mImpl = move(copy.mImpl);
	return *this;
}

ValueElement<double, true> MetaValue::get(std::size_t row, std::size_t col) const {
  return mImpl->get(row, col);
}
bool MetaValue::set(std::size_t row, std::size_t col, const ValueElement<double, true>& v) {
  return mImpl->set(row, col, v);
}
    
MetaValue MetaValue::operator+(const MetaValue& b) const {
  if(compatible(b)) {
    MetaValue temp(*this);
    temp.mImpl->binaryOp(BinaryOp::Add, *b.mImpl);
    return temp;
  } else
    return MetaValue();
}

MetaValue MetaValue::operator==(const MetaValue& b) const {
  if(compatible(b))
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::Equal, *b.mImpl));
  else
    return MetaValue();
}

MetaValue MetaValue::operator!=(const MetaValue& b) const {
  if(compatible(b))
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::NotEqual, *b.mImpl));
  else
    return MetaValue();
}

MetaValue& MetaValue::operator*=(const MetaScale& b) { 
	mImpl->scale(b);
	return *this;
}

MetaValue& MetaValue::operator/=(const MetaScale& b) { 
	mImpl->scale(b, true);
	return *this;
}

MetaValue MetaValue::prod() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Prod));
}

MetaValue MetaValue::sum() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Sum));
}

size_t MetaValue::size() const {
	return mImpl->get(Attributes::Size).size;
}

size_t MetaValue::cols() const {
	return mImpl->get(Attributes::Cols).cols;
}

size_t MetaValue::rows() const {
	return mImpl->get(Attributes::Rows).rows;
}

ID MetaValue::typeId() const {
	return mImpl->get(Attributes::TypeID).typeID;
}

bool MetaValue::hasUncertainty() const {
	return mImpl->get(Attributes::HasUncertainty).hasUncertainty;
}

bool MetaValue::valid() const {
  return typeId() != id::type::Base::value();
}

bool MetaValue::compatible(const MetaValue& b) const {
  return valid() && typeId() == b.typeId() && cols() == b.cols() && rows() == b.rows();
}
MetaValue::operator ValueType() const {
  return ValueType(typeId(), rows(), cols(), hasUncertainty());
}

ostream& MetaValue::print(ostream& o) const {
	return mImpl->print(o);
}
