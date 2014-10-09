#include <MetaFactory.h>
#include <MetaValueImplementation.h>

MetaFactoryImplementation::MetaFactoryImplementation() : 
  creators{
    &MetaValueImplementation<uint64_t>::factoryCreate,
    &MetaValueImplementation<uint64_t>::factoryCreate,
    &MetaValueImplementation<uint64_t>::factoryCreate,
    &MetaValueImplementation<uint64_t>::factoryCreate,
    &MetaValueImplementation<int64_t>::factoryCreate,
    &MetaValueImplementation<int64_t>::factoryCreate,
    &MetaValueImplementation<int64_t>::factoryCreate,
    &MetaValueImplementation<int64_t>::factoryCreate,
    &MetaValueImplementation<double>::factoryCreate,
    &MetaValueImplementation<double>::factoryCreate
  },
  converters{
    //{{id::type::UInt16::value(), id::type::UInt8::value()},&::convert<uint16_t, uint8_t>}
  }
{ }

MetaFactoryImplementation::~MetaFactoryImplementation() {

}

MetaValue MetaFactoryImplementation::create(const ValueType& type) const {
  return MetaValue(creators[type.typeId()](type.n(), type.hasUncertainty()));
}

MetaValue MetaFactoryImplementation::create(id::type::ID id, std::size_t n, bool u) const {
  return MetaValue(creators[id](n, u));
}

MetaValue MetaFactoryImplementation::convert(const ValueType& type, const MetaValue& value) const {
  if(value.typeId() == type.typeId())
    return value;
  ConverterKey key = {value.typeId(), type.typeId()};
  auto converter = converters.find(key);
  MetaValueBaseImplementation& converted = converter->second(value.mImpl);
 return MetaValue(converted);
}

template class Singleton<MetaFactoryImplementation>;
