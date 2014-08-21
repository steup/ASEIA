#include <MetaValueElement.h>
#include <IO.h>

#include <iostream>

using namespace std;

using V1 = ValueElement<float>;
using V2 = ValueElement<unsigned int>;

int main(){
  V1 a={{1,-2}};
  MetaValueElement mve(a);
  cout << "V1 - a: " << a << endl;
  V2 b;
  mve.extract(b);
  cout << "V2 - b: " << b << endl;
  return 0;
}
