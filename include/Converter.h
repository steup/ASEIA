#pragma once

#include <MetaValueImplementation.h>
#include <MetaFactory.h>

template<typename T0, typename T1>
struct Converter{
  using Base = MetaValueBaseImplementation;

  static Base& convert(const Base& in){
    MetaValueImplementation<T1> temp(in.n(), in.hasUncertainty());
    std::size_t i=0;
    for(const auto& elem : reinterpret_cast<const MetaValueImplementation<T0>&>(in).mData)
      temp.mData[i++]=elem;
    return temp.copy();
  }
  
  operator std::pair<const MetaFactory::ConverterKey, MetaFactory::ConverterValue>(){
    return {{id::type::id(T0()), id::type::id(T1())}, &Converter::convert};
  }
};
