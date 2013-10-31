#include <iostream>

#include <boost/units/pow.hpp>
#include <boost/units/systems/si/io.hpp>

#include <unit.h>

using namespace std;
using namespace boost::units::si;
using namespace boost::units;

int main(int argc, char** argv)
{
  auto temp  = pow<-1>(root<2>(hertz));
  auto noise = root<2>(seconds);
  Unit u(noise);
  cout << u << endl;
  cout << u << " != " << temp << " -> " << (u!=temp?"yes":"no") << endl;
  cout << u << " == " << temp << " -> " << (u==temp?"yes":"no") << endl;
  return 0;
}
