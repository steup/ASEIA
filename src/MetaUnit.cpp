#include <MetaUnit.h>

#include <algorithm>
#include <functional>

using namespace std;

MetaUnit MetaUnit::operator*(const MetaUnit& b) const {
  MetaUnit temp;
  transform(begin(), end(), b.begin(), temp.begin(), plus<int8_t>());
  return temp;
}

MetaUnit MetaUnit::operator/(const MetaUnit& b) const {
  MetaUnit temp;
  transform(begin(), end(), b.begin(), temp.begin(), minus<int8_t>());
  return temp;
}
