#pragma once

#include <ID.h>

#include <Serializer.h>
#include <DeSerializer.h>

#include <array>

#include <boost/units/systems/si/base.hpp>
#include <boost/units/is_unit_of_system.hpp>
#include <boost/mpl/for_each.hpp>

class UnitType{
  private:
    using StorageType = std::array<int8_t, id::unit::Base::value()>;

    StorageType mStorage;

    class DimensionConverter{
      private:
        StorageType& mStorage;
      public:
        DimensionConverter(StorageType& storage);

      template<typename Dim>
      void operator()(Dim d){
        auto id      = id::unit::id(typename Dim::tag_type());
        mStorage[id] = Dim::value_type::numerator();
      }
    };

    using iterator = StorageType::iterator;

    iterator begin();
    iterator end();

  public:
    using const_iterator = StorageType::const_iterator;

    UnitType() = default;

    template<typename Unit>
    UnitType(const Unit& u){
      static_assert( boost::units::is_unit_of_system< Unit, boost::units::si::system >::value, 
                     "Only SI-Units are supported"
      );
      for(auto& v : mStorage)
        v=0;
      DimensionConverter c(mStorage);
      boost::mpl::for_each<typename Unit::dimension_type>(c);
    }

    int8_t operator[](uint8_t i) const;

    const_iterator begin() const;

    const_iterator end() const;

    constexpr static std::size_t size() noexcept { return id::unit::Base::value(); }

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
