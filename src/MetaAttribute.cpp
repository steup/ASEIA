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
  const bool idTest = mID == b.mID;
  const bool unitTest = mUnit == b.mUnit;
  const bool scaleTest = mScale.reference() == b.mScale.reference();
  return  idTest && unitTest && scaleTest;
}

MetaAttribute& MetaAttribute::operator+=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else {
    if (mScale != b.mScale) {
      MetaAttribute temp(b);
      temp*=mScale/b.mScale;
      mValue += temp.mValue;
    } else
      mValue += b.mValue;
	} return *this;
}

MetaAttribute& MetaAttribute::operator-=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else
    if (mScale != b.mScale) {
      MetaAttribute temp(b);
      temp*=mScale/b.mScale;
      mValue -= temp.mValue;
    } else
      mValue -= b.mValue;
	return *this;
}

MetaAttribute& MetaAttribute::operator*=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else {
    mScale *= b.mScale;
    mValue *= b.value();
  }
	return *this;
}

MetaAttribute& MetaAttribute::operator/=(const MetaAttribute& b) {
	if(!check(b))
		mValue = MetaValue();
  else {
    mScale /= b.mScale;
    mValue /= b.value();
  }
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

MetaValue MetaAttribute::operator<(const MetaAttribute& b) const {
	if(!check(b))
    return MetaValue();

  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=b.mScale/mScale;
    return mValue < temp.mValue;
  } else
    return mValue < b.mValue;
}

MetaValue MetaAttribute::operator<=(const MetaAttribute& b) const {
	if(!check(b))
    return MetaValue();

  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=b.mScale/mScale;
    return mValue <= temp.mValue;
  } else
    return mValue <= b.mValue;
}

MetaValue MetaAttribute::operator>(const MetaAttribute& b) const {
	if(!check(b))
    return MetaValue();

  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=b.mScale/mScale;
    return mValue > temp.mValue;
  } else
    return mValue > b.mValue;
}

MetaValue MetaAttribute::operator>=(const MetaAttribute& b) const {
	if(!check(b))
    return MetaValue();

  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=b.mScale/mScale;
    return mValue >= temp.mValue;
  } else
    return mValue >= b.mValue;
}

MetaValue MetaAttribute::operator==(const MetaAttribute& b) const {
	if(!check(b))
    return MetaValue();

  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=b.mScale/mScale;
    return mValue == temp.mValue;
  } else
    return mValue == b.mValue;
}

MetaValue MetaAttribute::operator!=(const MetaAttribute& b) const {
	if(!check(b))
    return MetaValue();

  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=b.mScale/mScale;
    return mValue != temp.mValue;
  } else
    return mValue != b.mValue;
}


MetaAttribute::operator AttributeType() const {
  return AttributeType(id(), ValueType(value()), scale(), unit());
}

MetaAttribute operator*(const MetaValue& a, const MetaAttribute& b) {
  MetaAttribute temp(b);
  temp.value()=a*temp.value();
  return temp;
}

ostream& operator<<(ostream& o, const MetaAttribute& ma) {
  return o << id::attribute::name(ma.id()) << ": " << ma.value() << " " << ma.scale() << " " << ma.unit();
}
