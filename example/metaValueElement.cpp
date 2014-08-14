#include <MetaValueElement.h>
#include <IO.h>

#include <iostream>

using namespace std;

using V1 = ValueElement<int>;
using V2 = ValueElement<uint64_t>;

int main(){
  V1 a={{1,2}};
  MetaValueElement mve(a);
  cout << "V1 - a: " << a << endl;
  V2 b;
  cout << (mve.extract(b)?"":"not ") << "extracted" << endl;;
  cout << "V2 - b: " << b << endl;
  return 0;
}
