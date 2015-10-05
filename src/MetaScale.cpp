#include <MetaScale.h>
#include <boost/math/common_factor_rt.hpp>

void MetaScale::normalize() {
  uint32_t a = mNum<0?-mNum:mNum;
  uint32_t gcd = boost::math::gcd(a,mDenom);
  mNum   /= gcd;
  mDenom /= gcd;
}

MetaScale MetaScale::operator*(const MetaScale& b) const {
  MetaScale temp(*this);
  temp.mNum*=b.num();
  temp.mDenom*=b.denom();
  temp.normalize();
  return temp;
}

MetaScale MetaScale::operator/(const MetaScale& b) const {
  MetaScale temp(*this);
  temp.mNum*=b.denom();
  temp.mDenom*=b.num();
  temp.normalize();
  return temp;
}
