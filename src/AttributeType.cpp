#include <AttributeType.h>

id::attribute::ID AttributeType::attributeId() const { return mAttributeId; }
const UnitType&   AttributeType::unit()        const { return mUnit;        }
const ScaleType&  AttributeType::scale()       const { return mScale;       }
const ValueType&  AttributeType::value()       const { return mValue;       }

bool AttributeType::operator==(const AttributeType& b) const {
  return mAttributeId == b.mAttributeId &&
         mValue       == b.mValue       &&
         mScale       == b.mScale       &&
         mUnit        == b.mUnit        ;
}
