#pragma once

#include <ValueElement.h>
#include <Value.h>
#include <ostream>

template<typename T>
std::ostream& operator<<(std::ostream& o, const ValueElement<T, true>& e)
{
  return o << e.value() << "+-" << e.uncertainty();
}

template<typename T>
std::ostream& operator<<(std::ostream& o, const ValueElement<T, false>& e)
{
  return o << e.value();
}

template<typename T, std::size_t n, bool useUncertainty>
std::ostream& operator<<(std::ostream& o, const Value<T,n,useUncertainty>& v)
{
  o << "(";
  bool isFirst=true;
  for(auto e : v)
    if(isFirst){
      isFirst=false;
      o << e;
    }
    else
      o << ", " << e;
  return o << ")";
}
