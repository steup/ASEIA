#include <MetaValue.h>
#include <ValueType.h>
#include <MetaFactory.h>
#include <ID.h>

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

MetaValue::MetaValue() : mImpl(MVB::instance()) {}

MetaValue::MetaValue(double v, id::type::ID typeID)
  : MetaValue({{{v}}}, typeID, 1, 1, false)
{}

/*MetaValue::MetaValue(ElemInitType l, id::type::ID typeIDid=id::type::Double::value(), bool u=false)
  : MetaValue({{l}}, id, 1, 1, u)
{}

MetaValue::MetaValue(RowInitType l, id::type::ID typeIDid=id::type::Double::value(),
          size_t rows=0, bool u=false)
  : MetaValue({l}, id, rows, 1, u)
{}*/

MetaValue::MetaValue(::id::type::ID typeID, size_t rows, size_t cols, bool u)
  : MetaValue(MetaFactory::instance().create(typeID, rows, cols, u))
{}

MetaValue::MetaValue(InitType l, ::id::type::ID typeID,
                     size_t rows, size_t cols, bool u)
  : MetaValue(MetaFactory::instance().create(l, typeID, rows, cols, u))
{}

MetaValue::MetaValue(MetaValue::Ptr&& ptr){
		mImpl = move(ptr);
}

MetaValue::MetaValue(const MetaValue& copy) {
  if(this!=&copy)
	  mImpl = copy.mImpl->copy();
}

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
  if(this != &copy)
	  mImpl = copy.mImpl->copy();
	return *this;
}

ValueElement<double, true> MetaValue::get(std::size_t row, std::size_t col) const {
  return mImpl->get(row, col);
}

bool MetaValue::set(std::size_t row, std::size_t col, ElemInitType elem) {
  return mImpl->set(row, col, elem);
}

bool MetaValue::set(std::size_t row, std::size_t col, double elem) {
  return mImpl->set(row, col, {elem});
}

MetaValue& MetaValue::operator+=(const MetaValue& b) {
  if( !compatible(b) || !mImpl->binaryOp(BinaryOp::Add, *b.mImpl) )
    mImpl = MVB::instance();
  return *this;
}

MetaValue& MetaValue::operator-=(const MetaValue& b) {
  if(!compatible(b) || !mImpl->binaryOp(BinaryOp::Sub, *b.mImpl))
    mImpl = MVB::instance();
  return *this;
}
MetaValue& MetaValue::operator*=(const MetaValue& b) {
  if( !compatible(b) || !mImpl->binaryOp(BinaryOp::Mul, *b.mImpl))
    mImpl = MVB::instance();
  return *this;
}
MetaValue& MetaValue::operator/=(const MetaValue& b) {
  if( !compatible(b) || !mImpl->binaryOp(BinaryOp::Div, *b.mImpl))
    mImpl = MVB::instance();
  return *this;
}

MetaValue MetaValue::operator==(const MetaValue& b) const {
  if(compatible(b) && rows() == b.rows() && cols() == b.cols())
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::Equal, *b.mImpl));
  else
    return MetaValue();
}

MetaValue MetaValue::operator!=(const MetaValue& b) const {
  if(compatible(b) && rows() == b.rows() && cols() == b.cols())
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::NotEqual, *b.mImpl));
  else
    return MetaValue();
}

MetaValue MetaValue::operator<=(const MetaValue& b) const {
  if(compatible(b) && rows() == b.rows() && cols() == b.cols())
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::SmallEqual, *b.mImpl));
  else
    return MetaValue();
}

MetaValue MetaValue::operator>=(const MetaValue& b) const {
  if(compatible(b) && rows() == b.rows() && cols() == b.cols())
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::GreatEqual, *b.mImpl));
  else
    return MetaValue();
}

MetaValue MetaValue::operator<(const MetaValue& b) const {
  if(compatible(b) && rows() == b.rows() && cols() == b.cols())
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::Smaller, *b.mImpl));
  else
    return MetaValue();
}

MetaValue MetaValue::operator>(const MetaValue& b) const {
  if(compatible(b) && rows() == b.rows() && cols() == b.cols())
    return MetaValue(mImpl->binaryConstOp(BinaryConstOp::Greater, *b.mImpl));
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
MetaValue MetaValue::block(size_t i, size_t j, size_t numI, size_t numJ) const {
  return MetaValue(move(mImpl->block(i, j, numI, numJ)));
}

MetaValue& MetaValue::block(size_t i, size_t j, const MetaValue& v) {
  mImpl->block(i, j, move(v.mImpl->copy()));
  return *this;
}

MetaValue& MetaValue::block(size_t i, size_t j, MetaValue&& v) {
  mImpl->block(i, j, move(v.mImpl));
  return *this;
}

MetaValue MetaValue::col(size_t col) const {
  return MetaValue(move(mImpl->col(col)));
}

MetaValue MetaValue::row(size_t row) const {
  return MetaValue(move(mImpl->row(row)));
}

MetaValue MetaValue::prod() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Prod));
}

MetaValue MetaValue::sum() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Sum));
}

MetaValue MetaValue::transpose() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Transpose));
}

MetaValue MetaValue::norm() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Norm));
}

MetaValue MetaValue::identity() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Identity));
}

MetaValue MetaValue::zero() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Zero));
}

MetaValue& MetaValue::sin(){
  mImpl->unaryOp(UnaryOp::Sin);
  return *this;
}

MetaValue& MetaValue::cos(){
  mImpl->unaryOp(UnaryOp::Cos);
  return *this;
}

MetaValue& MetaValue::tan(){
  mImpl->unaryOp(UnaryOp::Tan);
  return *this;
}

MetaValue& MetaValue::asin(){
  mImpl->unaryOp(UnaryOp::ASin);
  return *this;
}

MetaValue& MetaValue::acos(){
  mImpl->unaryOp(UnaryOp::ACos);
  return *this;
}

MetaValue& MetaValue::atan(){
  mImpl->unaryOp(UnaryOp::ATan);
  return *this;
}

MetaValue& MetaValue::abs(){
  mImpl->unaryOp(UnaryOp::ATan);
  return *this;
}

MetaValue& MetaValue::min(){
  mImpl->unaryOp(UnaryOp::Min);
  return *this;
}

MetaValue& MetaValue::max(){
  mImpl->unaryOp(UnaryOp::Max);
  return *this;
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
  ValueType vT=(ValueType)*this;
  return vT.typeId() != id::type::Base::value() && vT.rows()>0 && cols() >0;
}

bool MetaValue::compatible(const MetaValue& b) const {
  return valid() && b.valid() && typeId() == b.typeId() && hasUncertainty() == b.hasUncertainty();
}
MetaValue::operator ValueType() const {
  return ValueType(typeId(), rows(), cols(), hasUncertainty());
}

ostream& MetaValue::print(ostream& o) const {
	return mImpl->print(o);
}
