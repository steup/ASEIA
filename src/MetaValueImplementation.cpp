#include <MetaValueImplementation.h>
#include <MetaFactory.h>

template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(std::size_t n, bool u) : mHasUncertainty(u) {
  mData.resize(n);
}

template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(T value) : mHasUncertainty(false) {
  mData.resize(1);
  mData[0]=ValueElement<T>(value);
}

template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(MetaValueImplementation::ElementInitType value) 
  : mHasUncertainty(true)
{
  mData.resize(1);
  mData[0]=ValueElement<T>(*value.begin(), *std::next(value.begin()));
}

template<typename T>
MetaValueImplementation<T>::MetaValueImplementation(MetaValueImplementation::InitType values) 
  : mHasUncertainty(true)
{
  mData.resize(values.end()-values.begin());
  std::size_t i = 0;
  for( const auto& v : values ) {
    mData[i++]=ValueElement<T>(*v.begin(), *std::next(v.begin()));
  }
}

template<typename T>
MetaValueBaseImplementation& MetaValueImplementation<T>::operator+=(const MetaValueBaseImplementation& b) {
  mData += reinterpret_cast<const MetaValueImplementation&>(b).mData;
  return *this;
}

template<typename T>
MetaValueBaseImplementation& MetaValueImplementation<T>::copy() const {
  try{
    return *new Type(*this);
  }catch(...){
    return Base::sInstance;
  }
}

template<typename T>
void MetaValueImplementation<T>::print(std::ostream& o) const {
  o << "[";
  for(const auto& e : mData)
    if(mHasUncertainty)
      o << e <<", ";
    else
      o << e.value() <<", ";
  o << "]";
}

template<typename T>
MetaValue MetaValueImplementation<T>::create(){
  MetaFactory& factory = MetaFactory::instance();
  return factory.create(id::type::id(T()), 0, false);
}

template<typename T>
MetaValue MetaValueImplementation<T>::create(T value){
  MetaFactory& factory = MetaFactory::instance();
  MetaValue temp = factory.create(id::type::id(T()), 1, false);
  if(temp.typeId()!=id::type::Base::value())
    reinterpret_cast<MetaValueImplementation&>(temp.implementation()).mData[0]=value;
  return temp;
}

template<typename T>
MetaValue MetaValueImplementation<T>::create(MetaValueImplementation<T>::InitType& i){
  MetaFactory& factory = MetaFactory::instance();

  std::size_t n=0;
  auto begin = i.begin();
  while(begin!=i.end())n++;

  MetaValue temp = factory.create(id::type::id(T()), n, true);
  if(temp.typeId()!=id::type::Base::value()) {
    MetaValueImplementation &impl = reinterpret_cast<MetaValueImplementation&>(temp.implementation());
    auto iter = i.begin();
    for(auto& v : impl.mData){
      v = *iter;
      std::next(iter);
    }
  }
  return temp;
}

template<typename T>
MetaValueBaseImplementation& MetaValueImplementation<T>::factoryCreate(std::size_t n, bool u) {
  MetaValueBaseImplementation* ptr = new MetaValueImplementation<T>(n, u);
  if(!ptr)
    return Base::sInstance;
  else
    return *ptr;
}

/*  template<typename T0, typename T1>
  I::BaseValue& convert(const I::BaseValue& a){
    I::BaseValue& temp = create<T1>(a.n(), a.hasUncertainty());
    //TODO
  }*/

//template class MetaValueImplementation<uint8_t>;
//template class MetaValueImplementation<uint16_t>;
//template class MetaValueImplementation<uint32_t>;
template class MetaValueImplementation<uint64_t>;
//template class MetaValueImplementation<int8_t>;
//template class MetaValueImplementation<int16_t>;
//template class MetaValueImplementation<int32_t>;
template class MetaValueImplementation<int64_t>;
//template class MetaValueImplementation<float>;
template class MetaValueImplementation<double>;
