#include <ScaleType.h>

int32_t ScaleType::num() const {
  return mNum;
}

void ScaleType::num(int32_t num) {
  mNum=num;
}

uint32_t ScaleType::denom() const {
  return mDenom;
}

void ScaleType::denom(uint32_t denom) {
  mDenom=denom;
}

bool ScaleType::operator==(const ScaleType& b) const {
  return mNum==b.mNum && mDenom == b.mDenom;
}
