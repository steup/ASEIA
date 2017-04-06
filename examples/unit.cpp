#include <IO.h>
#include <Unit.h>

#include <iostream>

int main(){
  std::cout << (Meter()^UnitExp<2>()) << std::endl;
  std::cout << Meter()*Meter() << std::endl;
  
  return 0;
}
