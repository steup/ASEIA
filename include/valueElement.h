#pragma once

#include <utility>

template<typename T, bool useUncertainty=true>
class ValueElement : public std::pair<T,T>
{
  public:
    using BaseType=std::pair<T,T>;
    ValueElement() : BaseType(0,0){}
    ValueElement(T v, T u) : BaseType(v,u){}

    const T value() const{return this->first;}
    T& value(){return this->first;}
    void value(T v){this->first=v;}
    const T uncertainty() const{return this->second;}
    T& uncertainty(){return this->second;}
    void uncertainty(T u){this->second=u;}

    constexpr static std::size_t size() noexcept {return sizeof(BaseType);}
};

template<typename T>
class ValueElement<T, false>
{
  private:
    T v;
  public:
    using BaseType=T;
    ValueElement() : v(0){}
    ValueElement(T c) : v(c){}

    const T value() const{return v;}
    T& value(){return v;}
    void value(T v){this->v=v;}

    constexpr static std::size_t size() noexcept {return sizeof(BaseType);}
};

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
