#include <MetaAttribute.h>
#include <MetaFactory.h>
#include <IO.h>

#include <iostream>

using namespace std;

int main() {
  MetaAttribute position(id::attribute::Position::value());
  MetaFactory::instance();
  position.value() = MetaFactory::instance().create({{0,0}, {1,1}, {2,2}}, id::type::Int8::value());
  position.unit()  = Meter();
  position.scale() = ratio<1,1000>();
  cout << "Position: " << position << endl;
  return 0;
}
