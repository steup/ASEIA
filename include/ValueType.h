#pragma once

#include <ID.h>
#include <ValueElementType.h>
#include <Value.h>
#include <Serializer.h>
#include <DeSerializer.h>

class ValueType : public ValueElementType{
  private:
    std::size_t mN;
  public:
    ValueType() : ValueElementType(), mN(0){}
    
    template<typename T, std::size_t n, bool u>
    ValueType(const Value<T, n, u> value) : ValueElementType(ValueElement<T,u>()), mN(n){}

    std::size_t n() const {return mN;}

    static constexpr unsigned int size(){return ValueElementType::size() + sizeof(std::size_t);}

    bool operator==(const ValueType& b){
      return ValueElementType::operator==(b) && mN == b.mN;
    }

    template<typename I> friend DeSerializer<I>& operator>>(DeSerializer<I>&, ValueType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const ValueType& value){
  return s << value.n() << static_cast<const ValueElementType&>(value);
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, ValueType& value){
  return d >> value.mN >> static_cast<ValueElementType&>(value);
}
