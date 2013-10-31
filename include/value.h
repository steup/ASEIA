#pragma once

#include <array>
#include <valueElement.h>

template<typename T, std::size_t n, bool useUncertainty=true>
using Value = std::array<ValueElement<T, useUncertainty>,n>;

template<typename T, std::size_t n, bool useUncertainty>
std::ostream& operator<<(std::ostream& o, const Value<T,n,useUncertainty>& v)
{
  o << "(";
  for(auto e : v)
    o << e << ", ";
  return o << ")";
}
