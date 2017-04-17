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

MetaAttribute& MetaAttribute::operator+=(const MetaAttribute& b) {
	if(!(mUnit == b.mUnit)) {
		mValue = MetaValue();
		return *this;
	}
	if(!(mScale == b.mScale)) {
		MetaValue sumB = b.value();
		sumB*=(b.mScale / mScale);
		mValue = mValue + sumB;
	} else
		mValue = mValue + b.value();
	return *this;
}

MetaAttribute& MetaAttribute::operator*=(const MetaScale& scale){
  this->scale() *= scale;
  this->value() /= scale;
  return *this;
}

MetaAttribute MetaAttribute::operator*(const MetaScale& scale) const {
  MetaAttribute temp(*this);
  return temp*=scale;
}

bool MetaAttribute::operator==(const MetaAttribute& b) const {
	return id() == b.id() &&  value() == b.value() && unit() == b.unit() && scale() == b.scale();
}

MetaAttribute::operator AttributeType() const {
  return AttributeType(id(), ValueType(value()), scale(), unit());
}

ostream& operator<<(ostream& o, const MetaAttribute& ma) {
  return o << id::attribute::name(ma.id()) << ": " << ma.value() << " " << ma.scale() << " " << ma.unit();
}
