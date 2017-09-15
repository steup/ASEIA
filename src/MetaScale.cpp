#include <MetaScale.h>
#include <boost/math/common_factor_rt.hpp>

void MetaScale::normalize() {
  uint32_t a = mNum<0?-mNum:mNum;
  uint32_t gcd = boost::math::gcd(a,mDenom);
  mNum   /= gcd;
  mDenom /= gcd;
}

MetaScale& MetaScale::operator*=(const MetaScale& b) {
  mNum*=b.num();
  mDenom*=b.denom();
  normalize();
  return *this;
}

MetaScale& MetaScale::operator/=(const MetaScale& b) {
  uint32_t temp = mNum*b.denom();
  mDenom*=b.num();
  mNum = temp;
  normalize();
  return *this;
}
MetaScale MetaScale::operator*(const MetaScale& b) const {
  return MetaScale(*this) *= b;
}

MetaScale MetaScale::operator/(const MetaScale& b) const {
  return MetaScale(*this) /= b;
}
