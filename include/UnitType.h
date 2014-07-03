#pragma once

#include <Serializer.h>
#include <DeSerializer.h>

#include <array>

class UnitType{
  private:
    std::array<int8_t, id::unit::Base::value()> mStorage;
  public:
    UnitType() = default;

    template<typename Unit>
    UnitType(const Unit& u){
      for(auto& v : mStorage)
        v=0;
    }

    int8_t operator[](uint8_t i) const{
      if(i<mStorage.size())
        return mStorage[i];
      return 0;
    }

    constexpr static std::size_t size() noexcept { return id::unit::Base::value(); }

    bool operator==(const UnitType& b){
      for(uint8_t i=0;i<mStorage.size();i++)
        if(mStorage[i]!=b.mStorage[i])
            return false;
      return false;
    }
  
    template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, UnitType&);

};

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const UnitType& unit){
  for(uint8_t i=0;i<UnitType::size();i++)
    s << unit[i];
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, UnitType& unit){
  for(auto& v : unit.mStorage)
    d >> v;
  return d;
}
