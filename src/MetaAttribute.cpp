#include <MetaAttribute.h>
#include <AttributeType.h>
#include <IDIO.h>
#include <IO.h>

#include <memory>
#include <ostream>

using namespace std;

using std::move;

MetaAttribute::MetaAttribute(const MetaAttribute& copy) {
	*this = copy;
}

MetaAttribute::MetaAttribute(MetaAttribute&& movee) {
	*this = movee;
}
MetaAttribute& MetaAttribute::operator=(const MetaAttribute& copy) {
	if(mUnit == copy.mUnit) {
		mScale = copy.mScale;
		mValue = copy.mValue;
	}
	return *this;
}

MetaAttribute& MetaAttribute::operator=(MetaAttribute&& copy) {
	if(&copy == this)
		return *this;
	if(mUnit == copy.mUnit) {
		mScale = move(copy.mScale);
		mValue = move(copy.mValue);
	}
	return *this;
}

MetaAttribute& MetaAttribute::operator+=(const MetaAttribute& b) {
	if(!(mUnit == b.mUnit)) {
		mValue = MetaValue();
		return *this;
	}
	if(!(mScale == b.mScale)) {
		MetaValue sumB = b.value();
		sumB.scale(b.mScale / mScale);
		mValue = mValue + sumB;
	} else
		mValue = mValue + b.value();
	return *this;
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
