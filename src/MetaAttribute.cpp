#include <MetaAttribute.h>
#include <MetaFactory.h>
#include <AttributeType.h>
#include <IDIO.h>
#include <IO.h>

#include <memory>
#include <utility>
#include <ostream>

using namespace std;

using std::move;

MetaAttribute::MetaAttribute(const AttributeType& at)
  : mID(at.id()), mValue(at.value()), mUnit(at.unit()), mScale(at.scale())
{}

bool MetaAttribute::check(const MetaAttribute& b) const {
  const bool idTest = mID == b.mID;
  const bool unitTest = mUnit == b.mUnit;
  const bool scaleTest = mScale.reference() == b.mScale.reference();
  return  idTest && unitTest && scaleTest;
}

MetaAttribute::MetaAttribute(const MetaAttribute& b)
  : mID(b.mID), mValue(b.mValue), mUnit(b.mUnit), mScale(b.mScale)
{}

MetaAttribute::MetaAttribute(MetaAttribute&& b)
  : mID(b.mID), mValue(move(b.mValue)), mUnit(b.mUnit), mScale(b.mScale)
{}

MetaAttribute& MetaAttribute::operator=(const MetaAttribute& b) {
  if(this==&b)
    return *this;

  mScale = b.mScale;
  mUnit = b.mUnit;
  mValue = b.mValue;
  return *this;
}

MetaAttribute& MetaAttribute::operator=(MetaAttribute&& b) {
  if(this==&b)
    return *this;

  mScale = b.mScale;
  mUnit = b.mUnit;
  mValue = move(b.mValue);
  return *this;
}

MetaAttribute& MetaAttribute::operator+=(const MetaAttribute& b) {
  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=mScale/b.mScale;
    mValue += temp.mValue;
  } else
    mValue += b.mValue;
	return *this;
}

MetaAttribute& MetaAttribute::operator-=(const MetaAttribute& b) {
  if (mScale != b.mScale) {
    MetaAttribute temp(b);
    temp*=mScale/b.mScale;
    mValue -= temp.mValue;
  } else
    mValue -= b.mValue;
	return *this;
}

MetaAttribute& MetaAttribute::operator*=(const MetaAttribute& b) {
  mScale *= b.mScale;
  mValue *= b.mValue;
  mUnit  *= b.mUnit;
	return *this;
}

MetaAttribute& MetaAttribute::operator/=(const MetaAttribute& b) {
  mScale /= b.mScale;
  mValue /= b.mValue;
  mUnit  /= b.mUnit;
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

MetaAttribute MetaAttribute::norm() const {
  MetaAttribute temp(*this);
  temp.value() = temp.value().norm();
  return temp;
}

MetaAttribute MetaAttribute::certain() const {
  MetaAttribute temp(*this);
  temp.value() = temp.value().value();
  return temp;
}

MetaAttribute MetaAttribute::uncertainty() const {
  MetaAttribute temp(*this);
  temp.value() = temp.value().uncertainty();
  return temp;
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
