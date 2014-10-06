#pragma once

#include <Singleton.h>
#include <MetaValue.h>
#include <ValueType.h>

#include <map>
#include <utility>

class MetaFactoryImplementation {
  private:
    using Base = implementation::BaseValue;
    // from id -> to id
    using ConverterKey = std::pair<id::type::ID, id::type::ID>;
    using ConverterValue = Base& (*)(const Base&);
    using CreatorValue = Base& (*)(std::size_t n, bool u);
    
    std::vector<CreatorValue> creators;
    std::map<ConverterKey, ConverterValue> converters;
  public:
    MetaFactoryImplementation();
    ~MetaFactoryImplementation();
    MetaValue create(const ValueType& type) const;
    MetaValue convert(const ValueType& type, const MetaValue& value) const;
};

using MetaFactory = Singleton<MetaFactoryImplementation>;
