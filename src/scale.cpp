#include <iostream>

using namespace std;

#include <scale.h>

int main()
{
  cout << milli << endl;
  cout << "m * m = u " << milli*milli << " -> " << (milli*milli==micro?"yes":"no") << endl;

  cout << "m * u = n " << milli*micro << " -> " << (milli*micro==nano?"yes":"no") << endl;
  cout << "m * m * m = n " << milli*milli*milli << " -> " << (milli*milli*milli==nano?"yes":"no") << endl;
  cout << "k * k * k * n = 1 " << kilo*kilo*kilo*nano << " -> " << (kilo*kilo*kilo*nano==1.0f?"yes":"no") << endl;
  DynamicPacketBuffer b;
  std::back_inserter(b) << nano;
  cout << nano << " - " << b << endl;
  return 0;
  
}
