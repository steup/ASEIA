#include <MetaFactory.h>

namespace I = implementation;

namespace {
  template<typename T>
  I::BaseValue& create(std::size_t n, bool u) {
    I::BaseValue* ptr = new(std::nothrow) I::Value<T>(n, u);
    if(!ptr)
      return I::BaseValue::sInstance;
    else
      return *ptr;
  }
  template<typename T0, typename T1>
  I::BaseValue& convert(const I::BaseValue& a){
    I::BaseValue& temp = create<T1>(a.n(), a.hasUncertainty());
    //TODO
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
  },
  converters{
    {{id::type::UInt16::value(), id::type::UInt8::value()},&::convert<uint16_t, uint8_t>}
  }
{ }

MetaFactoryImplementation::~MetaFactoryImplementation() {

}

MetaValue MetaFactoryImplementation::create(const ValueType& type) const {
  return MetaValue(creators[type.typeId()](type.n(), type.hasUncertainty()));
}

MetaValue MetaFactoryImplementation::convert(const ValueType& type, const MetaValue& value) const {
  if(value.typeId() == type.typeId())
    return value;
  ConverterKey key = {value.typeId(), type.typeId()};
  auto converter = converters.find(key);
  Base& converted = converter->second(*value.mImpl);
 return MetaValue(converted);
}

template class Singleton<MetaFactoryImplementation>;
