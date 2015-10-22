#include <MetaAttribute.h>

#include <memory>

using std::move;

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
