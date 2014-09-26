#include <iostream>

#include <ID.h>
#include <IDIO.h>

using namespace std;

int main(){
  id::attribute::Position pos;
  id::type::t2Type<int16_t>::type type;
  id::unit::Kelvin unit;
  cout << "Attr: " << id::attribute::name(pos.value()) << ": " << (uint16_t)pos.value() << endl;
  cout << "Type: " << id::type::name(type.value()) << ": " << (uint16_t)type.value() << " - " << decltype(type)::Type(1.0) << endl;
  cout << "Unit: " << id::unit::name(unit.value) << ": " << (uint16_t)unit.value << " - " << endl;
  return 0;
}
