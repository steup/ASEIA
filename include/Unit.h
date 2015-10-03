#pragma once

#include <ID.h>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/placeholders.hpp>

template<typename DimID, int value>
struct Dimension : public boost::mpl::int_<value> {
  using ID = DimID;
};

template<typename dims>
class Unit {
  public:
    using Dimensions = dims; 
    
  private:
    template<typename UnitB>
    struct add {
      using dimA   =          Dimensions;
      using dimB   = typename UnitB::Dimensions;
      using dimRes = typename boost::mpl::transform< dimA, dimB, 
                                                     boost::mpl::plus<
                                                       boost::mpl::_1,
                                                       boost::mpl::_2
                                                     > >::type;
      using type   =          Unit< dimRes >;
    };

    template<typename UnitB>
    struct sub {
      using dimA   =          Dimensions;
      using dimB   = typename UnitB::Dimensions;
      using dimRes = typename boost::mpl::transform< dimA, dimB, 
                                                     boost::mpl::minus<
                                                       boost::mpl::_1, 
                                                       boost::mpl::_2
                                                     > >::type;
      using type   =          Unit< dimRes >;
    };

    template<typename Exp>
    struct mult {
      using dimA   =          Dimensions;
      using dimRes = typename boost::mpl::transform< dimA, 
                                                     boost::mpl::times<
                                                       boost::mpl::_1,
                                                       Exp
                                                     > >::type;
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

namespace helper {
  using namespace boost::mpl;

  template<unsigned int n=id::unit::NumDim::value>
  struct ZeroVec{
    using type = typename push_back< typename ZeroVec<n-1>::type, int_<0>::type >::type;
  };

  template<>
  struct ZeroVec<0>{
    using type = vector_c<int>;
  };

  template<typename Dim>
  struct createUnit{
    using pre     = typename helper::ZeroVec<Dim::ID::value>::type;
    using item    = vector_c<int, Dim::value>;
    using post    = typename helper::ZeroVec<id::unit::NumDim::value-Dim::ID::value-1>::type;

    using preEnd  = typename end<pre>::type;
    using temp    = typename insert_range<pre, preEnd, item>::type;

    using tempEnd = typename end<temp>::type;
    using v       = typename insert_range<temp, tempEnd, post>::type;

    using type    = Unit<v>;
  };
}

template<unsigned int e>
using UnitExp = boost::mpl::int_<e>;

using Dimensionless = Unit< helper::ZeroVec< >::type >;
using Steradian     = helper::createUnit< Dimension< id::unit::Steradian, 1> >::type;
using Radian        = helper::createUnit< Dimension< id::unit::Radian   , 1> >::type;
using Candela       = helper::createUnit< Dimension< id::unit::Candela  , 1> >::type;
using Mole          = helper::createUnit< Dimension< id::unit::Mole     , 1> >::type;
using Kelvin        = helper::createUnit< Dimension< id::unit::Kelvin   , 1> >::type;
using Ampere        = helper::createUnit< Dimension< id::unit::Ampere   , 1> >::type;
using Second        = helper::createUnit< Dimension< id::unit::Second   , 1> >::type;
using Kilogram      = helper::createUnit< Dimension< id::unit::Kilogram , 1> >::type;
using Meter         = helper::createUnit< Dimension< id::unit::Meter    , 1> >::type;
