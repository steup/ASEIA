#include <MetaValueImplementation.h>

#include <iostream>

using namespace std;

using Double = MetaValueImplementation<double>;
using Int = MetaValueImplementation<int64_t>;

int main() {
  MetaValue d1 = Double::create({{1.1, 2.2}, {2.2, 0.0}});
  MetaValue d2 = Double::create({{2.2, 3.3}, {3.3, 0.0}});
  cout << "d1   (" << d1.size() << "): " << d1 << endl;
  cout << "d2   (" << d2.size() << "): " << d2 << endl;
  cout << "d1+d2    : " << d1+d2 << endl;
  MetaValue i1 = Int::create({{4, 5, 6}});
  MetaValue i2 = Int::create({{7, 8, 9}});
  MetaValue i3 = Int::create({{10}});
  cout << "i1   (" << i1.size() << "): " << i1 << endl;
  cout << "i2   (" << i2.size() << "): " << i2 << endl;
  cout << "i3   (" << i3.size() << "): " << i3 << endl;
  cout << "i1+i2    : " << i1+i2 << endl;
  cout << "i1+i3    : " << i1+i3 << endl;
  cout << "d1+i2    : " << d1+i2 << endl;
  return 0;
}

