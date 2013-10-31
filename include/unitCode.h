#pragma once

#include <ostream>
#include <cstdint>

enum UnitCode
{
  m=8,
  kg=7,
  s=6,
  cd=2,
  A=5,
  mol=3,
  bit=10,
  K=4,
  rad=1,
  srad=0,
  none=11
};

std::ostream& operator<<(std::ostream& o, const UnitCode c)
{
  switch(c)
  {
    case(UnitCode::m): return o << "m";
    case(UnitCode::kg): return o << "kg";
    case(UnitCode::s): return o << "s";
    case(UnitCode::cd): return o << "cd";
    case(UnitCode::A): return o << "A";
    case(UnitCode::bit): return o << "bit";
    case(UnitCode::rad): return o << "rad";
    case(UnitCode::srad): return o << "srad";
    case(UnitCode::mol): return o << "mol";
    case(UnitCode::K): return o << "K";
    case(UnitCode::none): return o << "";
  }
  return o;
}
