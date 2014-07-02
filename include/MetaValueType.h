#pragma once

#include <ID.h>
#include <MetaValueElementType.h>
#include <Value.h>
#include <Serializer.h>
#include <DeSerializer.h>

class MetaValueType : public MetaValueElementType{
  private:
    uint32_t  mN = 1;
  public:
    MetaValueType() = default;
    
    template<typename T, std::size_t n, bool u>
   MetaValueType(const Value<T, n, u> value) : MetaValueElementType(ValueElement<T,u>()), mN(n){}

    uint32_t n() const {return mN;}

    static constexpr unsigned int size(){return MetaValueElementType::size() + sizeof(mN);}

    bool operator==(const MetaValueType& b){
      return MetaValueElementType::operator==(b) && mN == b.mN;
    }

    template<typename I> friend DeSerializer<I>& operator>>(DeSerializer<I>&, MetaValueType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaValueType& value){
  return s << static_cast<const MetaValueElementType&>(value) << value.n();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaValueType& value){
  return d >> static_cast<MetaValueElementType&>(value) >> value.mN;
}
