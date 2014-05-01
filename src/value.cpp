#include <iostream>

#include <Value.h>

using V = Value<double, 3>;

int main(){
  V a({{1.1,1.5}, {2.2,2.8}, {3.9}});
  V b({{3,2}, {1,2} ,{3,3}});
  std::cout << "a  : " << a << std::endl;
  std::cout << "b  : " << b << std::endl;
  std::cout << "a+1: " << (a+1) << std::endl;
  std::cout << "2a : " << (a*2) << std::endl;
  std::cout << "a+b: " << (a+b) << std::endl;
  std::cout << "a*b: " << (a*b) << std::endl;
  return 0;
}
