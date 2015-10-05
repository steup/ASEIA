#include <MetaScale.h>
#include <IO.h>

#include <iostream>

using namespace std;

int main() {
  MetaScale one;
  MetaScale milli = ratio<1, 1000>();
  MetaScale kilo  = ratio<1000>();
  MetaScale mega  = kilo * kilo;
  cout << "one   = " << one << endl;
  cout << "milli = " << milli << endl;
  cout << "Mega  = " << mega << endl;
  cout << "milli * mega = " << (milli * mega) << endl;
  cout << "milli / mega = " << (milli / mega) << endl;
  return 0;
}
