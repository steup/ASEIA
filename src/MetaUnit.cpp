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
MetaUnit& MetaUnit::inverse() {
  for(int8_t& v: *this)
    v=-v;
  return *this;
}
MetaUnit& MetaUnit::sqrt() {
  for(int8_t& v: *this)
    v/=2;
  return *this;
}
