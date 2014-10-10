#pragma once

#include <Singleton.h>
#include <MetaValue.h>
#include <ValueType.h>
#include <ID.h>

#include <map>
#include <utility>

class MetaFactoryImplementation {
  private:
    using Implementation = MetaValueBaseImplementation;
    // from id -> to id
    using ConverterKey   = std::pair<id::type::ID, id::type::ID>;
    using ConverterValue = Implementation& (*)(const Implementation&);
    using CreatorKey     = id::type::ID;
    using CreatorValue   = Implementation& (*)(std::size_t n, bool u);
    
    std::map<CreatorKey, CreatorValue>     creators;
    std::map<ConverterKey, ConverterValue> converters;
  public:
    MetaFactoryImplementation();
    ~MetaFactoryImplementation();
    MetaValue create(const ValueType& type) const;
    MetaValue create(id::type::ID id, std::size_t n, bool u) const;
    MetaValue convert(const ValueType& type, const MetaValue& value) const;
};

using MetaFactory = Singleton<MetaFactoryImplementation>;
