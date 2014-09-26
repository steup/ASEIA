#include <IO.h>
#include <Unit.h>

#include <boost/mpl/vector.hpp>

#include <iostream>

int main(){
  auto meter2 = Meter()*Meter();
  std::cout << meter2 << std::endl;
  return 0;
}
