#include <MetaUnit.h>

#include <algorithm>
#include <functional>

using namespace std;

MetaUnit& MetaUnit::operator*=(const MetaUnit& b){
  MetaUnit temp;
  transform(begin(), end(), b.begin(), begin(), plus<int8_t>());
  return *this;
}

MetaUnit& MetaUnit::operator/=(const MetaUnit& b) {
  transform(begin(), end(), b.begin(), begin(), minus<int8_t>());
  return *this;
}
