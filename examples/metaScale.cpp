#include <MetaScale.h>
#include <IO.h>

#include <iostream>

using namespace std;

int main() {
  MetaScale one;
  MetaScale milli = Scale<std::milli>();
  MetaScale kilo  = Scale<std::kilo>();
  MetaScale mega  = kilo * kilo;
  cout << "one   = " << one << endl;
  cout << "milli = " << milli << endl;
  cout << "Mega  = " << mega << endl;
  cout << "milli * mega = " << (milli * mega) << endl;
  cout << "milli / mega = " << (milli / mega) << endl;
  return 0;
}
