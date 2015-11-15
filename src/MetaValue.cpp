#include <MetaValue.h>
#include <ValueType.h>

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
	mImpl = copy.mImpl->copy();
}

MetaValue::MetaValue(MetaValue&& copy) : mImpl(move(copy.mImpl)) { }

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

bool MetaValue::operator==(const MetaValue& b) const {
  if(compatible(b))
    return *mImpl==*b.mImpl;
  else
    return false;
}

bool MetaValue::valid() const {
  return mImpl->typeId() != id::type::Base::value();
}

bool MetaValue::compatible(const MetaValue& b) const {
  return valid() && mImpl->typeId() == b.mImpl->typeId() && mImpl->cols() == b.mImpl->cols() && mImpl->rows() == mImpl->rows();
}

MetaValue::operator ValueType() {
  return ValueType(typeId(), rows(), cols(), hasUncertainty());
}


