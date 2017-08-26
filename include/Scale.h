#pragma once

#include <ratio>
#include <boost/mpl/int.hpp>

template<typename Rat=std::ratio<1>,  uint32_t Ref=0>
struct Scale : public Rat{
  using Reference = boost::mpl::int_<Ref>;
  using Ratio = Rat;
  Scale() = default;
  constexpr uint32_t reference() const { return Reference::value; }
  constexpr Ratio ratio() const { return Ratio(); }
};
