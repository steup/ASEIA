#include <MetaFactory.h>
#include <MetaValueImplementation.h>

MetaFactoryImplementation::MetaFactoryImplementation() : 
  creators{
    {id::type::UInt8 ::value(), &MetaValueImplementation<uint64_t>::factoryCreate},
    {id::type::UInt16::value(), &MetaValueImplementation<uint64_t>::factoryCreate},
    {id::type::UInt32::value(), &MetaValueImplementation<uint64_t>::factoryCreate},
    {id::type::UInt64::value(), &MetaValueImplementation<uint64_t>::factoryCreate},
    {id::type::Int8  ::value(), &MetaValueImplementation<int64_t >::factoryCreate},
    {id::type::Int16 ::value(), &MetaValueImplementation<int64_t >::factoryCreate},
    {id::type::Int32 ::value(), &MetaValueImplementation<int64_t >::factoryCreate},
    {id::type::Int64 ::value(), &MetaValueImplementation<int64_t >::factoryCreate},
    {id::type::Float ::value(), &MetaValueImplementation<double  >::factoryCreate},
    {id::type::Double::value(), &MetaValueImplementation<double  >::factoryCreate},
  },
  converters{
    //{{id::type::UInt16::value(), id::type::UInt8::value()},&::convert<uint16_t, uint8_t>}
  }
{ }

MetaFactoryImplementation::~MetaFactoryImplementation() {

}

MetaValue MetaFactoryImplementation::create(const ValueType& type) const {
  auto iter = creators.find(type.typeId());
  if(iter!=creators.end())
    return MetaValue(iter->second(type.n(), type.hasUncertainty()));
  else
    return MetaValue();
}

MetaValue MetaFactoryImplementation::create(id::type::ID id, std::size_t n, bool u) const {
  auto iter = creators.find(id);
  if(iter!=creators.end())
    return MetaValue(iter->second(n, u));
  else
    return MetaValue();
}

MetaValue MetaFactoryImplementation::convert(const ValueType& type, const MetaValue& value) const {
  if(value.typeId() == type.typeId())
    return value;
  ConverterKey key = {value.typeId(), type.typeId()};
  auto converter = converters.find(key);
  if(converter==converters.end())
    return MetaValue();
  MetaValueBaseImplementation& converted = converter->second(value.implementation());
 return MetaValue(converted);
}

template class Singleton<MetaFactoryImplementation>;
