#pragma once

#include <Singleton.h>
#include <MetaValue.h>
#include <MetaAttribute.h>
#include <MetaEvent.h>
#include <ValueType.h>
#include <ValueElement.h>
#include <ID.h>

#include <map>
#include <utility>

class MetaFactoryImplementation {
  private:
    using Implementation = MetaValueBaseImplementation;
    using Ptr            = Implementation::Ptr;
    
  public:
    // from id -> to id
    using ConverterKey   = std::pair<id::type::ID, id::type::ID>;
    using ConverterValue = void (*)(const Implementation&, Implementation&);
    using Converter      = std::pair<const ConverterKey, ConverterValue>;

    using CreatorKey     = id::type::ID;
    using CreatorValue   = Ptr (*)(std::size_t rows, std::size_t cols, bool u);
    using Creator        = std::pair<const CreatorKey, CreatorValue>;
    
    std::map<CreatorKey, CreatorValue>     creators;
    std::map<ConverterKey, ConverterValue> converters;
  public:
    MetaFactoryImplementation();
    ~MetaFactoryImplementation();
    MetaValue create(const ValueType& type) const;
    MetaAttribute create(const AttributeType& type) const;
    MetaEvent create(const EventType& type) const;
    MetaValue create(id::type::ID id, std::size_t rows, std::size_t cols, bool u) const;
    MetaValue create(std::initializer_list<std::initializer_list<ValueElement<double>>> l, id::type::ID id=id::type::Double::value()) const;
    MetaValue convert(const ValueType& type, const MetaValue& value) const;
    MetaAttribute convert(const AttributeType& type, const MetaAttribute& value) const;
    MetaEvent convert(const EventType& type, const MetaEvent& value) const;
    void insert(const Converter& c) {converters.insert(c);}
};

using MetaFactory = Singleton<MetaFactoryImplementation>;
