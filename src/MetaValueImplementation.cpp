#include <MetaValueImplementation.h>
#include <MetaFactory.h>

template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(std::size_t n, bool u) : mHasUncertainty(u) {
  mData.resize(n);
}

template<typename T>
MetaValueBaseImplementation& MetaValueImplementation<T>::operator+=(const MetaValueBaseImplementation& b) {
  mData += reinterpret_cast<const MetaValueImplementation&>(b).mData;
  return *this;
}

/*template<typename T>
MetaValueBaseImplementation& MetaValueImplementation<T>::copy() const {
  try{
    return *new Type(*this);
  }catch(...){
    return Base::sInstance;
  }
}*/

template<typename T>
void MetaValueImplementation<T>::print(std::ostream& o) const {
  o << "(";
  bool first = true;
  for(const auto& e : mData){
    if(first)
      first=!first;
    else
      o << ", ";
    if(mHasUncertainty)
      o << e;
    else
      o << e.value();
  }
  o << ")";
}

template<typename T>
MetaValueBaseImplementation::Ptr MetaValueImplementation<T>::factoryCreate(std::size_t n, bool u) {
  return MetaValueImplementation<T>(n, u).copy();
}

/*  template<typename T0, typename T1>
  I::BaseValue& convert(const I::BaseValue& a){
    I::BaseValue& temp = create<T1>(a.n(), a.hasUncertainty());
    //TODO
  }*/

template class MetaValueImplementation<uint8_t>;
template class MetaValueImplementation<uint16_t>;
template class MetaValueImplementation<uint32_t>;
template class MetaValueImplementation<uint64_t>;
template class MetaValueImplementation<int8_t>;
template class MetaValueImplementation<int16_t>;
template class MetaValueImplementation<int32_t>;
template class MetaValueImplementation<int64_t>;
template class MetaValueImplementation<float>;
template class MetaValueImplementation<double>;
template class MetaValueImplementation<bool>;
