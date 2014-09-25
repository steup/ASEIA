#include <MetaFactory.h>

namespace I = implementation;

namespace {
  template<typename T>
  I::BaseValue* create(std::size_t n, bool u) {
    return new I::Value<T>(n, u);
  }
}

MetaFactoryImplementation::MetaFactoryImplementation() : 
  creators{
    &::create<uint8_t >,
    &::create<uint16_t>,
    &::create<uint32_t>,
    &::create<uint64_t>,
    &::create<int8_t  >,
    &::create<int16_t >,
    &::create<int32_t >,
    &::create<int64_t >,
    &::create<float   >,
    &::create<double  >
  }
{ }

MetaFactoryImplementation::~MetaFactoryImplementation() {

}

MetaValue MetaFactoryImplementation::create(const ValueType& type) const {
  return MetaValue(creators[type.typeId()](type.n(), type.hasUncertainty()));
}

/*MetaValue MetaFactoryImplementation::convert(const ValueType& type, const MetaValue& value) const {
 return converters[value.typeId(), type.typeId()](value);
}*/

template class Singleton<MetaFactoryImplementation>;
