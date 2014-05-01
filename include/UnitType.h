#pragma once

#include <array>

#include <boost/units/systems/si.hpp>
#include <boost/units/is_unit_of_system.hpp>
#include <boost/units/get_dimension.hpp>

#include <boost/mpl/for_each.hpp>

#include <unitCode.h>
#include <packetBuffer.h>
#include <unitExponent.h>

class Unit
{
  public:
    using DimensionType=std::array<UnitExponent, UnitCode::none>;
  private:
    DimensionType d;

    struct DimensionConverter
    {
      DimensionType* s;
      template<typename Dimension>
      void operator()(Dimension& dim)
      {
        uint8_t index=-Dimension::tag_type::value-1;
        (*s)[index]=UnitExponent(Dimension::value_type::numerator(), Dimension::value_type::denominator());
      }
    };

  public:
    Unit(){}
    template<typename BoostUnit>
    Unit(BoostUnit& u)
    {
      static_assert(boost::units::is_unit_of_system<BoostUnit, boost::units::si::system>::value, "Passed unit is not native to Si system");
      DimensionConverter s;
      s.s=&d;
      boost::mpl::for_each<typename boost::units::get_dimension<BoostUnit>::type>(s);
    }

    const DimensionType& dimension() const{return d;}

    bool operator==(const Unit& u) const
    {
      for(unsigned int i=0;i<d.size();i++)
        if(d[i]!=u.d[i])
          return false;
      return true;
    }

    bool operator!=(const Unit& u) const
    {
      return !((*this)==u);
    }

    constexpr static size_t size() noexcept {return UnitCode::none*UnitExponent::size();}
};

template<typename PacketBufferIterator>
PacketBufferIterator& operator<<(PacketBufferIterator& it, const Unit& u)
{
  for(auto v : u.dimension())
    it << v;
  return it;
}

template<typename PacketBufferConstIterator>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& it,Unit& u)
{
  for(auto& v : const_cast<Unit::DimensionType&>(u.dimension()))
    it >> v;
  return it;
}

std::ostream& operator<<(std::ostream& o, const Unit& u)
{
  unsigned int i=0;
  for(auto j : u.dimension())
  {
    if(j!=0)
    {
      o << (UnitCode)i++;
      if(j!=1)
        o << "^" << j;
    }
    else
      i++;
  }
  return o;
}
