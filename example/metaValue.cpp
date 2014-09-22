#include <MetaValue.h>

#include <iostream>

using namespace std;

int main() {
  MetaValue d1(1.1);
  MetaValue d2(2.2);
  cout << "d1   : " << d1 << endl;
  cout << "d2   : " << d2 << endl;
  cout << "d1+d2: " << d1+d2 << endl;
  return 0;
}
