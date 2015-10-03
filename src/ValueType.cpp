#include <ValueType.h>

bool ValueType::operator==(const ValueType& b) const{
  return mTypeId == b.mTypeId && mHasUncertainty == b.mHasUncertainty && mRows == b.mRows && mCols == b.mCols;
}

id::type::ID ValueType::typeId()         const { return mTypeId;         }
bool         ValueType::hasUncertainty() const { return mHasUncertainty; }
uint32_t     ValueType::rows()           const { return mRows;           }
uint32_t     ValueType::cols()           const { return mCols;           }
