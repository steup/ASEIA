#pragma once

#include <Singleton.h>
#include <MetaValue.h>
#include <MetaAttribute.h>
#include <MetaEvent.h>
#include <ValueType.h>
#include <ValueElement.h>
#include <ID.h>

#include <unordered_map>
#include <utility>
#include <functional>

namespace std {
  template<>
  struct hash<ValueType> {
    std::size_t operator()(ValueType v) const;
  };
  template<>
  struct hash<std::pair<ValueType, ValueType>> {
    std::size_t operator()(std::pair<ValueType, ValueType> v) const;
  };
}

class MetaFactoryImplementation {
  private:
    using Implementation = MetaValueBaseImplementation;
    using Ptr            = Implementation::Ptr;

    using ConverterKey     = std::pair<ValueType, ValueType>;
    using ConverterValue   = void (*)(const Implementation&, Implementation&);
    using ConverterStorage = std::unordered_map<ConverterKey, ConverterValue>;

    using CreatorKey       = ValueType;
    using CreatorValue     = Ptr (*)(std::size_t rows, std::size_t cols);
    using CreatorStorage   = std::unordered_map<CreatorKey, CreatorValue>;

    CreatorStorage creators;
    ConverterStorage converters;

  public:
    using Converter = ConverterStorage::value_type;
    using Creator   = CreatorStorage::value_type;
    
    MetaValue create(const ValueType& type) const;
    MetaAttribute create(const AttributeType& type) const;
    MetaEvent create(const EventType& type) const;
    MetaValue create(id::type::ID id, std::size_t rows, std::size_t cols, bool u) const;
    MetaValue create(std::initializer_list<std::initializer_list<ValueElement<double>>> l, id::type::ID id=id::type::Double::value(), bool u = true) const;
    MetaValue convert(const ValueType& type, const MetaValue& value) const;
    MetaAttribute convert(const AttributeType& type, const MetaAttribute& value) const;
    MetaEvent convert(const EventType& type, const MetaEvent& value) const;
    void insert(const Converter& c) {converters.insert(c);}
    void insert(const Creator& c) {creators.insert(c);}
};

using MetaFactory = Singleton<MetaFactoryImplementation>;
