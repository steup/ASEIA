#include <ValueElement.h>
#include <IO.h>

#include <iostream>

using V = ValueElement<int, true>;

int main(){
  V a={1,1};
  V b={2,3};
  V c={5,2};
  std::cout << "a  : " << a << std::endl;
  std::cout << "b  : " << b << std::endl;
  std::cout << "a+1: " << (a+1) << std::endl;
  std::cout << "2a : " << (2*a) << std::endl;
  std::cout << "a+b: " << (a+b) << std::endl;
  std::cout << "a*b: " << (a*b) << std::endl;
  std::cout << "a+c: " << (a+c) << std::endl;
  std::cout << "b*c: " << (b*c) << std::endl;
  return 0;
}
