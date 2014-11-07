#pragma once

#include <ID.h>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/find.hpp>
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

  template<typename ID>
  struct findOp{
    template<typename Dim>
    struct apply{
      using type = typename std::is_same<ID, typename Dim::ID>::type;
    };
  };

  template<typename inputVec, typename inputIter>
  struct extract{
    using type = typename deref< inputIter >::type;
  };

  template<typename inputVec>
  struct extract< inputVec, typename end< inputVec >::type>{
     using type = typename int_<0>::type;
  };

  template<typename inputVec, unsigned int n = id::unit::NumDim::value>
  struct createDimVec {
    using ID      = typename id::unit::id2Type< n-1 >::type;
    using pred    = findOp< ID >;
    using dimIter = typename boost::mpl::find_if< inputVec, pred >::type;
    using next    = typename createDimVec< inputVec, n-1 >::type;
    using value   = typename extract< inputVec, dimIter >::type;
    using type    = typename push_back< next , value >::type;
  };

  template<typename dimVec>
  struct createDimVec< dimVec, 0> {
    using type = typename vector<>::type;
  };
}

template<typename... Dimensions>
struct createUnit{
  using inputVec = typename boost::mpl::vector< Dimensions... >::type;
  using dimVec   = typename helper::createDimVec< inputVec >::type;
  using type = Unit< dimVec >;
};

using Dimensionless = createUnit<>::type;
using Steradian     = createUnit< Dimension< id::unit::Steradian, 1> >::type;
using Radian        = createUnit< Dimension< id::unit::Radian   , 1> >::type;
using Candela       = createUnit< Dimension< id::unit::Candela  , 1> >::type;
using Mole          = createUnit< Dimension< id::unit::Mole     , 1> >::type;
using Kelvin        = createUnit< Dimension< id::unit::Kelvin   , 1> >::type;
using Ampere        = createUnit< Dimension< id::unit::Ampere   , 1> >::type;
using Second        = createUnit< Dimension< id::unit::Second   , 1> >::type;
using Kilogram      = createUnit< Dimension< id::unit::Kilogram , 1> >::type;
using Meter         = createUnit< Dimension< id::unit::Meter    , 1> >::type;
