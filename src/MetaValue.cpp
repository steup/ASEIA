#include <MetaValue.h>

implementation::BaseValue implementation::BaseValue::sInstance;

MetaValue::~MetaValue() { 
  if(mImpl != &implementation::BaseValue::sInstance)
    delete mImpl; 
}

MetaValue& MetaValue::operator=(const MetaValue& b) {
  if(mImpl != &implementation::BaseValue::sInstance)
    delete mImpl;
  mImpl = b.mImpl->copy();
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
  return ValueType(typeId(), n(), hasUncertainty());
}

template class implementation::Value<uint8_t>;
template class implementation::Value<uint16_t>;
template class implementation::Value<uint32_t>;
template class implementation::Value<uint64_t>;
template class implementation::Value<int8_t>;
template class implementation::Value<int16_t>;
template class implementation::Value<int32_t>;
template class implementation::Value<int64_t>;
template class implementation::Value<float>;
template class implementation::Value<double>;
