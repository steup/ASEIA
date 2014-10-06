#pragma once

#include <ID.h>
#include <Unit.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <Foreach.h>

#include <array>

class UnitType{
  private:
    using StorageType = std::array<id::unit::ID, id::unit::NumDim::value>;

    StorageType mStorage;

    class DimensionConverter{
      private:
        StorageType& mStorage;
        unsigned int i = 0;
      public:
        DimensionConverter(StorageType& storage);

      template<typename Dim>
      void operator()(Dim d){
        mStorage[i++] = Dim::value;
      }
    };

    using iterator = StorageType::iterator;

    iterator begin();
    iterator end();

  public:
    using const_iterator = StorageType::const_iterator;

    UnitType() = default;

    template<typename Dimensions>
    UnitType(const Unit<Dimensions>& u){
      for(auto& v : mStorage)
        v=0;
      DimensionConverter c(mStorage);
      foreach<Dimensions>(c);
    }

    int8_t operator[](uint8_t i) const;

    const_iterator begin() const;

    const_iterator end() const;

    constexpr static std::size_t length() noexcept { return id::unit::NumDim::value; }

    constexpr static std::size_t size() noexcept { return length() * sizeof(id::unit::ID); }

    bool operator==(const UnitType& b) const;
  
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, UnitType&);

};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const UnitType& unit){
  for(auto v : unit)
    s << v;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, UnitType& unit){
  for(auto& v : unit)
    d >> v;
  return d;
}
