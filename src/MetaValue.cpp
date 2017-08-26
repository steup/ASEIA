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

MetaValue::MetaValue(const ValueType& vT)
  : MetaValue(vT.typeId(), vT.rows(), vT.cols(), vT.hasUncertainty())
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

MetaValue MetaValue::value() const{
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Value));
}

MetaValue MetaValue::uncertainty() const{
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Uncertainty));
}

// \todo Implement
static bool autoCast(ValueType& aT, ValueType& bT) {
  if(aT.hasUncertainty() || bT.hasUncertainty()) {
    aT.hasUncertainty(true);
    bT.hasUncertainty(true);
  }
  if(aT.typeId() == bT.typeId())
    return true;
  if(::id::type::smaller(aT.typeId(), bT.typeId())) {
    aT.typeId(bT.typeId());
    return true;
  }
  if(id::type::smaller(bT.typeId(), aT.typeId())) {
    bT.typeId(aT.typeId());
    return true;
  }
  return false;
}

static bool binaryOp(BinaryOp op, MetaValue& a, const MetaValue& b) {
  if(!a.valid() || !b.valid())
    return false;
  if( a.compatible(b))
    return a.implementation()->binaryOp(op, *b.implementation());
  ValueType aT=(ValueType)a, bT=(ValueType)b;
  if(!autoCast(aT, bT))
    return false;
  a = MetaFactory::instance().convert(aT, a);
  MetaValue tempB = MetaFactory::instance().convert(bT, b);
  return a.implementation()->binaryOp(op, *tempB.implementation());
}

static MetaValue binaryConstOp(BinaryConstOp op, const MetaValue& a, const MetaValue& b){
  if(!a.valid() || !b.valid() || a.rows() != b.rows() || a.cols() != b.cols())
    return MetaValue();
  if( a.compatible(b))
    return MetaValue(a.implementation()->binaryConstOp(op, *b.implementation()));
  ValueType aT=(ValueType)a, bT=(ValueType)b;
  if(!autoCast(aT, bT))
    return MetaValue();
  MetaValue tempA = MetaFactory::instance().convert(aT, a);
  MetaValue tempB = MetaFactory::instance().convert(bT, b);
  return MetaValue(tempA.implementation()->binaryConstOp(op, *tempB.implementation()));
}


MetaValue MetaValue::operator-() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Neg));
}

MetaValue& MetaValue::operator+=(const MetaValue& b) {
  if(!binaryOp(BinaryOp::Add, *this, b))
    mImpl=MVB::instance();
  return *this;
}

MetaValue& MetaValue::operator-=(const MetaValue& b) {
  if(!binaryOp(BinaryOp::Sub, *this, b))
    mImpl=MVB::instance();
  return *this;
}
MetaValue& MetaValue::operator*=(const MetaValue& b) {
  if(!binaryOp(BinaryOp::Mul, *this, b))
    mImpl=MVB::instance();
  return *this;
}
MetaValue& MetaValue::operator/=(const MetaValue& b) {
  if(!binaryOp(BinaryOp::Div, *this, b))
    mImpl=MVB::instance();
  return *this;
}

MetaValue MetaValue::dot(const MetaValue& b) const {
  return ((*this)*b).sum();
}

MetaValue MetaValue::operator==(const MetaValue& b) const {
  return binaryConstOp(BinaryConstOp::Equal, *this, b);
}

MetaValue MetaValue::operator!=(const MetaValue& b) const {
  return binaryConstOp(BinaryConstOp::NotEqual, *this, b);
}

MetaValue MetaValue::operator<=(const MetaValue& b) const {
  return binaryConstOp(BinaryConstOp::SmallEqual, *this, b);
}

MetaValue MetaValue::operator>=(const MetaValue& b) const {
  return binaryConstOp(BinaryConstOp::GreatEqual, *this, b);
}

MetaValue MetaValue::operator<(const MetaValue& b) const {
  return binaryConstOp(BinaryConstOp::Smaller, *this, b);
}

MetaValue MetaValue::operator>(const MetaValue& b) const {
  return binaryConstOp(BinaryConstOp::Greater, *this, b);
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

MetaValue MetaValue::ones() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::Ones));
}

MetaValue MetaValue::zeroValue() const {
  return MetaValue(mImpl->unaryConstOp(UnaryConstOp::ZeroValue));
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

MetaValue& MetaValue::sqrt(){
  mImpl->unaryOp(UnaryOp::Sqrt);
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
  return typeId() != ::id::type::Base() && rows()>0 && cols() >0;
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
