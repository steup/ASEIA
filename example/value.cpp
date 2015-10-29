#include <IO.h>
#include <Value.h>

#include <iostream>

using V = Value<float, 2, 3>;

using namespace std;

int main(){
  V a = {{{1.1,1.5}, {2.2,2.8}, {3.9}},
			   {{1.1,1.5}, {2.2,2.8} ,{3.9}}};
  V b = {{{3,2}, {1,2}, {3,3}},
         {{3,2}, {1,2}, {3,3}}};
  cout << "a  : " << endl << a             << endl;
  cout << "b  : " << endl << b             << endl;
  cout << "a+1: " << endl << (a+V::Ones()) << endl;
  cout << "2a : " << endl << (a*2)         << endl;
  cout << "a+b: " << endl << (a+b)         << endl;
  //cout << "a*b: " << (a*b) << endl;
  return 0;
}
