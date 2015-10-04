#include <MetaUnit.h>

#include <IO.h>
#include <iostream>

using namespace std;

using AccelUnit = decltype(Meter() / Second() / Second());

int main(){
  MetaUnit acceleration = AccelUnit();
  MetaUnit energy = acceleration * Kilogram() * Meter();
  MetaUnit power  = energy / Second();
  cout << "[Energy]       = " << energy << endl;
  cout << "[Power]        = " << power << endl;
  cout << "[Power * Time] = [Energy] : " << ((power * Second() == energy)?"true":"false") << endl;
  return 0;
}
