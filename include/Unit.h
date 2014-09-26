#pragma once

#include <ID.h>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/placeholders.hpp>


namespace mpl {
  using namespace boost::mpl;

  template<unsigned int n, typename dim = int_<-1> >
  struct createUnit {
    using match   =          integral_c<int8_t, 1>;
    using noMatch =          integral_c<int8_t, 0>;
    using element = typename if_c< n == dim::value + 1,
                                               match,
                                               noMatch
                             >::type;
    using next    = typename createUnit< n-1, dim >::type;
    using type    = typename push_back< next , element>::type;
  };

  template<typename dim>
  struct createUnit<0, dim> {
    using type = vector<>;
  };
}

template<typename dims>
class Unit {
  public:
    using Dimensions = dims; 
    
  private:
    
    template<typename UnitB>
    struct add {
      using dimA   =          Dimensions;
      using dimB   = typename UnitB::Dimensions;
      using dimRes = typename mpl::transform< dimA, dimB, mpl::plus<mpl::_1, mpl::_2> >::type;
      using type   =          Unit< dimRes >;
    };

    template<typename UnitB>
    struct sub {
      using dimA   =          Dimensions;
      using dimB   = typename UnitB::Dimensions;
      using dimRes = typename mpl::transform< dimA, dimB, mpl::minus<mpl::_1, mpl::_2> >::type;
      using type   =          Unit< dimRes >;
    };

    template<typename Int>
    struct mult {
      using dimA   =          Dimensions;
      using dimRes = typename mpl::transform< dimA, mpl::times<mpl::_1, Int> >::type;
      using type   =          Unit< dimRes >;
    };

  public:
    Unit operator+(const Unit&) const { return *this; }
    Unit operator-(const Unit&) const { return *this; }

    template<typename UnitB>
    auto operator*(const UnitB&) -> typename add<UnitB>::type
    { return typename add<UnitB>::type(); }

    template<typename UnitB>
    auto operator/(const UnitB&) -> typename sub<UnitB>::type
    { return typename sub<UnitB>::type(); }

    template<typename Int>
    auto operator^(const Int&) -> typename mult<Int>::type
    { return typename mult<Int>::type(); }
};

using Dimensionless = Unit<mpl::createUnit< id::unit::NumDim::value                     >::type>;
using Steradian     = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Steradian>::type>;
using Radian        = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Radian   >::type>;
using Candela       = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Candela  >::type>;
using Mole          = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Mole     >::type>;
using Kelvin        = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Kelvin   >::type>;
using Ampere        = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Ampere   >::type>;
using Second        = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Second   >::type>;
using Kilogram      = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Kilogram >::type>;
using Meter         = Unit<mpl::createUnit< id::unit::NumDim::value, id::unit::Meter    >::type>;
