#include <MetaValueElement.h>
#include <IO.h>

#include <iostream>

using namespace std;

using V1 = ValueElement<float>;
using V2 = ValueElement<unsigned int>;
using V3 = ValueElement<int>;

int main(){
  V1 a={{1.1, 0}};
  V2 b={{5, 2}};
  MetaValueElement mA(a);
  MetaValueElement mB(b);
  cout << "V1  -  a      : " << a << endl;
  cout << "V2  -  b      : " << b << endl;
  cout << "MVE - mA      : " << mA << endl;
  cout << "MVE - mB      : " << mB << endl;
  mA+=mB;
  cout << "MVE - mA += mB: " << mA << endl;
  cout << "MVE - mA + mB : " << mA + mB << endl;
  V3 c;
  mA.extract(c);
  cout << "V3 - mA: " << c << endl;
  return 0;
}
