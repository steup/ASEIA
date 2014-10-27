#include <MetaFactory.h>
#include <MetaValueImplementation.h>
#include <Converter.h>

MetaFactoryImplementation::MetaFactoryImplementation() : 
  creators{
    {id::type::UInt8 ::value(), &MetaValueImplementation<uint8_t >::factoryCreate},
    {id::type::UInt16::value(), &MetaValueImplementation<uint16_t>::factoryCreate},
    {id::type::UInt32::value(), &MetaValueImplementation<uint32_t>::factoryCreate},
    {id::type::UInt64::value(), &MetaValueImplementation<uint64_t>::factoryCreate},
    {id::type::Int8  ::value(), &MetaValueImplementation<int8_t  >::factoryCreate},
    {id::type::Int16 ::value(), &MetaValueImplementation<int16_t >::factoryCreate},
    {id::type::Int32 ::value(), &MetaValueImplementation<int32_t >::factoryCreate},
    {id::type::Int64 ::value(), &MetaValueImplementation<int64_t >::factoryCreate},
    {id::type::Float ::value(), &MetaValueImplementation<float   >::factoryCreate},
    {id::type::Double::value(), &MetaValueImplementation<double  >::factoryCreate},
  },
  converters{
    Converter<uint8_t, uint16_t>(), Converter<uint8_t, uint32_t>(),
    Converter<uint8_t, uint64_t>(), Converter<uint8_t, int8_t>(),
    Converter<uint8_t, int16_t>(),  Converter<uint8_t, int32_t>(),
    Converter<uint8_t, int64_t>(),  Converter<uint8_t, float>(),
    Converter<uint8_t, double>(),   Converter<uint8_t, bool>(),
    Converter<uint16_t, uint8_t>(), Converter<uint16_t, uint32_t>(),
    Converter<uint16_t, uint64_t>(), Converter<uint16_t, int8_t>(),
    Converter<uint16_t, int16_t>(),  Converter<uint16_t, int32_t>(),
    Converter<uint16_t, int64_t>(),  Converter<uint16_t, float>(),
    Converter<uint16_t, double>(),   Converter<uint16_t, bool>(),
    Converter<uint32_t, uint8_t>(), Converter<uint32_t, uint16_t>(),
    Converter<uint32_t, uint64_t>(), Converter<uint32_t, int8_t>(),
    Converter<uint32_t, int16_t>(),  Converter<uint32_t, int32_t>(),
    Converter<uint32_t, int64_t>(),  Converter<uint32_t, float>(),
    Converter<uint32_t, double>(),   Converter<uint32_t, bool>(),
    Converter<uint64_t, uint8_t>(), Converter<uint64_t, uint16_t>(),
    Converter<uint64_t, uint32_t>(), Converter<uint64_t, int8_t>(),
    Converter<uint64_t, int16_t>(),  Converter<uint64_t, int32_t>(),
    Converter<uint64_t, int64_t>(),  Converter<uint64_t, float>(),
    Converter<uint64_t, double>(),   Converter<uint64_t, bool>(),

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
