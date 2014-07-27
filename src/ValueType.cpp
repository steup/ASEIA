#include <ValueType.h>

bool ValueType::operator==(const ValueType& b) const{
  return mTypeId == b.mTypeId && mHasUncertainty == b.mHasUncertainty && mN == b.mN;
}

id::type::ID ValueType::typeId()         const { return mTypeId;         }
bool         ValueType::hasUncertainty() const { return mHasUncertainty; }
uint32_t     ValueType::n()              const { return mN;              }
