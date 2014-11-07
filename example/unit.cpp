#include <IO.h>
#include <Unit.h>

#include <iostream>

int main(){
  std::cout << createUnit< Dimension< id::unit::Meter, 2> >::type() << std::endl;
  std::cout << Meter()*Meter() << std::endl;
  return 0;
}
