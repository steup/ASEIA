#include <MetaAttribute.h>
#include <MetaFactory.h>
#include <IO.h>

#include <iostream>

using namespace std;

int main() {
  MetaAttribute position(id::attribute::Position::value());
  MetaAttribute distance(id::attribute::Distance::value());
  MetaFactory::instance();
  position.value() = MetaFactory::instance().create({{0,0}, {1,1}, {2,2}}, id::type::Int16::value());
  position.unit()  = Meter();
  position.scale() = Scale<std::deci>();
  distance.value() = MetaFactory::instance().create({{10,1}, {5,1}, {0,1}}, id::type::Int16::value());
  distance.unit()  = Meter();
  distance.scale() = Scale<>();
  cout << position << endl;
  cout << distance << endl;
	position += distance;
  cout << "Positions + Distance: " << endl << position << endl;
  return 0;
}
