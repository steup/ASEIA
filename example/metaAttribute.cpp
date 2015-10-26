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
  position.scale() = ratio<1,10>();
  distance.value() = MetaFactory::instance().create({{10,1}, {5,1}, {0,1}}, id::type::Int16::value());
  distance.unit()  = Meter();
  distance.scale() = ratio<1,1>();
  cout << "Pos       : " << endl << "\t" << position << endl;
  cout << "DIstance  : " << endl << "\t" << distance << endl;
	position += distance;
  cout << "Pos + dist: " << endl << "\t" << position << endl;
  return 0;
}
