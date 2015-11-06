#pragma once

#include <ID.h>
#include <Value.h>

#include <Serializer.h>
#include <DeSerializer.h>

class ValueType{
  private:
    id::type::ID mTypeId         = id::type::Base::value();
    bool         mHasUncertainty = false;
    int32_t      mRows           = 1;
    int32_t      mCols           = 1;
  public:
    union Converter{
      uint8_t data;
      struct{
        bool         uncertainty : 1;
        id::type::ID id          : 7;
      };
    };

    ValueType() = default;
    ValueType(const ValueType& copy) = default;

    ValueType(id::type::ID id, int32_t rows, int32_t cols, bool u) : mTypeId(id), mHasUncertainty(u), mRows(rows), mCols(cols) { }
    
    template<typename T, int32_t rows, int32_t cols, bool u>
    ValueType(Value<T, rows, cols, u>) : 
      mTypeId(id::type::id(T())),
      mHasUncertainty(u),
      mRows(rows),
      mCols(cols)
    {}

    id::type::ID typeId()         const;
    bool         hasUncertainty() const;
    uint32_t     cols()              const;
    uint32_t     rows()              const;

    static constexpr unsigned int size() { return sizeof(mTypeId) + sizeof(mRows) + sizeof(mCols);}

    bool operator==(const ValueType& b) const;

    template<typename I> friend DeSerializer<I>& operator>>(DeSerializer<I>&, ValueType&);
};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const ValueType& value){
  ValueType::Converter c;
  c.id          = value.typeId();
  c.uncertainty = value.hasUncertainty();
  return s << c.data << value.rows() << value.cols();
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, ValueType& value){
  ValueType::Converter c;
  d >> c.data >> value.mRows >> value.mCols;
  value.mTypeId         = c.id;
  value.mHasUncertainty = c.uncertainty;
  return d;
}
