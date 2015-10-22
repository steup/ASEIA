#include <MetaValue.h>

#include <memory>

using namespace std;

/*MetaValue::~MetaValue() { 
  if(mImpl != &MetaValueBaseImplementation::sInstance)
    delete mImpl;
}

MetaValue& MetaValue::operator=(const MetaValue& b) {
  if(mImpl != &MetaValueBaseImplementation::sInstance)
    delete mImpl;
  mImpl = &b.mImpl->copy();
  return *this;
}

MetaValue& MetaValue::operator=(MetaValueBaseImplementation& b) {
  if(mImpl != &MetaValueBaseImplementation::sInstance)
    delete mImpl;
  mImpl = &b;
  return *this;
}*/

MetaValue::MetaValue(MetaValue::Ptr&& ptr){
		mImpl = move(ptr);
}

MetaValue::MetaValue(const MetaValue& copy) : mImpl(MetaValueBaseImplementation::sInstance.copy()){
	*this = copy;
}

MetaValue::MetaValue(MetaValue&& copy) {
	*this = copy;
}

MetaValue& MetaValue::operator=(const MetaValue& copy) {
	mImpl = copy.mImpl->copy();
	return *this;
}

MetaValue& MetaValue::operator=(MetaValue&& copy) {
  mImpl = move(copy.mImpl);
	return *this;
}

MetaValue MetaValue::operator+(const MetaValue& b) const {
  if(compatible(b)) {
    MetaValue temp(*this);
    *temp.mImpl+=*b.mImpl;
    return temp;
  } else
    return MetaValue();
}

bool MetaValue::valid() const {
  return mImpl->typeId() != id::type::Base::value();
}

bool MetaValue::compatible(const MetaValue& b) const {
  return valid() && mImpl->typeId() == b.mImpl->typeId() && mImpl->n() == b.mImpl->n();
}

MetaValue::operator ValueType() {
  return ValueType(typeId(), n(), 1, hasUncertainty());
}


