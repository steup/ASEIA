#include <iostream>

#include <ID.h>
#include <boost/units/io.hpp>

using namespace std;

int main(){
  id::attribute::Position pos;
  id::type::getTypeID<int16_t>::type type;
  id::unit::Kelvin unit;
  cout << "Attr: " << pos.name() << ": " << (uint16_t)pos.value() << endl;
  cout << "Type: " << type.name() << ": " << (uint16_t)type.value() << " - " << decltype(type)::Type(1.0) << endl;
  cout << "Unit: " << unit.name() << ": " << (uint16_t)unit.value() << " - " << decltype(unit)::Unit() << endl;
  return 0;
}
