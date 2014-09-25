#pragma once

#include <Singleton.h>
#include <MetaValue.h>
#include <ValueType.h>

#include <map>
#include <utility>

class MetaFactoryImplementation {
  private:
    // from id -> to id
    //using ConverterKey = std::pair<id::type::ID, id::type::ID>;
    //using ConverterValue = void(*)(const MetaValue&, MetaValue&);
    using CreatorValue = implementation::BaseValue* (*)(std::size_t n, bool u);
    //std::map<ConverterKey, ConverterValue> converters;
    std::vector<CreatorValue> creators;
  public:
    MetaFactoryImplementation();
    ~MetaFactoryImplementation();
    MetaValue create(const ValueType& type) const;
    //MetaValue convert(const ValueType& type, const MetaValue& value) const;
};

using MetaFactory = Singleton<MetaFactoryImplementation>;
