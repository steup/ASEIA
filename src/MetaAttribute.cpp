#include <MetaAttribute.h>
#include <MetaFactory.h>
#include <AttributeType.h>
#include <IDIO.h>
#include <IO.h>

#include <memory>
#include <ostream>

using namespace std;

using std::move;

MetaAttribute::MetaAttribute(const AttributeType& at) {
	mValue = MetaFactory::instance().create(at.value());
	mUnit = at.unit();
	mScale = at.scale();
	mID = at.id();
}

bool MetaAttribute::check(const MetaAttribute& b) const {
  return mUnit == b.mUnit && mScale == b.mScale && (ValueType)mValue == (ValueType)b.mValue && mID == b.mID;
}

MetaAttribute& MetaAttribute::operator+=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else
    mValue += b.value();
	return *this;
}

MetaAttribute& MetaAttribute::operator-=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else
    mValue -=  b.value();
	return *this;
}

MetaAttribute& MetaAttribute::operator*=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else
    mValue *= b.value();
	return *this;
}

MetaAttribute& MetaAttribute::operator/=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else
    mValue /= b.value();
	return *this;
}

MetaAttribute& MetaAttribute::operator*=(const MetaScale& scale){
  this->scale() *= scale;
  this->value() /= scale;
  return *this;
}

MetaAttribute& MetaAttribute::operator/=(const MetaScale& scale){
  this->scale() /= scale;
  this->value() *= scale;
  return *this;
}

MetaAttribute operator+(const MetaAttribute& a, const MetaAttribute& b){
  MetaAttribute temp(a);
  return temp+=b;
}

MetaAttribute operator-(const MetaAttribute& a, const MetaAttribute& b){
  MetaAttribute temp(a);
  return temp-=b;
}

MetaAttribute operator*(const MetaAttribute& a, const MetaAttribute& b){
  MetaAttribute temp(a);
  return temp*=b;
}

MetaAttribute operator/(const MetaAttribute& a, const MetaAttribute& b){
  MetaAttribute temp(a);
  return temp/=b;
}

MetaAttribute operator*(const MetaAttribute& a, const MetaScale& scale){
  MetaAttribute temp(a);
  return temp*=scale;
}

MetaAttribute operator/(const MetaAttribute& a, const MetaScale& scale){
  MetaAttribute temp(a);
  return temp/=scale;
}

MetaAttribute operator*(const MetaScale& scale, const MetaAttribute& a){
  return a*scale;
}

bool MetaAttribute::operator==(const MetaAttribute& b) const {
	return check(b) && mValue == b.mValue;
}

MetaAttribute::operator AttributeType() const {
  return AttributeType(id(), ValueType(value()), scale(), unit());
}

ostream& operator<<(ostream& o, const MetaAttribute& ma) {
  return o << id::attribute::name(ma.id()) << ": " << ma.value() << " " << ma.scale() << " " << ma.unit();
}
