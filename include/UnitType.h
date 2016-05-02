#pragma once

#include <ID.h>
#include <Unit.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <Foreach.h>
#include <algorithm>

#include <array>

class UnitType{
  protected:
    using StorageType = std::array<int8_t, id::unit::NumDim::value>;

    StorageType mStorage;

    class DimensionConverter{
      private:
        StorageType& mStorage;
        unsigned int i = 0;
      public:
        DimensionConverter(StorageType& storage) : mStorage(storage) {}

      template<typename Dim>
      void operator()(Dim d){
        mStorage[i++] = Dim::value;
      }
    };

    using iterator = StorageType::iterator;

    iterator begin() { return mStorage.begin(); }
    iterator end() { return mStorage.end(); }

    int8_t& operator[](id::unit::ID id) { return mStorage[id]; }

  public:
    using const_iterator = StorageType::const_iterator;

    UnitType() {
      mStorage.fill(0);
    }

    template<typename Dimensions>
    UnitType(const Unit<Dimensions>& u){
      for(auto& v : mStorage)
        v=0;
      DimensionConverter c(mStorage);
      foreach<Dimensions>(c);
    }

    int8_t operator[](id::unit::ID id) const {
      if(id<id::unit::NumDim::value)
        return mStorage[id];
      else
        return 0;
    }

    const_iterator begin() const { return mStorage.cbegin(); }

    const_iterator end() const { return mStorage.cend(); }

    constexpr static std::size_t length() noexcept { return id::unit::NumDim::value; }

    constexpr static std::size_t size() noexcept { return length() * sizeof(id::unit::ID); }

    bool operator==(const UnitType& b) const {
      return std::equal(mStorage.cbegin(), mStorage.cend(), b.mStorage.cbegin());
    }
    
		bool operator!=(const UnitType& b) const {
      return !(*this==b);
    }
  
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
