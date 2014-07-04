#pragma once

#include <ID.h>
#include <Value.h>

#include <Serializer.h>
#include <DeSerializer.h>

class ValueType{
  private:
    id::type::ID mTypeId         = id::type::Base::value();
    bool         mHasUncertainty = false;
    uint32_t     mN              = 1;
  public:
    union Converter{
      uint8_t data;
      struct{
        bool         uncertainty : 1;
        id::type::ID id          : sizeof(id::type::ID)*8-1;
      };
    };

    ValueType() = default;
    
    template<typename T, std::size_t n, bool u>
    ValueType(Value<T, n, u>) : 
      mTypeId(id::type::id(T())),
      mHasUncertainty(u),
      mN(n)
    {}

    id::type::ID typeId()         const { return mTypeId;         }
    bool         hasUncertainty() const { return mHasUncertainty; }
    uint32_t     n()              const { return mN;              }

    static constexpr unsigned int size() { return sizeof(mTypeId) + sizeof(mN);}

    bool operator==(const ValueType& b){
      return mTypeId == b.mTypeId && mHasUncertainty && b.mHasUncertainty && mN == b.mN;
    }

    template<typename I> friend DeSerializer<I>& operator>>(DeSerializer<I>&, ValueType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const ValueType& value){
  ValueType::Converter c;
  c.id          = value.typeId();
  c.uncertainty = value.hasUncertainty();
  return s << c.data << value.n();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, ValueType& value){
  ValueType::Converter c;
  d >> c.data >> value.mN;
  value.mTypeId         = c.id;
  value.mHasUncertainty = c.uncertainty;
  return d;
}
